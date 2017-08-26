#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include <QObject>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include "recorder.h"


class MainConsole : public QObject
{
    Q_OBJECT

    //QTextStream qout(stdout);
    Recorder* recorder;

public:
    explicit MainConsole(QObject *parent = 0);
    ~MainConsole();
signals:

private slots:
    void onRecorderStatusChanged();

};

#endif // MAINCONSOLE_H
