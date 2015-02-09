#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include "centralwidget.h"
#include "titlebar.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSkinBackground(QString imgPath);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
public slots:
    void popupMenu();
    void trayIconActivated(QSystemTrayIcon::ActivationReason e);
private:
    QVBoxLayout *mainLayout;
    QWidget *containerW;
    TitleBar *titleBar;
    CentralWidget *cWidget;
    QSystemTrayIcon *trayIcon;
    QPoint last;
    QPoint pos0;
};

#endif // MAINWINDOW_H
