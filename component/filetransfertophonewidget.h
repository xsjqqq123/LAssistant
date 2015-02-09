#ifndef FILETRANSFERTOPHONEWIDGET_H
#define FILETRANSFERTOPHONEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "adbController/adbcontroller.h"
#include <QFileInfo>
#include <QCloseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include "../titlebar.h"
class FileTransferToPhoneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileTransferToPhoneWidget(QWidget *parent = 0,
                                       QString serialsNumber="",
                                       QStringList filepathList=QStringList(),
                                       QString savePath="",
                                       int type = 1);
protected:
    void closeEvent(QCloseEvent *event);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
signals:
    void finished();
    void refreshFileList();
public slots:
    void setTitleStr(QString txt);
    void startTask();
private:
    QVBoxLayout *topLayout;//加一层widget，使qss生效
    QWidget *container;//加一层，使qss生效
    QVBoxLayout *winLayout;
    TitleBar *titleBar;
    QGridLayout *mainGLayout;
    AdbController *adbController;
    QString serialsNumber;
    QStringList filepathList;
    QString savePath;
    QList<QLabel *> statusLblList;
    int type;//type 1:pc to phone ,2:phone to pc
    TransferThread *t;
    QEventLoop loop;
    bool isCancel;
    bool isFinished;

    QPoint last;
    QPoint pos0;
};

#endif // FILETRANSFERTOPHONEWIDGET_H
