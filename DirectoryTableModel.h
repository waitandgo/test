#ifndef DIRECTORYTABLEMODEL_H
#define DIRECTORYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QtSql>
#include <QException>
#include <QSharedPointer>
#include "DirectoryRecord.h"
#include "DirectorySyncHelper.h"

class DirectoryTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DirectoryTableModel(QObject *parent = 0);
    virtual ~DirectoryTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setTitleHeight(const int& titleHeight);
    const int getTitleHeight() const;

protected:
    void loadDirectoryThread();
    void saveDirectoryThread(QList<DirectoryRecord> &record);
    void loadDirectory();
    void saveDirectory();
    void initLoadDirectoryThread();
    void initConnect();
    void initTitles();
    void initRecords();
    void initSyncHelper();

private:
    QList<DirectoryRecord> _record;
    QList<DirectoryTitle> _title;
    int _titleHeight;
    DirectorySyncHelper* _portHelper;
    QSharedPointer<DirectoryConnect> _connect;

signals:
    void updateInfoStatusBar(QString);
    void updatePortStatusBar(QString);
    void disableRecordsButton(bool);
    void enableRecordsButton(bool);
    void syncDirectory();

public slots:
    void onAddRecord();
    void onLoadRecords();
    void onSaveRecords();
};

#endif // DIRECTORYTABLEMODEL_H
