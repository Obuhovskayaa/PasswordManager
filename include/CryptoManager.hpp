#ifndef CRYPTO_MANAGER_HPP
#define CRYPTO_MANAGER_HPP

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

class CryptoManager {
public:
    static QString encrypt(const QString &plainText, const QString &masterKey);
    static QString decrypt(const QString &cypherText, const QString &masterKey);
    static QString generateRandomMasterKey();
    static QString hashString(const QString& input);

private:
    static QByteArray deriveKey(const QString &pass);
};

#endif