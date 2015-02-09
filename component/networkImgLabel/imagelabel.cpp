#include "imagelabel.h"
#include <QPixmap>
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QTimer>
ImageLabel::ImageLabel(QObject *parent, QString url, int timeToDelay)
{
    //setAttribute(Qt::WA_TranslucentBackground, true);
    this->url=url;
    isImgLoaded =false;
    QMovie*movie=new QMovie(":/app/skin/imgs/loading.gif");
    setMovie(movie);
    QTimer::singleShot(timeToDelay,this,SLOT(getImage()));
    //qDebug()<< url;
}

ImageLabel::~ImageLabel()
{
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    emit clicked();
}
void ImageLabel::getImage()
{
    foreach (LoadImgThread *t, imgThreadList) {
        t->terminate();
    }
    imgThreadList.clear();
    LoadImgThread *t1 = new LoadImgThread(0,this->url);
    imgThreadList.append(t1);
    connect(t1,SIGNAL(imageReady(QByteArray)),this,SLOT(readImage(QByteArray)));
    connect(t1,SIGNAL(finished()),t1,SLOT(deleteLater()));
    t1->start();
}

void ImageLabel::readImage(QByteArray data)
{
    QPixmap pix;
    if(pix.loadFromData(data))
    {
        pix = pix.scaled(this->width(),this->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        this->setPixmap(pix);
    }else
    {
        pix.load(":/fileManager/skin/imgs/fileIcons/apk.png");
        pix = pix.scaled(this->width(),this->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        this->setPixmap(pix);
    }
    isImgLoaded = true;

}

