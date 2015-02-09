#include "softwarelistwidgetitem.h"

SoftWareListWidgetItem::SoftWareListWidgetItem(QWidget *parent, QString packageName, QString softwareName, QString iconPath, QString version) :
    QWidget(parent)
{
    adbController = new AdbController(this);
    mainLyout = new QHBoxLayout(this);
    softwareIconLbl = new QLabel(this);
    softwareNameLbl = new QLabel(this);
    softwareVersionLbl = new QLabel(this);
    managerBtn = new QPushButton("卸载",this);
    softwareIconLbl->setFixedSize(40,40);
    softwareNameLbl->setFixedWidth(200);
    softwareNameLbl->setWordWrap(true);
    //softwareVersionLbl->setFixedWidth(100);

    mainLyout->setAlignment(Qt::AlignLeft);
    mainLyout->addWidget(softwareIconLbl);
    mainLyout->addWidget(softwareNameLbl);
    mainLyout->addStretch();
    mainLyout->addWidget(softwareVersionLbl);
    mainLyout->addStretch();
    mainLyout->addWidget(managerBtn);

    this->packageName = packageName;
    if(QFileInfo(iconPath).exists())
    {
        QPixmap pix(iconPath);
        pix = pix.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        softwareIconLbl->setPixmap(pix);
    }else
    {
        QPixmap pix(":/fileManager/skin/imgs/fileIcons/apk.png");
        pix = pix.scaled(40,40,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        softwareIconLbl->setPixmap(pix);
    }
    if(!softwareName.isEmpty())
    {
        softwareNameLbl->setText(softwareName);
    }else
    {
        softwareNameLbl->setText(packageName);
    }
    if(!version.isEmpty())
    {
        softwareVersionLbl->setText("版本:"+version);
        this->version = version;
    }
    connect(managerBtn,SIGNAL(clicked()),this,SLOT(uninstall()));
    QTimer::singleShot(1000,this,SLOT(checkForUpdate()));
}

SoftWareListWidgetItem::~SoftWareListWidgetItem()
{

}

void SoftWareListWidgetItem::checkForUpdate()
{
    if(version.isEmpty())
    {
        return;
    }
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply1 = manager->get(QNetworkRequest(QUrl("http://mvgather.com/LAssistant/api/checkUpdate.php?softPackageName="+this->packageName)));
    QEventLoop loop1;
    QTimer::singleShot(10000,&loop1,SLOT(quit()));
    connect(this, SIGNAL(destroyed()), &loop1, SLOT(quit()));
    QObject::connect(reply1, SIGNAL(finished()), &loop1, SLOT(quit()));
    loop1.exec();
    QByteArray jsonData = reply1->readAll();

//qDebug()<<jsonData;
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj = doc.object();
        QString softVersionName = rootObj["softVersionName"].toString();
        this->downloadUrl = rootObj["downloadUrl"].toString();
        this->softSavePath = QDir::homePath()+"/.LAssistant/tmp/"+rootObj["softName"].toString()+rootObj["softVersionName"].toString()+".apk";
        QStringList list = softVersionName.split(".");
        int newVersionA1 = list.value(0,"0").toInt();
        int newVersionA2 = list.value(1,"0").toInt();
        int newVersionA3 = list.value(2,"0").toInt();

        QStringList list2 = this->version.split(".");
        int oldVersionA1 = list2.value(0,"0").toInt();
        int oldVersionA2 = list2.value(1,"0").toInt();
        int oldVersionA3 = list2.value(2,"0").toInt();

        bool isUpdate = false;
        if(newVersionA1>oldVersionA1)
        {
            isUpdate = true;
        }
        if(newVersionA1 == oldVersionA1 && newVersionA2>oldVersionA2)
        {
            isUpdate = true;
        }
        if(newVersionA1 == oldVersionA1 && newVersionA2 == oldVersionA2 && newVersionA3>oldVersionA3)
        {
            isUpdate = true;
        }
//qDebug()<<packageName<<isUpdate<<list<<list2;

        if(!isUpdate)
        {
            return;
        }
        disconnect(managerBtn,0,this,0);
        managerBtn->setText("升级"+softVersionName);
        managerBtn->setStyleSheet("background-color:rgba(56,156,255,120)");
        connect(managerBtn,SIGNAL(clicked()),this,SLOT(updateSoft()));

    }
    delete reply1;

}

void SoftWareListWidgetItem::uninstall()
{
    managerBtn->setEnabled(false);
    managerBtn->setText("卸载中");
    emitUninstall(packageName);
}

void SoftWareListWidgetItem::emitUninstall(QString packageName)
{
    emit uninstallSignal(packageName);
}

void SoftWareListWidgetItem::setUninstallBtnState(int state)
{
    if(state == -1)
    {
        managerBtn->setText("重试");
        managerBtn->setEnabled(true);
    }else if(state == 1)
    {
        managerBtn->setText("已卸载");
        managerBtn->setEnabled(false);
    }else
    {
        managerBtn->setText("卸载");
        managerBtn->setEnabled(false);
    }

}

void SoftWareListWidgetItem::updateSoft()
{
    managerBtn->setEnabled(false);
    managerBtn->setText("下载中");
    downloadThread = new DownloadThreadSoft(this,this->downloadUrl,this->softSavePath);
    connect(downloadThread,SIGNAL(downloadProcess(int)),this,SLOT(downloadProgress(int)));
    connect(downloadThread,SIGNAL(downloadFinished(int)),this,SLOT(downloadFinished(int)));
    connect(downloadThread,SIGNAL(downloadFinished(int)),downloadThread,SLOT(deleteLater()));
    connect(this,SIGNAL(destroyed()),downloadThread,SLOT(stopTask()));
}

void SoftWareListWidgetItem::downloadProgress(int v)
{
    downLoadPercent = v;
    managerBtn->setText("下载中:"+QString::number(v)+"%");
}

void SoftWareListWidgetItem::downloadFinished(int status)
{
    if(status != 0)
    {
        managerBtn->setText("重试下载");
        managerBtn->setEnabled(true);
        return;
    }
    managerBtn->setText("正在安装");
    bool b = adbController->installApkToPhone(this->serialsNumber,this->softSavePath,"");
    if(b)
    {
        managerBtn->setText("升级成功");
    }else
    {
        //managerBtn->setText("升级失败");
        managerBtn->setText("重试升级");
        managerBtn->setEnabled(true);
    }

}

void SoftWareListWidgetItem::setSerialsNumber(QString serialsNumber)
{
    this->serialsNumber = serialsNumber;
}
