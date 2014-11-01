#include <QString>
#include <QtTest>
#include <QLocale>
#include "../../dbalocalization.h"

class Test_DbaLocalization : public QObject
{
    Q_OBJECT

public:
    Test_DbaLocalization();

private Q_SLOTS:
    void readExistsLocalizations();
    void getFileOfLocalization();
    void getQtBaseFileOfLocalization();
};

Test_DbaLocalization::Test_DbaLocalization()
{
}

void Test_DbaLocalization::readExistsLocalizations()
{
    QStringList paths;
    paths << "../../FakeData/DbaLocalization/folder1"; // en
    paths << "../../FakeData/DbaLocalization/folder2"; // ru
    paths << "../../FakeData/DbaLocalization/folder3"; // de

    QMap<QLocale::Language,QString> FakeData;
    FakeData[QLocale::Russian] = "Russian";
    FakeData[QLocale::English] = "English";
    FakeData[QLocale::German]  = "German";

    QCOMPARE(DbaLocalization::readExistsLocalizations(paths), FakeData);
}

void Test_DbaLocalization::getFileOfLocalization()
{
    QStringList paths;
    paths << "../../FakeData/DbaLocalization/folder1"; // en
    paths << "../../FakeData/DbaLocalization/folder2"; // ru
    paths << "../../FakeData/DbaLocalization/folder3"; // de

    QCOMPARE(DbaLocalization::getFileOfLocalization(QLocale::English, paths),
             paths[0] + "/l10n/DatabaseAnime_en.qm");
    QCOMPARE(DbaLocalization::getFileOfLocalization(QLocale::Russian, paths),
             paths[1] + "/l10n/DatabaseAnime_ru.qm");
    QCOMPARE(DbaLocalization::getFileOfLocalization(QLocale::German, paths),
             paths[2] + "/l10n/DatabaseAnime_de.qm");
}

void Test_DbaLocalization::getQtBaseFileOfLocalization()
{
    QStringList paths;
    paths << "../../FakeData/DbaLocalization/folder1"; // en
    paths << "../../FakeData/DbaLocalization/folder2"; // ru
    paths << "../../FakeData/DbaLocalization/folder3"; // de

    QCOMPARE(DbaLocalization::getQtBaseFileOfLocalization(QLocale::English, paths),
             QString("") );
    QCOMPARE(DbaLocalization::getQtBaseFileOfLocalization(QLocale::Russian, paths),
             paths[1] + "/l10n/qtbase_ru.qm");
    QCOMPARE(DbaLocalization::getQtBaseFileOfLocalization(QLocale::German, paths),
             paths[2] + "/l10n/qtbase_de.qm");
}

QTEST_APPLESS_MAIN(Test_DbaLocalization)

#include "test_dbalocalization.moc"
