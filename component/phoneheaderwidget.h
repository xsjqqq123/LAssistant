#ifndef PHONEHEADERWIDGET_H
#define PHONEHEADERWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QActionGroup>
#include <QComboBox>
#include <QMenu>
#include <QMessageBox>
class PhoneHeaderWidget : public QWidget//提供左上角手机“头像”显示部件
{
    Q_OBJECT
public:
    explicit PhoneHeaderWidget(QWidget *parent = 0);

signals:
    void deviceChangeSignal(QString newDeviceModel);
public slots:
    //设备状态，serialNumber-connState,devicesModelMap:手机的serialNumber-model,设备id与型号
    void setDevicesStateData(QMap<QString, QString> devicesStateMap, QMap<QString, QString> devicesModelMap, QString currentDeviceSerialNumber);
    void emitDeviceChangeSignal(QAction *act);
    void popupMenu();
    void resetToDefault();
private:
    QGridLayout *mainGridL;
    QPushButton *phoneListExpandBtn;
    QLabel *phoneNameLbl;
    QActionGroup *phonesActionGroup;
    QStringList devicesSerialNumberList;//保存设备状态，serialNumber-connState
    QString currentDeviceSerialNumber;
};

#endif // PHONEHEADERWIDGET_H
