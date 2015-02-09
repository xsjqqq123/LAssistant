#ifndef SOFTWARELIST_H
#define SOFTWARELIST_H

#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QGridLayout>
#include <QFontMetrics>
#include <QDebug>
#include "myhoverbutton.h"
#include "../networkImgLabel/imagelabel.h"
class SoftwareList : public QScrollArea
{
    Q_OBJECT
public:
    SoftwareList(QWidget *parent = 0);
    ~SoftwareList();

signals:
    void downloadSignal(QString softPackageName);
    void pageChange(int page);
public slots:
    void setList(QByteArray jsonData);
    void emitDownloadSignal(QAbstractButton *btn);
    void clearItems();
    void prePage();
    void nextPage();
private:
    QWidget *areaWidget;
    QGridLayout *areaWidgetLayout;

    QButtonGroup *downBtnsGroup;
    int pageNum;
};

#endif // SOFTWARELIST_H
