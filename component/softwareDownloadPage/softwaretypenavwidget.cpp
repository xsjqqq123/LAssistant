#include "softwaretypenavwidget.h"

SoftwareTypeNavWidget::SoftwareTypeNavWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
    containerWL = new QGridLayout(this);
    containerW = new QWidget(this);
    containerWL->setMargin(0);
    containerWL->setSpacing(0);
    containerWL->addWidget(containerW);
    containerW->setObjectName("showWidget");
    gridLayout = new QGridLayout(containerW);
    btnsGroup = new QButtonGroup(this);
    connect(btnsGroup,SIGNAL(buttonClicked(int)),this,SLOT(emitTypeChange(int)));

}

SoftwareTypeNavWidget::~SoftwareTypeNavWidget()
{

}/*

void SoftwareTypeNavWidget::paintEvent(QPaintEvent *)
{
    QSettings settings("LAssistant", "xusongjie");
    settings.beginGroup("MainWindow");
    QString imgPath = settings.value("backgroudImg").toString();
    settings.endGroup();
    if(!QFileInfo(imgPath).exists())
        imgPath = ":/app/skin/imgs/mainWindow/grey.png";

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
//    painter.setBrush(QBrush(QColor(255,255,255,255)));
//    painter.drawRoundedRect(this->rect(),3,3);
    painter.drawImage(this->rect(),QImage(imgPath),this->rect(),Qt::MonoOnly);
    QPen pen(QColor(10,10,10,60));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(this->rect());
}*/

void SoftwareTypeNavWidget::leaveEvent(QEvent *)
{
    this->hide();
}

void SoftwareTypeNavWidget::setList(QByteArray jsonData)
{
    //将json转换成列表
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,error);
    if(error->error==QJsonParseError::NoError)
    {
        if(doc.isObject())
        {
            int row = 0;
            int column = 0;
            QJsonObject obj=doc.object();
            QJsonArray dataArray=obj["data"].toArray();
            foreach (QJsonValue dataChildv, dataArray) {
               QJsonObject dataChildObj = dataChildv.toObject();
               QString typeStr = dataChildObj.keys().first();
               //qDebug()<<typeStr;
               QJsonArray typeListA = dataChildObj[typeStr].toArray();

               QLabel *lbl = new QLabel(typeStr);
               gridLayout->addWidget(lbl,row,column,1,4);
               row ++;
               foreach (QJsonValue dataChildv, typeListA) {
                   QJsonObject typeObj = dataChildv.toObject();
                   QString name = typeObj["name"].toString();
                   QString requestId = typeObj["requestId"].toString();
                   QPushButton *btn = new QPushButton(name);
                   btnsGroup->addButton(btn,requestId.toInt());
                   gridLayout->addWidget(btn,row,column,1,1);
                   if(column == 3)
                   {
                       column = 0;
                       row++;
                   }else
                   {
                       column++;
                   }
               }
               column = 0;
               row ++;
            }
        }

    }else
    {
        qDebug()<<"SoftwareTypeNavWidget::setList:"<<error->errorString();
    }

}

void SoftwareTypeNavWidget::emitTypeChange(int typeCode)
{
    this->hide();
    emit typeChange(typeCode,1);
}

