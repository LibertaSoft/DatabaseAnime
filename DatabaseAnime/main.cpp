#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>

#include <QDebug>

#include "stylemanager.h"

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

/*! \~russian
    @mainpage Database Anime

    Codin Style - почти такой же как и [Qt Coding Style](http://wiki.qt.io/Qt_Coding_Style)

    Немного очевидных вещей:

    Основная концепция работы программы:
    При запуске программы, отображается MainWidnow, в нём происходит инициализация языка программы и прочее.
    MainWindow - предназначен для отображения основной информации об аниме, или прочего, в зависимости от выбранного раздела в выпадающем списке, в левом, верхнем углу, а так же отображения страницы с информацией о приложении и её первых разработчиках(если появятся ещё люди думаю это прийдётся от туда убрать, не пихать же туда полный список, верно?)
    При выходе из приложения сохраняются положение окна, текущий раздел и прочая информация.
    При нажатии на кнопку "Опции" - открывается модальный диалог FormSettings(ну, в шараге мы изучали VCL, а этот диалог был первым, потом менять название так и не стал)
    При нажатии кнопок "Добавить" или "Редактировать" происходит открытие, в зависимости от текущего выбранного раздела, диалога `DialogAddAnime`, `DialogAddManga`, `DialogAddAmv` или `DialogAddDorama`.

    - Взаимодействие с базой данных осуществляется посредством класса `MngrQuerys`.
    - Класс MngrConnection предоставляет API для управления соединением с БД.
    - Класс DefinesPath - предоставляет набор статических методов возвращающих пути к определённым местам в ФС
    - В файле globalEnum описаны основные перечисления использующиеся в программе
    - Класс shikimoriApi - инкапсулирует API для работы с сайтом [shikimori.org](http://shikimori.org)
    - Классы XmlDbaReader и XmlDbaWriter предоставляеют интерфейс для чтения и записи экспортного формата DBA.
    - Виджет ImageCover - представляет собой переписанный QLabel с возможностью Drag&Dropa изображений.
    - Виджет LookProgressBar - представляет собой ProgressBar с кнопками "+" и "-" отображающий прогресс просмотра, п так же поддерживает увеличение и уменьшение по средствам колёсика мыши.
    - Виджет StackWgt_MouseHideOver - Представляет собой StackWidget с возможностью отображения Обложки/Дерева каталогов при наведении мыши.
*/
int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(myMessageOutput);
#endif

    QApplication app(argc, argv);
    app.setOrganizationName("LibertaSoft");
    app.setOrganizationDomain("https://github.com/LibertaSoft");
    app.setApplicationName("DatabaseAnime");
    app.setApplicationVersion("1.3.1");
    app.setApplicationDisplayName( QObject::tr("Database Anime") );
    app.setWindowIcon( QIcon("://images/DBA_Icon.png") );

    {
        QSettings settings;
        int currentStyle = settings.value(Options::Style::CurrentStyleIndex, 0).toInt();
        QString currentStyleName = settings.value(Options::Style::CurrentStyleName, "").toString();
        if( (currentStyle > 0 ) && ( ! currentStyleName.isEmpty() ) ){
            app.setStyle( QStyleFactory::create("Fusion") );
            app.setPalette( StyleManager::getPaletteOfStyle( currentStyleName ) );
            app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
        }
    }

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
