#ifndef SOUSUOWIDGET_H
#define SOUSUOWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../component/musiclistwidget.h"
class SouSuoWidget : public QWidget//search
{
    Q_OBJECT
public:
    explicit SouSuoWidget(QWidget *parent = 0);
    MusicListWidget *musicListWidget;
signals:

public slots:
    void search(QString keyword, QString pageNumIndex);//pageNum start from 0
    void searchKeywordAct();
    void handlePageChange(QString page);
private:
    QGridLayout *mainGLayout;
    QHBoxLayout *searchHboxLayout;
    QLineEdit *searchEdit;
    QPushButton *searchBtn;
    QLabel *busyIndicator;

    QString keyword;
};

#endif // SOUSUOWIDGET_H
