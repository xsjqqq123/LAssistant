#include "headwidget.h"

HeadWidget::HeadWidget(QWidget *parent) : QWidget(parent)
{
    logoLbl = new QLabel(this);
    mainHboxLayout = new QHBoxLayout(this);
    mainHboxLayout->addWidget(logoLbl);

    logoLbl->setPixmap(QPixmap(":/kuwoMusic/KuwoMusic/imgs/logo.png"));
    //this->setStyleSheet("background-image: url(:/kuwoMusic/KuwoMusic/imgs/head_bg.jpg);background-repeat:no-repeat");

    setMaximumHeight(80);
}

HeadWidget::~HeadWidget()
{

}

