/***************************************************************************
 *   Copyright (C) 2011 - Olivier ROUITS                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <time.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "recorder.h"

int jack_process (jack_nframes_t nframes, void *recorder)
{
    return ((Recorder*)recorder)->jackProcess(nframes);
}


int jack_sync (jack_transport_state_t state, jack_position_t *pos, void *recorder)
{
    return ((Recorder*)recorder)->jackSync(state, pos);
}

Recorder::Recorder()
{
    sndFile = NULL;
    pauseActivationCount = 0;
    pauseLevel = -20;
    splitMode = false;
    dirPath = getpwuid(getuid())->pw_dir;

    if ((jclient = jack_client_open(JCLIENTNAME, jack_options_t(JackNullOption | JackNoStartServer | JackUseExactName), 0)) == 0) {
        throw "Can't connect to jack server";
    }

    iport1 = jack_port_register (jclient, "record_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    iport2 = jack_port_register (jclient, "record_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    oport1 = jack_port_register (jclient, "playback_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    oport2 = jack_port_register (jclient, "playback_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    jack_set_process_callback(jclient, jack_process, this);
    jack_set_sync_callback(jclient, jack_sync, this);

    bufferSize = jack_get_buffer_size(jclient) * 2 * sizeof(float);
    buffer = new float[jack_get_buffer_size(jclient) * 2];

    resetBuffer();
    setPauseActivationDelay(2);

    stop();

    jack_activate(jclient);
}

Recorder::~Recorder()
{
    stop();
    if (jclient != NULL) {
        jack_deactivate(jclient);
        jack_client_close(jclient);
    }
    if (buffer != NULL ) {
        delete buffer;
        buffer = NULL;
    }
    bufferSize = 0;
}

void Recorder::autoConnect()
{
    // this function is a bad idea
    const char** lsport = jack_get_ports(jclient,NULL,NULL,JackPortIsOutput);
    int portidx = 0;
    for ( const char* portname = *lsport; portname; portname = *(++lsport) ) {
        jack_port_t* port = jack_port_by_name(jclient, portname);
        if (!jack_port_is_mine(jclient, port)) {
            if (portidx % 2 == 0)
                jack_connect(jclient,portname, jack_port_name(iport1) );
            else
                jack_connect(jclient, portname, jack_port_name(iport2) );
            printf("%s\n",portname);
        }
        portidx++;
    }
    //jack_free(lsport);
}

void Recorder::resetConnect()
{
    //TODO: bad idea too...
}

void Recorder::setPauseActivationDelay(int secs)
{
    pauseActivationMax = (jack_get_sample_rate(jclient) * secs ) / jack_get_buffer_size(jclient);
}

QString& Recorder::start()
{
    // start always in wait mode.
    newFile();
    pauseActivationCount = pauseActivationMax + 1;
    recStatus = RECWAIT;
    return filePath;
}

QString& Recorder::stop()
{
    recStatus = RECOFF;
    pauseActivationCount = 0;
    closeFile();
    return filePath;
}

int Recorder::jackSync(jack_transport_state_t state, jack_position_t *pos)
{
    if (state == JackTransportStopped && recStatus != RECOFF) {
        stop();
    }
    else if (state == JackTransportStarting && recStatus == RECOFF) {
        start();
    }
    return TRUE;
}

int Recorder::jackProcess(jack_nframes_t nframes)
{
    jack_default_audio_sample_t *out1 = (jack_default_audio_sample_t *)jack_port_get_buffer (oport1, nframes);
    jack_default_audio_sample_t *out2 = (jack_default_audio_sample_t *)jack_port_get_buffer (oport2, nframes);

    jack_default_audio_sample_t *in1 =(jack_default_audio_sample_t *)jack_port_get_buffer (iport1, nframes);
    jack_default_audio_sample_t *in2 =(jack_default_audio_sample_t *)jack_port_get_buffer (iport2, nframes);

    computeLevel(in1, in2, nframes);

    if (recStatus != RECOFF) {
        if (isPauseLevel()) {
            if (pauseActivationCount < pauseActivationMax) {
                writeBuffer(out1, out2, nframes);
                pauseActivationCount++;
            }
            else if (pauseActivationCount == pauseActivationMax) {
                writeBufferFadeout(out1, out2, nframes);
                if (splitMode) {
                    stop();
                    start();
                }
                else
                    pauseActivationCount++;
            }
            else {
                recStatus = RECWAIT;
                outputNull(out1, out2, nframes);
            }
        }
        else {
            recStatus = RECON;
            if (pauseActivationCount > pauseActivationMax) {
                writeBufferFadein(out1, out2, nframes);
            }
            else {
                writeBuffer(out1, out2, nframes);
            }
            pauseActivationCount = 0;
        }
    }
    else {
        outputNull(out1, out2, nframes);
    }

    computeDiskSpace();

    setBuffer(in1, in2, nframes);

    return 0;
}

void Recorder::computeLevel(jack_default_audio_sample_t* in1, jack_default_audio_sample_t* in2, jack_nframes_t nframes) {
    leftLevel = computeLeveldB(in1, nframes);
    rightLevel = computeLeveldB(in2, nframes);
}

float Recorder::computeLeveldB(jack_default_audio_sample_t* in, jack_nframes_t nframes) {
    float sumsqr = 0;
    for (jack_nframes_t i = 0; i < nframes; i++) {
        sumsqr += in[i]*in[i];
    }
    float rms = sqrtf( sumsqr / ((float)nframes) );
    float db = log10f( rms ) * 10;
    return db < - 40 ? - 40 : db;
}

void Recorder::computeDiskSpace() {
    struct statfs stats;
    statfs(dirPath.toAscii().constData(), &stats);
    diskSpace = 100 - (stats.f_bavail * 100) / stats.f_blocks;
}

void Recorder::computeFilePath() {
    time_t t;
    struct tm *tparts;
    time(&t);
    tparts = localtime(&t);
    filePath.sprintf("%s/%s-%04d-%02d-%02dT%02d-%02d-%02d.%s", dirPath.toAscii().constData(), "qjackrcd", tparts->tm_year + 1900, tparts->tm_mon + 1, tparts->tm_mday,
            tparts->tm_hour, tparts->tm_min, tparts->tm_sec, "wav");
}

void Recorder::newFile() {
    SF_INFO sfinfo;
    sfinfo.format = SF_FORMAT_WAV|SF_FORMAT_FLOAT;
    sfinfo.channels = 2;
    sfinfo.samplerate = jack_get_sample_rate(jclient);
    sfinfo.frames = jack_get_buffer_size(jclient);

    computeFilePath();

    sndFile = sf_open (filePath.toAscii().constData(), SFM_WRITE, &sfinfo);
    fileSize = 88;
}

void Recorder::closeFile() {
    if (sndFile != NULL) {
        sf_close (sndFile);
        sndFile = NULL;
    }
    filePath = "";
    fileSize = 0;
}

void Recorder::resetBuffer() {
    memset(buffer, 0, bufferSize);
}

void Recorder::setBuffer(jack_default_audio_sample_t *in1, jack_default_audio_sample_t *in2, jack_nframes_t nframes) {
    int ibuf = 0;
    for(jack_nframes_t i = 0; i < nframes; i++) {
        buffer[ibuf++] = in1[i];
        buffer[ibuf++] = in2[i];
    }
}

void Recorder::writeBuffer(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes) {
    int ibuf = 0;
    for(jack_nframes_t i = 0; i < nframes; i++) {
        out1[i] = buffer[ibuf++];
        out2[i] = buffer[ibuf++];
    }
    sf_writef_float(sndFile, buffer, nframes);
    fileSize += bufferSize / 1024;
}

void Recorder::writeBufferFadein(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes) {
    float gain = 0;
    float gaininc = 1 / float(nframes);
    int ibuf = 0;
    for (jack_nframes_t i = 0; i < nframes; i++) {
        out1[i] = (buffer[ibuf++] *= gain);
        out2[i] = (buffer[ibuf++] *= gain);
        gain += gaininc;
    }
    sf_writef_float(sndFile, buffer, nframes);
    fileSize += bufferSize / 1024;
}

void Recorder::writeBufferFadeout(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes) {
    float gain = 1;
    float gaininc = 1 / float(nframes);
    int ibuf = 0;
    for (jack_nframes_t i = 0; i < nframes; i++) {
        out1[i] = (buffer[ibuf++] *= gain);
        out2[i] = (buffer[ibuf++] *= gain);
        gain -= gaininc;
    }
    sf_writef_float(sndFile, buffer, nframes);
    fileSize += bufferSize / 1024;
}

void Recorder::outputNull(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes) {
    memset(out1, 0, nframes * sizeof(jack_default_audio_sample_t));
    memset(out2, 0, nframes * sizeof(jack_default_audio_sample_t));
}
