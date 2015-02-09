#include "softwarelistwidget.h"
#include <QDebug>
SoftWareListWidget::SoftWareListWidget(QWidget *parent) :
    QWidget(parent)
{
    mainGlayout = new QGridLayout(this);
    installApkBtn = new QPushButton("安装Apk");
    installApkBtn->setFixedSize(80,30);
    installApkBtn->setObjectName("gradientBtn");
    busyLbl = new QLabel;
    busyLbl->setFixedSize(30,30);
    QMovie *movie = new QMovie(":/app/skin/imgs/loading.gif");
    busyLbl->setMovie(movie);
    movie->start();
    refreshListBtn = new QPushButton("刷新");
    refreshListBtn->setObjectName("gradientBtn");
    refreshListBtn->setFixedSize(60,30);
    softwareListAreaWidget = new QWidget;
    softwareListAreaWidget->setObjectName("transparentWidget");
    softwareListAreaWidgetLayout = new QVBoxLayout(softwareListAreaWidget);
    //softwareListAreaWidget->setMinimumSize(500,300);
    softwareListArea = new QScrollArea(this);
    softwareListArea->setObjectName("softwareListArea");
    softwareListArea->setWidgetResizable(true);
    //softwareListAreaWidget->setLayout(softwareListAreaWidgetLayout);
    softwareListArea->setWidget(softwareListAreaWidget);

    mainGlayout->addWidget(installApkBtn,0,0,1,1);
    mainGlayout->addItem(new QSpacerItem(10,10),0,0,1,3);//
    mainGlayout->addWidget(busyLbl,0,3,1,1);
    mainGlayout->addWidget(refreshListBtn,0,4,1,1);
    mainGlayout->addWidget(softwareListArea,1,0,5,5);

    adbController = new AdbController(this);

    connect(refreshListBtn,SIGNAL(clicked()),this,SLOT(refreshList()));
    connect(installApkBtn,SIGNAL(clicked()),this,SLOT(installAPP()));
    resetToDefault();
}

void SoftWareListWidget::setSerialsNumber(QString serialsNumber)
{
    installApkBtn->setEnabled(true);
    this->serialsNumber = serialsNumber;
    refreshList();
}

void SoftWareListWidget::getIconsInfoToPc()//将安卓程序得到的图标与包的信息文件转移到电脑目录
{
    QString apkPath = QDir::homePath()+"/.LAssistant/LAssistant1.0.apk";
    int tryTime = 3;
    QString data = adbController->getUserPackageList(this->serialsNumber);
    if(data.contains("com.lassistant"))
    {
        adbController->startAPP(this->serialsNumber,"com.lassistant","MainActivity");
    }else
    {
        while(tryTime--)
        {
            adbController->installApkToPhone(this->serialsNumber,apkPath,"-r");
            QString data = adbController->startAPP(this->serialsNumber,"com.lassistant","MainActivity");
            if(data.contains("Error"))
            {
                continue;
            }else
            {
                break;
            }
        }
    }
    for(int delay = 1;delay<10;delay++)
    {
        QEventLoop loop;
        QTimer::singleShot(0.5*1000,&loop,SLOT(quit()));
        loop.exec();
        data = adbController->getProcessInfo(this->serialsNumber);//正在运行的进程
        //qDebug()<<data;
        if(data.contains("lassistant"))
        {
            continue;
        }else
        {
            break;
        }

    }

    if(remoteIconDir.isEmpty())
    {
        this->remoteIconDir = adbController->findIconInfoDir(this->serialsNumber);
    }
    this->packageNameListStr = adbController->getUserPackageList(this->serialsNumber);
    QTextStream pnStrStream(&packageNameListStr);

    while(!pnStrStream.atEnd())
    {
        QString line = pnStrStream.readLine();
        QString packageName = line.split(":").value(1,"");
        if(QFileInfo(QDir::homePath()+QString("/.LAssistant/appsData/%0.png").arg(packageName)).exists())
        {
            continue;
        }else
        {
            adbController->pullFileToPc(this->serialsNumber,remoteIconDir+"/"+packageName+".png",QDir::homePath()+"/.LAssistant/appsData/");
        }
    }
    adbController->pullFileToPc(this->serialsNumber,remoteIconDir+"/packageInfo.txt",QDir::homePath()+"/.LAssistant/appsData/");

}

