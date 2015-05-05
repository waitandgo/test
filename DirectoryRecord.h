#ifndef DIRECTORYRECORD_H
#define DIRECTORYRECORD_H

#include <QString>

class DirectoryTitle
{
public:
    DirectoryTitle();
    DirectoryTitle(const QString &title);
    ~DirectoryTitle();

public:
    void setTitle(const QString &title);

    const QString getTitle() const;

protected:
    QString _title;
};


class DirectoryRecord
{
public:
    DirectoryRecord();
    DirectoryRecord(
            const QString &telephone,
            const QString &type,
            const QString &comment);
    ~DirectoryRecord();

 public:
    void setTelephone(const QString &telephone);
    void setType(const QString &type);
    void setComment(const QString &comment);

    const QString getTelephone() const;
    const QString getType() const;
    const QString getComment() const;

protected:
    QString _telephone;
    QString _type;
    QString _comment;
};

#endif // DIRECTORYRECORD_H
