#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QDate>
#include <QString>



class BaseModel
{
private:
    QString _id;
    QString _title;
    QString _altTitle;
    QDate   _date;
    QString _cover;
    QString _localPath;
    QString _url;
    QString _description;
public:
    BaseModel();
    QString id() const;
    void setId(const QString &id);
    QString title() const;
    void setTitle(const QString &title);
    QString altTitle() const;
    void setAltTitle(const QString &altTitle);
    QDate date() const;
    void setDate(const QDate &date);
    QString cover() const;
    void setCover(const QString &cover);
    QString localPath() const;
    void setLocalPath(const QString &localPath);
    QString url() const;
    void setUrl(const QString &url);
    QString description() const;
    void setDescription(const QString &description);
};

#endif // BASEMODEL_H
