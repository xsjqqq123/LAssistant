#include "phonefilemanagerpage.h"
#include <QDebug>
DirWidget::DirWidget(QWidget *parent):
    QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    pathBtnGroup = new QButtonGroup(this);
    connect(pathBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(pathChange(int)));
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,10,0,10);

    dirPath = "/";
    drawPathBtns();
}

void DirWidget::drawPathBtns()
{
    while(mainLayout->count() > 0)
    {
        QWidget* widget = mainLayout->itemAt(0)->widget();
        mainLayout->removeWidget(widget);
        delete widget;
    }
    QStringList dirPathList = dirPath.split("/");
    dirPathList.removeAll("");
    int id = 0;

    QPushButton *btnRoot = new QPushButton("/");
    btnRoot->setIcon(QIcon(":/fileManager/skin/imgs/fileIcons/harddrive.png"));
    btnRoot->setObjectName("phoneFileManagerPathBtns");
    //btn->setMaximumWidth(100);
    btnRoot->setMinimumHeight(30);
    pathBtnGroup->addButton(btnRoot,id);
    mainLayout->addWidget(btnRoot);

    foreach (QString name, dirPathList) {
        id++;
        QPushButton *btn = new QPushButton(name);
        btn->setObjectName("phoneFileManagerPathBtns");
        //btn->setMaximumWidth(100);
        btn->setMinimumHeight(30);
        pathBtnGroup->addButton(btn,id);
        mainLayout->addWidget(btn);
    }
    mainLayout->addStretch();

}

void DirWidget::setPath(QString dirPath)
{
    this->dirPath = dirPath;
    drawPathBtns();
}

QString DirWidget::getPath()
{
    return this->dirPath;
}

void DirWidget::pathChange(int btnId)
{
    QString newDirPath;
    QStringList dirPathList = this->dirPath.split("/");
    dirPathList.removeAll("");
    int idMark = 0;//
    foreach (QString a, dirPathList) {
        if(btnId == idMark)
            break;
        newDirPath+=a;
        newDirPath+="/";
        idMark++;

    }
    newDirPath.prepend("/");
    this->dirPath = newDirPath;
    emit pathChangeSignal(newDirPath);
    drawPathBtns();
    //qDebug()<<newDirPath;
}

PhoneFileManagerPage::PhoneFileManagerPage(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);
    initUi();
    isImgItemChange = false;
    fileListWidget->installEventFilter(this);

}

