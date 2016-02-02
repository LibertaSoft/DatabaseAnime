#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

enum class LogLevel {
    NO_LOG,
    CRITICAL,
    WARNING,
    INFO,
    DEBUG
};

class Logger
{
private:
    Logger *_instance = nullptr;

    LogLevel _logLevel = LogLevel::WARNING;
public:
    Logger* instance();

    void debug(const QString &message);
    void info(const QString &message);
    void warning(const QString &message);
    void critical(const QString &message);

    // set/get
    void setLogLevel( const LogLevel level );
    LogLevel getLogLevel();
};

#endif // LOGGER_H
