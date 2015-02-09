#include "phoneheaderwidget.h"

PhoneHeaderWidget::PhoneHeaderWidget(QWidget *parent) :
    QWidget(parent)
{
    mainGridL = new QGridLayout(this);
    phoneNameLbl = new QLabel("未连接");
    phoneListExpandBtn = new QPushButton;
    phonesActionGroup = new QActionGroup(this);
    //phoneNameLbl->setAlignment(Qt::AlignCenter);
    phoneNameLbl->setWordWrap(true);
    phoneListExpandBtn->setObjectName("phoneListExpandBtn");
    phoneListExpandBtn->setFlat(true);
    phoneListExpandBtn->setFocusPolicy(Qt::NoFocus);
    phoneListExpandBtn->setIcon(QIcon(":/app/skin/imgs/arrowDown.png"));
    phoneListExpandBtn->setIconSize(QSize(30,30));
    //phoneListExpandBtn->setMaximumSize(30,30);
    phoneListExpandBtn->setFixedSize(40,60);
    //phoneListExpandBtn->setToolTip("切换设备");

    mainGridL->addWidget(phoneNameLbl,0,0,1,3);
    mainGridL->addWidget(phoneListExpandBtn,0,3,1,1);
    connect(phonesActionGroup,SIGNAL(triggered(QAction*)),this,SLOT(emitDeviceChangeSignal(QAction*)));
    connect(phoneListExpandBtn,SIGNAL(clicked()),this,SLOT(popupMenu()));
}

void PhoneHeaderWidget::setDevicesStateData(QMap<QString, QString> devicesStateMap, QMap<QString, QString> devicesModelMap,QString currentDeviceSerialNumber)
{
    foreach (QAction *act, phonesActionGroup->actions()) {
        delete act;
    }
    devicesSerialNumberList.clear();
    if(devicesStateMap.count()==0)
    {
        this->phoneNameLbl->setText("未连接");
        return;
    }
    QMapIterator<QString, QString> i(devicesStateMap);
    while (i.hasNext()) {
        i.next();
        QString state;
        if(i.value() == "DEVICE")
        {
            state = "<在线>";
        }else
        {
            state = "<离线>";
        }

        QAction *act = new QAction(devicesModelMap.value(i.key()),this);
        act->setData(i.key());
        phonesActionGroup->addAction(act);
        devicesSerialNumberList.append(i.key());
    }
    this->currentDeviceSerialNumber = currentDeviceSerialNumber;
    this->phoneNameLbl->setText(devicesModelMap.value(currentDeviceSerialNumber));

}

void PhoneHeaderWidget::emitDeviceChangeSignal(QAction* act)
{
    this->currentDeviceSerialNumber = act->data().toString();
    emit deviceChangeSignal(this->currentDeviceSerialNumber);
}

void PhoneHeaderWidget::popupMenu()
{
    QMenu *menu = new QMenu;
    foreach (QAction *act, phonesActionGroup->actions()) {
        act->setCheckable(true);
        if(act->data().toString() == currentDeviceSerialNumber)
        {
            act->setChecked(true);
        }
        menu->addAction(act);
    }
    QAction *reAct = menu->exec(QCursor::pos());
    if(reAct == Q_NULLPTR)
    {
        return;
    }
    if(phonesActionGroup->actions().contains(reAct))
    {
        return;//已在phonesActionGroup的信号中处理
    }

}

void PhoneHeaderWidget::resetToDefault()
{
    foreach (QAction *act, phonesActionGroup->actions()) {
        delete act;
    }
    devicesSerialNumberList.clear();
    this->phoneNameLbl->setText("未连接");
    currentDeviceSerialNumber = "";
}
