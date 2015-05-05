#include "DirectoryRecord.h"

DirectoryRecord::DirectoryRecord()
{

}

DirectoryRecord::DirectoryRecord(const QString &telephone, const QString &type, const QString &comment) :
    _telephone(telephone), _type(type), _comment(comment)
{

}

DirectoryRecord::~DirectoryRecord()
{

}

void DirectoryRecord::setTelephone(const QString &telephone)
{
    _telephone = telephone;
}

void DirectoryRecord::setType(const QString &type)
{
    _type = type;
}

void DirectoryRecord::setComment(const QString &comment)
{
    _comment = comment;
}

const QString DirectoryRecord::getTelephone() const
{
    return _telephone;
}

const QString DirectoryRecord::getType() const
{
    return _type;
}

const QString DirectoryRecord::getComment() const
{
    return _comment;
}

DirectoryTitle::DirectoryTitle()
    : _title("")
{

}

DirectoryTitle::DirectoryTitle(const QString &title)
    : _title(title)
{

}

DirectoryTitle::~DirectoryTitle()
{

}

void DirectoryTitle::setTitle(const QString &title)
{
    _title = title;
}

const QString DirectoryTitle::getTitle() const
{
    return _title;
}
