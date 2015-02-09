#ifndef KUWOMUSICWIDGET_H
#define KUWOMUSICWIDGET_H

#include <QWidget>
#include "Head/headwidget.h"
#include "ContentList/contenttabwidget.h"
#include "Player/playerwidget.h"
#include <QGridLayout>
class KuwoMusicWidget : public QWidget//main class
{
    Q_OBJECT
public:
    explicit KuwoMusicWidget(QWidget *parent = 0);
    ~KuwoMusicWidget();

signals:

public slots:
private:
    QGridLayout *mainGLayout;
    HeadWidget *headWidget;
    PlayerWidget *playerWidget;
    ContentTabWidget *contentListWidget;
};

#endif // KUWOMUSICWIDGET_H
