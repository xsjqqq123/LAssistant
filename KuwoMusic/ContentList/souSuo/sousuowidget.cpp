#include "sousuowidget.h"
#include <QMovie>
#include <QDebug>
SouSuoWidget::SouSuoWidget(QWidget *parent) :
    QWidget(parent)
{
    mainGLayout = new QGridLayout(this);

    searchHboxLayout = new QHBoxLayout;
    searchEdit = new QLineEdit(this);
    searchBtn = new QPushButton(this);
    busyIndicator = new QLabel(this);
    QMovie *movie = new QMovie(":/app/skin/imgs/loading.gif");
    movie->start();
    musicListWidget = new MusicListWidget(this,true);

    searchEdit->setFixedSize(150,30);
    searchBtn->setFixedSize(30,30);
    searchBtn->setFlat(true);
    busyIndicator->setFixedSize(30,30);
    busyIndicator->setMovie(movie);
    busyIndicator->setVisible(false);
    searchBtn->setIconSize(QSize(30,30));
    searchBtn->setIcon(QIcon(":/kuwoMusic/KuwoMusic/imgs/search.png"));

    searchHboxLayout->setMargin(0);
    searchHboxLayout->setSpacing(0);
    searchHboxLayout->addWidget(searchEdit);
    searchHboxLayout->addWidget(searchBtn);

    //mainGLayout->addItem(new QSpacerItem(100,10,QSizePolicy::Expanding,QSizePolicy::Minimum),0,0,1,2);


    mainGLayout->addWidget(busyIndicator,0,0,1,1);
    mainGLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum),0,1,3,3);
    mainGLayout->addLayout(searchHboxLayout,0,4,1,1);
    mainGLayout->addWidget(musicListWidget,1,0,5,5);

    connect(searchBtn,SIGNAL(clicked()),this,SLOT(searchKeywordAct()));
    connect(searchEdit,SIGNAL(returnPressed()),this,SLOT(searchKeywordAct()));
    connect(musicListWidget,SIGNAL(pageChange(QString)),this,SLOT(handlePageChange(QString)));
}

void SouSuoWidget::search(QString keyword,QString pageNumIndex)
{
    busyIndicator->setVisible(true);
    this->keyword = keyword;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(QString("http://search.kuwo.cn/r.s?all=%0&ft=music&newsearch=1&itemset=web_2013&client=kt&cluster=0&pn=%1&rn=12&rformat=json&encoding=utf8&_=1419135484118")
                                                             .arg(keyword).arg(pageNumIndex))));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(6*1000,&loop,SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    data.replace("\'","\"");
    //qDebug()<<data;

    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(data,error);
    musicListWidget->clearAllItem();
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj=doc.object();
        QString TOTAL = rootObj["TOTAL"].toString();
        musicListWidget->setTitleText(QString("共找到 %0 条与 %1 相关的结果").arg(TOTAL).arg(keyword));

        double totalCount = TOTAL.toDouble()+1;
        QString totalPageNum;
        if((double(totalCount/12.0) - int(totalCount/12))>0)
        {
            totalPageNum = QString::number(int(totalCount/12)+1);
        }else
        {
            totalPageNum = QString::number(int(totalCount/12));
        }
        musicListWidget->setPageInfo(QString::number(pageNumIndex.toInt()+1),totalPageNum);
        QJsonArray abslistArray = rootObj["abslist"].toArray();
        int rowId = 1;
        foreach (QJsonValue v, abslistArray) {
            QJsonObject songSingleObj = v.toObject();
            QString MUSICRID = songSingleObj["MUSICRID"].toString();
            QString SONGNAME = songSingleObj["SONGNAME"].toString();
            QString ARTIST = songSingleObj["ARTIST"].toString();
            musicListWidget->addItem(MUSICRID,QString::number(rowId),SONGNAME,ARTIST);

            rowId++;
        }
    }else
    {
        musicListWidget->setTitleText(QString("共找到 0 条与 %1 相关的结果").arg(keyword));
        qDebug()<<"SouSuoWidget::search():"<<error->errorString();
    }
    busyIndicator->setVisible(false);
}

void SouSuoWidget::searchKeywordAct()
{
    QString keyword = searchEdit->text();
    if(keyword.isEmpty())
    {
        searchEdit->setText("请输入关键字");
        return;
    }
    this->search(searchEdit->text(),"0");
}

void SouSuoWidget::handlePageChange(QString page)
{
    QString pageIndex = QString::number(page.toInt()-1);
    search(this->keyword,pageIndex);
}
