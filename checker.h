#ifndef _CHECKER_H_
#define _CHECKER_H_

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QSystemTrayIcon;
class QNetworkReply;
class QMenu;

class PKUDeanChecker : public QObject
{
    Q_OBJECT;
public:
    PKUDeanChecker(QObject * parent = 0);
    virtual ~PKUDeanChecker();
signals:
    void exited();
public slots:
    void exec();
    void exit();
private:
    QNetworkAccessManager * mgr;
    QSystemTrayIcon * icon;
    QMenu * iconMenu;
    static const QString url;
    QString id;
    int courseCount;
    int errorCount;
private slots:
    void sendRequest();
    void openWebPage();
    void checkData(QNetworkReply * reply);
    void showAbout();
    void showAboutQt();
    void checkNow();
};

#endif /* _CHECKER_H_ */
