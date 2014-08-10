#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
//#include <QLibraryInfo>
#include <QSettings>
#include <QDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile f( QDir::homePath() + QDir::separator()
             + "." + QApplication::organizationName()
             + QDir::separator() + QApplication::applicationName()
             + QDir::separator() + "log.txt" );
    f.open(QFile::Append);
    switch (type) {
     case QtDebugMsg:
        f.write( "[Debug]\n"
                 "Message: " + msg.toUtf8() + "\n"
                 + "File: " + context.file + "\n"
                 + "Line: " + QString::number(context.line).toUtf8() + "\n"
                 + "Function: " + context.function
                 + "\n-----\n" );
        break;
     case QtWarningMsg:
        f.write( "[Warning]\n"
                 "Message: " + msg.toUtf8() + "\n"
                 + "File: " + context.file + "\n"
                 + "Line: " + QString::number(context.line).toUtf8() + "\n"
                 + "Function: " + context.function
                 + "\n-----\n" );
        break;
     case QtCriticalMsg:
        f.write( "[Critical]\n"
                 "Message: " + msg.toUtf8() + "\n"
                 + "File: " + context.file + "\n"
                 + "Line: " + QString::number(context.line).toUtf8() + "\n"
                 + "Function: " + context.function
                 + "\n-----\n" );
        break;
     case QtFatalMsg:
        f.write( "[Fatal]\n"
                 "Message: " + msg.toUtf8() + "\n"
                 + "File: " + context.file + "\n"
                 + "Line: " + QString::number(context.line).toUtf8() + "\n"
                 + "Function: " + context.function
                 + "\n-----\n" );
        f.close();
        abort();
     }
    f.close();
}

int main(int argc, char *argv[])
{
    #ifndef QT_DEBUG
        qInstallMessageHandler(myMessageOutput);
    #endif

//    QStringList paths = QCoreApplication::libraryPaths();
//    paths.append(".");
//    paths.append("imageformats");
//    paths.append("platforms");
//    paths.append("sqldrivers");
//    QCoreApplication::setLibraryPaths(paths);

    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("0.1.5 Pre-Alpha");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    QSettings settings;
    QString set_language       = settings.value( "Application/l10n", "<System>" ).toString();
    int     set_language_index = settings.value( "Application/l10n_index", 0 ).toInt();
    QTranslator qtTr;
    if( set_language_index == 0 ){
        /*, QLibraryInfo::location(QLibraryInfo::TranslationsPath)*/
        qtTr.load( QApplication::applicationDirPath() + QDir::separator() + "qtbase_" + QLocale::system().name().left(2) + ".qm" );
    }else{
        qtTr.load( QApplication::applicationDirPath() + QDir::separator() + "qtbase_" + set_language + ".qm" );
    }
    app.installTranslator(&qtTr);

    QTranslator dbaTr;
    if( set_language_index == 0 ){
        dbaTr.load( QApplication::applicationDirPath() + QDir::separator() + "DatabaseAnime_" + QLocale::system().name().left(2) + ".qm" );
    }else{
        dbaTr.load( QApplication::applicationDirPath() + QDir::separator() + "DatabaseAnime_" + set_language + ".qm" );
    }
    app.installTranslator(&dbaTr);

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
