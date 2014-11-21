#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if( type == QtDebugMsg){
//        qDebug() << msg; // out to console
        return;
    }

    QDir().mkpath( DefinesPath::log() );
    QFile f( DefinesPath::log() + QDate::currentDate().toString(Qt::ISODate) + ".log" );

    f.open( QFile::WriteOnly | QFile::Append);
    const QStringList level( QStringList() << "[Debug]" << "[Warning]" << "[Critical]" << "[Fatal]" );

    QString data( level.at(type) + '\n'
                     + "Message: " + msg.toUtf8() + "\n"
                     + "File: " + context.file + "\n"
                     + "Line: " + QString::number(context.line).toUtf8() + "\n"
                     + "Function: " + context.function
                     + "\n\n");
    f.write( data.toUtf8() );
    f.close();

    if( type == QtFatalMsg )
        abort();
}

int main(int argc, char *argv[])
{
    #ifndef QT_DEBUG
        qInstallMessageHandler(myMessageOutput);
    #endif


    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("1.2.0 Beta");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
