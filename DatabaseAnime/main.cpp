#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>

#include <QDebug>

/// Output to log-file
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

//    QSettings settings;

    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("1.2.1 Pre-Alpha");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    QSettings settings;
    int currentStyle = settings.value(Options::Style::CurrentStyle, 0).toInt();
    if(currentStyle > 0){

        qApp->setStyle(QStyleFactory::create("Fusion"));

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    }

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
