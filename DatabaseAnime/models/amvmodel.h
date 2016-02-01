#ifndef AMVMODEL_H
#define AMVMODEL_H

#include <core/models/basemodel.h>

class AmvModel : public BaseModel
{
private:
    QString _author;
    QString _contestant;
    QStringList _tags;
    QString _sourceVideo;
    QString _sourceMusic;
    QString _authorComment;

    int _score = 0;
//    bool _wantToLook = false;
    bool _editing = false;
public: // static
    static QStringList getTagsList();
public:
    AmvModel();
    AmvModel(const QString &id);

    bool save(const bool update);
    bool loadFromDatabase();

    KeyValue toKeyValue();
    bool fromKeyValue(const KeyValue &data);

    QString author() const;
    void setAuthor(const QString &author);
    QString contestant() const;
    void setContestant(const QString &contestant);
    QStringList tags() const;
    void setTags(const QStringList &tags);
    QString sourceVideo() const;
    void setSourceVideo(const QString &sourceVideo);
    QString sourceMusic() const;
    void setSourceMusic(const QString &sourceMusic);
    QString authorComment() const;
    void setAuthorComment(const QString &authorComment);
    bool editing() const;
    void setEditing(bool editing);
    int score() const;
    void setScore(int score);
};

#endif // AMVMODEL_H
