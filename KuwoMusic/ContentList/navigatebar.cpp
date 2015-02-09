#include "navigatebar.h"
#include <QDebug>
NavigateBar::NavigateBar(QWidget *parent) : QWidget(parent)
{
    hboxLayout = new QHBoxLayout(this);
    hboxLayout->setSpacing(3);
    hboxLayout->setMargin(1);
    paiHangBang = new QLabel("<a href='Tab:paiHangBang' style='text-decoration:none;color:white'>排行榜</a>",this);
    //geShou = new QLabel("<a href='Tab:geShou' style='text-decoration:none;color:white'>歌手</a>",this);
    //fenLei = new QLabel("<a href='Tab:fenLei' style='text-decoration:none;color:white'>分类</a>",this);
    souSuo = new QLabel("<a href='Tab:souSuo' style='text-decoration:none;color:white'>搜索</a>",this);
    xiazai = new QLabel("<a href='Tab:xiazai' style='text-decoration:none;color:white'>下载</a>",this);

    paiHangBang->setAlignment(Qt::AlignHCenter);
    //geShou->setAlignment(Qt::AlignHCenter);
    //fenLei->setAlignment(Qt::AlignHCenter);
    souSuo->setAlignment(Qt::AlignHCenter);
    xiazai->setAlignment(Qt::AlignHCenter);
    hboxLayout->addWidget(paiHangBang);
    //hboxLayout->addWidget(geShou);
    //hboxLayout->addWidget(fenLei);
    hboxLayout->addWidget(souSuo);
    hboxLayout->addWidget(xiazai);
    connect(paiHangBang,SIGNAL(linkActivated(QString)),this,SLOT(linkClick(QString)));
    //connect(geShou,SIGNAL(linkActivated(QString)),this,SLOT(linkClick(QString)));
    //connect(fenLei,SIGNAL(linkActivated(QString)),this,SLOT(linkClick(QString)));
    connect(souSuo,SIGNAL(linkActivated(QString)),this,SLOT(linkClick(QString)));
    connect(xiazai,SIGNAL(linkActivated(QString)),this,SLOT(linkClick(QString)));

    connect(paiHangBang,SIGNAL(linkHovered(QString)),this,SLOT(linkHover(QString)));
    //connect(geShou,SIGNAL(linkHovered(QString)),this,SLOT(linkHover(QString)));
    //connect(fenLei,SIGNAL(linkHovered(QString)),this,SLOT(linkHover(QString)));
    connect(souSuo,SIGNAL(linkHovered(QString)),this,SLOT(linkHover(QString)));
    connect(xiazai,SIGNAL(linkHovered(QString)),this,SLOT(linkHover(QString)));

    setFixedHeight(30);

    linkClick("Tab:paiHangBang");
}

NavigateBar::~NavigateBar()
{

}

void NavigateBar::linkClick(QString link)
{
    paiHangBang->setText("<a href='Tab:paiHangBang' style='text-decoration:none;color:white'>排行榜</a>");
    //geShou->setText("<a href='Tab:geShou' style='text-decoration:none;color:white'>歌手</a>");
    //fenLei->setText("<a href='Tab:fenLei' style='text-decoration:none;color:white'>分类</a>");
    souSuo->setText("<a href='Tab:souSuo' style='text-decoration:none;color:white'>搜索</a>");
    xiazai->setText("<a href='Tab:xiazai' style='text-decoration:none;color:white'>下载</a>");
    if(link.contains("paiHangBang"))
    {
        paiHangBang->setText("<a href='Tab:paiHangBang' style='text-decoration:none;color:#46B4E6'>排行榜</a>");
    }else if(link.contains("geShou"))
    {
        //geShou->setText("<a href='Tab:geShou' style='text-decoration:none;color:#46B4E6'>歌手</a>");
    }else if(link.contains("fenLei"))
    {
        //fenLei->setText("<a href='Tab:fenLei' style='text-decoration:none;color:#46B4E6'>分类</a>");
    }else if(link.contains("souSuo"))
    {
        souSuo->setText("<a href='Tab:souSuo' style='text-decoration:none;color:#46B4E6'>搜索</a>");
    }else if(link.contains("xiazai"))
    {
        xiazai->setText("<a href='Tab:xiazai' style='text-decoration:none;color:#46B4E6'>下载</a>");
    }
    emit tabChangeSignal(link);
}

void NavigateBar::linkHover(QString link)
{
    Q_UNUSED(link);
}

