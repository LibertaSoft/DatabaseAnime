#ifndef XMLDBAREADER_H
#define XMLDBAREADER_H

#include "globalenum.h"
#include <QtXml>
#include <QFile>
#include <QMap>

#include <QDebug>

class XmlDbaReader
{
    QXmlStreamReader            _streamReader;
    QXmlStreamReader::TokenType _token;
    QMap<QString,QVariant>      _currentData;
    sections::section           _currentSection;

    bool readItem();
public:
//    XmlDbaReader(QIODevice* stream);
    XmlDbaReader(QIODevice* stream);
    QMap<QString, QVariant> readNext();

    bool atEnd()const;
    bool hasError()const;
    sections::section currentSection()const;
};

#endif // XMLDBAREADER_H
