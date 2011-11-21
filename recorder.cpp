// $Id$
/***************************************************************************
 Copyright (C) 2011 - Olivier ROUITS <olivier.rouits@free.fr>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 ***************************************************************************/
// $LastChangedBy$
// $LastChangedRevision$
// $LastChangeDate$
// $HeadURL$

#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <time.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "recorder.h"

#include <QWaitCondition>

//=============================================================================
// Jack callback to object calls
//=============================================================================

int jack_process (jack_nframes_t nframes, void *recorder)
{
    return ((Recorder*)recorder)->jackProcess(nframes);
}

int jack_sync (jack_transport_state_t state, jack_position_t *pos, void *recorder)
{
    return ((Recorder*)recorder)->jackSync(state, pos);
}

void jack_shutdown (void *recorder)
{
    ((Recorder*)recorder)->jackShutdown();
}

//=============================================================================
// Recorder cont/dest methods
//=============================================================================

Recorder::Recorder()
{
    sndFile = NULL;
    dirPath = getpwuid(getuid())->pw_dir;
    overruns = 0;

    if ((jackClient = jack_client_open(REC_JK_NAME, jack_options_t(JackNullOption | JackUseExactName), 0)) == 0) {
        throw "Can't start or connect to jack server";
    }

    sampleRate = jack_get_sample_rate(jackClient);

    jackInputPort1 = jack_port_register (jackClient, "record_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    jackInputPort2 = jack_port_register (jackClient, "record_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

    jack_set_process_callback(jackClient, jack_process, this);
    jack_set_sync_callback(jackClient, jack_sync, this);
    jack_on_shutdown (jackClient, jack_shutdown, this);

    jackRingBuffer = jack_ringbuffer_create(REC_RINGBUFFER_SIZE);
    jack_ringbuffer_reset(jackRingBuffer);

    currentBuffer = new float[REC_BUFFER_FRAMES*2];
    memset(currentBuffer, 0, REC_BUFFER_SIZE);

    alternateBuffer = new float[REC_BUFFER_FRAMES*2];
    memset(alternateBuffer, 0, REC_BUFFER_SIZE);

    setRecording(false);
    setPaused(false);
    setSplitMode(false);
    setPauseActivationDelay(2);
    setPauseLevel(-20);

    start();
}

Recorder::~Recorder()
{
    setShutdown(true);
    wait(REC_WAIT_TIMEOUT_MS);
    if (currentBuffer) delete currentBuffer;
    if (alternateBuffer) delete alternateBuffer;
    if (jackRingBuffer) jack_ringbuffer_free(jackRingBuffer);
    if (jackClient) jack_client_close(jackClient);
}

//=============================================================================
// Recorder jack methods
//=============================================================================

int Recorder::jackSync(jack_transport_state_t state, jack_position_t *pos)
{
    if (state == JackTransportStopped)
        setRecording(false);
    else if (state == JackTransportStarting)
        setRecording(true);
    return TRUE;
}

int Recorder::jackProcess(jack_nframes_t nframes)
{
    int rc = 0;

    jack_default_audio_sample_t *in1 =(jack_default_audio_sample_t *)jack_port_get_buffer (jackInputPort1, nframes);
    jack_default_audio_sample_t *in2 =(jack_default_audio_sample_t *)jack_port_get_buffer (jackInputPort2, nframes);

    size_t rbspace = jack_ringbuffer_write_space(jackRingBuffer);

    if (rbspace < (2*nframes*REC_FRAME_SIZE)) {
        overruns++;
        rc = 1;
    }
    else {
        // tmp value to be shure of data convertion
        float value;

        // write interlived stereo data into the ringbuffer
        for(jack_nframes_t i = 0; i < nframes; i++) {
            value = in1[i];
            jack_ringbuffer_write (jackRingBuffer, (const char *)(&value), REC_FRAME_SIZE);
            value = in2[i];
            jack_ringbuffer_write (jackRingBuffer, (const char *)(&value), REC_FRAME_SIZE);
        }
    }
    // wake recorder thread because there is data to process
    if (dataReadyMutex.tryLock()) {
        dataReady.wakeAll();
        dataReadyMutex.unlock();
    }
    return rc;
}

void Recorder::jackShutdown()
{
    setShutdown(true);
}

//=============================================================================
// Recorder accessors
//=============================================================================

//=============================================================================
// Recorder public methods
//=============================================================================

void Recorder::autoConnect()
{
    // this function is a bad idea
    const char** lsport = jack_get_ports(jackClient,NULL,NULL,JackPortIsOutput);
    int portidx = 0;
    for ( const char* portname = *lsport; portname; portname = *(++lsport) ) {
        jack_port_t* port = jack_port_by_name(jackClient, portname);
        if (!jack_port_is_mine(jackClient, port)) {
            if (portidx % 2 == 0)
                jack_connect(jackClient,portname, jack_port_name(jackInputPort1) );
            else
                jack_connect(jackClient, portname, jack_port_name(jackInputPort2) );
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

//=============================================================================
// Recorder internal methods
//=============================================================================

void Recorder::run()
{
    jack_activate(jackClient);

    computePauseActivationMax();
    // to start always in pause mode if under pause level.
    pauseActivationCount = pauseActivationMax + 1;

    while (!isShutdown()) {
        while (jack_ringbuffer_read_space(jackRingBuffer) >= REC_BUFFER_SIZE) {

            // switch, read and compute level buffer
            switchBuffer();
            readCurrentBuffer();
            computeCurrentBufferLevels();

            // delay may have changed
            computePauseActivationMax();

            if (isRecording()) {
                // a file must be open on theses status values
                if (!isFile()) newFile();

                if (isPauseLevel()) {
                    if (pauseActivationCount < pauseActivationMax) {
                        writeAlternateBuffer();
                        pauseActivationCount++;
                        setPaused(false);
                    }
                    else if (pauseActivationCount == pauseActivationMax) {
                        fadeoutAlternateBuffer();
                        writeAlternateBuffer();
                        if (splitMode)
                            newFile();
                        pauseActivationCount++;
                        setPaused(false);
                    }
                    else
                        setPaused(true);
                }
                else {
                    if (pauseActivationCount > pauseActivationMax)
                        fadeinAlternateBuffer();
                    writeAlternateBuffer();
                    pauseActivationCount = 0;
                    setPaused(false);
                }
            }
            else {
                closeFile();
                // to re-start always in pause mode if under pause level.
                pauseActivationCount = pauseActivationMax + 1;
            }
            computeDiskSpace();
        }
        // wait for new data
        dataReady.wait(&dataReadyMutex, REC_WAIT_TIMEOUT_MS);
    }

    closeFile();
    jack_deactivate(jackClient);
}

void Recorder::computePauseActivationMax() {
    pauseActivationMax = (sampleRate * pauseActivationDelay ) / REC_BUFFER_FRAMES;
}

void Recorder::computeCurrentBufferLevels() {
    float sumsqr_l = 0;
    float sumsqr_r = 0;
    int ibuf = 0;
    for (int i = 0; i < REC_BUFFER_FRAMES; i++) {
        sumsqr_l += currentBuffer[ibuf]*currentBuffer[ibuf];
        ibuf++;
        sumsqr_r += currentBuffer[ibuf]*currentBuffer[ibuf];
        ibuf++;
    }
    float rms_l = sqrtf( sumsqr_l / ((float)REC_BUFFER_FRAMES) );
    float rms_r = sqrtf( sumsqr_r / ((float)REC_BUFFER_FRAMES) );
    float db_l = log10f( rms_l ) * 10;
    float db_r = log10f( rms_r ) * 10;
    leftLevel = db_l < - 40 ? - 40 : db_l;
    rightLevel = db_r < - 40 ? - 40 : db_r;
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
    currentFilePath.sprintf("%s/%s-%04d-%02d-%02dT%02d-%02d-%02d.%s", dirPath.toAscii().constData(), "qjackrcd", tparts->tm_year + 1900, tparts->tm_mon + 1, tparts->tm_mday,
                            tparts->tm_hour, tparts->tm_min, tparts->tm_sec, "wav");
}

void Recorder::newFile() {
    closeFile();

    SF_INFO sfinfo;
    sfinfo.format = SF_FORMAT_WAV|SF_FORMAT_FLOAT;
    sfinfo.channels = 2;
    sfinfo.samplerate = sampleRate;
    sfinfo.frames = REC_BUFFER_FRAMES;

    computeFilePath();

    sndFile = sf_open (currentFilePath.toAscii().constData(), SFM_WRITE, &sfinfo);
}

void Recorder::closeFile() {
    if ( isFile() ) {
        sf_close (sndFile);
        sndFile = NULL;
    }
    currentFilePath = "";
}

void Recorder::switchBuffer() {
    // switch current and alternate buffer
    float* tmpBuffer = currentBuffer;
    currentBuffer = alternateBuffer;
    alternateBuffer = tmpBuffer;

    // clean buffer
    memset(currentBuffer, 0, REC_BUFFER_SIZE);
}

void Recorder::readCurrentBuffer() {
    // read ringbuffer
    jack_ringbuffer_read(jackRingBuffer, (char*)(currentBuffer), REC_BUFFER_SIZE);
}

void Recorder::writeAlternateBuffer() {
    sf_writef_float(sndFile, alternateBuffer, REC_BUFFER_FRAMES);
}

void Recorder::fadeinAlternateBuffer() {
    float gain = 0;
    float gaininc = 1 / float(REC_BUFFER_FRAMES);
    int ibuf = 0;
    for (int i = 0; i < REC_BUFFER_FRAMES; i++) {
        alternateBuffer[ibuf++] *= gain;
        alternateBuffer[ibuf++] *= gain;
        gain += gaininc;
    }
}

void Recorder::fadeoutAlternateBuffer() {
    float gain = 1;
    float gaininc = 1 / float(REC_BUFFER_FRAMES);
    int ibuf = 0;
    for (int i = 0; i < REC_BUFFER_FRAMES; i++) {
        alternateBuffer[ibuf++] *= gain;
        alternateBuffer[ibuf++] *= gain;
        gain -= gaininc;
    }
}

