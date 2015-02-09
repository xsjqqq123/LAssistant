#ifndef NAVIGATEBAR_H
#define NAVIGATEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
class NavigateBar : public QWidget//共有：排行榜、歌手、分类、搜索\下载
{
    Q_OBJECT
public:
    explicit NavigateBar(QWidget *parent = 0);
    ~NavigateBar();

signals:
    void tabChangeSignal(QString tabLink);
public slots:
    void linkClick(QString link);
    void linkHover(QString link);
private:
    QHBoxLayout *hboxLayout;
    QLabel *paiHangBang;
    QLabel *geShou;
    QLabel *fenLei;
    QLabel *souSuo;
    QLabel *xiazai;
};

#endif // NAVIGATEBAR_H
