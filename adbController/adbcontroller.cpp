#include "adbcontroller.h"


void ConnectionThread::run()
{
    bool first = true;
    QString data;
    this->socket = new QTcpSocket();
    this->socket->connectToHost("127.0.0.1",5037,QTcpSocket::ReadWrite);
    if (this->socket->waitForConnected(10*60*1000))
    {
        this->socket->write("0012host:track-devices");
        this->socket->waitForReadyRead(2000);
        data = this->socket->read(4);
        if (data == "OKAY")
        {
            QRegExp rx_serialNumber(".*\\s");
            rx_serialNumber.setMinimal(true);
            while (true)
            {
                if (!first)
                    this->socket->waitForReadyRead(-1);
                first = false;
                data = this->socket->read(4);
                QByteArray bya = this->socket->readAll();
                QTextStream textStream(&bya);
                QStringList serialNumberStateList;
                while (!textStream.atEnd()) {
                    QString serialNumber,state;
                    QString line = textStream.readLine();
                    if(line.contains("no permissions"))
                    {
                        qDebug()<<"adb服务未root启动";
                        continue;
                    }
                    int i = rx_serialNumber.indexIn(line);
                    if(i>-1)
                    {
                        serialNumber = rx_serialNumber.cap(0).trimmed();
                        if(line.contains("device"))
                        {
                            state = "DEVICE";
                        }else
                        {
                            state = "OFFLINE";
                        }
                    }
                    serialNumberStateList.append(serialNumber+"#"+state);
                }
                emit connectionChanged(serialNumberStateList);
                /*
                data = this->socket->read(4);
                tmp=data;
                if (data == "")
                    continue;
                dataLength = data.toInt(&ok, 16);
                if (dataLength == 0)
                {
                    emit connectionChanged(FASTBOOT,"");
                }
                else
                {
                    data = this->socket->read(dataLength);
                    if (data.contains("device"))
                        serialLength = tmp.toInt(&ok, 16) - 8;
                    if (data.contains("recovery"))
                        serialLength = tmp.toInt(&ok, 16) - 10;

                    serialNumber = data;

                    serialNumber = serialNumber.left(serialLength);
                    if (data.contains("device"))//连接上手机
                        emit connectionChanged(DEVICE,serialNumber);
                    if (data.contains("recovery"))
                        emit connectionChanged(RECOVERY,serialNumber);
                    if (data.contains("offline"))
                        emit connectionChanged(FASTBOOT,serialNumber);
                }*/
            }
        }
        else
        {
            qDebug()<<"adb服务未启动";
            emit connectionChanged(QStringList()<<"adbNotRun");
            return;
        }
    }
    else
    {
        qDebug()<<"adb服务未启动";

        emit connectionChanged(QStringList()<<"adbNotRun");
        return;
    }
}

TransferThread::TransferThread(QWidget *parent, QString commandStr)
{
    this->commandStr=commandStr;
}

void TransferThread::run()
{
//    int i = process.execute(commandStr);
//    if(i == 0)//success
//    {
//        this->transferResultMark = 0;
//    }else
//    {
//        this->transferResultMark = -1;
//    }
    QEventLoop eventloop;
    QProcess process;
    connect(&process,SIGNAL(finished(int)),&eventloop,SLOT(quit()));
    process.start(commandStr);
    eventloop.exec();
    QString re = process.readAll();
    //qDebug()<<"re"<<re;
    if(!re.contains("Failure"))//success
    {
        this->transferResultMark = 0;

    }else
    {
        this->transferResultMark = -1;
        if(re.contains("INSTALL_FAILED_INSUFFICIENT_STORAGE"))
        {
            this->transferResultMark = 1;
            qDebug()<<"设备内存不足";
        }
    }
}

int TransferThread::getTransferResultMark()
{
    return this->transferResultMark;
}

void TransferThread::forceToExit()
{/*
    process.terminate();
    process.waitForFinished();*/
}


AdbController::AdbController(QWidget *parent, bool isThreadNeed) :
    QWidget(parent)
{
    this->adbPath = "adb";
    if(isThreadNeed)
        QTimer::singleShot(1000,this,SLOT(startThread()));
    connect(&connectionWatchThread, SIGNAL(connectionChanged(QStringList)), this, SLOT(slotConnectionChanged(QStringList)));
}

AdbController::~AdbController()
{
    if(this->connectionWatchThread.isRunning())
    {
        this->connectionWatchThread.terminate();
        this->connectionWatchThread.wait(5000);
    }
}

void AdbController::slotConnectionChanged(QStringList serialNumberStateList)
{
    //qDebug()<<serialNumberStateList;
    if(serialNumberStateList.value(0,"123").contains("adbNotRun"))
    {
        QMessageBox::warning(NULL,"提示：","adb服务未启动!程序无法工作。");
        exit(0);
    }
    emit connectionChanged(serialNumberStateList);
}

