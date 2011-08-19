#ifndef RECORDER_H
#define RECORDER_H

#include <jack/jack.h>
#include <sndfile.h>
#include <QString>

#define JCLIENTNAME "QJackRcd"
#define JSYSTEMNAME "system"
#define RECON 1
#define RECOFF 0
#define RECWAIT -1

class Recorder
{
    SNDFILE *sndFile;

    float *buffer;
    int bufferSize;

    QString dirPath;
    QString filePath;
    int fileSize;
    int diskSpace;

    jack_client_t *jclient;
    jack_port_t *iport1;
    jack_port_t *iport2;
    jack_port_t *oport1;
    jack_port_t *oport2;

    float pauseLevel;
    int pauseActivationMax;
    int recStatus;
    float leftLevel;
    float rightLevel;
    int pauseActivationCount;
    bool splitMode;

    void computeLevel(jack_default_audio_sample_t* in1, jack_default_audio_sample_t* in2, jack_nframes_t nframes);
    float computeLeveldB(jack_default_audio_sample_t* in, jack_nframes_t nframes);
    void computeDiskSpace();
    void computeFilePath();

    void newFile();
    void closeFile();

    void resetBuffer();
    void setBuffer(jack_default_audio_sample_t* in1, jack_default_audio_sample_t* in2, jack_nframes_t nframes);
    void writeBuffer(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes);
    void writeBufferFadeout(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes);
    void writeBufferFadein(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes);
    void outputNull(jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2, jack_nframes_t nframes);

public:
    Recorder();
    ~Recorder();

    QString& start();
    QString& stop();
    int jackProcess(jack_nframes_t nframes);
    int jackSync(jack_transport_state_t state, jack_position_t *pos);
    void setPauseActivationDelay(int secs);
    void autoConnect();
    void resetConnect();

    int getRecStatus() { return recStatus; }
    void setSplitMode(bool split) { splitMode = split; }
    bool isSplitMode() { return splitMode; }
    QString& getFilePath() { return filePath; }
    int getFileSize() { return fileSize; }
    int getDiskSpace() { return diskSpace; }

    void setPauseLevel(float level) { pauseLevel = level; }
    bool isPauseLevel() { return leftLevel <= pauseLevel && rightLevel <= pauseLevel; }
    float getPauseLevel() { return pauseLevel; }
    float getLeftLevel() { return leftLevel; }
    float getRightLevel() { return rightLevel; }
};

#endif // RECORDER_H
