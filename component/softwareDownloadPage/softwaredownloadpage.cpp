#include "softwaredownloadpage.h"

SoftwareDownloadPage::SoftwareDownloadPage(QWidget *parent) : QWidget(parent)
{
    hostUrl = "http://mvgather.com";
    manager = new QNetworkAccessManager(this);

    mainLayout = new QVBoxLayout(this);
    navBarLayout = new QHBoxLayout;
    navBtn = new MyhoverButton(this);
    navWidget = new SoftwareTypeNavWidget(navBtn);

    searchEdit = new QLineEdit(this);
    searchBtn =new QPushButton(QIcon(":/app/skin/imgs/search.png"),"",this);
    searchEdit->setObjectName("editSearch");
//    searchBtn->setIconSize(QSize(30,30));
    QHBoxLayout *sl = new QHBoxLayout;
    sl->setMargin(0);
    sl->setSpacing(0);
    sl->addWidget(searchEdit);
    sl->addWidget(searchBtn);
    connect(searchEdit,SIGNAL(returnPressed()),this,SLOT(getSearchJsonFromServer()));
    connect(searchBtn,SIGNAL(clicked()),this,SLOT(getSearchJsonFromServer()));

    downloadNavBtn = new MyhoverButton(this);
    downloadManagerWidget = new DownloadManagerWidget(downloadNavBtn);
    downloadManagerWidget->setFixedWidth(350);
    softwareList = new SoftwareList(this);
    navBtn->setObjectName("navBtn");
    navBtn->setFixedSize(80,40);
    navBtn->setText("导航");
    downloadNavBtn->setText("下载管理");
    downloadNavBtn->setObjectName("navBtn");
    downloadNavBtn->setFixedSize(80,40);

    //navWidget->setFocusPolicy(Qt::NoFocus);

    navBarLayout->addWidget(navBtn,1,Qt::AlignLeft);
    navBarLayout->addLayout(sl,1);
    navBarLayout->addWidget(downloadNavBtn,1,Qt::AlignRight);
    mainLayout->addLayout(navBarLayout);
    mainLayout->addWidget(softwareList);

    QTimer::singleShot(500,this,SLOT(delayLoadData()));

    connect(navBtn,SIGNAL(mouseEnterSiganl()),this,SLOT(showNavList()));
    connect(navWidget,SIGNAL(typeChange(int,int)),this,SLOT(getSoftJsonFromServer(int,int)));
    connect(downloadNavBtn,SIGNAL(mouseEnterSiganl()),this,SLOT(showDownloadNavList()));
    connect(softwareList,SIGNAL(downloadSignal(QString)),this,SLOT(addDownloadTask(QString)));
    connect(softwareList,SIGNAL(pageChange(int)),this,SLOT(pageChange(int)));
}

SoftwareDownloadPage::~SoftwareDownloadPage()
{
}

void SoftwareDownloadPage::setSerialsNumber(QString serialsNumber)
{
    this->serialsNumber = serialsNumber;
    downloadManagerWidget->setSerialsNumber(serialsNumber);
}

void SoftwareDownloadPage::showNavList()
{
    QPoint p = navBtn->mapToGlobal(QPoint(0,0));
    navWidget->move(p.x(),p.y());
    navWidget->show();
}

void SoftwareDownloadPage::showDownloadNavList()
{
    QPoint p = downloadNavBtn->mapToGlobal(QPoint(downloadNavBtn->width(),0));
    downloadManagerWidget->move(p.x()-downloadManagerWidget->width(),p.y());
    downloadManagerWidget->show();
}

void SoftwareDownloadPage::getNavJsonFromServer()
{
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(hostUrl+"/LAssistant/api/getNavJson.php")));
    while(true)
    {
        QEventLoop loop;
        QTimer::singleShot(10000,&loop,SLOT(quit()));
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();
        if(data.isEmpty())
        {
            continue;
        }else
        {
            navWidget->setList(data);
            break;
        }
    }

}

void SoftwareDownloadPage::getSoftJsonFromServer(int requestId,int pageNum)
{
    softwareList->clearItems();
    //qDebug()<<requestId;
    this->requestId = requestId;
    this->currentRequestUrl = hostUrl+"/LAssistant/api/getSoftJson.php?requestId="+QString::number(requestId);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(currentRequestUrl+"&page="+QString::number(pageNum))));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray jsonData = reply->readAll();
    //qDebug()<<jsonData;
    if(jsonData.isEmpty())
    {
        return;
    }else
    {

        softwareList->setList(jsonData);
    }
}

void SoftwareDownloadPage::getSearchJsonFromServer()
{
    softwareList->clearItems();
    QString keyword = searchEdit->text();
    if(keyword.isEmpty())
    {
        searchEdit->setText("请在此输入文字");
        return;
    }else if(keyword.contains(QRegExp(".*and.*|.*exec.*|.*insert.*|.*select.*|.*delete.*|.*update.*|.*count.*|.**.*|.*%.*|.*chr.*|.*mid.*|.*master.*|.*truncate.*|.*char.*|.*declare")))
    {
        searchEdit->setText("请换一个关键词");
        return;
    }
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(hostUrl+"/LAssistant/api/search.php?keyword="+keyword)));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray jsonData = reply->readAll();
    if(jsonData.isEmpty())
    {
        return;
    }else
    {
        softwareList->setList(jsonData);
    }
}

void SoftwareDownloadPage::delayLoadData()
{
    getNavJsonFromServer();
    getSoftJsonFromServer(0000,1);//主页
}

void SoftwareDownloadPage::addDownloadTask(QString softPackageName)
{
    downloadNavBtn->flashBackgroud();
    //由softId得到该软件的文件名与下载地址
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(hostUrl+"/LAssistant/api/getDownloadInfoJson.php?softPackageName="+softPackageName)));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray jsonData = reply->readAll();
//qDebug()<<jsonData;
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj = doc.object();
        QString softName = rootObj["softName"].toString();
        QString softVersionName = rootObj["softVersionName"].toString();
        QString downloadUrl = rootObj["downloadUrl"].toString();
        downloadManagerWidget->addDownloadTask(softName+softVersionName,downloadUrl,QDir::homePath()+"/.LAssistant/tmp/");

    }

}

QString SoftwareDownloadPage::getDirectUrl(QString url)
{
    int tryTimes=5;
    //尝试tryTimes次
    QString realUrl_temp = url;
    while(tryTimes --)
    {
        QEventLoop loop;
        QNetworkReply *reply = manager->head(QNetworkRequest(realUrl_temp));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QVariant varLocation = reply->header(QNetworkRequest::LocationHeader);
        //qDebug()<<reply->rawHeaderList();
        if(!varLocation.isNull())
        {
            realUrl_temp = varLocation.toString();
            //qDebug()<<realUrl_temp;
            continue;
        }
        delete reply;
        break;
    }
    url = realUrl_temp;
    return url;
}

void SoftwareDownloadPage::pageChange(int page)
{
    getSoftJsonFromServer(this->requestId,page);
}

