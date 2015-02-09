#ifndef SOFTWARELISTWIDGET_H
#define SOFTWARELISTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QTextStream>
#include <QApplication>
#include <QLabel>
#include <QMovie>
#include <QFileDialog>
#include <QEventLoop>
#include <QDesktopWidget>
#include "softwarelistwidgetitem.h"
#include "adbController/adbcontroller.h"
#include "installtasklistwidget.h"
class SoftWareListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoftWareListWidget(QWidget *parent = 0);

signals:

public slots:
    void setSerialsNumber(QString serialsNumber);
    void getIconsInfoToPc();
    void loadSoftWareList();
    void refreshList();
    void uninstallApp(QString packageName);
    void resetToDefault();
    void installAPP();
private:
    QGridLayout *mainGlayout;
    QScrollArea *softwareListArea;
    QWidget *softwareListAreaWidget;
    QVBoxLayout *softwareListAreaWidgetLayout;
    QPushButton *installApkBtn;
    QLabel *busyLbl;
    QPushButton *refreshListBtn;
    AdbController *adbController;
    QMap<QString,SoftWareListWidgetItem *> pName_item_map;
    QString serialsNumber;
    QString remoteIconDir;
    QString packageNameListStr;
};

#endif // SOFTWARELISTWIDGET_H
