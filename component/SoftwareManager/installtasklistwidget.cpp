#include "installtasklistwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
InstallTaskListWidget::InstallTaskListWidget(QWidget *parent, QString serialsNumber, QStringList filepathList, QString arg) :
    QWidget(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Tool|Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMinimumWidth(300);
    QVBoxLayout *l = new QVBoxLayout(this);
    containerWidget = new QWidget(this);
    containerWidget->setObjectName("showWidget");
    l->setMargin(0);
    l->setSpacing(0);

    l->addWidget(containerWidget);
    winLayout = new QVBoxLayout(containerWidget);
    titleBar = new TitleBar(this,false,false,true);
    mainGLayout = new QGridLayout;
    mainGLayout->setMargin(5);
    mainGLayout->setSpacing(3);
    titleBar->setObjectName("titleBar");
    titleBar->setTitleStr("正在安装软件到手机");
    winLayout->setAlignment(Qt::AlignTop);
    winLayout->setMargin(0);
    winLayout->setSpacing(0);
    winLayout->addWidget(titleBar);
    winLayout->addLayout(mainGLayout);
    adbController = new AdbController(this);
    this->serialsNumber = serialsNumber;
    this->filepathList.append(filepathList);
    this->arg = arg;
    int row = 0;
    foreach (QString file, filepathList) {
        QLabel *fileNameLbl = new QLabel(QFileInfo(file).fileName());
        QLabel *statusLbl = new QLabel("--等待中--");
        mainGLayout->addWidget(fileNameLbl,row,0);
        mainGLayout->addWidget(statusLbl,row,1);
        statusLblList.append(statusLbl);
        row ++;
    }
    isCancel = false;
    isFinished = false;

    connect(titleBar,SIGNAL(closeWinSignal()),this,SLOT(close()));
}

void InstallTaskListWidget::closeEvent(QCloseEvent *event)
{
    if(!isCancel)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "询问", "你想取消本次安装吗？\n这将会等待当前任务结束，并取消后续任务。", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            isCancel = true;
        }
        event->ignore();
        return;
    }
    if(isCancel && !isFinished)
    {
        QMessageBox::warning(NULL,"提示","请等待正在安装的任务结束",QMessageBox::Ok);
        event->ignore();
        return;
    }
    emit finished();
}

void InstallTaskListWidget::startTask()
{
    int row = 0;
    foreach (QString file, this->filepathList) {
        statusLblList.value(row)->setText("--正在安装--");
        bool b = adbController->installApkToPhone(this->serialsNumber,file,this->arg);
        if(b)
        {
            statusLblList.value(row)->setText("--安装成功--");
        }else
        {
            statusLblList.value(row)->setText("--<font color:red>安装失败！</font>--");
        }
        if(isCancel)
        {
            for(int i = (row+1);i<statusLblList.count();i++)
            {
                statusLblList.value(i)->setText("--取消任务！--");
            }
            isFinished = true;
            return;
        }
        row ++;
    }
    isCancel = true;
    isFinished = true;
}
void InstallTaskListWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint temp = e->pos();
    const int pos_min_x = titleBar->pos().x();
    const int pos_max_x = pos_min_x + titleBar->width();
    const int pos_min_y = titleBar->pos().y();
    const int pos_max_y = pos_min_y + titleBar->height();
    if ( temp.x()>pos_min_x && temp.x()<pos_max_x && temp.y()>pos_min_y && temp.y()<pos_max_y )
    {

        //只对标题栏范围内的鼠标事件进行处理
        QPoint newpos = e->globalPos();
        QPoint upleft = pos0 + newpos - last;
        move(upleft);
    }
}

void InstallTaskListWidget::mousePressEvent(QMouseEvent *e)
{
    QPoint temp = e->pos();
    const int pos_min_x = titleBar->pos().x();
    const int pos_max_x = pos_min_x + titleBar->width();
    const int pos_min_y = titleBar->pos().y();
    const int pos_max_y = pos_min_y + titleBar->height();
    if ( temp.x()>pos_min_x && temp.x()<pos_max_x && temp.y()>pos_min_y && temp.y()<pos_max_y )
    {

        //只对标题栏范围内的鼠标事件进行处理。
        last = e->globalPos();
        pos0 = e->globalPos() - e->pos();
    }
}
