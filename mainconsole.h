#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include <QObject>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QSocketNotifier>
#include "recorder.h"


class MainConsole : public QObject
{
    Q_OBJECT

    QCoreApplication *application;
    Recorder* recorder;
    QTextStream qout;
    QTextStream qin;
    QSocketNotifier qinNotifier;


    QString toGraphText(float level, float fixedLevel);
    QString toBoolText(bool value) {return value ? "true" : "false";}

public:
    explicit MainConsole(Recorder *recorder, QObject *parent = 0);
    ~MainConsole();
signals:
    void quit();

private slots:
    void onRecorderStatusChanged();
    void onInput();

};

#endif // MAINCONSOLE_H
