#ifndef SERIALPARAMSLAYOUT_H
#define SERIALPARAMSLAYOUT_H

#include <QWidget>

namespace Ui {
class SerialParamsLayout;
}

class SerialParamsLayout : public QWidget
{
    Q_OBJECT

public:
    explicit SerialParamsLayout(QWidget *parent = nullptr);
    ~SerialParamsLayout();
    int layoutNumber;
    void setName(QString name);
    void setValue(QString value);


private:
    Ui::SerialParamsLayout *ui;
};

#endif // SERIALPARAMSLAYOUT_H
