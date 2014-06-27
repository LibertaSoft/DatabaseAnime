#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("0.0.41 Alpha");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
