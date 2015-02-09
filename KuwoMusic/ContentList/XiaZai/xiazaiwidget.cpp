#include "xiazaiwidget.h"

XiaZaiWidget::XiaZaiWidget(QWidget *parent) : QWidget(parent)
{
    downloadArea = new QScrollArea(this);
    mainGLayout = new QGridLayout(this);
    downloadArea->setWidgetResizable(true);
    downloadArea->setObjectName("downloadArea");
    mainGLayout->addWidget(downloadArea);
    downloadAreaWidget = new QWidget(this);
    downloadAreaWidget->setObjectName("transparentWidget");
    downloadAreaWidgetLayout = new QVBoxLayout(downloadAreaWidget);
    downloadAreaWidgetLayout->setAlignment(Qt::AlignTop);
    downloadArea->setWidget(downloadAreaWidget);

}

XiaZaiWidget::~XiaZaiWidget()
{

}

void XiaZaiWidget::addDownloadTask(QString id, QString fileName)
{
    QString url("http://antiserver.kuwo.cn/anti.s?type=convert_url&response=url&format=mp3&rid="+id);

    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QEventLoop loop;
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(6*1000,&loop,SLOT(quit()));
    loop.exec();
    url = reply->readAll();


    QString fileSavePath;
    if(QDir(QDir::homePath()+"/音乐").exists())
    {
        fileSavePath = QDir::homePath()+"/音乐/"+fileName+".mp3";
    }else if(QDir(QDir::homePath()+"/Music").exists())
    {
        fileSavePath = QDir::homePath()+"/Music/"+fileName+"mp3";
    }else
    {
        fileSavePath = QDir::homePath()+"/"+fileName+"mp3";
    }
    DownloadItem *d = new DownloadItem(0,url,fileSavePath);
    downloadAreaWidgetLayout->addWidget(d);

}

