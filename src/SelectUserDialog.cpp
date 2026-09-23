#include "SelectUserDialog.hpp"

SelectUserDialog::SelectUserDialog(const QStringList &users, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Select a profile");
    setFixedSize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_userList = new QListWidget(this);
    m_userList->addItems(users);

    if (m_userList->count() > 0) {
        m_userList->setCurrentRow(0);
    }

    m_enterBtn = new QPushButton("Enter the profile", this);
    m_newUserButton = new QPushButton("Create new profile", this);

    layout->addWidget(m_userList);
    layout->addWidget(m_enterBtn);
    layout->addWidget(m_newUserButton);

    connect(m_enterBtn, &QPushButton::clicked, this, &Select)

}