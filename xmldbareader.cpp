#include "xmldbareader.h"

XmlDbaReader::XmlDbaReader(QIODevice* stream)
    :_streamReader(stream)
{}

bool XmlDbaReader::isDbaFormat()
{
    #pragma DTD_Needed
    _token = _streamReader.readNext();
     if (_token == QXmlStreamReader::StartElement){
         // Need DTD Doctype ...
         if( _streamReader.name() == "DatabaseAnime" )
             return true;
     }
     return false;
}

bool XmlDbaReader::readHeader()
{
    #pragma DTD_Needed
    _token = _streamReader.readNext();
     if (_token == QXmlStreamReader::StartElement){
         // Need DTD Doctype ...
         if( _streamReader.name() == "DatabaseAnime" )
             return true;
     }
     return false;
}

bool XmlDbaReader::readHeader(quint64 &countAnime, quint64 &countManga, quint64 &countAmv, quint64 &countDorama)
{
    #pragma DTD_Needed
    _token = _streamReader.readNext(); // <?xml ... ?>
    _token = _streamReader.readNext();
    if (_token == QXmlStreamReader::StartElement){
        // Need DTD Doctype ...
            if( _streamReader.name() == "DatabaseAnime" ){
            QXmlStreamAttributes attr = _streamReader.attributes();
            countAnime  = attr.value("CountAnime" ).toULongLong();
            countManga  = attr.value("CountManga" ).toULongLong();
            countAmv    = attr.value("CountAmv"   ).toULongLong();
            countDorama = attr.value("CountDorama").toULongLong();
            return true;
        }
    }
    qDebug() << "is not dba format";
    qDebug() << _streamReader.name();
    return false;
}

QMap<QString, QVariant> XmlDbaReader::readNext()
{

    _currentData.clear();
    if( _streamReader.atEnd() ){
        qCritical() << "[XmlDbaReader::readNext] " << _streamReader.errorString();
        return _currentData;
    }

    _token = _streamReader.readNext();
    if (_token == QXmlStreamReader::StartElement){

        if ( _streamReader.name() == "Anime")
            _currentSection = sections::anime;
        else if ( _streamReader.name() == "Manga")
            _currentSection = sections::manga;
        else if ( _streamReader.name() == "Amv")
            _currentSection = sections::amv;
        else if ( _streamReader.name() == "Dorama")
            _currentSection = sections::dorama;

        if( _streamReader.name() == "item" ){
            if( ! readItem() ){
                qCritical() << "[XmlDbaReader::readNext] " << _streamReader.errorString();
                _currentData.clear();
            }
        }
    }
    if( _streamReader.hasError() ){
        qCritical() << "[XmlDbaReader::readNext] " << _streamReader.errorString();
        return _currentData;
    }
    return _currentData;
}


bool XmlDbaReader::readItem()
{
    _streamReader.readNext();
    while ( !(_streamReader.tokenType() == QXmlStreamReader::EndElement
              && _streamReader.name() == "item") ){
        if( _streamReader.hasError() ){
            qCritical() << "[XmlDbaReader::readItem] " << _streamReader.errorString();
            return false;
        }
        if (_streamReader.tokenType() == QXmlStreamReader::StartElement){
             QString tagName( _streamReader.name().toString() );
             _streamReader.readNext();
             _currentData[tagName] = _streamReader.text().toString();
        }
        _streamReader.readNext();
    }
    return true;
}

bool XmlDbaReader::atEnd() const
{
    return _streamReader.atEnd();
}

bool XmlDbaReader::hasError() const
{
    return _streamReader.hasError();
}

sections::section XmlDbaReader::currentSection() const
{
    return _currentSection;
}
