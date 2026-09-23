#include "PasswordModel.hpp"
#include <cstdint>
#include "Utils.hpp"
#include "DatabaseManager.hpp"

PasswordModel::PasswordModel(QObject *parent) 
    : QAbstractItemModel(parent)
{
    m_entries = DatabaseManager::instance().getAllEntries();
    updateIdToRow();
}

int PasswordModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

int PasswordModel::rowCount(const QModelIndex &parent) const {
    
    if (!parent.isValid()) {
        return m_entries.size();
    }
    if (!parent.parent().isValid()) {
        return 3;
    }
    return 0;
}

QModelIndex PasswordModel::index(int row, int column, const QModelIndex &parent) const {

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    if (!parent.isValid()) {
        return createIndex(row, column, pack(m_entries[row].id(), ItemType::Service));
    }    
    uint32_t dbId = unpackId(parent.internalId());
    return createIndex(row, column, pack(dbId, static_cast<ItemType>(row + 1)));
}

QModelIndex PasswordModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }
    if (unpackType(child.internalId()) == ItemType::Service) {
        return QModelIndex();
    } 

    uint32_t dbId = unpackId(child.internalId());
    int parentRow = m_idToRow.value(dbId, -1);
    if (parentRow == -1) {
        return QModelIndex();
    }
    return createIndex(parentRow, 0, pack(dbId, ItemType::Service));
    
}

QVariant PasswordModel::data(const QModelIndex &index, int role) const {

    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        ItemType type = unpackType(index.internalId());
        uint32_t dbId = unpackId(index.internalId());
        int row = m_idToRow.value(dbId, -1);
        if (row == -1) {
            return QVariant();
        }
        const Entry &entry = m_entries[row];
        
        switch(type) {
            case ItemType::Service: return entry.name();
            case ItemType::Login: return entry.login();
            case ItemType::Password: return entry.password();
            case ItemType::Note: return entry.note();
        }
    }
    return QVariant();
}   

quintptr PasswordModel::pack(uint32_t dbId, ItemType type) { 
    if constexpr (Utils::currentArch() == Utils::Arch::x64) {
        return static_cast<quintptr>(dbId) << 32 | static_cast<quintptr>(type);
    } else {
        return static_cast<quintptr>(dbId) << 8 | static_cast<quintptr>(type);
    }
}

uint32_t PasswordModel::unpackId(quintptr internalId) {
    if constexpr (Utils::currentArch() == Utils::Arch::x64) {
        return static_cast<uint32_t>(internalId >> 32);
    } else {
        return static_cast<uint32_t>(internalId >> 8);
    }
}

PasswordModel::ItemType PasswordModel::unpackType(quintptr internalId) {
    if constexpr (Utils::currentArch() == Utils::Arch::x64) {
        return static_cast<ItemType>(internalId & 0xFFFFFFFF);
    } else {
        return static_cast<ItemType>(internalId & 0xFF);
    }
}

Qt::ItemFlags PasswordModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PasswordModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole || !index.isValid()) {
        return false;
    }

    uint32_t dbId = unpackId(index.internalId());
    ItemType type = unpackType(index.internalId());
    int row = m_idToRow.value(dbId, -1);
    QString newValue = value.toString();

    if (row == -1) {
        return false;
    }

    Entry& e = m_entries[row];
    QString columnName;
    switch(type) {
        case ItemType::Service: {
            if (e.name() == newValue) return true;
            e.setName(newValue);
            columnName = "service";
            break;
        }
        case ItemType::Login: {
            if (e.login() == newValue) return true;
            e.setLogin(newValue);
            columnName = "login";
            break;
        }
        case ItemType::Password: {
            if (e.password() == newValue) return true;
            e.setPassword(newValue);
            columnName = "password";
            break;
        } 
        case ItemType::Note: {
            if (e.note() == newValue) return true;
            e.setNote(newValue);
            columnName = "note";
            break;
        }
        default: return false;
    }
    if (DatabaseManager::instance().updateEntryField(dbId, columnName, newValue)) {
        emit dataChanged(index, index, {Qt::EditRole, Qt::DisplayRole});
        return true;
    }
    return false;

}

void PasswordModel::addEntry() {
    Entry newEntry("New entry", "login", "password", "note");
    if (DatabaseManager::instance().addEntry(newEntry)) {
        beginInsertRows(QModelIndex(), m_entries.size(), m_entries.size());
        m_entries.append(newEntry);
        updateIdToRow();
        endInsertRows();
    }
}

void PasswordModel::removeEntry(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }
    uint32_t dbId = unpackId(index.internalId());
    int row = m_idToRow.value(dbId, -1);

    if (row == -1) {
        return;
    }
    if (DatabaseManager::instance().removeEntry(static_cast<int>(dbId))) {
        beginRemoveRows(QModelIndex(), row, row);
        m_entries.removeAt(row);
        updateIdToRow();
        endRemoveRows();
    }  
}

void PasswordModel::updateIdToRow() {
    m_idToRow.clear();
    for (size_t i = 0; i < m_entries.size(); ++i) {
        m_idToRow.insert(m_entries[i].id(), i);
    }
}




