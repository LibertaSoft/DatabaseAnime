#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
