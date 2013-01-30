#include <checker.h>
#include <QtGui>
#include <QtNetwork>

#define INTERVAL 30
#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17"
#define MAX_ERROR_COUNT 10
#define MESSAGE_TIME_OUT 600

const QString PKUDeanChecker::url =
    QString("http://dean.pku.edu.cn/student/grade.php?PHPSESSID=%1");

PKUDeanChecker::PKUDeanChecker(QObject * parent) : QObject(parent), courseCount(-1), errorCount(0)
{
    mgr = new QNetworkAccessManager(this);
    icon = new QSystemTrayIcon(QIcon(":/pku.png"), this);
    QObject::connect(mgr, SIGNAL(finished(QNetworkReply *)),
                     this, SLOT(checkData(QNetworkReply *)));
    iconMenu = new QMenu;
    QAction * openAct = iconMenu
        ->addAction(QString::fromUtf8("打开成绩单页面"));
    QAction * aboutAct = iconMenu
        ->addAction(QString::fromUtf8("关于")); 
    QAction * aboutQtAct = iconMenu
        ->addAction(QString::fromUtf8("关于Qt"));
    QAction * exitAct = iconMenu
        ->addAction(QString::fromUtf8("退出"));
    QObject::connect(openAct, SIGNAL(triggered()),
                     this, SLOT(openWebPage()));
    QObject::connect(aboutAct, SIGNAL(triggered()),
                     this, SLOT(showAbout()));
    QObject::connect(aboutQtAct, SIGNAL(triggered()),
                     this, SLOT(showAboutQt()));
    QObject::connect(exitAct, SIGNAL(triggered()),
                     this, SLOT(exit()));
    icon->setContextMenu(iconMenu);

    icon->setToolTip("PKU Dean Checker");
}

void PKUDeanChecker::exec()
{
    QDialog dialog;
    dialog.setWindowTitle("PKU Dean Checker");
    QVBoxLayout * layout = new QVBoxLayout;
    QLabel * idLabel = new QLabel(QString::fromUtf8("输入Session ID:"));
    QLineEdit * idEdit = new QLineEdit;
    layout->addWidget(idLabel);
    layout->addWidget(idEdit);

    //TODO: Add an button "Get session ID"
    
    QDialogButtonBox * btnBox =
        new QDialogButtonBox(QDialogButtonBox::Ok|
                             QDialogButtonBox::Close);
    layout->addWidget(btnBox);

    dialog.setLayout(layout);

    QObject::connect(btnBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(btnBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    
    if(dialog.exec()==QDialog::Rejected){
        this->exit();
        return;
    }

    id = idEdit->text();
    
    icon->show();
    QTimer * timer = new QTimer(this);
    timer->setInterval(INTERVAL*1000);
    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(sendRequest()));
    QObject::connect(this, SIGNAL(exited()),
                     timer, SLOT(stop()));
    QObject::connect(this, SIGNAL(exited()),
                     timer,SLOT(deleteLater()));
    QTimer::singleShot(5000, this, SLOT(sendRequest()));
    timer->start();
}

void PKUDeanChecker::exit()
{
    icon->hide();
    emit exited();
}

PKUDeanChecker::~PKUDeanChecker()
{
    delete iconMenu;
}

void PKUDeanChecker::sendRequest()
{
    qDebug("[At %s]Sending request to %s",
           QTime::currentTime().toString().toAscii().data(),
           url.arg(id).toAscii().data());
    QNetworkRequest request(QUrl(url.arg(id)));
    request.setRawHeader("User-Agent", USER_AGENT);
    mgr->get(request);
}

void PKUDeanChecker::openWebPage()
{
    QDesktopServices::openUrl(QUrl(url.arg(id)));
}

void PKUDeanChecker::checkData(QNetworkReply * reply)
{
    if(errorCount>=MAX_ERROR_COUNT) {
        icon->showMessage("PKU Dean Checker",
                          QString::fromUtf8("错误出现%1次，放弃并退出！")
                          .arg(MAX_ERROR_COUNT),
                          QSystemTrayIcon::Critical,
                          5000);
        QTimer::singleShot(5000, this, SLOT(exit()));
        return;
    }    
    if(reply->error()!=QNetworkReply::NoError) {
        qWarning("Request failed, error code : %d", reply->error());
        ++errorCount;
        return;
    }
    int count = 0;
    int pos = 0;
    QString content =
        QTextCodec::codecForName("GBK")
        ->toUnicode(reply->readAll());
//    qDebug("Webpage content:\n%s", content.toUtf8().data());
    QRegExp reg("<tr>");
    while(pos>=0){
        pos = reg.indexIn(content, pos);
        if(pos>=0){
            ++pos;
            ++count;
        }
    }
    --count;
    qDebug("[At %s]Get course count: %d",
           QTime::currentTime().toString()
           .toAscii().data(),           
           count);
    if(count==-1) {
            qWarning("Reply parse error, invalid session ID?");
            icon->showMessage("PKU Dean Checker",
                              QString::fromUtf8("解析失败，是否输入了无效的Session ID?"),
                              QSystemTrayIcon::Warning,
                              5000);
            ++errorCount;
            return;
    }
    if(courseCount==-1)
            icon->showMessage("PKU Dean Checker",
                              QString::fromUtf8("已公布成绩课程数：%1").arg(count));
    else if(count!=courseCount) {
        //TODO: Show more details about courses here.
        icon->showMessage("PKU Dean Checker",
                          QString::fromUtf8("有%1门课程成绩刚刚公布")
                          .arg(count-courseCount),
                          QSystemTrayIcon::Information,
                          MESSAGE_TIME_OUT*1000);
    }
    courseCount = count;
}

void PKUDeanChecker::showAbout()
{
    QMessageBox::about(NULL, QString::fromUtf8("关于"),
                       QString::fromUtf8(
                       "PKU Dean Checker\n"
                       "Copyright (C) 2013 Huang KangJing\n"
                       "\n"
                       "This program is free software: you can redistribute it and/or modify\n"
                       "it under the terms of the GNU General Public License as published by\n"
                       "the Free Software Foundation, either version 3 of the License, or\n"
                       "(at your option) any later version.\n"
                       "\n"
                       "This program is distributed in the hope that it will be useful,\n"
                       "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                       "GNU General Public License for more details.\n"
                       "\n"
                       "You should have received a copy of the GNU General Public License\n"
                       "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
                       "\n"
                       "版权所有 (C) 2013 黄康靖\n"
                       "\n"
                       "请向<huangkangjing@gmail.com>报告程序错误与缺陷。\n")
        );
}

void PKUDeanChecker::showAboutQt()
{
    QMessageBox::aboutQt(NULL, QString::fromUtf8("关于Qt"));
}
    
