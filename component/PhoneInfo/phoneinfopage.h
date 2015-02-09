#ifndef PHONEINFOPAGE_H
#define PHONEINFOPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include "phoneinforectangle.h"
class PhoneInfoPage : public QWidget
{
    Q_OBJECT
public:
    explicit PhoneInfoPage(QWidget *parent = 0);

signals:

public slots:
    void setDeviceScreenPicture(QString pixPath);//png
    void setPhoneContentText(QString model, QString company,QString version, QString memoryTotal, QString isRoot);//手机型号 制造商 系统版本 内存大小
    void resetToDefault();
private:
    QGridLayout *mainGridL;
    QLabel *phoneScreenLbl;
    PhoneInfoRectangle *phoneInfoRectangle;
};

#endif // PHONEINFOPAGE_H
