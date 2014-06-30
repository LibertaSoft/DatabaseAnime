#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <mainwindow.h>

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("0.0.46 Alpha");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    QTranslator qtTr;
    qtTr.load( "qtbase_" + QLocale::system().name()/*, QLibraryInfo::location(QLibraryInfo::TranslationsPath)*/ );
    app.installTranslator(&qtTr);

    QTranslator dbaTr;
    dbaTr.load( "DatabaseAnime_" + QLocale::system().name() );
    app.installTranslator(&dbaTr);

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
