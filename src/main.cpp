#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include "MainWindow.hpp"
#include "LoginDialog.hpp"
#include "SetupDialog.hpp"
#include "CryptoManager.hpp"
#include "DatabaseManager.hpp"
#include <QClipboard>
#include <QAbstractButton>
#include <QPushButton> 

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QSettings settings("NGCompany", "PasswordManager");
    // settings.clear(); 
    // QString dbPath = QCoreApplication::applicationDirPath() + "/passwords.db";
    // QFile::remove(dbPath);
    QString encryptedKey = settings.value("encrypted_master_key").toString();
    QString storedPinHash = settings.value("pin_hash").toString();
    QString storedMasterKeyHash = settings.value("master_key_hash").toString();
    
    bool authorized = false;
    QString masterKey = "";

    while (!authorized) {
        if (storedMasterKeyHash.isEmpty()) {
            masterKey = CryptoManager::generateRandomMasterKey();

            QMessageBox msgBox;
            msgBox.setWindowTitle("Your Master Key");
            msgBox.setText("Save it in a safe place.\n" + masterKey);
            msgBox.setIcon(QMessageBox::Information);

            QAbstractButton *copyButton = msgBox.addButton("Copy to Clipboard", QMessageBox::ActionRole);
            msgBox.addButton(QMessageBox::Ok);

            msgBox.exec();

            if (msgBox.clickedButton() == copyButton) {
                QApplication::clipboard()->setText(masterKey);
                QMessageBox::information(nullptr, "Copied", "Master Key was copied.");
            }
            settings.setValue("master_key_hash", CryptoManager::hashString(masterKey));
            storedMasterKeyHash = settings.value("master_key_hash").toString();
        } 
        if (storedPinHash.isEmpty()) {
            SetupDialog setupDialog;
            if (setupDialog.exec() == QDialog::Accepted) {
                QString pin = setupDialog.getPassword();
                settings.setValue("encrypted_master_key", CryptoManager::encrypt(masterKey, pin));
                settings.setValue("pin_hash", CryptoManager::hashString(pin));

                encryptedKey = settings.value("encrypted_master_key").toString();
                storedPinHash = settings.value("pin_hash").toString();
                masterKey = "";
            } else {
                return 0;
            }
        } else {
            LoginDialog loginDialog;
            int res = loginDialog.exec();

            if (res == QDialog::Accepted) {
                QString pin = loginDialog.getPassword();

                if (CryptoManager::hashString(pin) != storedPinHash) {
                    QMessageBox::warning(nullptr, "Error", "Wrong password");
                    continue;
                } 
                authorized = true;
                masterKey = CryptoManager::decrypt(encryptedKey, pin);
                
            } else if (res == 67) {
                bool ok = false;
                QString recoveryKey = QInputDialog::getText(nullptr, "Access recovery",
                    "Input your Master Key: ", QLineEdit::Normal, "", &ok);
                
                if (ok && !recoveryKey.isEmpty()) {
                    recoveryKey = recoveryKey.trimmed();

                    if (CryptoManager::hashString(recoveryKey) != storedMasterKeyHash) {
                        QMessageBox::warning(nullptr, "Error", "Wrong recovery key");
                        continue;
                    }
                    masterKey = recoveryKey;
                    
                    SetupDialog setupDialog;
                    if (setupDialog.exec() == QDialog::Accepted) {
                        QString pin = setupDialog.getPassword();
                        settings.setValue("encrypted_master_key", CryptoManager::encrypt(masterKey, pin));
                        settings.setValue("pin_hash", CryptoManager::hashString(pin));
                        masterKey = "";
                    } else {
                        return 0;
                    }
                }
            } else {
                return 0;
            }
        }
    }


    MainWindow mainWindow(masterKey);
    mainWindow.show();

    return app.exec();

}