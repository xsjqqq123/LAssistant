#ifndef ADBCONTROLLER_H
#define ADBCONTROLLER_H

#include <QWidget>
#include <QTcpSocket>
#include <QThread>
#include <QProcess>
#include <QRegExp>
#include <QTimer>
#include <QFileInfo>
#include <QEventLoop>
#include <QTextStream>
#include <QRegExp>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
//设备所处的四个状态OFFLINE,DEVICE,RECOVERY,FASTBOOT
class ConnectionThread : public QThread
{
    Q_OBJECT
public:
    void run();

    QTcpSocket *socket;
signals:
    //void connectionChanged(int,QString);
    void connectionChanged(QStringList serialNumberStateList);//[serialNumber#state]\[serialNumber#state]
};

class TransferThread : public QThread
{
    Q_OBJECT
public:
    explicit TransferThread(QWidget *parent = 0,QString commandStr = "");
    void run();
    int getTransferResultMark();
    void forceToExit();
private:
    QProcess process;
    QString commandStr;
    int transferResultMark;//0:success;-1:failed
};

class AdbController : public QWidget
{
    Q_OBJECT
public:
    explicit AdbController(QWidget *parent = 0,bool isThreadNeed = false);
    ~AdbController();
signals:
    void connectionChanged(QStringList serialNumberStateList);
public slots:
    void slotConnectionChanged(QStringList serialNumberStateList);
    void starServer();
    QString getDeviceModel(QString serialNumber);//获取手机型号
    QStringList getPhone_buildProp_Info(QString serialNumber);//返回格式:手机型号#系统版本#内核版本#运存大小
    QStringList getMemoryInfo(QString serialNumber);//获取内存信息
    bool checkRoot(QString serialNumber);//检测手机是否root
    void shootDeviceScreen(QString serialNumber,QString savePath);//截图
    QString getDirOrFileList(QString serialNumber,QString path);//获取文件或目录列表
    QString getDirOrFileList_easyMode(QString serialNumber, QString path);
    QString getUserPackageList(QString serialNumber);//获取用户安装的软件列表
    QString findIconInfoDir(QString serialNumber);// 找到安卓守护程序 包的图片与信息 保存目录
    bool installApkToPhone(QString serialNumber, QString filePath, QString arg);
    QString startAPP(QString serialNumber, QString packageName, QString activity);
    QString getProcessInfo(QString serialNumber);
    bool uninstallApp(QString serialNumber,QString packageName,bool isSaveData);//如果加 -k 参数,为卸载软件但是保留配置和缓存文件.
    void renameFileOrDir(QString serialNumber,QString prePath,QString afterPath);
    bool pushFileToPhone(QString serialNumber,QString localPath,QString remotePath);
    bool pullFileToPc(QString serialNumber,QString remotePath,QString localPath);
    bool deleteFile(QString serialNumber,QString remotePath);//删除文件
    bool mkdir(QString serialNumber,QString remoteDirPath);//
    bool deleteDir(QString serialNumber,QString remotePath);//删除remotePath的文件夹及其里面的所有文件
    void restartConnectionWatchThread();
    void startThread();
private:
    QString adbPath;
    ConnectionThread connectionWatchThread;//监视连入的设备
    QThread thread;
};

#endif // ADBCONTROLLER_H
