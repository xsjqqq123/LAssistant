#ifndef XIAZAIWIDGET_H
#define XIAZAIWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFileInfo>
#include <QDir>
#include "downloadthread.h"
#include "downloaditem.h"
class XiaZaiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XiaZaiWidget(QWidget *parent = 0);
    ~XiaZaiWidget();

signals:

public slots:
    void addDownloadTask(QString id, QString fileName);
private:
    QGridLayout *mainGLayout;
    QScrollArea *downloadArea;
    QWidget *downloadAreaWidget;
    QVBoxLayout *downloadAreaWidgetLayout;//用于添加下载项
};

#endif // XIAZAIWIDGET_H
