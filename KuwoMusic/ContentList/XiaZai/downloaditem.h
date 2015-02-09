#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H

#include <QWidget>
#include "downloadthread.h"
namespace Ui {
class DownloadItem;
}

class DownloadItem : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadItem(QWidget *parent = 0,QString url="",QString fileSavePath="");
    ~DownloadItem();
signals:
    void deleteSiganl();
public slots:
    void downloadProcess(qint64 downloadBytes,qint64 totalBytes);
    void downloadFinished();
    void emitDeleteSignal();
private:
    Ui::DownloadItem *ui;
    DownloadThread *dThread;
};

#endif // DOWNLOADITEM_H
