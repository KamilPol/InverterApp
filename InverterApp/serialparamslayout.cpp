#include "serialparamslayout.h"
#include "ui_serialparamslayout.h"

SerialParamsLayout::SerialParamsLayout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialParamsLayout)
{
    ui->setupUi(this);
}

SerialParamsLayout::~SerialParamsLayout()
{
    delete ui;
}

void SerialParamsLayout::setName(QString name)
{
    ui->nameParam->setText(name);
}
void SerialParamsLayout::setValue(QString value)
{
    ui->valueParam->setText(value);
}