void AdbController::starServer()
{
    QProcess process;
    if(QFileInfo("/usr/bin/adb").exists())
    {
        process.start("adb",QStringList()<<"kill-server");
        process.waitForStarted(5*1000);
        process.waitForFinished(20*1000);
//        QEventLoop eventloop;
//        QProcess process;
//        connect(&process,SIGNAL(finished(int)),&eventloop,SLOT(quit()));
//        process.start("gksudo",QStringList()<<"-m"<<"adb启动需要root权限以识别手机。请输入密码："<<"adb start-server");
//        eventloop.exec();
        process.execute("gksudo",QStringList()<<"-m"<<"adb启动需要root权限以识别手机。请输入密码："<<"adb start-server");
//        process.start("gksudo",QStringList()<<"-m"<<"adb启动需要root权限以识别手机。请输入密码："<<"adb start-server");
//        process.waitForStarted(5*1000);
//        process.waitForFinished(20*1000);
    }else
    {
        qDebug()<<"adb is not exist";
    }

}

QString AdbController::getDeviceModel(QString serialNumber)
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<"cat /system/build.prop");
    //qDebug()<<process.arguments();
    process.waitForStarted(10*1000);
    process.waitForFinished(20*1000);
    QString re = process.readAll();
    QRegExp rx("ro.product.model=.*\\n");
    rx.setMinimal(true);
    if(rx.indexIn(re)>-1)
    {
        return rx.cap(0).replace(QRegExp("ro.product.model=|\\n"),"");
    }else
    {
        return "未知型号";
    }


}

QStringList AdbController::getPhone_buildProp_Info(QString serialNumber)//返回格式:手机型号 制造商 系统版本 内存大小
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<"cat /system/build.prop");
    process.waitForStarted(10*1000);
    process.waitForFinished(20*1000);
    QString re = process.readAll();
    QRegExp rx1("ro.product.model.*\\n");
    QRegExp rx2("ro.product.manufacturer.*\\n");
    QRegExp rx3("ro.build.version.release.*\\n");
    QRegExp rx4("ro.product.ram.*\\n");
    rx1.setMinimal(true);
    rx2.setMinimal(true);
    rx3.setMinimal(true);
    rx4.setMinimal(true);
    QStringList strList;
    if(rx1.indexIn(re)>-1)
    {
        strList.append(rx1.cap(0).replace(QRegExp("ro.product.model|=|\\n"),""));
    }else
    {
        strList.append("未知");
    }
    if(rx2.indexIn(re)>-1)
    {
        strList.append(rx2.cap(0).replace(QRegExp("ro.product.manufacturer|=|\\n"),""));
    }else
    {
        strList.append("未知");
    }
    if(rx3.indexIn(re)>-1)
    {
        strList.append(rx3.cap(0).replace(QRegExp("ro.build.version.release|=|\\n"),""));
    }else
    {
        strList.append("未知");
    }
    if(rx4.indexIn(re)>-1)
    {
        strList.append(rx4.cap(0).replace(QRegExp("ro.product.ram|=|\\n"),""));
    }else
    {
        strList.append("未知");
    }
    return strList;
}

QStringList AdbController::getMemoryInfo(QString serialNumber)//返回格式： 总内存大小 可用内存
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<"cat /proc/meminfo");
    process.waitForStarted(5*1000);
    process.waitForFinished(20*1000);
    QString re = process.readAll();
    QRegExp rx1("MemTotal:.*kB");
    QRegExp rx2("MemFree:.*kB");
    rx1.setMinimal(true);
    rx2.setMinimal(true);
    QStringList strList;
    if(rx1.indexIn(re)>-1)
    {
        double kb = rx1.cap(0).replace(QRegExp("MemTotal:|kB"),"").trimmed().toDouble();
        strList.append(QString::number(int(kb/1024))+" MB");
    }else
    {
        strList.append("未知");
    }
    if(rx2.indexIn(re)>-1)
    {
        double kb = rx2.cap(0).replace(QRegExp("MemFree:|kB"),"").trimmed().toDouble();
        strList.append(QString::number(int(kb/1024))+" MB");
    }else
    {
        strList.append("未知");
    }
    return strList;
}

bool AdbController::checkRoot(QString serialNumber)
{
//    QProcess process;
//    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<"su");
//    process.waitForStarted(5*1000);
//    process.write("exit\n");
//    process.waitForFinished(1*1000);
//    QString b = process.readAll();
    //qDebug()<<b;
//    if(b.contains("#"))
//    {
//        return true;
//    }else
//    {
//        return false;
//    }
}

