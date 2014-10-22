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

    //QFontDatabase::addApplicationFont("://DejaVuSans.ttf ");

    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("1.1.0");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    QSettings settings;
    QString set_language       = settings.value( "Application/l10n", "<System>" ).toString();
    short     set_language_index = settings.value( "Application/l10n_index", 0 ).toInt();
    QTranslator qtTr;

    /*
    qDebug() << "Standard Locations:";
    qDebug() << QStandardPaths::displayName( QStandardPaths::DataLocation ) << QStandardPaths::writableLocation( QStandardPaths::DataLocation );
    qDebug() << "Cache:     " << QStandardPaths::writableLocation( QStandardPaths::CacheLocation );
    qDebug() << "Config:    " << QStandardPaths::writableLocation( QStandardPaths::ConfigLocation );
    qDebug() << "Desktop:   " << QStandardPaths::writableLocation( QStandardPaths::DesktopLocation );
    qDebug() << "Documents: " << QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation );
    qDebug() << "Download:  " << QStandardPaths::writableLocation( QStandardPaths::DownloadLocation );
    qDebug() << "Font:      " << QStandardPaths::writableLocation( QStandardPaths::FontsLocation );
    qDebug() << "GenCache:  " << QStandardPaths::writableLocation( QStandardPaths::GenericCacheLocation );
    qDebug() << "RunTime:   " << QStandardPaths::writableLocation( QStandardPaths::RuntimeLocation );
    qDebug() << "Temp:      " << QStandardPaths::writableLocation( QStandardPaths::TempLocation );
    qDebug() << "App:       " << QStandardPaths::writableLocation( QStandardPaths::ApplicationsLocation );
qDebug()<< QStandardPaths::standardLocations( QStandardPaths::DataLocation );

    */

    const QString sharePath( QApplication::applicationDirPath() + QDir::separator()
                             + ".." + QDir::separator()
                             + "share" + QDir::separator()
                             + "DatabaseAnime" + QDir::separator()
                             );

    if( set_language_index == 0 ){
        /*, QLibraryInfo::location(QLibraryInfo::TranslationsPath)*/
        qtTr.load( sharePath + "l10n" + QDir::separator()
                   + "qtbase_" + QLocale::system().name().section('_', 0, 0) + ".qm" );
    }else{
        qtTr.load( sharePath + "l10n" + QDir::separator()
                   + "qtbase_" + set_language + ".qm" );
    }
    app.installTranslator(&qtTr);

    QTranslator dbaTr;
    if( set_language_index == 0 ){
        dbaTr.load( sharePath + "l10n" + QDir::separator()
                    + "DatabaseAnime_" + QLocale::system().name().section('_', 0, 0) + ".qm" );
    }else{
        dbaTr.load( sharePath + "l10n" + QDir::separator()
                    + "DatabaseAnime_" + set_language + ".qm" );
    }
    app.installTranslator(&dbaTr);

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
