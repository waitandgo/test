#ifndef DIRECTORYCONNECT_H
#define DIRECTORYCONNECT_H

#include <QtSql>
#include <QException>
#include <QString>

class DbmsError : public QException
{
public:
    DbmsError(const QString &error)
        : _error(error)
    {}
    ~DbmsError() throw()
    {}

    const QString getError() const
    {
        return _error;
    }

protected:
    QString _error;
};

class DirectoryConnect
{
public:
    DirectoryConnect();
    virtual ~DirectoryConnect();

    void open();
    void close();

protected:
    QSqlDatabase _connect;
    QString _path;
};

#endif // DIRECTORYCONNECT_H
