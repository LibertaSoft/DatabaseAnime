#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("LibreSoft");
    app.setOrganizationDomain("https://github/libresoft");
    app.setApplicationName("Database Anime");

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
