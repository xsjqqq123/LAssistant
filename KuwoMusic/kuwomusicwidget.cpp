#include "kuwomusicwidget.h"
#include <QDebug>
KuwoMusicWidget::KuwoMusicWidget(QWidget *parent) : QWidget(parent)
{
    mainGLayout = new QGridLayout(this);
    headWidget = new HeadWidget(this);
    playerWidget = new PlayerWidget(this);
    contentListWidget = new ContentTabWidget(this);

    mainGLayout->setAlignment(Qt::AlignTop);
    mainGLayout->addWidget(headWidget,0,0,1,2);
    mainGLayout->addWidget(playerWidget,0,2,1,3);
    mainGLayout->addWidget(contentListWidget,1,0,5,5);
    connect(contentListWidget->paiHangBangWidget->musicListWidget,SIGNAL(playSignal(QString,QString)),playerWidget,SLOT(playKuwoMusic(QString,QString)));
    connect(contentListWidget->souSuoWidget->musicListWidget,SIGNAL(playSignal(QString,QString)),playerWidget,SLOT(playKuwoMusic(QString,QString)));
}

KuwoMusicWidget::~KuwoMusicWidget()
{

}

