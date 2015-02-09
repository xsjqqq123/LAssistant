#include "loadimgthread.h"
#include <QEventLoop>
#include <QTimer>
LoadImgThread::LoadImgThread(QObject *parent, QString url) :
    QThread(parent)
{
    this->url=url;
}

void LoadImgThread::run()
{
    manager = new QNetworkAccessManager;
    QByteArray data;
    int tryTimes=5;
    //尝试tryTimes次
    QString realUrl_temp = this->url;
    while(tryTimes --)
    {
        QEventLoop loop;
        QNetworkReply *reply = manager->head(QNetworkRequest(realUrl_temp));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QVariant varLocation = reply->header(QNetworkRequest::LocationHeader);
        //qDebug()<<reply->rawHeaderList();
        if(!varLocation.isNull())
        {
            realUrl_temp = varLocation.toString();
            //qDebug()<<realUrl_temp;
            continue;
        }
        delete reply;
        break;
    }
    this->url = realUrl_temp;
    QNetworkReply *reply1 = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop1;
    QTimer::singleShot(20000,&loop1,SLOT(quit()));
    QObject::connect(reply1, SIGNAL(finished()), &loop1, SLOT(quit()));
    loop1.exec();
    data = reply1->readAll();
    emit imageReady(data);

    delete reply1;
    delete manager;
}
