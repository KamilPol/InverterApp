#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "ui_mainwindow.h"
#include "serialparamslayout.h"
#include <QVector>
#include <QtCharts/QtCharts>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int totalParamLayouts;
    QVector<SerialParamsLayout*> SerialParamsPtr;

private slots:
    void on_scanButton_clicked();

    void on_connectButton_clicked();

    void on_exitButton_clicked();

    void readFromPort();

    void on_pushButton_clicked();

    void on_sendButton_clicked();

    void on_disconnectButton_clicked();

    void on_iQPushButton_clicked();

    void on_iDPushButton_clicked();

    void on_setSpeedPushButton_clicked();

    void on_ONpushButton_clicked();

    void on_OFFpushButton_clicked();

    void on_sendText_returnPressed();

    void on_SpeedLineEdit_returnPressed();

    void on_iQLineEdit_returnPressed();

    void on_iDLineEdit_returnPressed();

    SerialParamsLayout* addNewParamLayout(QString ParamName="name", QString ParamValue= "value");

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *UARTdevice;
    QTextEdit *logs;

    void printBauds();
    void scanPorts();
    QList <SerialParamsLayout*> createdLayoutsPtrs;
    QList <QLineSeries*> paramsSeries;
    QChart* chart = new QChart();
    QList<QList<QString>> values;
    QList<float> valuesParam1Float;

    int parametersCount =0;
    int graphXaxis =0;
    int currentParameterListIndex = 0;
};
#endif // MAINWINDOW_H
