#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(830,600);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/app/skin/imgs/logo.png"));

    containerW = new QWidget(this);
    mainLayout = new QVBoxLayout(containerW);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    titleBar = new TitleBar(this);
    titleBar->setObjectName("titleBar");
    cWidget = new CentralWidget(this);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(cWidget);
    setCentralWidget(containerW);

    connect(titleBar,SIGNAL(minimumWinSignal()),this,SLOT(hide()));
    connect(titleBar,SIGNAL(closeWinSignal()),this,SLOT(close()));
    connect(titleBar,SIGNAL(settingSignal()),this,SLOT(popupMenu()));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/3);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/app/skin/imgs/logo.png"));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    QSettings settings("LAssistant", "xusongjie");
    settings.beginGroup("MainWindow");
    QString imgPath = settings.value("backgroudImg").toString();
    setSkinBackground(imgPath);
    settings.endGroup();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setSkinBackground(QString imgPath)
{
    if(!QFileInfo(imgPath).exists())
        imgPath = ":/app/skin/imgs/mainWindow/grey.png";
    QFile file(":/qss/skin/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString str = file.readAll();
    qApp->setStyleSheet(str.replace("backgroudImg",imgPath));

    QSettings settings("LAssistant", "xusongjie");
    settings.beginGroup("MainWindow");
    settings.setValue("backgroudImg",imgPath);
    settings.endGroup();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
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

void MainWindow::mousePressEvent(QMouseEvent *e)
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

void MainWindow::popupMenu()
{
    QMenu *menu = new QMenu;

    QMenu *skinMenu = menu->addMenu("背景换肤");
    QAction *skinGreyAct = new QAction("唯美灰",menu);
    QAction *skinPurpleAct = new QAction("温暖紫",menu);
    QAction *skinSelectAct = new QAction("自定义背景",menu);
    skinMenu->addAction(skinGreyAct);
    skinMenu->addAction(skinPurpleAct);
    skinMenu->addAction(skinSelectAct);

    QAction *clearCacheAct = new QAction("清空缓存",menu);

    QAction *aboutAct = new QAction("关于",menu);
    menu->addMenu(skinMenu);
    menu->addAction(clearCacheAct);
    menu->addAction(aboutAct);

    QAction *reAct = menu->exec(QCursor::pos());
    if(reAct == Q_NULLPTR)
    {
        return;//
    }
    QSettings settings("LAssistant", "xusongjie");

    settings.beginGroup("MainWindow");
    if(reAct == skinGreyAct)
    {
        setSkinBackground(":/app/skin/imgs/mainWindow/grey.png");
    }else if(reAct == skinPurpleAct)
    {
        setSkinBackground(":/app/skin/imgs/mainWindow/purple.png");
    }else if(reAct == skinSelectAct)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("选择背景图"),
                                                        QDir::homePath(),
                                                        tr("Images (*.png *.bmp *.jpg)"));
        if(fileName!="")
            setSkinBackground(fileName);
    }else if (reAct == aboutAct) {
        QMessageBox::about(NULL,"关于本软件","软件作者：\n"
                           "xusongjie<376186432@qq.com>\n"
                           "主页：http://mvgather.com\n"
                           "软件版本:1.1");
    }else if(reAct == clearCacheAct)
    {
        QDir d(QDir::homePath()+"/.LAssistant/tmp");
        d.setFilter(QDir::Files);
        int j=d.count()-1;
        for (int i=0;i<=j;i++)
            d.remove(d[i]);

        QDir d2(QDir::homePath()+"/.LAssistant/appsData");
        d2.setFilter(QDir::Files);
        j=d2.count()-1;
        for (int i=0;i<=j;i++)
            d2.remove(d2[i]);
    }
    settings.endGroup();
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason e)
{
    this->show();
}
