#include "LoginDialog.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

LoginDialog::LoginDialog(QWidget *parent) 
    : QDialog(parent)
{
    setupUi();
}

void LoginDialog::setupUi() {

    m_titleLabel = new QLabel("Welcome");

    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(18);

    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_enterButton = new QPushButton("Enter");
    m_enterButton->setEnabled(false);
    m_forgotPasswordButton = new QPushButton("Forgot password");
    m_forgotPasswordButton->setFlat(true);
    m_forgotPasswordButton->setStyleSheet("color: blue; text-decoration: underline;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(m_enterButton);
    mainLayout->addWidget(m_forgotPasswordButton);

    connect(m_enterButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_forgotPasswordButton, &QPushButton::clicked, [this](){
        done(67);
    });
    connect(m_passwordEdit, &QLineEdit::textChanged, [this](){
        m_enterButton->setEnabled(!(m_passwordEdit->text().isEmpty()));
    });

    resize(400, 300);

}

QString LoginDialog::getPassword() const {
    return m_passwordEdit->text();
}