void SoftWareListWidget::loadSoftWareList()
{
    pName_item_map.clear();
    QTextStream pnStrStream(&packageNameListStr);
    QFile file(QDir::homePath()+"/.LAssistant/appsData/packageInfo.txt");
    if(file.exists())
    {
        QMap<QString,QString> mapTemp;//key:packageName , value:"softName#version"
        QTextStream tmpStream(&file);
        bool b = file.open(QFile::ReadOnly);

        while(!tmpStream.atEnd())
        {
            QString line = tmpStream.readLine();
            mapTemp.insert(line.split("$").value(0,"com"),line.split("$").value(1,""));
        }
        //qDebug()<<"mapTemp:"<<mapTemp<<"\n\n";
        int doubleMark = 0;
        while(!pnStrStream.atEnd())
        {

            QString line = pnStrStream.readLine();
            QString packageName = line.split(":").value(1,"");
            QString info = mapTemp.value(packageName,"");
            QStringList infoList = info.split("#");
            if(packageName.isEmpty())
            {
                continue;
            }
            SoftWareListWidgetItem *item = new SoftWareListWidgetItem(0,packageName,infoList.value(0,""),""+QDir::homePath()+"/.LAssistant/appsData/"+packageName+".png",infoList.value(1,""));
            item->setMinimumSize(400,60);
            item->setSerialsNumber(this->serialsNumber);
            if(doubleMark==1)
            {
                item->setObjectName("itemDoubleLineShow");
                doubleMark = 0;

            }else
            {
                doubleMark++;
            }
            softwareListAreaWidgetLayout->addWidget(item);
            pName_item_map.insert(packageName,item);
            connect(item,SIGNAL(uninstallSignal(QString)),this,SLOT(uninstallApp(QString)));
        }
    }else
    {//从内置数据库加载

    }

}

void SoftWareListWidget::refreshList()
{
    QFile file(QDir::homePath()+"/.LAssistant/appsData/packageInfo.txt");
    if(file.exists())
    {
        file.remove();
    }
    while(softwareListAreaWidgetLayout->count() > 0)
    {
        QWidget* widget = softwareListAreaWidgetLayout->itemAt(0)->widget();
        softwareListAreaWidgetLayout->removeWidget(widget);
        delete widget;
    }
    busyLbl->setVisible(true);
    refreshListBtn->setEnabled(false);
    QEventLoop loop;
    QTimer::singleShot(200,&loop,SLOT(quit()));
    loop.exec();

    getIconsInfoToPc();
    loadSoftWareList();
    refreshListBtn->setEnabled((true));
    busyLbl->setVisible(false);
}

void SoftWareListWidget::uninstallApp(QString packageName)
{
    SoftWareListWidgetItem *item = pName_item_map.value(packageName);
    if(item == Q_NULLPTR)
        return;
    bool b = adbController->uninstallApp(this->serialsNumber,packageName,true);
    if(b == true)
    {
        item->setUninstallBtnState(1);//-1 retry; 1 success
    }else
    {
        item->setUninstallBtnState(-1);//-1 retry; 1 success
    }

}

void SoftWareListWidget::resetToDefault()
{
    this->remoteIconDir = "";
    this->serialsNumber = "";
    this->packageNameListStr = "";
    refreshListBtn->setEnabled(false);
    busyLbl->setVisible(false);
    installApkBtn->setEnabled(false);
    pName_item_map.clear();
    while(softwareListAreaWidgetLayout->count() > 0)
    {
        QWidget* widget = softwareListAreaWidgetLayout->itemAt(0)->widget();
        softwareListAreaWidgetLayout->removeWidget(widget);
        delete widget;
    }
}

void SoftWareListWidget::installAPP()
{
    QStringList fileList = QFileDialog::getOpenFileNames(//
                NULL,
                "选择需要安装到手机的应用文件",
                QDir::homePath(),"android apps (*.apk *.Apk *.APK)");
    if(fileList.count()==0)
    {
        return;
    }
    QString arg = "-r";
    InstallTaskListWidget *w = new InstallTaskListWidget(0,this->serialsNumber,fileList,arg);
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    w->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    w->show();
    w->startTask();
}
