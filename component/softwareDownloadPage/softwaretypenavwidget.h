#ifndef SOFTWARETYPENAVWIDGET_H
#define SOFTWARETYPENAVWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QGridLayout>
#include <QFocusEvent>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
class SoftwareTypeNavWidget : public QWidget//用于导航，识别哪个软件分类
{
    Q_OBJECT
public:
    explicit SoftwareTypeNavWidget(QWidget *parent = 0);
    ~SoftwareTypeNavWidget();
protected:
//    virtual void paintEvent(QPaintEvent *);
    virtual void leaveEvent(QEvent *);

signals:
    void typeChange(int typCode,int pageNum);
public slots:
    void setList(QByteArray jsonData);
    void emitTypeChange(int typeCode);
private:
    QGridLayout *containerWL;
    QWidget *containerW;
    QGridLayout *gridLayout;
    QButtonGroup *btnsGroup;
};

#endif // SOFTWARETYPENAVWIDGET_H
