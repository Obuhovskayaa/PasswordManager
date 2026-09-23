#include "SetupDialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFont>
#include <QRegularExpressionValidator>
#include <QAction>
#include <QStyle>

SetupDialog::SetupDialog(QWidget *parent) {
    setupUi();
    setupLayout();
    setupConnections();
}

void SetupDialog::setupUi() {

    m_titleLabel = new QLabel("Welcome");

    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(18);

    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_infoLabel = new QLabel("Enter 4-6 digits");
    m_infoLabel->setStyleSheet("color: grey; font-size: 11px;");
    m_infoLabel->setAlignment(Qt::AlignCenter);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Set a password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_eyeAction = m_passwordEdit->addAction(
        style()->standardIcon(QStyle::SP_MessageBoxInformation),
        QLineEdit::TrailingPosition
    );

    QRegularExpression re("^\\d{4,6}$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(re, this);

    m_passwordEdit->setValidator(validator);

    m_repeatPasswordEdit = new QLineEdit();
    m_repeatPasswordEdit->setPlaceholderText("Repeat the password");
    m_repeatPasswordEdit->setEchoMode(QLineEdit::Password);

    m_repeatPasswordEdit->setValidator(validator);

    m_setPasswordButton = new QPushButton("Set");
    m_setPasswordButton->setEnabled(false);

    resize(400, 300);

}

void SetupDialog::setupLayout() {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_infoLabel);
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(m_repeatPasswordEdit);
    mainLayout->addStretch();
    mainLayout->addWidget(m_setPasswordButton);
}

void SetupDialog::setupConnections() {

    connect(m_passwordEdit, &QLineEdit::textChanged, this, &SetupDialog::updateValidationState);
    connect(m_repeatPasswordEdit, &QLineEdit::textChanged, this, &SetupDialog::updateValidationState);

    connect(m_eyeAction, &QAction::triggered, [this]() {
        if (m_passwordEdit->echoMode() == QLineEdit::Password) {
            m_passwordEdit->setEchoMode(QLineEdit::Normal);

        } else {
            m_passwordEdit->setEchoMode(QLineEdit::Password);
        }
    });

    connect(m_setPasswordButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(m_passwordEdit, &QLineEdit::returnPressed, [this]() {
        m_repeatPasswordEdit->setFocus();
    });
    connect(m_repeatPasswordEdit, &QLineEdit::returnPressed, [this]() {
        if (m_setPasswordButton->isEnabled()) {
            m_setPasswordButton->animateClick();
        }
    });
}

void SetupDialog::updateValidationState() {
    
    const auto& p1 = m_passwordEdit->text();
    const auto& p2 = m_repeatPasswordEdit->text();

    bool valid = m_passwordEdit->hasAcceptableInput();

    bool match = (p1 == p2 && !p1.isEmpty());

    if (p1.isEmpty() || !valid) {
        m_infoLabel->setText("Enter 4-6 digits");
        m_infoLabel->setStyleSheet("color: grey;");
    } else if (p2.size() != p1.size()) {
        m_infoLabel->setText("Valid password, repeat it");
        m_infoLabel->setStyleSheet("color: grey;");
    } else if (!match) {
        m_infoLabel->setText("Passwords do not match");
        m_infoLabel->setStyleSheet("color: red;");
    } else {
        m_infoLabel->setText("Great! Set it");
        m_infoLabel->setStyleSheet("color: green;");
    }

    m_setPasswordButton->setEnabled(valid && match);
    
}

QString SetupDialog::getPassword() const {
    return m_passwordEdit->text();
}

void SetupDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Down) {
        focusNextChild();
    } else if (event->key() == Qt::Key_Up) {
        focusPreviousChild();
    } else {
        QDialog::keyPressEvent(event);
    }
}