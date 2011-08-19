#ifndef PPROCESSOR_H
#define PPROCESSOR_H

#include <QString>

class PProcessor
{

    QString cmdLine;
    QString fileName;
    QString launchedFileName;

public:
    PProcessor();
    ~PProcessor();
    void launchFileProcess();

    QString& getCmdLine() {return cmdLine;}
    void setCmdLine(QString& line) {cmdLine = line;}
    QString& getFileName() {return fileName;}
    void setFileName(QString& fname) {fileName = fname;}
    QString& getLaunchedFileName() {return launchedFileName;}
};

#endif // PPROCESSOR_H
