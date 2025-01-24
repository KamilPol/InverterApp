#include "mainwindow.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->UARTdevice = new QSerialPort;
    logs = ui->logsTextEdit;


    // plot test

    chart->legend()->hide();
    paramsSeries.append(new QLineSeries());
    paramsSeries.append(new QLineSeries());
    chart->addSeries(paramsSeries[0]);
    chart->addSeries(paramsSeries[1]);
    chart->createDefaultAxes();
    // chart->axes(Qt::Vertical).first()->setRange(0,20);
    // chart->axes(Qt::Horizontal).first()->setRange(0,1500);
    chart->setVisible(true);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setVisible(true);
    //setCentralWidget(chartView);
    ui->graphLayout->addWidget(chartView);
    ///

    totalParamLayouts=0;

    printBauds();
    scanPorts();
    values.resize(501);
    valuesParam1Float.resize(501);

}

MainWindow::~MainWindow()
{
    delete ui;
    this->UARTdevice->close();
    for (int i = 0; i < paramsSeries.count(); ++i) {
         delete paramsSeries[i];
    }
    for (int i=0; i<createdLayoutsPtrs.count(); i++)
    {
        delete createdLayoutsPtrs [i];
    }

}

void MainWindow::printBauds()
{
    QList<QString> bauds;
    bauds.append("500000");
    bauds.append("115200");
    bauds.append("9600");
    for(int i = 0; i<bauds.count(); i++ )
    {
        ui->baudBox->addItem(bauds.at(i));
    }
}
void MainWindow::scanPorts()
{
    QList<QSerialPortInfo> devices = QSerialPortInfo::availablePorts();
    ui->devicesBox->clear();
    for (int i=0; i <  devices.length(); i++)
    {
        qDebug()<<devices.at(i).portName()<<devices.at(i).description();
        ui->devicesBox->addItem(devices.at(i).portName() + " " + devices.at(i).description());
        // ui->logsTextEdit->append(devices.at(i).portName());
    }
}

void MainWindow::on_scanButton_clicked()
{
    scanPorts();
}

void MainWindow::on_connectButton_clicked()
{
    if(ui->devicesBox->count() == 0) {
        logs->append("No available devices to connect!");
        return;
    }

    QString portName = ui->devicesBox->currentText().split(" ").first();
    qDebug()<<portName;
    this->UARTdevice->setPortName(portName);

    if(UARTdevice->open(QSerialPort::ReadWrite)) {
        this->UARTdevice->setBaudRate(ui->baudBox->currentText().toInt());
        this->UARTdevice->setDataBits(QSerialPort::Data8);
        this->UARTdevice->setParity(QSerialPort::NoParity);
        this->UARTdevice->setStopBits(QSerialPort::OneStop);
        this->UARTdevice->setFlowControl(QSerialPort::NoFlowControl);
        logs->append("Otwarto port szeregowy " + portName + "!\n");
        //QThread::msleep(10);
        connect(this->UARTdevice, SIGNAL(readyRead()), this, SLOT(readFromPort()));
    } else {
        logs->append("Otwarcie portu szeregowego się nie powiodło!");
    }
}

void MainWindow::on_exitButton_clicked()
{
    if(this->UARTdevice->isOpen())
    {
        this->UARTdevice->close();
        qDebug()<<"Zamknięto połączenie.";
    } else
    {
        qDebug()<<"Port nie jest otwarty!";
        return;
    }
}

