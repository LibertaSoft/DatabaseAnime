#include "mainwindow.h"
#include "definespath.h"

#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile f( QStandardPaths::writableLocation( QStandardPaths::CacheLocation )
             + QDir::separator() + QDate::currentDate().toString(Qt::ISODate) + ".log" );
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
    app.setApplicationVersion("1.1.1 Pre-Alpha");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    QSettings settings;
    QString set_language       = settings.value( "Application/l10n", "<System>" ).toString();
    short     set_language_index = settings.value( "Application/l10n_index", 0 ).toInt();
    QTranslator qtTr;

    /*
    qDebug() << QStandardPaths::standardLocations( QStandardPaths::CacheLocation );
    qDebug() << QStandardPaths::standardLocations( QStandardPaths::ConfigLocation );
    qDebug() << QStandardPaths::standardLocations( QStandardPaths::DataLocation );
    qDebug() << QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation );
    // */
    /*
    qDebug() << "Standard Locations:";
    qDebug() << QStandardPaths::displayName( QStandardPaths::DataLocation ) << QStandardPaths::writableLocation( QStandardPaths::DataLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::CacheLocation ) << QStandardPaths::writableLocation( QStandardPaths::CacheLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::ConfigLocation ) << QStandardPaths::writableLocation( QStandardPaths::ConfigLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::DesktopLocation ) << QStandardPaths::writableLocation( QStandardPaths::DesktopLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::DocumentsLocation ) << QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::DownloadLocation ) << QStandardPaths::writableLocation( QStandardPaths::DownloadLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::FontsLocation ) << QStandardPaths::writableLocation( QStandardPaths::FontsLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::GenericCacheLocation ) << QStandardPaths::writableLocation( QStandardPaths::GenericCacheLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::RuntimeLocation ) << QStandardPaths::writableLocation( QStandardPaths::RuntimeLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::TempLocation ) << QStandardPaths::writableLocation( QStandardPaths::TempLocation );
    qDebug() << QStandardPaths::displayName( QStandardPaths::ApplicationsLocation ) << QStandardPaths::writableLocation( QStandardPaths::ApplicationsLocation );

    // */

    if( set_language_index == 0 ){
        qtTr.load( DefinesPath::l10n()
                   + "qtbase_" + QLocale::system().name().section('_', 0, 0) + ".qm" );
    }else{
        qtTr.load( DefinesPath::l10n()
                   + "qtbase_" + set_language + ".qm" );
    }
    app.installTranslator(&qtTr);

    QTranslator dbaTr;
    if( set_language_index == 0 ){
        dbaTr.load( DefinesPath::l10n()
                    + "DatabaseAnime_" + QLocale::system().name().section('_', 0, 0) + ".qm" );
    }else{
        dbaTr.load( DefinesPath::l10n()
                    + "DatabaseAnime_" + set_language + ".qm" );
    }
    app.installTranslator(&dbaTr);

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
