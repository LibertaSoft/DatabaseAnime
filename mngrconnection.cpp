#include "mngrconnection.h"

#include <QDir>
#include <QUrl>
#include "definespath.h"
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

    if( !QDir().mkpath( DefinesPath::dbPath() ) ){
        qCritical() << "Cannot createed work directory"
                    << "\nPath: " << DefinesPath::dbPath();
        QMessageBox::critical(0 , QObject::tr("Critical"),
                              QObject::tr("It was not succeeded to create a directory for a database.") );
    }else{
        db.setDatabaseName( DefinesPath::dbPath(true) );
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
