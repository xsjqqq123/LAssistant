#ifndef DOWNLOADMANAGERWIDGET_H
#define DOWNLOADMANAGERWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QProcess>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QDir>
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QPen>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QBuffer>
#include <QEventLoop>
#include <QRegExp>
#include "../SoftwareManager/installtasklistwidget.h"

class DownloadThreadSoft : public QObject
{
    Q_OBJECT
public:
    explicit DownloadThreadSoft(QObject *parent = 0,QString url="",QString fileSavePath="");
    ~DownloadThreadSoft();
signals:
    void downloadProcess(int percent);
    void downloadFinished(int status);//status 0为成功
public slots:
    void readyRead();
    void finished(int status);
    void stopTask();
private:
    QProcess *process;
};
class SoftDownloadItem : public QWidget
{
    Q_OBJECT
public:
    explicit SoftDownloadItem(QWidget *parent = 0, QString softName = "未知", QString Url="", QString saveDir=QDir::homePath());
    ~SoftDownloadItem();

signals:
public slots:
    void startTask();
    void stopTask();
    bool isForInstall();
    QString getSoftSavePath();
    void downloadProgress(int v);
    void downloadFinished(int status);//0代表成功，-1失败
private:
    QHBoxLayout *hBoxLayout;
    QCheckBox *checkBox;
    QLabel *softNameLbl;
    QProgressBar *progressBar;
    QPushButton *deleteTaskBtn;
    QProcess *process;
    QString softSavePath;
    QString Url;
    bool isDownloadFinished;
    DownloadThreadSoft *downloadThread;
};

class DownloadManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadManagerWidget(QWidget *parent = 0);
    ~DownloadManagerWidget();
protected:
    virtual void leaveEvent(QEvent *);
signals:

public slots:
    void setSerialsNumber(QString serialsNumber);
    void addDownloadTask(QString softName,QString directUrl,QString saveDir);
    void installSelected();
private:
    QGridLayout *containerWL;
    QWidget *containerW;
    QVBoxLayout *mainLayout;
    QPushButton *installBtn;
    QScrollArea *area;
    QWidget *areaWidget;
    QVBoxLayout *areaWidgetLayout;
    QList<SoftDownloadItem *> itemList;
    QString serialsNumber;
};

#endif // DOWNLOADMANAGERWIDGET_H
