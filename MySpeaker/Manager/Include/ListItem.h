#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QVariant>
#include <QHash>

namespace Models
{

class ListItem : public QObject
{
    Q_OBJECT
public :
    ListItem(QObject *parent = 0) : QObject(parent) {}
    virtual ~ListItem() {}
    virtual int id() const {return 0;}
    virtual QVariant data(int role) const = 0;
	virtual bool     setData(int role, const QVariant &value) { Q_UNUSED(role); Q_UNUSED(value); return true; }
    virtual QHash<int, QByteArray> roleNames() const = 0;
    virtual void triggerItemUpdate() {emit dataChanged();}
    bool operator<(const ListItem &nextElem) {Q_UNUSED(nextElem); return false;}
signals:
    void dataChanged();
};

}

#endif // LISTITEM_H
