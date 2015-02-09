#ifndef PAGENUMBAR_H
#define PAGENUMBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
class PageNumBar : public QWidget
{
    Q_OBJECT
public:
    explicit PageNumBar(QWidget *parent = 0);
    ~PageNumBar();

signals:
    void pageChange(QString pageNum);
public:
    void set(QString currentPageNum,QString totalPageNum);
public slots:
    void emitPageChange(QString pageNum);

private:
    QHBoxLayout *hboxLayout;
    QLabel *pageNumLbl;
    QString currentPageNum;
    QString totalPageNum;
};

#endif // PAGENUMBAR_H