void MainWindow::readFromPort()
{
    //this->UARTdevice->flush();

    while(this->UARTdevice->canReadLine())
    {
        QString line = this->UARTdevice->readLine();
        QList<QString> parsedLine;
        QList<QString> names;

        logs->insertPlainText(line);
        if ((ui->autoScroll->checkState()))
        {
            QTextCursor logsTextCursor = ui->logsTextEdit->textCursor();
            logsTextCursor.movePosition(QTextCursor::End);
            logs->setTextCursor(logsTextCursor);
        }
        int activeTabIndex = ui->tabWidget->currentIndex();
        line = line.remove('\n');
        parsedLine = line.split(",");

        if ((parsedLine.count() == parametersCount) || (parametersCount == 0))
        {
            values[currentParameterListIndex].clear();

            for (int i = 0; i < parsedLine.count(); i++)
            {
                names.append(parsedLine.at(i).split(":").first());
                values[currentParameterListIndex].append(parsedLine.at(i).split(":").last());
                //qDebug() << values.count();

                if (activeTabIndex == 1)
                {
                    if (totalParamLayouts<parsedLine.count())
                    {
                        createdLayoutsPtrs.append(addNewParamLayout());
                        qDebug()<<"New parameter added to GUI" << names[i];
                        parametersCount = parsedLine.count();
                    }
                    createdLayoutsPtrs.at(i)->setName(names.at(i));
                    createdLayoutsPtrs.at(i)->setValue(values[currentParameterListIndex].at(i));
                }
            }
            if (activeTabIndex == 1)
            {
                // if (!(values.at(currentParameterListIndex).at(2).isEmpty()))
                // {
                valuesParam1Float[currentParameterListIndex]=values[currentParameterListIndex][2].toFloat();
               // }
                auto minIt = std::min_element(valuesParam1Float.begin(), valuesParam1Float.end());
                float minValue = (minIt != valuesParam1Float.end()) ? *minIt : 0.0;

                // Znalezienie maksymalnej wartości
                auto maxIt = std::max_element(valuesParam1Float.begin(), valuesParam1Float.end());
                float maxValue = (maxIt != valuesParam1Float.end()) ? *maxIt : 0.0;

                // Wyświetlenie wyników
               // qDebug() << valuesParam1Float;
                //qDebug() << "Minimalna wartość:" << minValue;
               // qDebug() << "Maksymalna wartość:" << maxValue;
                //series->append(graphXaxis,values[0].toFloat());

                // chart->axisX()->setRange(graphXaxis-500<0? 0 : graphXaxis-500,graphXaxis);
                // chart->axisY()->setRange(0, 10);
                chart->axes(Qt::Vertical).first()->setRange(minValue - minValue*0.1  , maxValue+maxValue*0.1);
                chart->axes(Qt::Horizontal).first()->setRange(values[currentParameterListIndex][0].toFloat()-10000<0? 0 : values[currentParameterListIndex][0].toFloat()-10000, values[currentParameterListIndex][0].toFloat()+20);
                // chart->axes(Qt::Vertical).first()->setRange(0,20);
                // chart->axes(Qt::Horizontal).first()->setRange(0, graphXaxis);
                paramsSeries[0]->append(values[currentParameterListIndex][0].toFloat(),values[currentParameterListIndex][2].toFloat());
                //paramsSeries[1]->append(values[currentParameterListIndex][0].toFloat(),values[currentParameterListIndex][3].toFloat());

                qDebug() << "i" << graphXaxis;
                qDebug() << "val" <<paramsSeries[0]->count();

                if (paramsSeries[0]->at(0).x() < values[currentParameterListIndex][0].toFloat()-11000)
                {
                    paramsSeries[0]->removePoints(0,1);
                    graphXaxis++;
                }
                // // {
                // if
                //

                    // paramsSeries[1]->remove(0,paramsSeries.count()-50);
                   // qDebug() << values[currentParameterListIndex][0].toFloat()-1000;
                // }
                currentParameterListIndex < 500 ? currentParameterListIndex++ : currentParameterListIndex =0;
                //qDebug() << valuesFloat;
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    logs->clear();
}

void MainWindow::on_sendButton_clicked()
{
    if(this->UARTdevice->isOpen())
    {
        // QString myString = ui->sendText->text();
        // char text [256];

        // for(int i = 0; i <myString.length(); i++)
        // text[i] = static_cast<char>(myString.at(i));

        QString myString = ui->sendText->text();
        QByteArray data = myString.toLocal8Bit() + "\n";
        qDebug() << data;
        UARTdevice->write(data);
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    if(this->UARTdevice->isOpen())
    {
        this->UARTdevice->close();
        logs->append ("Zamknięto połączenie.");
    }
}

void MainWindow::on_iQPushButton_clicked()
{
    QString myString = ui->iQLineEdit->text();
    QByteArray data = "q" + myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_iDPushButton_clicked()
{
    QString myString = ui->iDLineEdit->text();
    QByteArray data = "d" + myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_setSpeedPushButton_clicked()
{
    QString myString = ui->SpeedLineEdit->text();
    QByteArray data = "v" + myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_ONpushButton_clicked()
{
    QString myString = ui->SpeedLineEdit->text();
    QByteArray data = "1\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_OFFpushButton_clicked()
{
    QString myString = ui->SpeedLineEdit->text();
    QByteArray data = "0\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_sendText_returnPressed()
{
    QString myString = ui->sendText->text();
    QByteArray data = myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_SpeedLineEdit_returnPressed()
{
    QString myString = ui->SpeedLineEdit->text();
    QByteArray data = "v" + myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_iQLineEdit_returnPressed()
{
    QString myString = ui->iQLineEdit->text();
    QByteArray data = "q" + myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);
}

void MainWindow::on_iDLineEdit_returnPressed()
{
    QString myString = ui->iDLineEdit->text();
    QByteArray data = "d" + myString.toLocal8Bit() + "\n";
    qDebug() << data;
    UARTdevice->write(data);

}

SerialParamsLayout* MainWindow::addNewParamLayout(QString ParamName, QString ParamValue)
{
    SerialParamsLayout *newParamLayout = new SerialParamsLayout(this);
    ui->SerialParametersLayout->addWidget(newParamLayout);

    newParamLayout->setAttribute(Qt::WA_DeleteOnClose,true);
    newParamLayout->layoutNumber = totalParamLayouts;
    newParamLayout->setName(ParamName);
    newParamLayout->setValue(ParamValue);
    SerialParamsPtr.append(newParamLayout);
    totalParamLayouts++;
    return newParamLayout;
}


void MainWindow::on_pushButton_2_clicked()
{

}

