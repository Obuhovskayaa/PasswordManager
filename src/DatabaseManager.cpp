#include "DatabaseManager.hpp"
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;  
}

bool DatabaseManager::connectToDatabase() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
            return true;
        }
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/passwords.db");

        if (!db.open()) {
            return false;
        }

        QSqlQuery query;
        return query.exec(
            "CREATE TABLE IF NOT EXISTS passwords ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "service TEXT, "
            "login TEXT, "
            "password TEXT, "
            "note TEXT)"
        );
}

QList<Entry> DatabaseManager::getAllEntries() {
    QList<Entry> entries;
    QSqlQuery query("SELECT id, service, login, password, note FROM passwords");
    while (query.next()) {
        Entry e;
        e.setId(query.value("id").toInt());
        e.setName(query.value("service").toString());
        e.setLogin(query.value("login").toString());
        e.setPassword(query.value("password").toString());
        e.setNote(query.value("note").toString());
        entries.append(e);
    }
    return entries;
}

bool DatabaseManager::addEntry(Entry& e) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO passwords (service, login, password, note)"
        "VALUES (:service, :login, :password, :note)"
    );
    query.bindValue(":service", e.name());
    query.bindValue(":login", e.login());
    query.bindValue(":password", e.password());
    query.bindValue(":note", e.note());

    if (query.exec()) {
        e.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool DatabaseManager::updateEntryField(int id, const QString &columnName, const QString &newValue) {
    QSqlQuery query;
    QString queryString = QString("UPDATE passwords SET %1 = :value WHERE id = :id")
        .arg(columnName);

    query.prepare(queryString);
    query.bindValue(":value", newValue);
    query.bindValue(":id", id);

    return query.exec();
}

bool DatabaseManager::removeEntry(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM passwords WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete error: " << query.lastError().text();
        return false;
    }
    return true;
}

