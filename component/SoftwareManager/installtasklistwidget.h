#ifndef INSTALLTASKLISTWIDGET_H
#define INSTALLTASKLISTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "adbController/adbcontroller.h"
#include <QFileInfo>
#include <QCloseEvent>
#include <QLabel>
#include "../../titlebar.h"
class InstallTaskListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InstallTaskListWidget(QWidget *parent = 0,QString serialsNumber="", QStringList filepathList=QStringList(),QString arg="");
protected:
    void closeEvent(QCloseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *);
signals:
    void finished();
public slots:
    void startTask();
private:
    QVBoxLayout *winLayout;
    QWidget *containerWidget;
    TitleBar *titleBar;

    QGridLayout *mainGLayout;
    AdbController *adbController;
    QString serialsNumber;
    QStringList filepathList;
    QString arg;//adb install [-l] [-r] [-s] <file>
    QList<QLabel *> statusLblList;
    bool isCancel;
    bool isFinished;
    QPoint last;
    QPoint pos0;
};

#endif // INSTALLTASKLISTWIDGET_H
