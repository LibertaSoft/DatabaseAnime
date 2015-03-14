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
    bool                        _dbaFormat;

    bool readItem();
public:
    XmlDbaReader(QIODevice* stream);
    bool isDbaFormat() const;
    bool readHeader();
    bool readHeader(quint64 &countAnime, quint64 &countManga, quint64 &countAmv, quint64 &countDorama);
    QMap<QString, QVariant> readNext();

    bool atEnd()const;
    bool hasError()const;
    sections::section currentSection()const;
};

#endif // XMLDBAREADER_H
