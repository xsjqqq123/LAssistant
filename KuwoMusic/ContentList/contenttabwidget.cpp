#include "contenttabwidget.h"

ContentTabWidget::ContentTabWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
    navigateBar = new NavigateBar(this);
    tabWidget = new QStackedWidget;
    splitterLine = new QFrame(this);

    splitterLine->setFixedHeight(2);
    splitterLine->setStyleSheet("background-color:#69BAF9;");
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(navigateBar,0,0,1,5);
    mainLayout->addWidget(splitterLine,1,0,1,5);
    mainLayout->addWidget(tabWidget,2,0,5,5);

    paiHangBangWidget = new PaiHangBangWidget(this);
    souSuoWidget = new SouSuoWidget(this);
    xiaZaiWidget = new XiaZaiWidget(this);

    tabWidget->addWidget(paiHangBangWidget);
    tabWidget->addWidget(souSuoWidget);
    tabWidget->addWidget(xiaZaiWidget);
    //setStyleSheet("#navigateBar{border-bottom:2px solid #46B4E6}");
    connect(navigateBar,SIGNAL(tabChangeSignal(QString)),this,SLOT(switchTab(QString)));
    connect(paiHangBangWidget->musicListWidget,SIGNAL(downloadSignal(QString,QString)),xiaZaiWidget,SLOT(addDownloadTask(QString,QString)));
    connect(souSuoWidget->musicListWidget,SIGNAL(downloadSignal(QString,QString)),xiaZaiWidget,SLOT(addDownloadTask(QString,QString)));

}

ContentTabWidget::~ContentTabWidget()
{

    //qDebug()<<souSuoWidget->width()<<souSuoWidget->height();
}

void ContentTabWidget::switchTab(QString link)
{
    if(link.contains("paiHangBang"))
    {
        tabWidget->setCurrentIndex(0);
    }else if(link.contains("souSuo"))
    {
        tabWidget->setCurrentIndex(1);
    }else if(link.contains("xiazai"))
    {
        tabWidget->setCurrentIndex(2);
    }
}

