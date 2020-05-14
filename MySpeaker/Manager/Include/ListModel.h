#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QtAlgorithms>
#include <QQmlEngine>
#include "ListItem.h"

bool compareFunc(void *a, void *b);

namespace Models
{

class ListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool sorting READ sortingEnabled WRITE setSorting NOTIFY sortingChanged)
public:
    explicit ListModel(ListItem *prototype, QObject *parent = 0);
    ~ListModel();

    // REIMPLEMENTED METHODS
    Q_INVOKABLE int                     rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant                data(const QModelIndex &index, int role) const;
    Q_INVOKABLE bool                    setData(const QModelIndex &index, const QVariant &value, int role);
    Q_INVOKABLE QHash<int, QByteArray>  roleNames() const;

    Q_INVOKABLE void        appendRowFromQml(QObject *item);
    void                    appendRow(ListItem *item);
    void                    appendRows(QList<ListItem *> &items);
    void                    insertRow(int row, ListItem *item);
    ListItem*               takeRow(int row = -2, const QModelIndex &index = QModelIndex());
    QList<ListItem *>       takeRows(int row = -2, int count = -1, const QModelIndex &index = QModelIndex());
    Q_INVOKABLE bool        removeRow(int row, const QModelIndex &index = QModelIndex());
    bool                    removeRows(int row, int count, const QModelIndex &index = QModelIndex());

    ListItem*               find(int itemId) const;
    int                     getRowFromItem(ListItem *item) const;
    QModelIndex             indexFromItem(ListItem *item) const;
    QList<ListItem *>       toList() const;

    Q_INVOKABLE QVariant    get(int index);
    Q_INVOKABLE int         rowIndexFromId(int id);
    Q_INVOKABLE void        clear();

    bool                    sortingEnabled() const;
    void                    setSorting(bool value);

protected:
    ListItem *prototype;
    QList<ListItem *> items;
    bool        sortEnabled;

private:
    void                    sort();

private slots :
    void        updateItem();

signals :
    void        countChanged(int);
    void        sortingChanged(bool);
};

}

#endif // LISTMODEL_H
