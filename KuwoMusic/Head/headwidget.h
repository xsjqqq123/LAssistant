#ifndef HEADWIDGET_H
#define HEADWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
class HeadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadWidget(QWidget *parent = 0);
    ~HeadWidget();

signals:

public slots:
private:
    QHBoxLayout *mainHboxLayout;
    QLabel *logoLbl;
};

#endif // HEADWIDGET_H
