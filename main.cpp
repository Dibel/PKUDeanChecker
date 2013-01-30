#include <QApplication>
#include <QtGlobal>
#include "checker.h"
#include <cstdio>
#include <cstdlib>

void MessageHandler(QtMsgType type, const char *msg)
{
#ifdef _WIN32
    FILE * f = fopen("log.txt","a");
    switch (type) {
    case QtDebugMsg:
#ifdef _DEBUG_ON
        fprintf(f, "[Debug]%s\n", msg);
#endif /* _DEBUG_ON */
        break;
    case QtWarningMsg:
        fprintf(f, "[Warning]%s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(f, "[Critical]%s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(f, "[Fatal]%s\n", msg);
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
    qInstallMsgHandler(MessageHandler);
    QApplication app(argc,argv);
    app.setQuitOnLastWindowClosed(false);
    PKUDeanChecker checker;
    QObject::connect(&checker, SIGNAL(exited()),
                     &app, SLOT(quit()));
    checker.exec();
    return app.exec();
}
