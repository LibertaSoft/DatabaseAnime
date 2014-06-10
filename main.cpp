#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
