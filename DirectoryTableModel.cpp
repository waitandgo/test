#include "DirectoryTableModel.h"
#include <QSize>
#include <QBrush>
#include <QMessageBox>
#include <QDir>
#include <QException>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QSystemSemaphore>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

QReadWriteLock directoryRecordsRW(QReadWriteLock::Recursive);

DirectoryTableModel::DirectoryTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    _titleHeight(25)
{
    initConnect();
    initSyncHelper();
    initLoadDirectoryThread();
    initTitles();
}

DirectoryTableModel::~DirectoryTableModel()
{

}

int DirectoryTableModel::rowCount(const QModelIndex &parent) const
{
    return _record.size();
}

int DirectoryTableModel::columnCount(const QModelIndex &parent) const
{
    return _title.size();
}

QVariant DirectoryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        QVariant value;
        directoryRecordsRW.lockForRead();

        if (index.column() == 0)
            value = _record.at(index.row()).getTelephone();
        else if (index.column() == 1)
            value = _record.at(index.row()).getType();
        else if (index.column() == 2)
            value = _record.at(index.row()).getComment();

        directoryRecordsRW.unlock();
        return value;
    }

    return QVariant();
}

QVariant DirectoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
            return _title.at(section).getTitle();
        else if (orientation == Qt::Vertical)
            return (section + 1);
    }

    return QVariant();
}

bool DirectoryTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        directoryRecordsRW.lockForWrite();

        if (index.column() == 0)
            _record[index.row()].setTelephone(value.toString());
        else if (index.column() == 1)
            _record[index.row()].setType(value.toString());
        else if (index.column() == 2)
            _record[index.row()].setComment(value.toString());

        directoryRecordsRW.unlock();
    }

    return true;
}

Qt::ItemFlags DirectoryTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() >= 0 && index.column() <= 2)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::NoItemFlags;
}

void DirectoryTableModel::setTitleHeight(const int &titleHeight)
{
    _titleHeight = titleHeight;
}

const int DirectoryTableModel::getTitleHeight() const
{
    return _titleHeight;
}

void DirectoryTableModel::loadDirectoryThread()
{
    directoryRecordsRW.lockForWrite();

    emit disableRecordsButton(true);
    emit updateInfoStatusBar("Загрузка телефонов...");

    try
    {
        beginResetModel();
        _record.clear();

        _connect->open();

        QSqlQuery query;
        if (!query.exec("select telephone, type, comment from Directory"))
            throw DbmsError("Не удалось получить телефоны.");

        while (query.next())
        {
            _record << DirectoryRecord(query.value(0).toString(),
                                       query.value(1).toString(),
                                       query.value(2).toString());
        }

        emit updateInfoStatusBar(QString("Телефонов загружено (%1).").arg(_record.size()));
    }
    catch (DbmsError &e)
    {
        emit updateInfoStatusBar(e.getError());
    }
    catch (...)
    {
        emit updateInfoStatusBar("Ошибка при загрузке телефонов.");
    }

    _connect->close();

    directoryRecordsRW.unlock();
    endResetModel();
    emit enableRecordsButton(true);
}

void DirectoryTableModel::saveDirectoryThread(QList<DirectoryRecord> &record)
{
    try
    {
        directoryRecordsRW.lockForRead();

        emit disableRecordsButton(true);
        emit updateInfoStatusBar("Сохранение телефонов...");

        _connect->open();

        QSqlQuery query;
        if (!query.exec("delete from Directory"))
            throw DbmsError("Не удалось удалить телефоны.");

        for (int i = 0; i < record.size(); i++)
        {
            query.prepare("insert into Directory values (:telephone, :type, :comment)");
            query.bindValue(":telephone", record.at(i).getTelephone());
            query.bindValue(":type", record.at(i).getType());
            query.bindValue(":comment", record.at(i).getComment());

            if (!query.exec())
                throw DbmsError("Не удалось сохранить телефоны.");

            emit updateInfoStatusBar(QString("Телефонов сохранено (%1).").arg(i + 1));
        }

        emit updateInfoStatusBar(QString("Сохранение завершено, телефонов сохранено (%1).").arg(_record.size()));
    }
    catch (DbmsError &e)
    {
        emit updateInfoStatusBar(e.getError());
    }
    catch (...)
    {
        emit updateInfoStatusBar("Ошибка при сохранении телефонов.");
    }

    _connect->close();
    emit enableRecordsButton(true);
    directoryRecordsRW.unlock();

    emit syncDirectory();
}

void DirectoryTableModel::loadDirectory()
{
    try
    {
        QFuture<void> thread = QtConcurrent::run(
                    this, &DirectoryTableModel::loadDirectoryThread);
    }
    catch (...)
    {

    }
}

void DirectoryTableModel::saveDirectory()
{
    try
    {
        directoryRecordsRW.lockForRead();
        QFuture<void> thread = QtConcurrent::run(
                    this, &DirectoryTableModel::saveDirectoryThread, _record);
    }
    catch (...)
    {

    }

    directoryRecordsRW.unlock();
}

void DirectoryTableModel::initLoadDirectoryThread()
{
    QFuture<void> thread = QtConcurrent::run(
                this, &DirectoryTableModel::loadDirectoryThread);
}

void DirectoryTableModel::initConnect()
{
    _connect = QSharedPointer<DirectoryConnect>(new DirectoryConnect);
}

void DirectoryTableModel::initTitles()
{
    _title << DirectoryTitle("Номер")
           << DirectoryTitle("Тип")
           << DirectoryTitle("Комментарий");
}

void DirectoryTableModel::initRecords()
{
    loadDirectory();
}

void DirectoryTableModel::initSyncHelper()
{
   _portHelper = new DirectorySyncHelper(this, _connect);
   connect(_portHelper, SIGNAL(reloadDirectory()), this, SLOT(onLoadRecords()));
   connect(this, SIGNAL(syncDirectory()), _portHelper, SLOT(onSyncDirectory()));
}

void DirectoryTableModel::onAddRecord()
{
    beginResetModel();

    directoryRecordsRW.lockForWrite();

    try
    {
        _record << DirectoryRecord("", "", "");
        emit updateInfoStatusBar("Добавлен новый телефон.");
    }
    catch (...)
    {
        emit updateInfoStatusBar("Не удалось добавить новый телефон!");
    }

    directoryRecordsRW.unlock();

    endResetModel();
}

void DirectoryTableModel::onLoadRecords()
{
    loadDirectory();
}

void DirectoryTableModel::onSaveRecords()
{
    saveDirectory();
}
