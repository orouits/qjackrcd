#include "mainconsole.h"
#include <QCoreApplication>

MainConsole::MainConsole(Recorder *recorder, QObject *parent) : QObject(parent), qout(stdout), qin(stdin), qinNotifier(fileno(stdin), QSocketNotifier::Read, this)
{
    // the recorder given by the application.
    this->recorder = recorder;
    application = QCoreApplication::instance();

    qout << application->applicationName() << " " << tr("running in console mode") << endl;
    qout << tr("Congiguration:") << endl;
    qout << "pauseLevel(DB)" << "\t" << recorder->getPauseLevel() << endl;
    qout << "pauseDelay(sec)" << "\t" << recorder->getPauseActivationDelay() << endl;
    qout << "splitMode" << "\t" << toBoolText(recorder->isSplitMode()) << endl;
    qout << "recordAtLaunch" << "\t" << toBoolText(recorder->isRecordAtLaunch()) << endl;
    qout << "connections1" << "\t" << recorder->getJackConnections1() << endl;
    qout << "connections2" << "\t" << recorder->getJackConnections2() << endl;
    qout << "outputDir" << "\t" << recorder->getOutputDir().absolutePath() << endl;
    qout << "processCmdLine" << "\t" << recorder->getProcessCmdLine() << endl;
    qout << "jackAutoMode" << "\t" << toBoolText(recorder->isJackAutoMode()) << endl;
    qout << "jackTransMode" << "\t" << toBoolText(recorder->isJackTransMode()) << endl;

    connect(recorder, SIGNAL(statusChanged()), this, SLOT(onRecorderStatusChanged()));
    connect(&qinNotifier, SIGNAL(activated(int)), this, SLOT(onInput()));
    connect(this, SIGNAL(quit()), application, SLOT(quit()));
}


MainConsole::~MainConsole()
{

}

void MainConsole::onRecorderStatusChanged()
{
    qout << "\r";

    if (recorder->isRecording()) {
        if (recorder->isPaused()) {
            qout << tr("Waiting for sound...");
        }
        else {
            qout << tr("Recording...");
        }

    }
    else if (!recorder->isRecordEnabled()) {
        qout << tr("Disabled");
    }
    else {
        qout << tr("Ready");
    }
    qout << " ";
    qout << recorder->getCurrentRecordSize()/1024 << "KB - ";
    qout << recorder->getTotalRecordSize()/1024 << "KB ";

    qout << "> " << toGraphText((recorder->getLeftLevel() + recorder->getRightLevel()) / 2, recorder->getPauseLevel());
    qout.flush();
}
void MainConsole::onInput()
{
    qout << tr("Quit") << endl;
    emit quit();
}

QString MainConsole::toGraphText(float level, float fixedLevel)
{
    QString str;
    int min = -40;
    int max = 3;

    for (int i = min; i<=max; i++)
    {
        if (i == (int)fixedLevel) str.append("|");
        else if (i <= level) str.append("#");
        else str.append(".");
    }
    return str;
}