void PhoneFileManagerPage::initUi()
{
    mainGridL = new QGridLayout(this);
    dirWidget = new DirWidget(this);
    fileListWidget = new QListWidget(this);
    addFileBtn = new QPushButton("添加文件",this);
    addFileBtn->setObjectName("gradientBtn");
    adbController = new AdbController(this,false);

    addFileBtn->setFixedSize(80,30);
    addFileBtn->setEnabled(false);

    fileListWidget->setObjectName("fileListWidget");
    fileListWidget->setDragEnabled(true);
    fileListWidget->setAcceptDrops(true);
    fileListWidget->setDefaultDropAction(Qt::MoveAction);
    fileListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    fileListWidget->setViewMode(QListView::IconMode);
    fileListWidget->setResizeMode(QListView::Adjust);
    fileListWidget->setWrapping(true);
    fileListWidget->setWordWrap(true);
    fileListWidget->setIconSize(QSize(50,50));
    fileListWidget->setMovement(QListView::Free);
    fileListWidget->setSpacing(10);
    fileListWidget->setEditTriggers(QAbstractItemView::SelectedClicked);
    fileListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    mainGridL->addWidget(dirWidget,0,0,1,1);
    mainGridL->addWidget(addFileBtn,0,1,1,1);
    mainGridL->addWidget(fileListWidget,1,0,1,2);

    connect(dirWidget,SIGNAL(pathChangeSignal(QString)),this,SLOT(setPath(QString)));
    connect(addFileBtn,SIGNAL(clicked()),this,SLOT(addFileToMobile()));
    connect(fileListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClickedSlot(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(itemChangedSlot(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(fileListWidgetItemDBclick(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequestedSlot(QPoint)));
}

bool PhoneFileManagerPage::isNotAllowEdit()
{
    //
    QString b = dirWidget->getPath();
    if(b == "/" || b=="/mnt/"||b=="/storage/")
    {
        return true;
    }else
    {
        return false;
    }
}

bool PhoneFileManagerPage::eventFilter(QObject *target, QEvent *event)//
{
    if (target == fileListWidget) {
        if (event->type() == QEvent::DragEnter) {
            QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
            dee->acceptProposedAction();
            return true;
        } else if (event->type() == QEvent::Drop) {
            QDropEvent *de = dynamic_cast<QDropEvent *>(event);
            QList<QUrl> urls = de->mimeData()->urls();

            if (urls.isEmpty()) { return true; }

            QStringList fileList;
            foreach (QUrl url, urls) {
                fileList.append(url.toLocalFile());
            }
            QString savePath = dirWidget->getPath();
            FileTransferToPhoneWidget *w = new FileTransferToPhoneWidget(NULL,this->currentDeviceSerialNumber,fileList,savePath,1);

            w->setTitleStr("当前传输：电脑-->手机");
            w->show();
            connect(w, SIGNAL(refreshFileList()), this, SLOT(refreshListWidget()));
            w->startTask();

            return true;
        }
    }

    return QWidget::eventFilter(target, event);
}

void PhoneFileManagerPage::setCurrentDevice(QString serialNumber)
{
    this->currentDeviceSerialNumber = serialNumber;
    setPath("/");
    addFileBtn->setEnabled(false);
}

void PhoneFileManagerPage::setPath(QString dirPath)
{
    fileListWidget->clear();
    dirWidget->setPath(dirPath);
    if(isNotAllowEdit())
    {
        addFileBtn->setEnabled(false);
    }else
    {
        addFileBtn->setEnabled(true);
    }
    QString fileOrDirData = adbController->getDirOrFileList(this->currentDeviceSerialNumber,dirPath);
    //qDebug()<<fileOrDirData;
    if(fileOrDirData.isEmpty())
    {
        return;
    }
    QStringList dirList;
    QStringList fileList;
    QTextStream tS(&fileOrDirData);
    //
    while (!tS.atEnd()) {
        QString line = tS.readLine();
        QString name;//保存文件或目录名
        if(line.startsWith("d"))//目录
        {
            name = line.split(":").value(1,"abc").replace(0,3,"");
            if(dirWidget->getPath() == "/")//删除无关目录，防误操作
            {
                if(!(name.startsWith("mnt") || name.startsWith("storage")))
                {
                    continue;
                }
            }
            if(dirWidget->getPath() == "/mnt/" && !name.startsWith("sdcard"))//删除无关目录，防误操作
            {
                continue;
            }
            dirList.append(name);
        }else//文件
        {
            if(dirWidget->getPath() == "/" || dirWidget->getPath() == "/mnt/" || dirWidget->getPath() == "/mnt/")
            {
                continue;
            }
            name = line.split(":").value(1,"abc").replace(0,3,"");
            fileList.append(name);
        }

    }
    dirList.sort();
    fileList.sort();

    foreach (QString dir, dirList) {
        QString itemIconPath;
        QListWidgetItem *item = new QListWidgetItem;

        if(dir.startsWith("sdcard"))
        {
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            itemIconPath = ":/fileManager/skin/imgs/fileIcons/harddrive.png";
        }else if(dir.startsWith("storage"))
        {
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            itemIconPath = ":/fileManager/skin/imgs/fileIcons/folder.png";
        }else
        {
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable);
            itemIconPath = ":/fileManager/skin/imgs/fileIcons/folder.png";
        }
        item->setSizeHint(QSize(100,100));
        QPixmap pix(itemIconPath);
        pix = pix.scaled(100,80,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        item->setData(Qt::UserRole,"dir");
        item->setToolTip(dir);
        item->setIcon(QIcon(pix));
        item->setText(dir);
        fileListWidget->addItem(item);

    }

    QStringList iconSuffixList;
    iconSuffixList.append("svg");
    iconSuffixList.append("png");
    foreach (QString file, fileList) {
        QString itemIconPath;
        QListWidgetItem *item = new QListWidgetItem;
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable);
        QString suffix = file.split(".").last();
        foreach (QString iconSuffix, iconSuffixList) {
            itemIconPath = QString(":/fileManager/skin/imgs/fileIcons/%0.%1").arg(suffix).arg(iconSuffix);
            if(QFileInfo(itemIconPath).exists())
            {
                break;
            }else
            {
                itemIconPath = QString(":/fileManager/skin/imgs/fileIcons/unknown.svg");
                continue;
            }
        }
        item->setSizeHint(QSize(100,120));
        QPixmap pix(itemIconPath);
        pix = pix.scaled(100,80,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        item->setData(Qt::UserRole,"file");
        item->setToolTip(file);
        item->setIcon(QIcon(pix));
        item->setText(file);
        fileListWidget->addItem(item);
    }
    //图片预览功能
//    qDebug()<<"图片预览功能";
    QList<QListWidgetItem *> imgItemList = fileListWidget->findItems(".*jpg|.*png|.*bmp",Qt::MatchRegExp);
    foreach (QListWidgetItem *imgItem, imgItemList) {
        if(dirWidget->getPath() != dirPath || imgItem==Q_NULLPTR)//防止中途换目录
            break;
        QString sdcardPath = dirPath+imgItem->text();

        QString imgTmpPath = QDir::homePath()+"/.LAssistant/tmp/"+imgItem->text();
        if(!QFileInfo(imgTmpPath).exists())
        {
            adbController->pullFileToPc(this->currentDeviceSerialNumber,sdcardPath,imgTmpPath);
        }
        QPixmap pix(imgTmpPath);
        pix = pix.scaled(100,80,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        if(dirWidget->getPath() != dirPath || imgItem==Q_NULLPTR)//防止中途换目录
            break;
        imgItem->setIcon(QIcon(pix));
        isImgItemChange = true;

    }
}

void PhoneFileManagerPage::fileListWidgetItemDBclick(QListWidgetItem *item)
{
    if(item->data(Qt::UserRole).toString() == "dir")
    {
        setPath(dirWidget->getPath()+item->text()+"/");
    }
}

void PhoneFileManagerPage::itemClickedSlot(QListWidgetItem *item)
{
    this->storeItemText = item->text();
}

void PhoneFileManagerPage::itemChangedSlot(QListWidgetItem *item)
{
    if(isImgItemChange == true)//图片项变化不调用
    {
        isImgItemChange = false;
        return;
    }
    QString prePath = this->dirWidget->getPath() + storeItemText;
    QString afterPath = this->dirWidget->getPath() + item->text();
    if(prePath != afterPath)
        adbController->renameFileOrDir(this->currentDeviceSerialNumber,prePath,afterPath);
}

void PhoneFileManagerPage::customContextMenuRequestedSlot(QPoint p)
{
    if(isNotAllowEdit())
    {
        return;
    }
    //QListWidgetItem * item = fileListWidget->itemAt(p);
    QList<QListWidgetItem *> items =  fileListWidget->selectedItems();
    if(items.count()>0)
    {
        //qDebug()<<"某项上右击";
        QMenu *menu = new QMenu;
        QAction *previewAct = new QAction("预览",menu);
        QAction *transferToPcAct = new QAction("传输到电脑",menu);
        QAction *deleteAct = new QAction("删除该项",menu);
        if(items.count()==1)
        {
            QListWidgetItem *item = items.first();
            if(item->data(Qt::UserRole).toString() != "dir")
                menu->addAction(previewAct);
        }

        menu->addAction(transferToPcAct);
        menu->addAction(deleteAct);
        QAction *reAct = menu->exec(QCursor::pos());

        if(reAct == transferToPcAct)
        {
            QStringList fileList;
            foreach (QListWidgetItem * item, items) {
                fileList.append(dirWidget->getPath()+item->text());
            }
            if(tmpSavePath.isEmpty())
            {
                QString dir = QFileDialog::getExistingDirectory(this, tr("选择本次会话默认保存文件目录"),
                                                                QDir::homePath(),
                                                                QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);
                if(!dir.isEmpty())
                {
                    tmpSavePath = dir;
                }else
                {
                    return;
                }
            }
            FileTransferToPhoneWidget *w = new FileTransferToPhoneWidget(0,this->currentDeviceSerialNumber,fileList,tmpSavePath,2);

            w->setTitleStr("当前传输：手机-->电脑");
            w->show();
            w->startTask();
        }else if(reAct == deleteAct)
        {
            foreach (QListWidgetItem * item, items) {
                if(item->data(Qt::UserRole).toString()=="file")
                {
                    adbController->deleteFile(this->currentDeviceSerialNumber,dirWidget->getPath()+item->text());
                }else//dir
                {
                    adbController->deleteDir(this->currentDeviceSerialNumber,dirWidget->getPath()+item->text());
                }
                fileListWidget->removeItemWidget(item);
                delete item;
            }
        }else if(reAct == previewAct)
        {
            QListWidgetItem *item = items.first();
            if(item==Q_NULLPTR)//防止中途换目录
                return;
            QString sdcardPath = dirWidget->getPath()+item->text();

            QString tmpPath = QDir::homePath()+"/.LAssistant/tmp/"+item->text();
            adbController->pullFileToPc(this->currentDeviceSerialNumber,sdcardPath,tmpPath);
            QDesktopServices::openUrl(QUrl::fromLocalFile(tmpPath));
        }

    }else
    {
        //qDebug()<<"空白处右击";
        QMenu *menu = new QMenu;
        QAction *mkdirAct = new QAction("新建文件夹",menu);
        menu->addAction(mkdirAct);
        QAction *reAct = menu->exec(QCursor::pos());

        if(reAct == mkdirAct)
        {
            bool ok;
            QString text = QInputDialog::getText(this, tr("请输入文件夹名"),
                                                 tr("名称:"), QLineEdit::Normal,
                                                 "新建文件夹", &ok);
            if (ok && !text.isEmpty())
            {
                bool b = adbController->mkdir(this->currentDeviceSerialNumber,dirWidget->getPath()+"/"+text);
                if(b)
                {
                    refreshListWidget();
                }else
                {
                    QMessageBox::warning(this,"提示","创建失败，请检查文件夹名称。");
                }
            }
        }
    }
}

void PhoneFileManagerPage::addFileToMobile()
{
    QStringList fileList = QFileDialog::getOpenFileNames(
                this,
                "选择需要传输到手机的文件",
                QDir::homePath());
    if(fileList.count()==0)
    {
        return;
    }
    QString savePath = dirWidget->getPath();
    FileTransferToPhoneWidget *w = new FileTransferToPhoneWidget(NULL,this->currentDeviceSerialNumber,fileList,savePath,1);
    w->setTitleStr("当前传输：电脑-->手机");
    w->show();
    connect(w, SIGNAL(refreshFileList()), this, SLOT(refreshListWidget()));
    w->startTask();
}

void PhoneFileManagerPage::refreshListWidget()
{
    setPath(dirWidget->getPath());
}

void PhoneFileManagerPage::resetToDefault()
{
    fileListWidget->clear();
    dirWidget->setPath("/");
}
