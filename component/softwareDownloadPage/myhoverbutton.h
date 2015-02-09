#ifndef MYHOVERBUTTON_H
#define MYHOVERBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
class MyhoverButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyhoverButton(QWidget *parent = 0);
    ~MyhoverButton();
    void flashBackgroud();
    void setData(QString data);
    QString getData();
protected:
    void enterEvent(QEvent * event);
signals:
    void mouseEnterSiganl();
private:
    QString usrDataStr;
};

#endif // MYHOVERBUTTON_H
