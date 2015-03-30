#ifndef DATA_H
#define DATA_H

#include <QString>

class BasicData
{
    quint64 _id;
    QString _title;
    QString _altTitle;
    QString _url;
    QString _localDir;
    QString _coverPath;
    QString _year;
    BasicData(quint64 id, QString &title);
public:

    quint64 getId() const;
    void setId(const quint64 &value);
    QString getTitle() const;
    void setTitle(const QString &value);
    QString getAltTitle() const;
    void setAltTitle(const QString &value);
    QString getUrl() const;
    void setUrl(const QString &value);
    QString getLocalDir() const;
    void setLocalDir(const QString &value);
    QString getCoverPath() const;
    void setCoverPath(const QString &value);
    QString getYear() const;
    void setYear(const QString &value);
};

#endif // DATA_H
