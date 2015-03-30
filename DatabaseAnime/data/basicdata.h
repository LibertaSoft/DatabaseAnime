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

public:
    BasicData(quint64 id, QString &title);
    quint64 getId() const;
    void setId(const quint64 &id);
    QString getTitle() const;
    void setTitle(const QString &title);
    QString getAltTitle() const;
    void setAltTitle(const QString &altTitle);
    QString getUrl() const;
    void setUrl(const QString &url);
    QString getLocalDir() const;
    void setLocalDir(const QString &localDir);
    QString getCoverPath() const;
    void setCoverPath(const QString &coverPath);
    QString getYear() const;
    void setYear(const QString &year);
};

#endif // DATA_H
