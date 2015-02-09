#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QTime>
class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();
    void resetToDefault();
signals:

public slots:
    void play(QString url);
    void playKuwoMusic(QString id, QString songName);
    void getPlayInfo();
    void handlePlayerStateChanged(QMediaPlayer::State state);
    void playBtnClick();
    void seek(int p);
private:
    QMediaPlayer *player;
    QGridLayout *mainLayout;
    QLabel *headLbl;
    QLabel *durationLbl;
    QLabel *songNameLbl;
    QPushButton *playBtn;
    QPushButton *nextBtn;
    QSlider *progressBar;
    QTimer *timer;
};

#endif // PLAYERWIDGET_H
