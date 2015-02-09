#include "downloadmanagerwidget.h"

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
    containerWL = new QGridLayout(this);
    containerW = new QWidget(this);
    containerWL->setMargin(0);
    containerWL->setSpacing(0);
    containerWL->addWidget(containerW);
    containerW->setObjectName("showWidget");

    mainLayout = new QVBoxLayout(containerW);
    installBtn = new QPushButton("安装所选到手机",this);
    installBtn->setFixedSize(120,30);
    area = new QScrollArea(this);
    areaWidget = new QWidget(this);
    areaWidgetLayout = new QVBoxLayout(areaWidget);
    areaWidgetLayout->setAlignment(Qt::AlignTop);

    mainLayout->addWidget(installBtn,Qt::AlignRight);
    mainLayout->addWidget(areaWidget);

    connect(installBtn,SIGNAL(clicked()),this,SLOT(installSelected()));
}

DownloadManagerWidget::~DownloadManagerWidget()
{

}

void DownloadManagerWidget::leaveEvent(QEvent *)
{
    this->hide();
}

void DownloadManagerWidget::setSerialsNumber(QString serialsNumber)
{
    this->serialsNumber = serialsNumber;
}

void DownloadManagerWidget::addDownloadTask(QString softName, QString directUrl, QString saveDir)
{
    SoftDownloadItem *item = new SoftDownloadItem(this,softName,directUrl,saveDir);
    areaWidgetLayout->addWidget(item);
    itemList.append(item);
}

void DownloadManagerWidget::installSelected()
{
    installBtn->setEnabled(false);
    QString arg = "-r";
    QStringList fileList;
    foreach (SoftDownloadItem *item, itemList) {
        if(item->isForInstall())
        {
            //弹出安装窗口
            QString apkPath = item->getSoftSavePath();
            fileList.append(apkPath);
        }
    }
    if(fileList.count()>0)
    {
        InstallTaskListWidget *w = new InstallTaskListWidget(0,this->serialsNumber,fileList,arg);
        QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
        w->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
        w->show();
        w->startTask();
    }
    installBtn->setEnabled(true);
}


SoftDownloadItem::SoftDownloadItem(QWidget *parent,QString softName, QString Url, QString saveDir)
{
    isDownloadFinished = false;
    hBoxLayout = new QHBoxLayout(this);
    checkBox = new QCheckBox(this);
    checkBox->setChecked(true);
    softNameLbl = new QLabel(softName);
    progressBar = new QProgressBar(this);
    progressBar->setObjectName("downloadBar");
    deleteTaskBtn = new QPushButton(QIcon(":/app/skin/imgs/delete.png"),"");
    deleteTaskBtn->setObjectName("transparentBtn");
    hBoxLayout->addWidget(checkBox);
    hBoxLayout->addWidget(softNameLbl);
    hBoxLayout->addWidget(progressBar);
    hBoxLayout->addWidget(deleteTaskBtn);

    connect(deleteTaskBtn,SIGNAL(clicked()),this,SLOT(stopTask()));
    this->softSavePath = saveDir+softName+".apk";
    this->Url = Url;
    startTask();
}

SoftDownloadItem::~SoftDownloadItem()
{

}

void SoftDownloadItem::startTask()
{
    downloadThread = new DownloadThreadSoft(this,this->Url,this->softSavePath);
    connect(downloadThread,SIGNAL(downloadProcess(int)),this,SLOT(downloadProgress(int)));
    connect(downloadThread,SIGNAL(downloadFinished(int)),this,SLOT(downloadFinished(int)));
    connect(downloadThread,SIGNAL(downloadFinished(int)),downloadThread,SLOT(deleteLater()));
}

void SoftDownloadItem::stopTask()
{
    this->hide();
    downloadThread->stopTask();
}

bool SoftDownloadItem::isForInstall()
{
    if(checkBox->isChecked() && isDownloadFinished &&this->isVisible())
    {
        return true;
    }else
    {
        return false;
    }
}

QString SoftDownloadItem::getSoftSavePath()
{
    return this->softSavePath;
}

void SoftDownloadItem::downloadProgress(int v)
{
    progressBar->setValue(v);
}

void SoftDownloadItem::downloadFinished(int status)
{
    if(status==0)
    {
        progressBar->setValue(100);
        isDownloadFinished = true;
    }else
    {
        progressBar->setValue(0);
    }


}


DownloadThreadSoft::DownloadThreadSoft(QObject *parent,QString url,QString fileSavePath) : QObject(parent)
{

    QFile file(fileSavePath);
    if(file.exists())
    {
        file.remove();
        file.setFileName(fileSavePath+".st");
        file.remove();
    }
    process = new QProcess(this);
    connect(process,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(process,SIGNAL(finished(int)),this,SLOT(finished(int)));
    process->start("axel",QStringList()<<"-a"<<"-n"<<"5"<<"-o"<<fileSavePath<<url);
}

DownloadThreadSoft::~DownloadThreadSoft()
{

}
void DownloadThreadSoft::readyRead()
{
    QString str = process->readAll();
    QStringList resultList = str.split("\n");
    if(resultList.isEmpty())
    {
        return;
    }
    QString line = resultList.last();
    QRegExp rx("\\[.*%");
    rx.setMinimal(true);
    int i = rx.indexIn(line);
    if(i == -1)
    {
        return;
    }
    QString v = rx.cap(0).remove(QRegExp("\\[|%")).trimmed();
    emit downloadProcess(v.toInt());
}

void DownloadThreadSoft::finished(int status)
{
    emit downloadFinished(status);
    //qDebug()<<"finished";
}

void DownloadThreadSoft::stopTask()
{

    process->terminate();
    process->waitForFinished(1000);
}

