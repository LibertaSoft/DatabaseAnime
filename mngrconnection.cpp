#include "mngrconnection.h"

#include <QDir>
#include <QUrl>
#include <QMessageBox>

MngrConnection::MngrConnection()
{
    const QString dbUser("");
    const QString dbHost("");
    const QString dbPass("");

    if( !QSqlDatabase::isDriverAvailable("QSQLITE") ){
        qCritical() << "Cannot avalible QSQLITE driver";
        QMessageBox::critical(0 , QObject::tr("Critical"),
                              QObject::tr("Cannot avalible database driver") );
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    const QString DBPath(QDir::homePath() + QDir::separator() +"."
                         + QApplication::organizationName()
                         + QDir::separator() + QApplication::applicationName() + QDir::separator());

    QSettings settings;
    if( !QDir().mkpath( settings.value("WorkDirectory", DBPath).toString() ) ){
        qCritical() << "Cannot createed work directory"
                    << "\nPath: "
                    << settings.value("WorkDirectory", DBPath).toString();
        QMessageBox::critical(0 , QObject::tr("Critical"),
                              QObject::tr("It was not succeeded to create a directory for a database.") );
    }else{
        db.setDatabaseName( settings.value("WorkDirectory", DBPath).toString() + QDir::separator() + "DatabaseAnime.sqlite");
        db.setUserName( dbUser );
        db.setHostName( dbHost );
        db.setPassword( dbPass );
    }
}

bool MngrConnection::open()
{
    return db.open();
}

void MngrConnection::close()
{
    db.close();
}

bool MngrConnection::transaction()
{
    return db.transaction();
}

bool MngrConnection::commit()
{
    return db.commit();
}

bool MngrConnection::rollback()
{
    return db.rollback();
}

bool MngrConnection::isOpen()
{
    return db.isOpen();
}
