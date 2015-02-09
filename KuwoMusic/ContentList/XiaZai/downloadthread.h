#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QBuffer>
#include <QFile>
#include <QTimer>
#include <QDebug>
class DownloadThread : public QThread
{
    Q_OBJECT
public:
    explicit DownloadThread(QObject *parent = 0,QString url="",QString fileSavePath="");
    ~DownloadThread();
protected:
    void run();
signals:
    void downloadProcess(qint64 downloadBytes,qint64 totalBytes);
    void downloadFinished();
public slots:
    void readyRead();
    void finished();
private:
    QString url;
    QString fileSavePath;
    qint64 downloadBytes;
    qint64 totalBytes;
    QNetworkReply *reply;
    QBuffer fileBuffer;
};
#endif // DOWNLOADTHREAD_H
