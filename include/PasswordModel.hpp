#ifndef PASSWORD_MODEL_HPP
#define PASSWORD_MODEL_HPP

#include <QAbstractItemModel>
#include <QList>
#include <QHash>
#include "Entry.hpp"

class PasswordModel : public QAbstractItemModel {
    Q_OBJECT
public:
    enum class ItemType : uint32_t {
        Service = 0,
        Login = 1,
        Password = 2,
        Note = 3
    };

    explicit PasswordModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void addEntry();
    void removeEntry(const QModelIndex &index);

    static quintptr pack(uint32_t dbId, ItemType type);
    static uint32_t unpackId(quintptr internalId);
    static ItemType unpackType(quintptr internalId);

private:
    QHash<uint32_t, int> m_idToRow;
    QList<Entry> m_entries;
    void updateIdToRow();

};

#endif