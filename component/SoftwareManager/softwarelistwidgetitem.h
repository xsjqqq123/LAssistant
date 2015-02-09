#ifndef SOFTWARELISTWIDGETITEM_H
#define SOFTWARELISTWIDGETITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include "adbController/adbcontroller.h"
#include "../softwareDownloadPage/downloadmanagerwidget.h"
#include <QDebug>
class SoftWareListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftWareListWidgetItem(QWidget *parent = 0,QString packageName="",QString softwareName="",QString iconPath = "",QString version = "");
    ~SoftWareListWidgetItem();

signals:
    void uninstallSignal(QString packageName);
public slots:
    void uninstall();
    void emitUninstall(QString packageName);
    void setUninstallBtnState(int state);//-1 failed;1 success;0 normal
    void updateSoft();
    void downloadProgress(int v);
    void downloadFinished(int status);
    void setSerialsNumber(QString serialsNumber);
    void checkForUpdate();
private:
    QHBoxLayout *mainLyout;
    QString packageName;
    QString version;
    QString downloadUrl;
    QString softSavePath;
    QLabel *softwareIconLbl;
    QLabel *softwareNameLbl;
    QLabel *softwareVersionLbl;
    QPushButton *managerBtn;
    QString serialsNumber;

    AdbController *adbController;
    DownloadThreadSoft *downloadThread;
    int downLoadPercent;
};

#endif // SOFTWARELISTWIDGETITEM_H
