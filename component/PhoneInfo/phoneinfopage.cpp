#include "phoneinfopage.h"

PhoneInfoPage::PhoneInfoPage(QWidget *parent) :
    QWidget(parent)
{
    mainGridL = new QGridLayout(this);
    phoneScreenLbl = new QLabel;
    //mainGridL->setAlignment(Qt::AlignTop);
    phoneScreenLbl->setMaximumHeight(500);
    phoneScreenLbl->setMinimumHeight(500);
    phoneInfoRectangle = new PhoneInfoRectangle(this);
    mainGridL->addWidget(phoneScreenLbl,0,0,1,1);
    mainGridL->addWidget(phoneInfoRectangle,0,1,1,1);
    //mainGridL->addItem(new QSpacerItem(10,10),1,1,1,1);
//    QPixmap pix(":/app/skin/imgs/mobileTest.png");
//    pix = pix.scaledToHeight(phoneScreenLbl->height(),Qt::SmoothTransformation);
//    phoneScreenLbl->setPixmap(pix);

    resetToDefault();
}

void PhoneInfoPage::setDeviceScreenPicture(QString pixPath)
{
    if(QFile(pixPath).exists())
    {
        QPixmap pix(pixPath);
        pix = pix.scaledToHeight(phoneScreenLbl->height(),Qt::SmoothTransformation);
        phoneScreenLbl->setPixmap(pix);
    }/*else
    {
        QPixmap pix(":/app/skin/imgs/mobileTest.png");
        pix = pix.scaledToHeight(phoneScreenLbl->height(),Qt::SmoothTransformation);
        phoneScreenLbl->setPixmap(pix);
    }*/
}

void PhoneInfoPage::setPhoneContentText(QString model, QString company,QString version, QString memoryTotal, QString isRoot)
{
    phoneInfoRectangle->setContentText(QString("\n 手机型号:  %0 \n\n 制造商:  %1 \n\n 系统版本:  %2 \n\n 内存大小:  %3 \n\n")
                                       .arg(model)
                                       .arg(company)
                                       .arg(version)
                                       .arg(memoryTotal));
}

void PhoneInfoPage::resetToDefault()
{
    setDeviceScreenPicture("");
    phoneInfoRectangle->setContentText("\n 手机型号: \n\n 制造商: \n\n 系统版本: \n\n 内存大小:");
}
