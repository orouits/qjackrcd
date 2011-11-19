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

#define REC_STATUS_ON 2
#define REC_STATUS_WAIT 1
#define REC_STATUS_OFF 0
#define REC_STATUS_SHUTDOWN -1

#define REC_RINGBUFFER_FRAMES 16384
#define REC_RINGBUFFER_SIZE (REC_RINGBUFFER_FRAMES*sizeof(float)*2)
#define REC_BUFFER_FRAMES 1024
#define REC_BUFFER_SIZE (REC_BUFFER_FRAMES*sizeof(float)*2)

#define REC_WAIT_TIMEOUT_MS 2000

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
    int status;
    float leftLevel;
    float rightLevel;
    int pauseActivationCount;
    bool splitMode;
    int overruns;
    int sampleRate;

    void computeBufferLevels();
    void computeDiskSpace();
    void computeFilePath();

    void newFile();
    void closeFile();

    void switchReadBuffer();
    void writeAlternateBuffer();
    void writeAlternateBufferFadeout();
    void writeAlternateBufferFadein();

public:
    Recorder();
    ~Recorder();

    void run();

    int jackProcess(jack_nframes_t nframes);
    int jackSync(jack_transport_state_t state, jack_position_t *pos);
    void jackShutdown();

    void startRecording();
    void stopRecording();
    void autoConnect();
    void resetConnect();

    int getStatus() { return status; }
    void setPauseActivationDelay(int secs) {pauseActivationDelay = secs;}
    int getPauseActivationDelay() {return pauseActivationDelay;}
    void setSplitMode(bool split) { splitMode = split; }
    bool isSplitMode() { return splitMode; }
    QString getCurrentFilePath() { return currentFilePath; }
    int getDiskSpace() { return diskSpace; }
    int getOverruns() { return overruns; }

    void setPauseLevel(float level) { pauseLevel = level; }
    bool isPauseLevel() { return leftLevel <= pauseLevel && rightLevel <= pauseLevel; }
    float getPauseLevel() { return pauseLevel; }
    float getLeftLevel() { return leftLevel; }
    float getRightLevel() { return rightLevel; }
};

#endif // RECORDER_H
