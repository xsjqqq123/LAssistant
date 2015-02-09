#include "phoneinforectangle.h"

PhoneInfoRectangle::PhoneInfoRectangle(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    headerLbl = new QLabel("手机信息",this);
    contentLbl = new QLabel(this);

    headerLbl->setMaximumHeight(30);
    contentLbl->setAlignment(Qt::AlignLeft);
    contentLbl->setMargin(10);
    mainLayout->addWidget(headerLbl);
    mainLayout->addWidget(contentLbl);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    headerLbl->setStyleSheet("border:1px solid rgba(10,10,10,50);background-color:rgba(10,10,10,30);border-top-left-radius: 5px;border-top-right-radius: 5px;");
    contentLbl->setStyleSheet("border:1px solid rgba(10,10,10,50);border-bottom-left-radius: 5px;border-bottom-right-radius: 5px;");

}

void PhoneInfoRectangle::setContentText(QString txt)
{
    contentLbl->setText(txt);
}
