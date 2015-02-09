#include "filetransfertophonewidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

FileTransferToPhoneWidget::FileTransferToPhoneWidget(QWidget *parent, QString serialsNumber, QStringList filepathList, QString savePath, int type)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMinimumWidth(350);
    setMinimumHeight(80);

    topLayout = new QVBoxLayout(this);
    topLayout->setMargin(0);
    topLayout->setSpacing(0);
    container = new QWidget;
    topLayout->addWidget(container);

    container->setObjectName("showWidget");

    winLayout = new QVBoxLayout(container);
    winLayout->setAlignment(Qt::AlignTop);
    winLayout->setMargin(0);
    winLayout->setSpacing(0);
    titleBar = new TitleBar(this,false,false,true);
    titleBar->setObjectName("titleBar");
    winLayout->addWidget(titleBar);
    connect(titleBar,SIGNAL(closeWinSignal()),this,SLOT(close()));

    mainGLayout = new QGridLayout;
    winLayout->addLayout(mainGLayout);
    adbController = new AdbController(this);
    this->serialsNumber = serialsNumber;
    this->filepathList.append(filepathList);
    this->savePath = savePath;
    this->type = type;
    int row = 0;
    foreach (QString file, filepathList) {
        QLabel *fileNameLbl = new QLabel(QFileInfo(file).fileName());
        QLabel *statusLbl = new QLabel("--等待中--");//

        fileNameLbl->setAlignment(Qt::AlignLeft);
        statusLbl->setAlignment(Qt::AlignHCenter);
        fileNameLbl->setWordWrap(true);
        fileNameLbl->setMargin(3);
        fileNameLbl->adjustSize();
        mainGLayout->addWidget(fileNameLbl,row,0);
        mainGLayout->addWidget(statusLbl,row,1);
        statusLblList.append(statusLbl);
        row ++;
    }
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    isCancel = false;
    isFinished = false;
}

void FileTransferToPhoneWidget::closeEvent(QCloseEvent *event)
{
    if(!isCancel)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "询问", "你想取消本次传送吗？\n这将会等待当前正在传输的任务结束，并取消后续任务。", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            isCancel = true;
        }
        event->ignore();
        return;
    }
    if(isCancel && !isFinished)
    {
        QMessageBox::warning(this,"提示","请等待正在传输的任务结束",QMessageBox::Ok);
        event->ignore();
        return;
    }
    emit finished();
}

void FileTransferToPhoneWidget::mouseMoveEvent(QMouseEvent *e)
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

void FileTransferToPhoneWidget::mousePressEvent(QMouseEvent *e)
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

void FileTransferToPhoneWidget::setTitleStr(QString txt)
{
    titleBar->setTitleStr(txt);
}

void FileTransferToPhoneWidget::startTask()
{
    int row = 0;
    foreach (QString file, filepathList) {

        statusLblList.value(row)->setText("--正在传输--");
        int b;
        if(this->type == 1)
        {
            if(QFileInfo(file).isDir())
            {
                t = new TransferThread(this,QString("adb -s %0 push \"%1\" \"%2\"").arg(this->serialsNumber,file,savePath+"/"+QFileInfo(file).fileName()));
            }else
            {
                t = new TransferThread(this,QString("adb -s %0 push \"%1\" \"%2\"").arg(this->serialsNumber,file,savePath));
            }

            //b = adbController->pushFileToPhone(serialsNumber,file,savePath);
        }else
        {
            t = new TransferThread(this,QString("adb -s %0 pull \"%1\" \"%2\"").arg(this->serialsNumber,file,savePath));
            //b = adbController->pullFileToPc(serialsNumber,file,savePath);
        }
        connect(t,SIGNAL(finished()),&loop,SLOT(quit()));
        t->start();
        loop.exec();
        b = t->getTransferResultMark();
        delete t;
        if(b==0)
        {
            statusLblList.value(row)->setText("--传输成功--");
        }else
        {
            statusLblList.value(row)->setText("--传输失败！--");
        }
        if(this->type == 1)
        {
            emit refreshFileList();
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