void AdbController::shootDeviceScreen(QString serialNumber, QString savePath)
{
    QEventLoop eventloop;
    QTimer::singleShot(20*1000, &eventloop, SLOT(quit()));
    QProcess process;
    connect(&process,SIGNAL(finished(int)),&eventloop,SLOT(quit()));
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<"/system/bin/screencap -p /data/local/tmp/lScreenshoot.png");
    eventloop.exec();
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"pull"<<"/data/local/tmp/lScreenshoot.png"<<savePath);
    eventloop.exec();
}

QString AdbController::getDirOrFileList(QString serialNumber, QString path)
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("ls -l \"%0\"").arg(path));
    process.waitForStarted(10*1000);
    process.waitForFinished(20*1000);
    return process.readAll();
}
QString AdbController::getDirOrFileList_easyMode(QString serialNumber, QString path)
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("ls \"%0\"").arg(path));
    process.waitForStarted(10*1000);
    process.waitForFinished(20*1000);
    return process.readAll();
}

QString AdbController::getUserPackageList(QString serialNumber)//返回格式<package:com.qianxun.kankan>
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("pm list packages -3"));
    process.waitForStarted(10*1000);
    process.waitForFinished(20*1000);
    return process.readAll();
}

QString AdbController::findIconInfoDir(QString serialNumber)
{
    QProcess process;
    QString tempStr;
    QString dir;
    tempStr = getDirOrFileList_easyMode(serialNumber,"/");
    if(tempStr.contains("storage"))
    {
        //find from /storage/
        process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("find /storage/ -name \"packageInfo.txt\""));
        process.waitForStarted(10*1000);
        process.waitForFinished(20*1000);
        tempStr = process.readAll();
        if(tempStr.contains("packageInfo.txt"))
        {
            QTextStream st(&tempStr);
            while(!st.atEnd())
            {
                QString line = st.readLine();
                if(line.contains("packageInfo.txt"))
                {
                    return QFileInfo(line).dir().path();
                }
            }
        }
        //不能找到，可能命令不存在，用枚举的方式……
        tempStr = getDirOrFileList_easyMode(serialNumber,"/storage/");
        QTextStream st(&tempStr);
        while(!st.atEnd())
        {
            QString line = st.readLine();
            QString str = getDirOrFileList_easyMode(serialNumber,QString("/storage/%0/").arg(line));
            if(str.contains("LAssistant"))
            {
                dir = QString("/storage/%0/LAssistant").arg(line);
                return dir;
            }
        }
    }
    if(tempStr.contains("mnt"))
    {

        //find from /mnt/
        process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("find /mnt/ -name \"packageInfo.txt\""));
        process.waitForStarted(10*1000);
        process.waitForFinished(20*1000);
        tempStr = process.readAll();
        if(tempStr.contains("packageInfo.txt"))
        {
            QTextStream st(&tempStr);
            while(!st.atEnd())
            {
                QString line = st.readLine();
                if(line.contains("packageInfo.txt"))
                {
                    return QFileInfo(line).dir().path();
                }
            }
        }
        //不能找到

        tempStr = getDirOrFileList_easyMode(serialNumber,"/mnt/");
//        qDebug()<<"mnt"<<tempStr;
        QTextStream st(&tempStr);
        while(!st.atEnd())
        {
            QString line = st.readLine();
            if(line.contains("sdcard"))
            {
                QString str = getDirOrFileList_easyMode(serialNumber,QString("/mnt/%0/").arg(line));
                //qDebug()<<"mnt"<<str;
                if(str.contains("LAssistant"))
                {
                    dir = QString("/mnt/%0/LAssistant").arg(line);
                    return dir;
                }
            }
        }

    }
    if(tempStr.contains("sdcard"))
    {
        //find from /sdcardx/
        for(int i=0;i<3;i++)
        {
            tempStr = getDirOrFileList_easyMode(serialNumber,QString("/sdcard%0/").arg(QString::number(i)));
            if(tempStr.contains("LAssistant"))
            {
                dir = QString("/sdcard%0/LAssistant").arg(QString::number(i));
                return dir;
            }
        }

    }

    return dir;
}

bool AdbController::installApkToPhone(QString serialNumber,QString filePath,QString arg)
{
    if(!QFileInfo(filePath).exists())
    {
        qDebug()<<"AdbController::installApkToPhone() file not exit"<<filePath;
        return false;
    }
    if(arg.isEmpty())
    {
        arg.append("-r");
    }

    QEventLoop loop;
    TransferThread *t = new TransferThread(0,QString("adb -s %0 install \"%1\" \"%2\"").arg(serialNumber,arg,filePath));
    connect(t, SIGNAL(finished()), &loop, SLOT(quit()));
    t->start();
    loop.exec();
    int i = t->getTransferResultMark();
    if(i == 0)//success
    {
        return true;
    }else if(i==1)
    {
        QMessageBox::warning(0,"提示","手机内存不足，安装失败。\n请先清理正在运行的软件，以空出安装空间。");
        return false;
    }else
    {
        return false;
    }
    delete t;
}

