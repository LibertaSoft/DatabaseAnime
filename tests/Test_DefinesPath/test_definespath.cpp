#include <QString>
#include "../../definespath.h"
#include <QtTest>

class Test_DefinesPath : public QObject
{
    Q_OBJECT

public:
    Test_DefinesPath();

private Q_SLOTS:
    void home();
    void share();

    void appData();
        void dbPath();
        void animeCovers();
        void mangaCovers();
        void amvCovers();
        void doramaCovers();

};

Test_DefinesPath::Test_DefinesPath()
{}

void Test_DefinesPath::home()
{
    QCOMPARE(DefinesPath::home(), QStandardPaths::writableLocation(QStandardPaths::HomeLocation) );
}

void Test_DefinesPath::share(){
    // Compare end whith '/'
    QCOMPARE(DefinesPath::share(), QStandardPaths::standardLocations( QStandardPaths::DataLocation ) );
}

void Test_DefinesPath::appData()
{
    QCOMPARE(DefinesPath::appData(true),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator() );

    QSettings s;
    s.setValue("WorkDirectory", "/tmp/LibertaSoft/DatabaseAnine/");
    QCOMPARE(DefinesPath::appData(false), QString("/tmp/LibertaSoft/DatabaseAnine/") );
    
    s.setValue("WorkDirectory", "/tmp/LibertaSoft/DatabaseAnine");
    QCOMPARE(DefinesPath::appData(false), QString("/tmp/LibertaSoft/DatabaseAnine/") );

    s.remove("WorkDirectory");
}

void Test_DefinesPath::dbPath()
{
    QCOMPARE(DefinesPath::dbPath(false),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator() );
    QCOMPARE(DefinesPath::dbPath(true),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator()
             + QString("DatabaseAnime.sqlite") );
}

void Test_DefinesPath::animeCovers()
{
    QCOMPARE(DefinesPath::animeCovers(),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator()
             + "animeCovers" + QDir::separator() );
    QCOMPARE(DefinesPath::animeCovers("/tmp/"),
             QString("/tmp/animeCovers") + QDir::separator() );
}
void Test_DefinesPath::mangaCovers()
{
    QCOMPARE(DefinesPath::mangaCovers(),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator()
             + "mangaCovers" + QDir::separator() );
    QCOMPARE(DefinesPath::mangaCovers("/tmp/"),
             QString("/tmp/mangaCovers") + QDir::separator() );
}
void Test_DefinesPath::amvCovers()
{
    QCOMPARE(DefinesPath::amvCovers(),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator()
             + "amvCovers" + QDir::separator() );
    QCOMPARE(DefinesPath::amvCovers("/tmp/"),
             QString("/tmp/amvCovers") + QDir::separator() );
}
void Test_DefinesPath::doramaCovers()
{
    QCOMPARE(DefinesPath::doramaCovers(),
             QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator()
             + "doramaCovers" + QDir::separator() );
    QCOMPARE(DefinesPath::doramaCovers("/tmp/"),
             QString("/tmp/doramaCovers") + QDir::separator() );
}

QTEST_APPLESS_MAIN(Test_DefinesPath)

#include "test_definespath.moc"
