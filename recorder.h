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

#ifndef RECORDER_H
#define RECORDER_H

#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include <sndfile.h>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#define REC_JK_NAME "QJackRcd"
#define REC_JK_SYSTEMNAME "system"

#define REC_FRAME_SIZE sizeof(float)
#define REC_RINGBUFFER_FRAMES (64*1024)
#define REC_RINGBUFFER_SIZE (2*REC_RINGBUFFER_FRAMES*REC_FRAME_SIZE)
#define REC_BUFFER_FRAMES (2*1024)
#define REC_BUFFER_SIZE (2*REC_BUFFER_FRAMES*REC_FRAME_SIZE)

#define REC_WAIT_TIMEOUT_MS 1000

class Recorder: public QThread
{
    SNDFILE *sndFile;

    float *currentBuffer;
    float *alternateBuffer;

    QString dirPath;
    int diskSpace;

    QString currentFilePath;

    QMutex dataReadyMutex;
    QWaitCondition dataReady;

    jack_client_t *jackClient;
    jack_ringbuffer_t *jackRingBuffer;
    jack_port_t *jackInputPort1;
    jack_port_t *jackInputPort2;

    float pauseLevel;
    int pauseActivationMax;
    int pauseActivationDelay;
    int pauseActivationCount;
    bool splitMode;

    float leftLevel;
    float rightLevel;

    int overruns;
    int sampleRate;

    bool paused;
    bool recording;
    bool shutdown;

    void computeDiskSpace();
    void computeFilePath();
    void computePauseActivationMax();

    void newFile();
    void closeFile();

    void switchBuffer();
    void readCurrentBuffer();
    void computeCurrentBufferLevels();
    void writeAlternateBuffer();
    void fadeoutAlternateBuffer();
    void fadeinAlternateBuffer();

protected:

    void run();
    bool isFile() { return sndFile != NULL; }
    bool isPauseLevel() { return leftLevel <= pauseLevel && rightLevel <= pauseLevel; }
    void setShutdown(bool value) { shutdown = value; }

public:

    Recorder();
    ~Recorder();

    int jackProcess(jack_nframes_t nframes);
    int jackSync(jack_transport_state_t state, jack_position_t *pos);
    void jackShutdown();

    void autoConnect();
    void resetConnect();

    bool isShutdown() { return shutdown; }
    void setRecording(bool value) { recording = value; }
    bool isRecording() { return recording; }
    void setPaused(bool value) { paused = value; }
    bool isPaused() { return paused; }
    void setPauseActivationDelay(int secs) {pauseActivationDelay = secs;}
    int getPauseActivationDelay() {return pauseActivationDelay;}
    void setSplitMode(bool split) { splitMode = split; }
    bool isSplitMode() { return splitMode; }
    QString getCurrentFilePath() { return currentFilePath; }
    int getDiskSpace() { return diskSpace; }
    int getOverruns() { return overruns; }
    void setPauseLevel(float level) { pauseLevel = level; }
    float getPauseLevel() { return pauseLevel; }
    float getLeftLevel() { return leftLevel; }
    float getRightLevel() { return rightLevel; }
};

#endif // RECORDER_H
