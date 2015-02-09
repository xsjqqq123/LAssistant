#ifndef SOFTWAREDOWNLOADPAGE_H
#define SOFTWAREDOWNLOADPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLineEdit>
#include "softwarelist.h"
#include "softwaretypenavwidget.h"
#include "myhoverbutton.h"
#include "downloadmanagerwidget.h"
class SoftwareDownloadPage : public QWidget
{
    Q_OBJECT
public:
    explicit SoftwareDownloadPage(QWidget *parent = 0);
    ~SoftwareDownloadPage();

signals:

public slots:
    void setSerialsNumber(QString serialsNumber);
    void showNavList();
    void showDownloadNavList();
    void getNavJsonFromServer();
    void getSoftJsonFromServer(int requestId,int pageNum);
    void getSearchJsonFromServer();
    void delayLoadData();
    void addDownloadTask(QString softPackageName);
    QString getDirectUrl(QString url);
    void pageChange(int page);
private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *navBarLayout;

    MyhoverButton *navBtn;
    QLineEdit *searchEdit;
    QPushButton *searchBtn;
    SoftwareList *softwareList;
    MyhoverButton *downloadNavBtn;
    DownloadManagerWidget *downloadManagerWidget;

    SoftwareTypeNavWidget *navWidget;

    QNetworkAccessManager *manager;
    QString hostUrl;
    QString serialsNumber;
    QString currentRequestUrl;
    int requestId;
};

#endif // SOFTWAREDOWNLOADPAGE_H
