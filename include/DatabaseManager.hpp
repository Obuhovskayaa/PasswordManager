#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <QtSql>
#include <QCoreApplication>
#include <QList>
#include "Entry.hpp"

class DatabaseManager {
public:
    static DatabaseManager& instance();
    
    bool connectToDatabase(const QString& masterKey);
    QList<Entry> getAllEntries();
    bool addEntry(Entry& e);
    bool updateEntryField(int id, const QString &columnName, const QString &newValue);
    bool removeEntry(int id);


private:
    DatabaseManager(){}
    QString m_masterKey;
};

#endif