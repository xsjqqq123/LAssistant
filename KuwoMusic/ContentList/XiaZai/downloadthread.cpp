#include "downloadthread.h"

DownloadThread::DownloadThread(QObject *parent,QString url,QString fileSavePath) : QThread(parent)
{
    this->downloadBytes = 0;
    this->totalBytes = 0;
    this->url = url;
    this->fileSavePath = fileSavePath;
}

DownloadThread::~DownloadThread()
{

}

void DownloadThread::run()
{
    QEventLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply_tmp;
    //获取文件realUrl
    int tryTime = 5;
    QString realUrl = this->url;
    while (tryTime--) {
        reply_tmp = manager.head(QNetworkRequest(realUrl));
        QObject::connect(reply_tmp,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        QTimer::singleShot(6*1000,&loop,SLOT(quit()));
        loop.exec();
        QString urlTmp = reply_tmp->rawHeader("Location");
        if(!urlTmp.isEmpty())
        {
            realUrl = urlTmp;
        }else
        {
            QVariant var = reply_tmp->header(QNetworkRequest::ContentLengthHeader);
            this->totalBytes = var.toLongLong();
            break;
        }
    }
    //    QList<QByteArray> list = reply_tmp->rawHeaderList();
    //    foreach (QByteArray a, list) {
    //        qDebug()<< a<<reply_tmp->rawHeader(a);
    //    }
    delete reply_tmp;
    reply = manager.get(QNetworkRequest(QUrl(realUrl)));
    connect(reply,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(finished()));
    exec();
}

void DownloadThread::readyRead()
{
    fileBuffer.open(QBuffer::Append);
    QByteArray data = reply->readAll();
    fileBuffer.write(data);
    fileBuffer.close();
    emit downloadProcess(fileBuffer.data().size(),this->totalBytes);
    //qDebug()<<fileBuffer.data().size()<<this->totalBytes;
}

void DownloadThread::finished()
{
    QFile file(this->fileSavePath);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug()<<this->fileSavePath<<" can not open!";
        return;
    }
    file.write(this->fileBuffer.data());
    emit downloadFinished();
    //qDebug()<<"finished";
    this->quit();
}

