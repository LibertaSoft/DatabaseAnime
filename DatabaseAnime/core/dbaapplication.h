#pragma once

#include "core/utils/logger.h"

#include <QLocale>
#include <QApplication>

class DbaApplication : public QApplication
{
private:
    QLocale::Language _language = QLocale::English;
    Logger *_logger = nullptr;
public:
    DbaApplication(int &argc, char **argv);
    ~DbaApplication();

public: // set/get
    void setLanguage(const QLocale::Language &language);
    QLocale::Language getLanguage();

    void setLogger( Logger *logger );
    Logger* logger();
};
