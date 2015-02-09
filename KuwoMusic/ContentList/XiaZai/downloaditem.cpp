#include "downloaditem.h"
#include "ui_downloaditem.h"

DownloadItem::DownloadItem(QWidget *parent, QString url, QString fileSavePath) :
    QWidget(parent),
    ui(new Ui::DownloadItem)
{
    ui->setupUi(this);
    dThread = new DownloadThread(this,url,fileSavePath);
    connect(dThread,SIGNAL(downloadProcess(qint64,qint64)),this,SLOT(downloadProcess(qint64,qint64)));
    connect(dThread,SIGNAL(finished()),this,SLOT(downloadFinished()));
    dThread->start();

    ui->fileNamelbl->setText(fileSavePath);
    ui->deleteBtn->setFlat(true);
    ui->deleteBtn->setIconSize(QSize(30,30));
    ui->deleteBtn->setIcon(QIcon(":/app/skin/imgs/delete.png"));
    ui->progressBar->setObjectName("downloadBar");
    connect(ui->deleteBtn,SIGNAL(clicked()),SLOT(emitDeleteSignal()));
}

DownloadItem::~DownloadItem()
{
    delete ui;
}

void DownloadItem::downloadProcess(qint64 downloadBytes, qint64 totalBytes)
{
    int v = downloadBytes*100/totalBytes;
    ui->progressBar->setValue(v);
}

void DownloadItem::downloadFinished()
{
    ui->progressBar->setValue(100);
}

void DownloadItem::emitDeleteSignal()
{
    dThread->quit();
    this->hide();
    emit deleteSiganl();
}
