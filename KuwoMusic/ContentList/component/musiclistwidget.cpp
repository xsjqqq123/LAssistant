#include "musiclistwidget.h"
#include <QDebug>
MusicListWidget::MusicListWidget(QWidget *parent, bool isPageBarVisible) : QWidget(parent)
{
    mainGLayout = new QGridLayout(this);
    titleLbl = new QLabel(this);
    listArea = new QScrollArea(this);
    listAreaWidget = new QWidget(listArea);
    listAreaWidget->setObjectName("transparentWidget");
    listAreaWidgetLayout = new QVBoxLayout(listAreaWidget);

    listArea->setObjectName("MusicListWidgetListArea");
    listAreaWidgetLayout->setSpacing(1);
    listAreaWidgetLayout->setMargin(2);
    //listAreaWidget->setMinimumHeight(400);
    listArea->setWidget(listAreaWidget);
    listArea->setWidgetResizable(true);

    titleLbl->setFixedHeight(40);
    busyIndicator = new QLabel(this);
    busyIndicator->setFixedSize(40,40);
    busyIndicator->setAlignment(Qt::AlignRight);
    QMovie *movie = new QMovie(":/app/skin/imgs/loading.gif");
    busyIndicator->setMovie(movie);
    movie->start();
    busyIndicator->setVisible(false);
    mainGLayout->addWidget(titleLbl,0,0,1,4);
    mainGLayout->addWidget(busyIndicator,0,4,1,1);
    mainGLayout->addWidget(listArea,1,0,5,5);
    if(isPageBarVisible)
    {
        pageNumBar = new PageNumBar(this);
        pageNumBar->setMaximumHeight(35);
        mainGLayout->addWidget(pageNumBar,6,0,1,5);
        connect(pageNumBar,SIGNAL(pageChange(QString)),this,SLOT(emitPageChange(QString)));
    }



}

MusicListWidget::~MusicListWidget()
{

}

void MusicListWidget::setIndicatorVisible(bool b)
{
    busyIndicator->setVisible(b);
}

void MusicListWidget::setPageInfo(QString currentPageNum, QString totalPageNum)
{
    if(pageNumBar != Q_NULLPTR)
        pageNumBar->set(currentPageNum, totalPageNum);
}

void MusicListWidget::addItem(QString songId, QString lblId, QString songName, QString songSinger)
{

    MusicListWidgetItem *item = new MusicListWidgetItem(0,songId,lblId,songName,songSinger);
    connect(item,SIGNAL(playSignal(QString,QString)),this,SLOT(emitPlay(QString,QString)));
    connect(item,SIGNAL(downloadSignal(QString,QString)),this,SLOT(emitDownload(QString,QString)));
    listAreaWidgetLayout->addWidget(item);
}

void MusicListWidget::setTitleText(QString text)
{
    titleLbl->setText(text);
}

void MusicListWidget::clearAllItem()
{
    while (listAreaWidgetLayout->count()>0) {
        QWidget *w = listAreaWidgetLayout->itemAt(0)->widget();
        listAreaWidgetLayout->removeWidget(w);
        delete w;
    }
    listAreaWidget->resize(10,10);
}

void MusicListWidget::emitPageChange(QString pageNum)
{
    emit pageChange(pageNum);
}

void MusicListWidget::emitPlay(QString songId,QString songName)
{
    emit playSignal(songId,songName);
}

void MusicListWidget::emitDownload(QString songId,QString songName)
{
    emit downloadSignal(songId,songName);
}



MusicListWidgetItem::MusicListWidgetItem(QWidget *parent, QString songId, QString lblId, QString songName, QString songSinger)
{
    this->songId = songId;
    this->songName = songName;
    this->songSinger = songSinger;
    hboxLayout = new QHBoxLayout(this);
    idLbl = new QLabel(lblId,this);
    songNameLbl = new QLabel(songName,this);
    songSingerLbl = new QLabel(songSinger,this);
    songPlayBtn = new QPushButton(this);
    songDownloadBtn = new QPushButton(this);
    songPlayBtn->setFlat(true);
    songDownloadBtn->setFlat(true);
    songPlayBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/play.png"));
    songDownloadBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/download.png"));
    songPlayBtn->setIconSize(QSize(30,30));
    songDownloadBtn->setIconSize(QSize(30,30));

    idLbl->setFixedSize(40,30);
    //songNameLbl->setFixedSize(150,30);
    songNameLbl->setWordWrap(true);
    songSingerLbl->setWordWrap(true);
    //songNameLbl->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
    songSingerLbl->setFixedSize(100,35);
    songPlayBtn->setFixedSize(30,30);
    songDownloadBtn->setFixedSize(30,30);

    hboxLayout->addWidget(idLbl);
    hboxLayout->addWidget(songNameLbl);
    hboxLayout->addWidget(songSingerLbl);
    hboxLayout->addWidget(songPlayBtn);
    hboxLayout->addWidget(songDownloadBtn);

    connect(songPlayBtn,SIGNAL(clicked()),this,SLOT(emitPlay()));
    connect(songDownloadBtn,SIGNAL(clicked()),this,SLOT(emitDownload()));
}

MusicListWidgetItem::~MusicListWidgetItem()
{

}

void MusicListWidgetItem::emitPlay()
{
    emit playSignal(this->songId,this->songName);
}

void MusicListWidgetItem::emitDownload()
{
    songDownloadBtn->setEnabled(false);
    emit downloadSignal(this->songId,this->songName);
}
