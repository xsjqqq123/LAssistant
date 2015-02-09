#include "pagenumbar.h"
#include <QDebug>
PageNumBar::PageNumBar(QWidget *parent) : QWidget(parent)
{
    //setMaximumHeight(40);
    hboxLayout = new QHBoxLayout(this);
    pageNumLbl = new QLabel(this);
    pageNumLbl->setAlignment(Qt::AlignHCenter);
    hboxLayout->addWidget(pageNumLbl);
    connect(pageNumLbl,SIGNAL(linkActivated(QString)),this,SLOT(emitPageChange(QString)));
}

PageNumBar::~PageNumBar()
{

}

void PageNumBar::set(QString currentPageNum, QString totalPageNum)
{
    this->currentPageNum = currentPageNum;
    this->totalPageNum = totalPageNum;
    int page = currentPageNum.toInt();
    int totalPage = totalPageNum.toInt();
    if(totalPage<1)
    {
        return;
    }
    QString str;
    str.append("<a href='1' style='text-decoration:none;color:black'>首页</a> ");
    if((page-1)>0)
    {
        str.append("<a href='"+QString::number(page-1)+"' style='text-decoration:none;color:black'>前一页</a> ");
    }
    if((page-2)>0)
    {
        str.append("<a href='"+QString::number(page-2)+"' style='text-decoration:none;color:black'>"+QString::number(page-2)+"</a> ");
    }
    if((page-1)>0)
    {
        str.append("<a href='"+QString::number(page-1)+"' style='text-decoration:none;color:black'>"+QString::number(page-1)+"</a> ");
    }
    str.append("<a href='"+QString::number(page)+"' style='text-decoration:none;color:#69BAF9'>"+QString::number(page)+"</a> ");
    if((page+1)<=totalPage)
    {
        str.append("<a href='"+QString::number(page+1)+"' style='text-decoration:none;color:black'>"+QString::number(page+1)+"</a> ");
    }
    if((page+2)<=totalPage)
    {
        str.append("<a href='"+QString::number(page+2)+"' style='text-decoration:none;color:black'>"+QString::number(page+2)+"</a> ");
    }
    if((page+1)<=totalPage)
    {
        str.append("<a href='"+QString::number(page+1)+"' style='text-decoration:none;color:black'>后一页</a> ");
    }
    str.append("<a href='"+QString::number(totalPage)+"' style='text-decoration:none;color:black'>末页</a> ");
    pageNumLbl->setText(str);
}


void PageNumBar::emitPageChange(QString pageNum)
{
    //set(pageNum,this->totalPageNum);
    //qDebug()<<"PageNumBar::emitPageChange()";
    emit pageChange(pageNum);
}

