#include "DirectoryConnect.h"
#include <QDir>
#include <QMutex>

QMutex dbmsMutex;

DirectoryConnect::DirectoryConnect()
{
    QString _path = QDir::currentPath() + QDir::separator() + "db/db.sqlite";
    _connect = QSqlDatabase::addDatabase("QSQLITE");
    _connect.setDatabaseName(_path);
}

DirectoryConnect::~DirectoryConnect()
{

}

void DirectoryConnect::open()
{
    dbmsMutex.lock();
    if (!_connect.open())
    {
        dbmsMutex.unlock();
        throw DbmsError("Не удалось установить соединение с СУБД.");
    }
}

void DirectoryConnect::close()
{
    if (_connect.isOpen())
        _connect.close();
    dbmsMutex.unlock();
}