QString AdbController::startAPP(QString serialNumber, QString packageName,QString activity)
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("am start -n \"%0/%1.%2\"").arg(packageName).arg(packageName).arg(activity));
    process.waitForStarted(5*1000);
    process.waitForFinished(10*1000);
    return process.readAll();
}

QString AdbController::getProcessInfo(QString serialNumber)
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("ps"));
    process.waitForStarted(5*1000);
    process.waitForFinished(10*1000);
    return process.readAll();
}

bool AdbController::uninstallApp(QString serialNumber, QString packageName, bool isSaveData)
{
    QEventLoop loop;
    TransferThread *t;
    if(isSaveData)
    {
        t = new TransferThread(0,QString("adb -s %0 shell pm uninstall \"%1\" \"%2\"").arg(serialNumber,"-k",packageName));
    }else
    {
        t = new TransferThread(0,QString("adb -s %0 shell pm uninstall \"%1\"").arg(serialNumber,packageName));
    }
    connect(t, SIGNAL(finished()), &loop, SLOT(quit()));
    t->start();
    loop.exec();
    int i = t->getTransferResultMark();
    if(i == 0)//success
    {
        return true;
    }else
    {
        return false;
    }
    delete t;
}

void AdbController::renameFileOrDir(QString serialNumber, QString prePath, QString afterPath)
{
    QProcess process;
    process.start("adb",QStringList()<<"-s"<<serialNumber<<"shell"<<QString("rename \"%0\" \"%1\"").arg(prePath).arg(afterPath));
    process.waitForStarted(5*1000);
    process.waitForFinished(20*1000);
    //qDebug()<<process.readAll();
}

bool AdbController::pushFileToPhone(QString serialNumber, QString localPath, QString remotePath)
{
    QEventLoop loop;
    TransferThread *t = new TransferThread(0,QString("adb -s %0 push \"%1\" \"%2\"").arg(serialNumber,localPath,remotePath));
    connect(t, SIGNAL(finished()), &loop, SLOT(quit()));
    t->start();
    loop.exec();
    int i = t->getTransferResultMark();
    if(i == 0)//success
    {
        return true;
    }else
    {
        return false;
    }
    delete t;
}

bool AdbController::pullFileToPc(QString serialNumber, QString remotePath, QString localPath)
{
    QEventLoop loop;
    TransferThread *t = new TransferThread(0,QString("adb -s %0 pull \"%1\" \"%2\"").arg(serialNumber,remotePath,localPath));
    connect(t, SIGNAL(finished()), &loop, SLOT(quit()));
    t->start();
    loop.exec();
    int i = t->getTransferResultMark();
    if(i == 0)//success
    {
        return true;
    }else
    {
        return false;
    }
    delete t;
}

bool AdbController::deleteFile(QString serialNumber, QString remotePath)
{
    QProcess process;
    //process.start("adb",QStringList()<<"-s"<<serialNumber<<"push"<<QString("\"%0\"").arg(localPath)<<QString("\"%0\"").arg(remotePath));
    process.start(QString("adb -s %0 shell rm \"%1\"").arg(serialNumber,remotePath));
    process.waitForStarted(5*1000);
    process.waitForFinished(10*1000);
    QString re = process.readAll();
    if(re.length()==0)//success
    {
        return true;
    }else
    {
        return false;
    }
}

bool AdbController::mkdir(QString serialNumber, QString remoteDirPath)
{
    QProcess process;
    process.start(QString("adb -s %0 shell mkdir \"%1\"").arg(serialNumber,remoteDirPath));
    process.waitForStarted(5*1000);
    process.waitForFinished(10*1000);
    QString re = process.readAll();
    if(re.length()==0)//success
    {
        return true;
    }else
    {
        return false;
    }
}

bool AdbController::deleteDir(QString serialNumber, QString remotePath)
{
    QProcess process;
    //process.start("adb",QStringList()<<"-s"<<serialNumber<<"push"<<QString("\"%0\"").arg(localPath)<<QString("\"%0\"").arg(remotePath));
    process.start(QString("adb -s %0 shell rm -r \"%1\"").arg(serialNumber,remotePath));
    process.waitForStarted(5*1000);
    process.waitForFinished(10*1000);
    QString re = process.readAll();
    if(re.length()==0)//success
    {
        return true;
    }else
    {
        return false;
    }
}

void AdbController::restartConnectionWatchThread()
{
    this->connectionWatchThread.terminate();
    this->connectionWatchThread.start();
}

void AdbController::startThread()
{
    this->connectionWatchThread.start();
}


