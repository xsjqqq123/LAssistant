#include "titlebar.h"

TitleBar::TitleBar(QWidget *parent, bool isSettingBtnShow, bool isMinBtnShow, bool isCloseShow) : QWidget(parent)
{
    setFixedHeight(30);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    titleLbl = new QLabel("L安卓手机助手");
    settingBtn = new QPushButton;
    minBtn = new QPushButton;
    closeBtn = new QPushButton(this);
    settingBtn->setObjectName("titleBarBtns");
    minBtn->setObjectName("titleBarBtns");
    closeBtn->setObjectName("titleBarBtns");
    settingBtn->setFixedSize(30,30);
    minBtn->setFixedSize(30,30);
    closeBtn->setFixedSize(30,30);
    settingBtn->setIconSize(QSize(25,25));
    minBtn->setIconSize(QSize(30,30));
    closeBtn->setIconSize(QSize(30,30));
    settingBtn->setIcon(QIcon(":/app/skin/imgs/mainWindow/titleBar/control.png"));
    minBtn->setIcon(QIcon(":/app/skin/imgs/mainWindow/titleBar/minus.png"));
    closeBtn->setIcon(QIcon(":/app/skin/imgs/mainWindow/titleBar/close.png"));

    mainLayout->addWidget(titleLbl,1,Qt::AlignHCenter);
    if(isSettingBtnShow)
        mainLayout->addWidget(settingBtn,0,Qt::AlignRight);
    if(isMinBtnShow)
        mainLayout->addWidget(minBtn,0,Qt::AlignRight);
    if(isCloseShow)
        mainLayout->addWidget(closeBtn,0,Qt::AlignRight);

    settingBtn->setFocusPolicy(Qt::NoFocus);
    minBtn->setFocusPolicy(Qt::NoFocus);
    closeBtn->setFocusPolicy(Qt::NoFocus);

    connect(minBtn,SIGNAL(clicked()),this,SLOT(emitMinimumWinSignal()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(emitCloseWinSignal()));
    connect(settingBtn,SIGNAL(clicked()),this,SLOT(emitSettingSignal()));
}

TitleBar::~TitleBar()
{

}

void TitleBar::emitMinimumWinSignal()
{
    emit minimumWinSignal();
}

void TitleBar::emitCloseWinSignal()
{
    emit closeWinSignal();
}

void TitleBar::emitSettingSignal()
{
    emit settingSignal();
}

void TitleBar::setTitleStr(QString str)
{
    titleLbl->setText(str);
}

