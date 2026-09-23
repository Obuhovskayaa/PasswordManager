#ifndef SETUP_DIALOG_HPP
#define SETUP_DIALOG_HPP

#include <QDialog>
#include <QKeyEvent>

class QLineEdit;
class QPushButton;
class QLabel;
class PasswordModel;

class SetupDialog : public QDialog {
    Q_OBJECT
public:
    SetupDialog(QWidget *parent = nullptr);
    QString getPassword() const;

protected:
    void keyPressEvent(QKeyEvent *event) override;   

private slots:
    void updateValidationState();

private:
    void setupUi(); 
    void setupLayout();
    void setupConnections();
    QLineEdit *m_passwordEdit;
    QLineEdit *m_repeatPasswordEdit;
    QPushButton *m_setPasswordButton;
    QLabel *m_titleLabel;
    QLabel *m_infoLabel;
    QAction *m_eyeAction;
    PasswordModel *m_passwordModel;

};

#endif