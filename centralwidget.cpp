#include "centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent)
{
    mainGridL = new QGridLayout;
    mainSplitter = new QSplitter;
    mainSplitter->setHandleWidth(1);
    mainSplitter->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainGridL->addWidget(mainSplitter);
    setLayout(mainGridL);
    initUi();
    adbController = new AdbController(this,true);
    adbController->starServer();
    connect(adbController,SIGNAL(connectionChanged(QStringList)),this,SLOT(slotConnectionChanged(QStringList)));
    this->adbPath = "adb";
    shootDeviceScreenTimer = new QTimer(this);
    connect(shootDeviceScreenTimer,SIGNAL(timeout()),this,SLOT(shootDeviceScreen()));
    shootDeviceScreenTimer->start(5000);
    copyDamonToPath();
}

CentralWidget::~CentralWidget()
{
}

void CentralWidget::initUi()
{
    leftPanelContainer = new QWidget(this);
    leftPanelContainer->setObjectName("leftPanelContainer");
    phoneHeaderWidget = new PhoneHeaderWidget(this);
    navListWidget = new QListWidget(this);
    navListWidget->setObjectName("navListWidget");
    QVBoxLayout *leftPanelL = new QVBoxLayout(leftPanelContainer);
    leftPanelL->setMargin(3);
    leftPanelL->setSpacing(1);
    leftPanelContainer->setStyleSheet("QWidget#leftPanelContainer{border:none;border-right:2px solid rgba(10,10,10,30);}");
    leftPanelContainer->setMinimumWidth(150);
    navListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    navListWidget->setFocusPolicy(Qt::NoFocus);
    navListWidget->setMovement(QListView::Static);
    navListWidget->setWordWrap(true);
    navListWidget->setSpacing(5);
    navListWidget->setIconSize(QSize(40,40));

    QListWidgetItem *itemPhone = new QListWidgetItem(QIcon(":/app/skin/imgs/info.png"),"我的手机");
    navListWidget->addItem(itemPhone);
    navListWidget->addItem(new QListWidgetItem(QIcon(":/app/skin/imgs/folder.png"),"存储管理"));
    navListWidget->addItem(new QListWidgetItem(QIcon(":/app/skin/imgs/software.png"),"软件管理"));
    navListWidget->addItem(new QListWidgetItem(QIcon(":/app/skin/imgs/softDown.png"),"软件下载"));
    navListWidget->addItem(new QListWidgetItem(QIcon(":/app/skin/imgs/music.png"),"音乐下载"));
    navListWidget->setCurrentRow(0);

    contentStackedWidget = new QStackedWidget(this);
    phoneInfoPage = new PhoneInfoPage(this);
    phoneFileManagerPage = new PhoneFileManagerPage(this);
    phoneSoftwareManagerPage = new SoftWareListWidget(this);
    softwareDownloadPage = new SoftwareDownloadPage(this);
    kuwoMusicWidget = new KuwoMusicWidget(this);

    leftPanelL->addWidget(phoneHeaderWidget);
    leftPanelL->addWidget(navListWidget);

    contentStackedWidget->addWidget(phoneInfoPage);
    contentStackedWidget->addWidget(phoneFileManagerPage);
    contentStackedWidget->addWidget(phoneSoftwareManagerPage);
    contentStackedWidget->addWidget(softwareDownloadPage);
    contentStackedWidget->addWidget(kuwoMusicWidget);

    mainSplitter->addWidget(leftPanelContainer);
    mainSplitter->addWidget(contentStackedWidget);
    mainSplitter->setStretchFactor(0,20);
    mainSplitter->setStretchFactor(1,80);
    connect(navListWidget,SIGNAL(currentRowChanged(int)),contentStackedWidget,SLOT(setCurrentIndex(int)));
    connect(phoneHeaderWidget,SIGNAL(deviceChangeSignal(QString)),this,SLOT(setCurrentDevice(QString)));
}

