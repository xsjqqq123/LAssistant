#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTimer>
#include <component/phoneheaderwidget.h>
#include "component/PhoneInfo//phoneinfopage.h"
#include "component/FileManager/phonefilemanagerpage.h"
#include "component/SoftwareManager/softwarelistwidget.h"
#include "component/softwareDownloadPage/softwaredownloadpage.h"
#include "adbController/adbcontroller.h"
#include "KuwoMusic/kuwomusicwidget.h"
class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();
    void initUi();
public slots:
    void slotConnectionChanged(QStringList serialNumberStateList);
    void setCurrentDevice(QString serialNumber);
    void shootDeviceScreen();
    void resetToDefault();
    void copyDamonToPath();
private:
    QGridLayout *mainGridL;
    QSplitter *mainSplitter;
    QWidget *leftPanelContainer;//左侧面板
    PhoneHeaderWidget *phoneHeaderWidget;//左侧导航栏设备切换部件
    QListWidget *navListWidget;//左侧导航栏切换部件
    QStackedWidget *contentStackedWidget;

    PhoneInfoPage *phoneInfoPage;
    PhoneFileManagerPage *phoneFileManagerPage;
    SoftWareListWidget *phoneSoftwareManagerPage;
    SoftwareDownloadPage *softwareDownloadPage;//
    KuwoMusicWidget *kuwoMusicWidget;

    AdbController *adbController;
    QMap<QString, QString> devicesStateMap;//保存设备状态，serialNumber-connState
    QMap<QString, QString> devicesModelMap;//设备型号，serialNumber-model
    QString adbPath;

    QString currentDeviceSerialNumber;//serialNumber

    QTimer *shootDeviceScreenTimer;
};

#endif // CENTRALWIDGET_H
