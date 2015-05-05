#ifndef DIRECTORYSYNCHELPER_H
#define DIRECTORYSYNCHELPER_H

#include <QObject>
#include <QSharedPointer>
#include <QUdpSocket>
#include "DirectoryConnect.h"

class DirectorySyncHelper : public QObject
{
    Q_OBJECT
public:
    DirectorySyncHelper(QObject *parent = 0, QSharedPointer<DirectoryConnect> connect = QSharedPointer<DirectoryConnect>(0));
    virtual ~DirectorySyncHelper();

    const quint16 getPort() const;

    void initUdp();

protected:
    QSharedPointer<DirectoryConnect> _connect;
    quint16 _port;
    QUdpSocket* _udp;

signals:
    void reloadDirectory();

public slots:
    void read();
    void onSyncDirectory();
};

#endif // DIRECTORYSYNCHELPER_H
