#-------------------------------------------------
#
# Project created by QtCreator 2014-12-05T14:37:47
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =
TEMPLATE = app

target.files += LAssistants
target.path = /usr/bin/

icons.files += data/icons/LAssistants.png
icons.path = /usr/share/pixmaps/

desktop.files += data/desktop/LAssistant.desktop.desktop
desktop.path = /usr/share/applications/

SOURCES += main.cpp\
        mainwindow.cpp \
    centralwidget.cpp \
    component/phoneheaderwidget.cpp \
    component/PhoneInfo/phoneinfopage.cpp \
    component/FileManager/phonefilemanagerpage.cpp \
    adbController/adbcontroller.cpp \
    component/PhoneInfo/phoneinforectangle.cpp \
    component/filetransfertophonewidget.cpp \
    component/SoftwareManager/softwarelistwidget.cpp \
    component/SoftwareManager/softwarelistwidgetitem.cpp \
    component/SoftwareManager/installtasklistwidget.cpp \
    KuwoMusic/kuwomusicwidget.cpp \
    KuwoMusic/Head/headwidget.cpp \
    KuwoMusic/Player/playerwidget.cpp \
    KuwoMusic/ContentList/navigatebar.cpp \
    KuwoMusic/ContentList/PaiHangBang/paihangbangwidget.cpp \
    KuwoMusic/ContentList/component/musiclistwidget.cpp \
    KuwoMusic/ContentList/component/pagenumbar.cpp \
    KuwoMusic/ContentList/contenttabwidget.cpp \
    KuwoMusic/ContentList/souSuo/sousuowidget.cpp \
    KuwoMusic/ContentList/XiaZai/xiazaiwidget.cpp \
    KuwoMusic/ContentList/XiaZai/downloadthread.cpp \
    KuwoMusic/ContentList/XiaZai/downloaditem.cpp \
    titlebar.cpp \
    component/softwareDownloadPage/softwaredownloadpage.cpp \
    component/softwareDownloadPage/softwarelist.cpp \
    component/softwareDownloadPage/softwaretypenavwidget.cpp \
    component/softwareDownloadPage/myhoverbutton.cpp \
    component/networkImgLabel/imagelabel.cpp \
    component/networkImgLabel/loadimgthread.cpp \
    component/softwareDownloadPage/downloadmanagerwidget.cpp

HEADERS  += mainwindow.h \
    centralwidget.h \
    component/phoneheaderwidget.h \
    component/PhoneInfo/phoneinfopage.h \
    component/FileManager/phonefilemanagerpage.h \
    adbController/adbcontroller.h \
    component/PhoneInfo/phoneinforectangle.h \
    component/filetransfertophonewidget.h \
    component/SoftwareManager/softwarelistwidget.h \
    component/SoftwareManager/softwarelistwidgetitem.h \
    component/SoftwareManager/installtasklistwidget.h \
    KuwoMusic/kuwomusicwidget.h \
    KuwoMusic/Head/headwidget.h \
    KuwoMusic/Player/playerwidget.h \
    KuwoMusic/ContentList/navigatebar.h \
    KuwoMusic/ContentList/PaiHangBang/paihangbangwidget.h \
    KuwoMusic/ContentList/component/musiclistwidget.h \
    KuwoMusic/ContentList/component/pagenumbar.h \
    KuwoMusic/ContentList/contenttabwidget.h \
    KuwoMusic/ContentList/souSuo/sousuowidget.h \
    KuwoMusic/ContentList/XiaZai/xiazaiwidget.h \
    KuwoMusic/ContentList/XiaZai/downloadthread.h \
    KuwoMusic/ContentList/XiaZai/downloaditem.h \
    titlebar.h \
    component/softwareDownloadPage/softwaredownloadpage.h \
    component/softwareDownloadPage/softwarelist.h \
    component/softwareDownloadPage/softwaretypenavwidget.h \
    component/softwareDownloadPage/myhoverbutton.h \
    component/networkImgLabel/imagelabel.h \
    component/networkImgLabel/loadimgthread.h \
    component/softwareDownloadPage/downloadmanagerwidget.h

FORMS += \
    KuwoMusic/ContentList/XiaZai/downloaditem.ui

RESOURCES += \
    sources.qrc

INSTALLS += target icons desktop
