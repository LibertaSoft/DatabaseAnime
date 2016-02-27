#pragma once

#include "core/utils/logger.h"

#include <QLocale>
#include <QApplication>
#include <QTranslator>
#include <QHash>
#include <QSettings>

class DbaApplication : public QApplication
{
private:
    QLocale _locale;
    Logger *_logger = nullptr;
    QTranslator _qtBaseTranslator;
    QTranslator _appTranslator;
    QSettings *_settings = nullptr;

    void setProgramInfo();
    void loadLocalization();

public:
    DbaApplication(int &argc, char **argv);
    ~DbaApplication();

public: // set/get
    void setLocale(const QLocale &locale);
    QLocale getLocale();

    void setLogger( Logger *logger );
    Logger* logger();
    QSettings* settings();
};
