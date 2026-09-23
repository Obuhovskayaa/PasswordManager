#include "Entry.hpp"
#include <QJsonObject>

Entry::Entry(const QString &name, const QString &login,
         const QString &password, const QString &note)
         : m_name(name), m_login(login),
          m_password(password), m_note(note)
{}

int Entry::id() const {
    return m_id;
}

QString Entry::name() const {
    return m_name;
}

QString Entry::login() const {
    return m_login;
}

QString Entry::password() const {
    return m_password;
}

QString Entry::note() const {
    return m_note;
}

void Entry::setId(int id) {
    m_id = id;
}

void Entry::setName(const QString &name) {
    m_name = name;
}

void Entry::setLogin(const QString &login) {
    m_login = login;
}

void Entry::setPassword(const QString &password) {
    m_password = password;
}

void Entry::setNote(const QString &note) {
    m_note = note;
}