#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0,bool isSettingBtnShow = true,bool isMinBtnShow = true,bool isCloseShow = true);
    ~TitleBar();

signals:
    void minimumWinSignal();
    void closeWinSignal();
    void settingSignal();
public slots:
    void emitMinimumWinSignal();
    void emitCloseWinSignal();
    void emitSettingSignal();
    void setTitleStr(QString str);
private:
    QHBoxLayout *mainLayout;
    QLabel *titleLbl;
    QPushButton *settingBtn;
    QPushButton *minBtn;
    QPushButton *closeBtn;
};

#endif // TITLEBAR_H
