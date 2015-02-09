#ifndef PAIHANGBANGWIDGET_H
#define PAIHANGBANGWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMovie>
#include <QTimer>
#include "../component/musiclistwidget.h"
class PaiHangBangWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaiHangBangWidget(QWidget *parent = 0);
    ~PaiHangBangWidget();
    MusicListWidget *musicListWidget;
signals:
public slots:
    void loadData(int row = 0);
private:
    QGridLayout *mainLayout;
    QListWidget *leftNaviList;
    QLabel *busyIndicator;

};

#endif // PAIHANGBANGWIDGET_H
