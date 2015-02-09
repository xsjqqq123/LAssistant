#include "myhoverbutton.h"

MyhoverButton::MyhoverButton(QWidget *parent)
{
}

MyhoverButton::~MyhoverButton()
{

}

void MyhoverButton::flashBackgroud()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(0.2);
    animation->setKeyValueAt(0.5,1);
    animation->setEndValue(0.2);

    animation->start();
}

void MyhoverButton::setData(QString data)
{
    this->usrDataStr = data;
}

QString MyhoverButton::getData()
{
    return this->usrDataStr;
}

void MyhoverButton::enterEvent(QEvent *event)
{
    emit mouseEnterSiganl();
}

