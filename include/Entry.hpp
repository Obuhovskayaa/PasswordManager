#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <QString>

class Entry {

public:
    Entry(const QString &name = "", const QString &login = "",
         const QString &password = "", const QString &note = "");

    int id() const;
    QString name() const;
    QString login() const;
    QString password() const;
    QString note() const;

    void setId(int id);
    void setName(const QString &name);
    void setLogin(const QString &login);
    void setPassword(const QString &password);
    void setNote(const QString &note);

private:
    int m_id;
    QString m_name;
    QString m_login;
    QString m_password;
    QString m_note;

};

#endif