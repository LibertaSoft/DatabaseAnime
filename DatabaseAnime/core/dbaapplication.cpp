#include "dbaapplication.h"

#include <QIcon>

DbaApplication::DbaApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setOrganizationName("LibertaSoft");
    setOrganizationDomain("https://github.com/LibertaSoft");
    setApplicationName("DatabaseAnime");
    setApplicationVersion("1.4.2");
    setApplicationDisplayName( QObject::tr("Database Anime") );
    setWindowIcon( QIcon("://images/DBA_Icon.png") );

    setLogger( new Logger );
}

DbaApplication::~DbaApplication()
{
    delete _logger;
}

void DbaApplication::setLanguage(const QLocale::Language &language)
{
    _language = language;
}

QLocale::Language DbaApplication::getLanguage()
{
    return _language;
}

/*!
 * \brief DbaApplication::setLogger
 * \param logger - право владения объектом класс берёт на себя.
 */
void DbaApplication::setLogger(Logger *logger)
{
    delete _logger;
    _logger = logger;
}

Logger *DbaApplication::logger()
{
    return _logger;
}
