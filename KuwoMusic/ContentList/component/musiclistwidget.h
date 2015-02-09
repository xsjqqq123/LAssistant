#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMovie>
#include "pagenumbar.h"
class MusicListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicListWidgetItem(QWidget *parent = 0,
                                 QString songId = "",
                                 QString lblId = "",
                                 QString songName = "",
                                 QString songSinger = "");
    ~MusicListWidgetItem();
signals:
    void playSignal(QString songId,QString songName);
    void downloadSignal(QString songId,QString songName);
public slots:
    void emitPlay();
    void emitDownload();
private:
    QHBoxLayout *hboxLayout;
    QLabel *idLbl;
    QLabel *songNameLbl;
    QLabel *songSingerLbl;
    QPushButton *songPlayBtn;
    QPushButton *songDownloadBtn;

    QString songId;
    QString songName;
    QString songSinger;
};

class MusicListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicListWidget(QWidget *parent = 0,bool isPageBarVisible = false);
    ~MusicListWidget();
    void setIndicatorVisible(bool b);
signals:
    void playSignal(QString songId,QString songName);
    void downloadSignal(QString songId,QString songName);
    void pageChange(QString pageNum);
public slots:
    void setPageInfo(QString currentPageNum, QString totalPageNum);
    void addItem(QString songId = "",
                 QString lblId = "",
                 QString songName = "",
                 QString songSinger = "");
    void setTitleText(QString text);
    void clearAllItem();
    //
    void emitPageChange(QString pageNum);
    void emitPlay(QString songId,QString songName);
    void emitDownload(QString songId,QString songName);
private:
    QGridLayout *mainGLayout;
    QLabel *titleLbl;
    PageNumBar *pageNumBar;
    QScrollArea *listArea;
    QWidget *listAreaWidget;
    QVBoxLayout *listAreaWidgetLayout;
    QLabel *busyIndicator;
};

#endif // MUSICLISTWIDGET_H
