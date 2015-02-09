#include "paihangbangwidget.h"
#include <QDebug>
PaiHangBangWidget::PaiHangBangWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
    leftNaviList = new QListWidget(this);
    musicListWidget = new MusicListWidget(this,false);
    //mainLayout->setAlignment(Qt::AlignTop);
    leftNaviList->setObjectName("PaiHangBangWidgetLeftNaviList");
    leftNaviList->setFixedWidth(60);
    leftNaviList->setGridSize(QSize(55,35));
    leftNaviList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    leftNaviList->setMinimumHeight(400);
    //leftNaviList->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    leftNaviList->addItems(QStringList()<<"新歌榜"<<"热歌榜"<<"飙升榜"<<"真声音"<<"欧美榜"<<"华语榜"<<"日韩榜"<<"先锋榜");
    leftNaviList->setCurrentRow(0);

    mainLayout->addWidget(leftNaviList,1,0,1,1);
    mainLayout->addWidget(musicListWidget,0,1,2,1);

    //mainLayout->addItem(new QSpacerItem(30,30,QSizePolicy::Maximum,QSizePolicy::Maximum),6,0,1,4);


    connect(leftNaviList,SIGNAL(currentRowChanged(int)),this,SLOT(loadData(int)));
    QTimer::singleShot(3000,this,SLOT(loadData()));



}

PaiHangBangWidget::~PaiHangBangWidget()
{

}


void PaiHangBangWidget::loadData(int row)
{
    musicListWidget->setIndicatorVisible(true);
    QString url("http://kbangserver.kuwo.cn/ksong.s?from=pc&fmt=json&type=bang&data=content&pn=0&rn=100");
    url.append("&"+QString::number(QDateTime::currentMSecsSinceEpoch()));
    switch (row) {
    case 0://"新歌榜"<<"热歌榜"<<"飙升榜"<<"真声音"<<"欧美榜"<<"华语榜"<<"日韩榜"<<"先锋榜"
        url.append("&id=17");
        break;
    case 1:
        url.append("&id=16");
        break;
    case 2:
        url.append("&id=93");
        break;
    case 3:
        url.append("&id=106");
        break;
    case 4:
        url.append("&id=22");
        break;
    case 5:
        url.append("&id=62");
        break;
    case 6:
        url.append("&id=23");
        break;
    case 7:
        url.append("&id=104");
        break;

    default:
        url.append("&id=17");
        break;
    }
    QNetworkAccessManager *manager = new QNetworkAccessManager;

    QEventLoop loop;
    QTimer::singleShot(6*1000,&loop,SLOT(quit()));
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();

    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(data,error);
    musicListWidget->clearAllItem();
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj=doc.object();
        QJsonArray musiclistArray = rootObj["musiclist"].toArray();
        musicListWidget->setTitleText("共100首歌曲");
        int row = 1;
        foreach (QJsonValue v, musiclistArray) {
            QJsonObject singleMusicObj = v.toObject();
            QString id = singleMusicObj["id"].toString();
            QString name = singleMusicObj["name"].toString();
            QString artist = singleMusicObj["artist"].toString();

            musicListWidget->addItem("MUSIC_"+id,QString::number(row),name,artist);
            row++;
        }
    }else
    {
        qDebug()<<"PaiHangBangWidget::loadData"<<error->errorString();
    }
    musicListWidget->setIndicatorVisible(false);
}