void CentralWidget::slotConnectionChanged(QStringList serialNumberStateList)
{
    devicesStateMap.clear();
    devicesModelMap.clear();
    foreach (QString strItem, serialNumberStateList) {
        QString serialNumber = strItem.split("#").value(0,"???");
        QString state = strItem.split("#").value(1,"???");
        this->devicesStateMap.insert(serialNumber,state);
    }
    bool changeToNextPhone = true;
    foreach (QString serialNumber, devicesStateMap.keys()) {
        QString temp = adbController->getDeviceModel(serialNumber);
        devicesModelMap.insert(serialNumber,temp);
        if(this->currentDeviceSerialNumber == serialNumber)
        {
            changeToNextPhone = false;
        }
    }
    if(changeToNextPhone && devicesStateMap.count()>0)
    {
        resetToDefault();
        setCurrentDevice(devicesStateMap.firstKey());
    }else if(devicesStateMap.count()==0)
    {
        resetToDefault();
    }else
    {
        setCurrentDevice(this->currentDeviceSerialNumber);
    }

    //qDebug()<< devicesStateMap<<"\n"<<devicesModelMap<<"\n\n";


}

void CentralWidget::setCurrentDevice(QString serialNumber)
{
    if(serialNumber.isEmpty())
    {
        qDebug()<<"CentralWidget::setCurrentDevice serialNumber isEmpty()";
        return;
    }
    phoneHeaderWidget->setDevicesStateData(devicesStateMap,devicesModelMap,this->currentDeviceSerialNumber);//更新手机列表用
    if(this->currentDeviceSerialNumber == serialNumber)
    {
        return;
    }
    resetToDefault();
    this->currentDeviceSerialNumber = serialNumber;

    phoneHeaderWidget->setDevicesStateData(devicesStateMap,devicesModelMap,this->currentDeviceSerialNumber);

    QStringList infos = adbController->getPhone_buildProp_Info(serialNumber);//手机型号 制造商 系统版本 内存大小
    bool isRoot = adbController->checkRoot(serialNumber);
    phoneInfoPage->setPhoneContentText(infos.value(0,""),
                                       infos.value(1,""),
                                       infos.value(2,""),
                                       adbController->getMemoryInfo(this->currentDeviceSerialNumber).value(0,"未知"),
                                       //infos.value(3,""),
                                       isRoot?"已Root":"未Root");
    shootDeviceScreen();
    phoneFileManagerPage->setCurrentDevice(serialNumber);
    phoneSoftwareManagerPage->setSerialsNumber(serialNumber);
    softwareDownloadPage->setSerialsNumber(serialNumber);


}

void CentralWidget::shootDeviceScreen()
{
    if(devicesStateMap.value(this->currentDeviceSerialNumber) == "DEVICE" && navListWidget->currentRow() == 0)//连接状态与处于手机信息显示界面才截图
    {
        QString preNumber = this->currentDeviceSerialNumber;
        adbController->shootDeviceScreen(this->currentDeviceSerialNumber,"/tmp/lScreenshoot.png");
        if(preNumber == this->currentDeviceSerialNumber)
        {
            phoneInfoPage->setDeviceScreenPicture("/tmp/lScreenshoot.png");
        }

    }else if(devicesStateMap.value(this->currentDeviceSerialNumber) == 0)
    {
        phoneInfoPage->setDeviceScreenPicture("");
    }


}

void CentralWidget::resetToDefault()
{
    this->currentDeviceSerialNumber = "";
    phoneHeaderWidget->resetToDefault();
    phoneInfoPage->resetToDefault();
    phoneFileManagerPage->resetToDefault();
    phoneSoftwareManagerPage->resetToDefault();
}

void CentralWidget::copyDamonToPath()
{
    //home/xusongjie/.LAssistant
    if(!QDir(QDir::homePath()+"/.LAssistant/").exists())
    {
        QDir dir;
        dir.mkdir(QDir::homePath()+"/.LAssistant/");
    }
    QFile::copy(":/addons/addons/LAssistant.apk",QDir::homePath()+"/.LAssistant/LAssistant1.0.apk");
}
