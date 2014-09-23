#include <QApplication>
#include <QtGlobal>
#include "checker.h"
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <QtPlugin>
//Q_IMPORT_PLUGIN(qcncodecs);
#endif /* _WIN32 */

void MessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const char *msgData = msg.toStdString().data();
#ifdef _WIN32
    FILE * f = fopen("log.txt","a");
    switch (type) {
    case QtDebugMsg:
#ifdef _DEBUG_ON
        fprintf(f, "[Debug]%s\n", msgData);
#endif /* _DEBUG_ON */
        break;
    case QtWarningMsg:
        fprintf(f, "[Warning]%s\n", msgData);
        break;
    case QtCriticalMsg:
        fprintf(f, "[Critical]%s\n", msgData);
        break;
    case QtFatalMsg:
        fprintf(f, "[Fatal]%s\n", msgData);
        fclose(f);
        abort();
    }
    fclose(f);
#else /* _WIN32 */
    switch (type) {
    case QtDebugMsg:
#ifdef _DEBUG_ON
        fprintf(stderr, "[Debug]%s\n", msg);
#endif /* _DEBUG_ON */
        break;
    case QtWarningMsg:
        fprintf(stderr, "[Warning]%s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "[Critical]%s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "[Fatal]%s\n", msg);
        abort();
    }
#endif /* _WIN32 */    
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(MessageHandler);
    QApplication app(argc,argv);
    app.setQuitOnLastWindowClosed(false);
    PKUDeanChecker checker;
    QObject::connect(&checker, SIGNAL(exited()),
                     &app, SLOT(quit()));
    checker.exec();
    return app.exec();
}
