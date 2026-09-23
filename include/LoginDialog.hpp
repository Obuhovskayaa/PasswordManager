
#ifndef LOGIN_DIALOG_HPP
#define LOGIN_DIALOG_HPP

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getPassword() const;

private:
    void setupUi();
    QLineEdit *m_passwordEdit;
    QPushButton *m_enterButton;
    QPushButton *m_forgotPasswordButton;
    QLabel *m_titleLabel;


};

#endif