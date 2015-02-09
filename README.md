# [LAssistant]
LAssistant是运行于linux系统上的安卓手机软件管理工具，用Qt5编写，它能帮助您管理手机存储卡文件，安装及删除软件，及提供一些其它服务。

**LAssistant is free software licensed under the term of LGPL v2.1. If you use LAssistant or its constituent libraries, you must adhere to the terms of the license in question.
#### 软件主页
主页与ubuntu包下载地址：http://www.mvgather.com


# For Developers

#### 依赖

[![Qt](http://qt-project.org/images/qt13a/Qt-logo.png "Qt5")](http://www.qt.io/)
[![adb](http://developer.android.com/assets/images/dac_logo@2x.png "adb")](http://developer.android.com/index.html)

Latest Qt5 and adb releases are preferred.

#### 构建与运行

首先，需要将安卓开发套件里的“adb”这个二进制工具文件复制到程序能找到它的地方，一般放在：/usr/bin/ 目录。
如果您正在使用ubuntu系统，可以直接安装adb:sudo apt-get install android-tools-adb

然后，启动adb服务：adb需以root方式。以ubuntu为例，请在终端中输入：sudo adb start-server，如果未以root方式启动，可能无法识别您的手机，请先停止服务：adb kill-server，然后再root方式启动adb。

最后，插入您的手机，用Qt5带的Qt Creator工具打开源码包中的 LAssistant.pro 然后“构建”即可成功。程序运行后，即可管理您的手机。

