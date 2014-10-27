#include "xmldbareader.h"

XmlDbaReader::XmlDbaReader(QIODevice* stream)
    :_streamReader(stream)
{}

QMap<QString, QVariant> XmlDbaReader::readNext()
{

    _currentData.clear();
    if( _streamReader.atEnd() || _streamReader.hasError() ){
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
//        QCoreApplication::processEvents();
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
