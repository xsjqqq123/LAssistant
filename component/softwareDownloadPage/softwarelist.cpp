#include "softwarelist.h"

SoftwareList::SoftwareList(QWidget *parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidgetResizable(true);
    areaWidget = new QWidget(this);
    areaWidget->setObjectName("transparentWidget");
    setWidget(areaWidget);
    areaWidgetLayout = new QGridLayout(areaWidget);
    areaWidgetLayout->setAlignment(Qt::AlignTop);
    downBtnsGroup = new QButtonGroup(this);
    pageNum = 1;
    connect(downBtnsGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(emitDownloadSignal(QAbstractButton*)));
}

SoftwareList::~SoftwareList()
{

}

void SoftwareList::prePage()
{
    this->pageNum--;
    emit pageChange(this->pageNum);
}
void SoftwareList::nextPage()
{
    this->pageNum++;
    emit pageChange(this->pageNum);
}
void SoftwareList::setList(QByteArray jsonData)
{
    //将json转成软件列表的形式
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,error);
    if(error->error==QJsonParseError::NoError)
    {
        if(doc.isObject())
        {
            int row = 0;
            int column = 0;
            int timeDelayLoadImg = 0;
            QJsonObject obj=doc.object();
            QJsonArray dataArray=obj["data"].toArray();
            foreach (QJsonValue dataChildv, dataArray) {
                QJsonObject dataChildObj = dataChildv.toObject();
                QString typeStr = dataChildObj.keys().first();
//                qDebug()<<typeStr;
                QJsonArray typeListA = dataChildObj[typeStr].toArray();

                QLabel *lbl = new QLabel("<font size='4'>"+typeStr+"</font>");
                areaWidgetLayout->addWidget(lbl,row,column,1,4);
                row ++;
                foreach (QJsonValue dataChildv, typeListA) {
                    QJsonObject typeObj = dataChildv.toObject();
//                    qDebug()<<typeObj;
                    QString softId = typeObj["softId"].toString();
                    QString softPackageName = typeObj["softPackageName"].toString();
                    QString softName = typeObj["softName"].toString();
                    QString softVersionName = typeObj["softVersionName"].toString();
                    QString softCompany = typeObj["softCompany"].toString();
                    QString softDescription = typeObj["softDescription"].toString();
                    QString iconImgUrl = typeObj["iconImgUrl"].toString();
                    //QString downloadCount = typeObj["downloadCount"].toString();

                    QWidget *itemContainer = new QWidget;
                    QGridLayout *itemContainerL = new QGridLayout(itemContainer);
                    itemContainer->setObjectName("softwareItemWidget");
                    //itemContainer->setMaximumWidth(200);
                    itemContainer->setFixedSize(200,130);

                    ImageLabel *iconLbl = new ImageLabel(0,iconImgUrl,timeDelayLoadImg);
                    QLabel *infoLbl = new QLabel(QString(softName+"("+softVersionName+") "));
                    QLabel *softDescriptionLbl = new QLabel("<font color='#ADADAD'>"+softDescription+"</font>");
                    MyhoverButton *dBtn = new MyhoverButton(itemContainer);
                    dBtn->setText("下载");
                    dBtn->setData(softPackageName);
                    dBtn->setObjectName("gradientBtn");
                    iconLbl->setFixedSize(48,48);
                    softDescriptionLbl->setWordWrap(true);
                    softDescriptionLbl->setToolTip(softDescription);
                    softDescriptionLbl->setAlignment(Qt::AlignTop);
                    dBtn->setFixedSize(40,25);
                    dBtn->setFocusPolicy(Qt::NoFocus);
                    downBtnsGroup->addButton(dBtn);

                    itemContainerL->addWidget(iconLbl,0,0,2,2);
                    itemContainerL->addWidget(dBtn,2,0,1,2,Qt::AlignHCenter);
                    itemContainerL->addWidget(infoLbl,0,2,1,3);
                    itemContainerL->addWidget(softDescriptionLbl,1,2,3,3);
                    itemContainerL->addItem(new QSpacerItem(10,10),3,0,1,2);

                    areaWidgetLayout->addWidget(itemContainer,row,column,1,1);
                    if(column == 2)//每行3列
                    {
                        column = 0;
                        row++;
                    }else
                    {
                        column++;
                    }
                    timeDelayLoadImg +=100;//100ms
                }
                column = 0;
                row ++;
            }
            int total = obj["total"].toInt();
            //qDebug()<<obj["total"]<<total;
            if(total>0)
            {
                int page = obj["page"].toInt();
                this->pageNum = page;


                int rowIndex = areaWidgetLayout->rowCount();
                if(page>1)
                {
                    QPushButton *preBtn = new QPushButton("上一页");
                    areaWidgetLayout->addWidget(preBtn,rowIndex,0,1,1);
                    connect(preBtn,SIGNAL(clicked()),this,SLOT(prePage()));
                }
                QLabel *lbl = new QLabel("第"+QString::number(page)+"/"+QString::number(total/18+1)+"页  "+"共"+QString::number(total)+"条");
                lbl->setAlignment(Qt::AlignHCenter);
                areaWidgetLayout->addWidget(lbl,rowIndex,1,1,1);
                if(page<(total/18+1))
                {
                    QPushButton *nextBtn = new QPushButton("下一页");
                    areaWidgetLayout->addWidget(nextBtn,rowIndex,2,1,1);
                    connect(nextBtn,SIGNAL(clicked()),this,SLOT(nextPage()));
                }


            }
        }

    }else
    {
        qDebug()<<"SoftwareList::setList:"<<error->errorString();
    }

}

void SoftwareList::emitDownloadSignal(QAbstractButton *btn)
{
    MyhoverButton *btn2 = qobject_cast<MyhoverButton*>(btn);
    if(btn2 != Q_NULLPTR)
    {
        btn2->setEnabled(false);
        emit downloadSignal(btn2->getData());
//        qDebug()<<btn2->getData();
    }

}

void SoftwareList::clearItems()
{
    while(areaWidgetLayout->count() > 0)
    {
        QWidget* widget = areaWidgetLayout->itemAt(0)->widget();
        areaWidgetLayout->removeWidget(widget);
        delete widget;
    }
}
