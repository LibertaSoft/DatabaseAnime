#include "xmldbawriter.h"

XmlDbaWriter::XmlDbaWriter(QIODevice* stream)
    :_streamWriter(stream)
{
    _streamWriter.setAutoFormatting(true);
//    _streamWriter.writeDTD("<!DOCTYPE DatabaseAnime>");
}

QString XmlDbaWriter::getSectionName(sections::section section)
{
    switch( section ){
    case sections::anime :
        return QString("Anime");
    case sections::manga :
        return QString("Manga");
    case sections::amv :
        return QString("Amv");
    case sections::dorama :
        return QString("Dorama");
    case sections::none : ;
    }
    return QString::null;
}

void XmlDbaWriter::startSection(sections::section section)
{
    _streamWriter.writeStartElement( getSectionName(section) );
}

void XmlDbaWriter::startSection()
{
    _streamWriter.writeStartElement( getSectionName(_currentSection) );
}

void XmlDbaWriter::endSection()
{
    _streamWriter.writeEndElement();
}

void XmlDbaWriter::startElement(QString name)
{
    _streamWriter.writeStartElement(name);
}

void XmlDbaWriter::endElement()
{
    _streamWriter.writeEndElement();
}

void XmlDbaWriter::startDocument()
{
    _streamWriter.writeStartDocument();
}

void XmlDbaWriter::endDocument()
{
    _streamWriter.writeEndDocument();
}

bool XmlDbaWriter::writeNext(QMap<QString, QString> data)
{
    _streamWriter.writeStartElement("item");

    QMapIterator<QString, QString> i(data);
    while( i.hasNext() ){
        i.next();
        _streamWriter.writeTextElement(i.key(), i.value());
    }

    _streamWriter.writeEndElement(); // item
}

bool XmlDbaWriter::hasError() const
{
    return _streamWriter.hasError();
}

sections::section XmlDbaWriter::getCurrentSection() const
{
    return _currentSection;
}

void XmlDbaWriter::setSection(sections::section section)
{
    _currentSection = section;
}
