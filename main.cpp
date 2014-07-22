#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <mainwindow.h>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("0.0.78 Pre-Alpha");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    QSettings settings;
    QString set_language       = settings.value( "Application/l10n", "<System>" ).toString();
    int     set_language_index = settings.value( "Application/l10n_index", 0 ).toInt();
    QTranslator qtTr;
    if( set_language_index == 0 ){
        /*, QLibraryInfo::location(QLibraryInfo::TranslationsPath)*/
        qtTr.load( "qtbase_" + QLocale::system().name() );
    }else{
        qtTr.load( QApplication::applicationDirPath() + "/qtbase_" + set_language + ".qm" );
    }
    app.installTranslator(&qtTr);

    QTranslator dbaTr;
    if( set_language_index == 0 ){
        dbaTr.load( "DatabaseAnime_" + QLocale::system().name() );
    }else{
        dbaTr.load( QApplication::applicationDirPath() + "/DatabaseAnime_" + set_language + ".qm" );
    }
    app.installTranslator(&dbaTr);

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
