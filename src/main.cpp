#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QStringList>
#include <QString>
#include "MainWindow.hpp"
#include "LoginDialog.hpp"
#include "SetupDialog.hpp"

QString showCreateUserDialog() {

}

QString showSelectUserDialog() {

}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    QSettings settings("NGCompany", "PasswordManager");
    QStringList users = settings.value("userList").toStringList();
    QString currentUser;
    bool authorized = false;
   
    while (!authorized) {
        if (!settings.contains("passwordHash")) {
            SetupDialog setupDialog;

            if (setupDialog.exec() == QDialog::Accepted) {
                QByteArray hash = QCryptographicHash::hash(
                    setupDialog.getPassword().toUtf8(), QCryptographicHash::Sha256
                ).toHex();
                settings.setValue("passwordHash", hash);
                authorized = true;
            }
            else {
                return 0;
            }
        } else {
            LoginDialog loginDialog;
            int result = loginDialog.exec();

            if (result == QDialog::Accepted) {

                QByteArray inputHash = QCryptographicHash::hash(
                    loginDialog.getPassword().toUtf8(), QCryptographicHash::Sha256
                ).toHex();

                if (inputHash == settings.value("passwordHash").toByteArray()) {
                    authorized = true;
                } else {
                    QMessageBox::warning(nullptr, "Error", "Wrong password");
                }
            } else if (result == 67) {
                settings.clear();
            } else {
                return 0;
            }
        }
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();

}