#ifndef PHONEINFORECTANGLE_H
#define PHONEINFORECTANGLE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
class PhoneInfoRectangle : public QWidget
{
    Q_OBJECT
public:
    explicit PhoneInfoRectangle(QWidget *parent = 0);

signals:

public slots:
    void setContentText(QString txt);
private:
    QVBoxLayout *mainLayout;
    QLabel *headerLbl;
    QLabel *contentLbl;
};

#endif // PHONEINFORECTANGLE_H
