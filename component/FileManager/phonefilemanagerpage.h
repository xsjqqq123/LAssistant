#ifndef PHONEFILEMANAGERPAGE_H
#define PHONEFILEMANAGERPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QListWidget>
#include <QTextStream>
#include <QRegExp>
#include <QFileInfo>
#include <QFileDialog>
#include <QMenu>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include "../../adbController/adbcontroller.h"
#include "../filetransfertophonewidget.h"
class DirWidget : public QWidget//文件路径
{
    Q_OBJECT
public:
    explicit DirWidget(QWidget *parent = 0);
signals:
    void pathChangeSignal(QString dirPath);
public slots:
    void drawPathBtns();
    void setPath(QString dirPath);
    QString getPath();
    void pathChange(int btnId);

private:
    QHBoxLayout *mainLayout;
    QButtonGroup *pathBtnGroup;
    QString dirPath;

};

class PhoneFileManagerPage : public QWidget
{
    Q_OBJECT
public:
    explicit PhoneFileManagerPage(QWidget *parent = 0);
    void initUi();
    bool isNotAllowEdit();
protected:
    bool eventFilter(QObject *target, QEvent *event);
public slots:
    void setCurrentDevice(QString serialNumber);
    void setPath(QString dirPath);
    void fileListWidgetItemDBclick(QListWidgetItem* item);
    void itemClickedSlot(QListWidgetItem* item);
    void itemChangedSlot(QListWidgetItem* item);
    void customContextMenuRequestedSlot(QPoint p);
    void addFileToMobile();
    void refreshListWidget();
    void resetToDefault();
private:
    QGridLayout *mainGridL;
    DirWidget *dirWidget;
    QPushButton *addFileBtn;
    QListWidget *fileListWidget;
    QString currentDeviceSerialNumber;
    AdbController *adbController;
    QString storeItemText;
    QString tmpSavePath;
    bool isImgItemChange;
};

#endif // PHONEFILEMANAGERPAGE_H
