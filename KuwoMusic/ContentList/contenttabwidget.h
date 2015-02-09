#ifndef CONTENTLISTWIDGET_H
#define CONTENTLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include "navigatebar.h"
#include "PaiHangBang/paihangbangwidget.h"
#include "souSuo/sousuowidget.h"
#include "XiaZai/xiazaiwidget.h"
class ContentTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContentTabWidget(QWidget *parent = 0);
    ~ContentTabWidget();
     PaiHangBangWidget *paiHangBangWidget;
     SouSuoWidget *souSuoWidget;
     XiaZaiWidget *xiaZaiWidget;
signals:

public slots:
     void switchTab(QString link);
private:
    QGridLayout *mainLayout;
    QStackedWidget *tabWidget;
    NavigateBar *navigateBar;
    QFrame *splitterLine;
};

#endif // CONTENTLISTWIDGET_H
