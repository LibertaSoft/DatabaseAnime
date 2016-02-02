#include "logger.h"

Logger *Logger::instance()
{
    if( _instance == nullptr ){
        _instance = new Logger;
    }
    return _instance;
}

void Logger::debug(const QString &message)
{
    qDebug() << message;
}

void Logger::info(const QString &message)
{
    qInfo() << message;
}

void Logger::warning(const QString &message)
{
    qWarning() << message;
}

void Logger::critical(const QString &message)
{
    qCritical() << message;
}

void Logger::setLogLevel(const LogLevel level)
{
    _logLevel = level;
}

LogLevel Logger::getLogLevel()
{
    return _logLevel;
}
