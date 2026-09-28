#include "DatabaseManager.hpp"
#include <QDebug>
#include "CryptoManager.hpp"

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;  
}

bool DatabaseManager::connectToDatabase(const QString& masterKey) {
    m_masterKey = masterKey;
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

        e.setName(CryptoManager::decrypt(query.value("service").toString(), m_masterKey));
        e.setLogin(CryptoManager::decrypt(query.value("login").toString(), m_masterKey));
        e.setPassword(CryptoManager::decrypt(query.value("password").toString(), m_masterKey));
        e.setNote(CryptoManager::decrypt(query.value("note").toString(), m_masterKey));

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
    query.bindValue(":service", CryptoManager::encrypt(e.name(), m_masterKey));
    query.bindValue(":login", CryptoManager::encrypt(e.login(), m_masterKey));
    query.bindValue(":password", CryptoManager::encrypt(e.password(), m_masterKey));
    query.bindValue(":note", CryptoManager::encrypt(e.note(), m_masterKey));

    if (query.exec()) {
        e.setId(query.lastInsertId().toInt());
        return true;
    }
    return false;
}

bool DatabaseManager::updateEntryField(int id, const QString &columnName, const QString &newValue) {
    QSqlQuery query;

    qDebug() << "Update field: ";
    qDebug() << "Column:" << columnName << "| Original Value:" << newValue;
    QString encryptedValue = CryptoManager::encrypt(newValue, m_masterKey);

    qDebug() << "Encrypted (Base64):" << encryptedValue;

    QString queryString = QString("UPDATE passwords SET %1 = :value WHERE id = :id")
        .arg(columnName);

    query.prepare(queryString);
    query.bindValue(":value", encryptedValue);
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

