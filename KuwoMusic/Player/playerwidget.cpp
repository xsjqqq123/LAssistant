#include "playerwidget.h"
#include <QDebug>
PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent)
{
    //setMaximumHeight(80);
    player = new QMediaPlayer(this);
    mainLayout = new QGridLayout(this);
    headLbl = new QLabel;
    durationLbl = new QLabel;
    songNameLbl = new QLabel;
    playBtn = new QPushButton;
    nextBtn = new QPushButton;
    progressBar = new QSlider;//

    mainLayout->setMargin(2);
    mainLayout->setSpacing(2);
    headLbl->setFixedSize(100,100);
    playBtn->setFocusPolicy(Qt::NoFocus);
    playBtn->setFixedSize(50,50);
    nextBtn->setFixedSize(50,50);
    playBtn->setFlat(true);
    nextBtn->setFlat(true);
    playBtn->setIconSize(QSize(50,50));
    nextBtn->setIconSize(QSize(50,50));
    playBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/play-pause.png"));
    nextBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/next.png"));

    //songNameLbl->setWordWrap(true);
    songNameLbl->setMaximumHeight(35);
    //songNameLbl->setFixedSize(70,30);
    progressBar->setOrientation(Qt::Horizontal);
    progressBar->setRange(0,1000);


    //mainLayout->addWidget(headLbl,0,0,3,3);
    mainLayout->addWidget(progressBar,0,0,1,7);
    mainLayout->addWidget(songNameLbl,1,0,1,5);
    mainLayout->addWidget(durationLbl,2,0,1,5);
    //mainLayout->addWidget(playBtn,1,5,1,1);
    mainLayout->addWidget(playBtn,1,6,2,2);
    //mainLayout->addWidget(nextBtn,1,6,1,1);


    resetToDefault();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(getPlayInfo()));
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(handlePlayerStateChanged(QMediaPlayer::State)));
    connect(playBtn,SIGNAL(clicked()),this,SLOT(playBtnClick()));
    connect(progressBar,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
}

PlayerWidget::~PlayerWidget()
{

}

void PlayerWidget::resetToDefault()
{
    songNameLbl->setText("");
    durationLbl->setText("00:00|00:00");
    progressBar->setValue(0);
}

void PlayerWidget::play(QString url)
{
    player->setMedia(QUrl(url));
    player->play();
}

void PlayerWidget::playKuwoMusic(QString id,QString songName)
{
    timer->stop();
    QString url("http://antiserver.kuwo.cn/anti.s?type=convert_url&response=url&format=mp3&rid="+id);

    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QEventLoop loop;
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(6*1000,&loop,SLOT(quit()));
    loop.exec();
    QString data = reply->readAll();
    this->play(data);

    QFont font(songNameLbl->font());
    QFontMetrics fm(font);
    songNameLbl->setText(fm.elidedText(songName, Qt::ElideRight, songNameLbl->width()));
    timer->start(1000);
}

void PlayerWidget::getPlayInfo()
{
    qint64 duration = player->duration();
    qint64 position = player->position();
    int percent = position*1000/duration;
    progressBar->setValue(percent);

    QTime durationTime(0,0,0,0);
    durationTime = durationTime.addMSecs(duration);
    QString t1 = durationTime.toString("mm:ss");

    QTime positionTime(0,0,0,0);
    positionTime = positionTime.addMSecs(position);
    QString t2 = positionTime.toString("mm:ss");

    durationLbl->setText(t2+"|"+t1);
}

void PlayerWidget::handlePlayerStateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        playBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/play-start.png"));
    }else if(state == QMediaPlayer::PausedState)
    {
        playBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/play-pause.png"));
    }else if(state == QMediaPlayer::StoppedState)
    {
        playBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/play-pause.png"));
        resetToDefault();
    }
}

void PlayerWidget::playBtnClick()
{
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
    }else if(player->state() == QMediaPlayer::PausedState)
    {
        player->play();
    }
}

void PlayerWidget::seek(int p)
{
    player->setPosition(p*player->duration()/1000);
}

