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
/**
* @file recorder.cpp
* $Author$
* $Date$
* $Revision$
* @brief Implementation of Recorder class
*/

#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "recorder.h"

#include <QWaitCondition>
#include <QProcess>
#include <QFileInfo>
#include <QDateTime>

//=============================================================================
// Internal defines
//=============================================================================

#define RCD_JK_INPUT_PORTNAME_1 "record_1"
#define RCD_JK_INPUT_PORTNAME_2 "record_2"

#define RCD_WAIT_TIMEOUT_MS 1000
#define RCD_SIG_CHANGE_COUNT 4

#define RCD_FRAME_SIZE sizeof(float)
#define RCD_RINGBUFFER_FRAMES (64*1024)
#define RCD_RINGBUFFER_SIZE (2*RCD_RINGBUFFER_FRAMES*RCD_FRAME_SIZE)
#define RCD_BUFFER_FRAMES (2*1024)
#define RCD_BUFFER_SIZE (2*RCD_BUFFER_FRAMES*RCD_FRAME_SIZE)

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

void jack_portreg (jack_port_id_t port_id, int reg, void *recorder)
{
    ((Recorder*)recorder)->jackPortReg(port_id, reg);
}

void jack_shutdown (void *recorder)
{
    ((Recorder*)recorder)->jackShutdown();
}

//=============================================================================
// Recorder cont/dest methods
//=============================================================================

