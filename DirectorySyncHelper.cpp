#include "DirectorySyncHelper.h"

DirectorySyncHelper::DirectorySyncHelper(QObject *parent, QSharedPointer<DirectoryConnect> connect) :
    QObject(parent), _port(44404), _connect(connect)
{
    try
    {
        _connect->open();

        QSqlQuery query;
        if (!query.exec("select max(port) from DirectoryPort"))
            throw DbmsError("Не удалось получить порт.");

        while (query.next())
        {
            _port = query.value(0).toInt() + 1;
        }

        query.prepare("insert into DirectoryPort values (:port)");
        query.bindValue(":port", _port);

        if (!query.exec())
            throw DbmsError("Не удалось сохранить порт.");

        initUdp();
    }
    catch (...)
    {
        _connect->close();
        throw;
    }

    _connect->close();
}


DirectorySyncHelper::~DirectorySyncHelper()
{
    try
    {
        _connect->open();

        QSqlQuery query;
        query.prepare("delete from DirectoryPort where port = :port");
        query.bindValue(":port", _port);
        if (!query.exec())
            throw DbmsError("Не удалось удалить порт.");
    }
    catch (...)
    {

    }

    _connect->close();
}

const quint16 DirectorySyncHelper::getPort() const
{
    return _port;
}

void DirectorySyncHelper::initUdp()
{
    _udp = new QUdpSocket(this);
    _udp->bind(QHostAddress::LocalHost, getPort());

    connect(_udp, SIGNAL(readyRead()), this, SLOT(read()));
}

void DirectorySyncHelper::read()
{
    while (_udp->hasPendingDatagrams())
    {
        QHostAddress sender;
        quint16 senderPort;
        QByteArray data;
        data.resize(_udp->pendingDatagramSize());
        _udp->readDatagram(data.data(), data.size(),&sender, &senderPort);
        QDataStream in(&data, QIODevice::ReadOnly);
        QString str;
        in >> str;
        emit reloadDirectory();
    }
}

void DirectorySyncHelper::onSyncDirectory()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << "syncDirectory";

    try
    {
        _connect->open();

        QSqlQuery query;
        if (query.exec("select * from DirectoryPort"))
        {
            while (query.next())
            {
                _udp->writeDatagram(data, QHostAddress::LocalHost, query.value(0).toUInt());
            }
        }
    }
    catch (...)
    {

    }
    _connect->close();
}
