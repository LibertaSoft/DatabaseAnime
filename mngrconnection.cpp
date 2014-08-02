#include "mngrconnection.h"

#include <QDir>
#include <QUrl>

MngrConnection::MngrConnection()
{
    const QString dbUser("");
    const QString dbHost("");
    const QString dbPass("");

    db = QSqlDatabase::addDatabase("QSQLITE");
    QUrl DBPath( QDir::homePath() + "/."+QApplication::organizationName()+"/"+QApplication::applicationName() );
    if( !QDir().mkpath( DBPath.toString() ) ){
        qCritical() << "Cannot createed app directory in home path"
                    << "\nPath: "
                    << DBPath;
//        QMessageBox::warning( 0, QObject::tr("Warning"), QObject::tr("It was not succeeded to create a directory for a database.") );
    }else{
        db.setDatabaseName( DBPath.toString() +"/DatabaseAnime.sqlite");
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