Recorder::Recorder(QString jackName)
{
    this->jackName = jackName;
    sndFile = NULL;
    dirPath = QDir::home();
    currentFilePath = "";
    processFilePath = "";
    processCmdLine = "";
    overruns = 0;
    pauseActivationMax = 0;
    pauseActivationCount = pauseActivationMax + 1;

    if ((jackClient = jack_client_open(jackName.toAscii(), jack_options_t(JackNullOption | JackUseExactName), 0)) == 0) {
        throw "Can't start or connect to jack server";
    }

    sampleRate = jack_get_sample_rate(jackClient);

    jackInputPort1 = jack_port_register (jackClient, RCD_JK_INPUT_PORTNAME_1, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    jackInputPort2 = jack_port_register (jackClient, RCD_JK_INPUT_PORTNAME_2, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

    jack_set_process_callback(jackClient, jack_process, this);
    jack_set_sync_callback(jackClient, jack_sync, this);
    jack_on_shutdown (jackClient, jack_shutdown, this);
    jack_set_port_registration_callback(jackClient, jack_portreg, this);

    jackRingBuffer = jack_ringbuffer_create(RCD_RINGBUFFER_SIZE);
    jack_ringbuffer_reset(jackRingBuffer);

    currentBuffer = new float[RCD_BUFFER_FRAMES*2];
    memset(currentBuffer, 0, RCD_BUFFER_SIZE);

    alternateBuffer = new float[RCD_BUFFER_FRAMES*2];
    memset(alternateBuffer, 0, RCD_BUFFER_SIZE);

    setRecording(false);
    setShutdown(false);
    setSplitMode(false);
    setPauseActivationDelay(3);
    setPauseLevel(-20);

    // start the recorder thread
    start();
}

Recorder::~Recorder()
{
    setShutdown(true);
    // wait for recorder thread shutdown
    wait(RCD_WAIT_TIMEOUT_MS);

    // free / close objects
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

    // the ringbuffer will transmit data to reorder thread (non RT)
    size_t rbspace = jack_ringbuffer_write_space(jackRingBuffer);

    if (rbspace < (2*nframes*RCD_FRAME_SIZE)) {
        // the ringbuffer is full, IO thread is too late because of IO locks or overloading
        overruns++;
        rc = 1;
    }
    else {
        // tmp value to be shure of data convertion
        float value;

        // write interlived stereo data into the ringbuffer
        for(jack_nframes_t i = 0; i < nframes; i++) {
            value = in1[i];
            jack_ringbuffer_write (jackRingBuffer, (const char *)(&value), RCD_FRAME_SIZE);
            value = in2[i];
            jack_ringbuffer_write (jackRingBuffer, (const char *)(&value), RCD_FRAME_SIZE);
        }
    }
    // wakeup recorder thread because there is data to process
    if (dataReadyMutex.tryLock()) {
        dataReady.wakeAll();
        dataReadyMutex.unlock();
    }
    return rc;
}

void Recorder::jackPortReg(jack_port_id_t port_id, int reg) {
    if (reg) {
        // if a port is registerred, its ID is put in queue for processing by the recorder thread
        jackPortRegQueue.enqueue(port_id);
    }
}

void Recorder::jackShutdown()
{
    setShutdown(true);
}

//=============================================================================
// Recorder public methods
//=============================================================================


//=============================================================================
// Recorder internal methods
//=============================================================================

// The recorder thread run function, all recording algorithm is manged from here
void Recorder::run()
{
    int loopCounter = 0;

    // start jack incomming sound
    jack_activate(jackClient);

    // this computed attribute must be initialized before entering the main loop
    computePauseActivationMax();

    // to start always in pause mode if under pause level.
    pauseActivationCount = pauseActivationMax + 1;

    // initial signal for listeners
    emit statusChanged();

    // the main loop (while shutdown state is off)
    while (!isShutdown()) {

        // check if therre is some things to do about registration ports (queue not empty)
        checkJackAutoConnect();

        // while ringbuffer has data.
        while (jack_ringbuffer_read_space(jackRingBuffer) >= RCD_BUFFER_SIZE) {

            // switch alternate to current buffer and clean it
            switchBuffer();

            // read the current buffer from ringbuffer
            readCurrentBuffer();

            // comute levels (DB)
            computeCurrentBufferLevels();

            // delay may have changed
            computePauseActivationMax();

            if (isRecording()) {
                // a file must be open on theses status values
                if (!isFile()) newFile();

                if (isPauseLevel()) {
                    if (pauseActivationCount < pauseActivationMax) {
                        // the activation delay is not reached, continue to write previous buffer
                        writeAlternateBuffer();
                        pauseActivationCount++;
                    }
                    else if (pauseActivationCount == pauseActivationMax) {
                        // the activation delay is reached, fadeout previous buffer to eliminate noises and write it.
                        fadeoutAlternateBuffer();
                        writeAlternateBuffer();
                        if (splitMode) {
                            // new file will close current file
                            newFile();
                        }
                        pauseActivationCount++;
                    }
                }
                else {
                    if (pauseActivationCount > pauseActivationMax)
                        // we were in pause satuts, fadein previous buffer to eliminate noises and write it.
                        fadeinAlternateBuffer();
                    writeAlternateBuffer();
                    pauseActivationCount = 0;
                }
            }
            else {
                closeFile();
                // to re-start always in pause mode.
                pauseActivationCount = pauseActivationMax + 1;
            }
            // update disk space compute
            computeDiskSpace();

            if (loopCounter >= RCD_SIG_CHANGE_COUNT) {
                // notify listeners
                emit statusChanged();
                loopCounter = 0;
            }
            else loopCounter++;
        }
        // wait for new data
        dataReady.wait(&dataReadyMutex, RCD_WAIT_TIMEOUT_MS);
    }

    // to be shure that file is closed
    closeFile();

    // stop jack incomming sound
    jack_deactivate(jackClient);
}

void Recorder::computePauseActivationMax() {
    pauseActivationMax = (sampleRate * pauseActivationDelay ) / RCD_BUFFER_FRAMES;
}

void Recorder::computeCurrentBufferLevels() {
    float sumsqr_l = 0;
    float sumsqr_r = 0;
    int ibuf = 0;
    for (int i = 0; i < RCD_BUFFER_FRAMES; i++) {
        sumsqr_l += currentBuffer[ibuf]*currentBuffer[ibuf];
        ibuf++;
        sumsqr_r += currentBuffer[ibuf]*currentBuffer[ibuf];
        ibuf++;
    }
    float rms_l = sqrtf( sumsqr_l / ((float)RCD_BUFFER_FRAMES) );
    float rms_r = sqrtf( sumsqr_r / ((float)RCD_BUFFER_FRAMES) );
    float db_l = log10f( rms_l ) * 10;
    float db_r = log10f( rms_r ) * 10;
    leftLevel = db_l < - 40 ? - 40 : db_l;
    rightLevel = db_r < - 40 ? - 40 : db_r;
}

void Recorder::computeDiskSpace() {
    struct statfs stats;
    statfs(dirPath.path().toAscii().constData(), &stats);
    diskSpace = 100 - (stats.f_bavail * 100) / stats.f_blocks;
}

void Recorder::computeFilePath() {
    currentFilePath = dirPath.absoluteFilePath(
                jackName.toLower()
                + "-" + QDateTime::currentDateTime().toString("yyyy-MM-ddThh-mm-ss")
                + ".wav" );
}

void Recorder::newFile() {
    // new file begins always by closing current if exists
    closeFile();

    SF_INFO sfinfo;
    sfinfo.format = SF_FORMAT_WAV|SF_FORMAT_FLOAT;
    sfinfo.channels = 2;
    sfinfo.samplerate = sampleRate;
    sfinfo.frames = RCD_BUFFER_FRAMES;

    computeFilePath();

    sndFile = sf_open (currentFilePath.toAscii().constData(), SFM_WRITE, &sfinfo);
}

void Recorder::closeFile() {
    // this method is safe if no file is open.
    if ( isFile() ) {
        sf_close (sndFile);
        sndFile = NULL;
        // closing file allways involve post processing to start.
        processFile();
    }
    currentFilePath = "";
}

void Recorder::switchBuffer() {
    // switch current and alternate buffer
    float* tmpBuffer = currentBuffer;
    currentBuffer = alternateBuffer;
    alternateBuffer = tmpBuffer;

    // clean buffer
    memset(currentBuffer, 0, RCD_BUFFER_SIZE);
}

void Recorder::readCurrentBuffer() {
    // read ringbuffer
    jack_ringbuffer_read(jackRingBuffer, (char*)(currentBuffer), RCD_BUFFER_SIZE);
}

void Recorder::writeAlternateBuffer() {
    sf_writef_float(sndFile, alternateBuffer, RCD_BUFFER_FRAMES);
}

void Recorder::fadeinAlternateBuffer() {
    float gain = 0;
    float gaininc = 1 / float(RCD_BUFFER_FRAMES);
    int ibuf = 0;
    for (int i = 0; i < RCD_BUFFER_FRAMES; i++) {
        alternateBuffer[ibuf++] *= gain;
        alternateBuffer[ibuf++] *= gain;
        gain += gaininc;
    }
}

void Recorder::fadeoutAlternateBuffer() {
    float gain = 1;
    float gaininc = 1 / float(RCD_BUFFER_FRAMES);
    int ibuf = 0;
    for (int i = 0; i < RCD_BUFFER_FRAMES; i++) {
        alternateBuffer[ibuf++] *= gain;
        alternateBuffer[ibuf++] *= gain;
        gain -= gaininc;
    }
}

void Recorder::processFile()
{
    // only do post process if a command line is defined
    if (!processCmdLine.isEmpty() && !currentFilePath.isEmpty()) {
        QStringList args;
        args.append("-c");
        args.append(processCmdLine);
        args.append(currentFilePath);
        QProcess pr;
        pr.startDetached("bash", args);
        processFilePath = currentFilePath;
    }
}

QString Recorder::getJackConnections(jack_port_t* jackPort) {
    const char** connections = NULL;
    QString result = "";
    if ((connections = jack_port_get_all_connections (jackClient, jackPort)) != NULL) {
        for (int i = 0; connections[i]; i++) {
            result += QString(connections[i]) + ";";
            //printf ("%s\n", connections[i]);
        }
        jack_free ((void*)connections);
    }
    return result;
}

void Recorder::setJackConnections(QString cnxLine, jack_port_t* jackPort) {
    QStringList strList = cnxLine.split(';', QString::SkipEmptyParts);
    for (int i = 0; i < strList.count() ; i++) {
        jack_connect(jackClient, strList.at(i).toAscii().constData(), jack_port_name(jackPort) );
    }
}


void Recorder::checkJackAutoConnect() {
    while (!jackPortRegQueue.empty()) {
        jack_port_t* port = jack_port_by_id(jackClient, jackPortRegQueue.dequeue());
        if (jack_port_flags(port) & JackPortIsOutput) {
            QString portName = jack_port_name(port);
            if (jack_port_connected(jackInputPort1) == 0)
                jack_connect(jackClient, portName.toAscii().constData(), jack_port_name(jackInputPort1) );
            else if (jack_port_connected(jackInputPort2) == 0)
                jack_connect(jackClient, portName.toAscii().constData(), jack_port_name(jackInputPort2) );
        }
    }
}

