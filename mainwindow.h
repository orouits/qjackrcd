#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QIcon>
#include <QtGui/QMainWindow>
#include "recorder.h"
#include "pprocessor.h"

#define REFRESHOKMS 200
#define REFRESHERRMS 1000

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Recorder *recorder;
    PProcessor *pprocessor;
    QTimer *timer;
    QIcon *iconGreen;
    QIcon *iconRed;
    QIcon *iconOrange;

private slots:
    void on_pauseLevelSpin_valueChanged(double );
    void on_pauseDelaySpin_valueChanged(double );
    void on_recButton_clicked();
    void on_timer();
    void on_pauseSplitCheck_stateChanged(int );
    void on_postActionCombo_currentIndexChanged(int index);
    void on_postCmdEdit_textChanged(QString );
};

#endif // MAINWINDOW_H
