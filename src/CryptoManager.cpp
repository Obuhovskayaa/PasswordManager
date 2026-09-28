#include "CryptoManager.hpp"
#include "qaesencryption.h"
#include <QCryptographicHash>
#include <QRandomGenerator>

QString CryptoManager::encrypt(const QString &plainText, const QString &masterKey) {
    if (plainText.isEmpty()) {
        return "";
    }
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC, QAESEncryption::PKCS7);
    QByteArray key = QCryptographicHash::hash(masterKey.toUtf8(), QCryptographicHash::Sha256);
    QByteArray iv = QCryptographicHash::hash(key, QCryptographicHash::Md5);

    QByteArray encodedText = encryption.encode(plainText.toUtf8(), key, iv);

    return QString::fromLatin1(encodedText.toBase64());
}

QString CryptoManager::decrypt(const QString &cipheredText, const QString &masterKey) {
    if (cipheredText.isEmpty()) {
        return "";
    }
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC, QAESEncryption::PKCS7);
    QByteArray key = QCryptographicHash::hash(masterKey.toUtf8(), QCryptographicHash::Sha256);
    QByteArray iv = QCryptographicHash::hash(key, QCryptographicHash::Md5);

    QByteArray decodedText = encryption.decode(QByteArray::fromBase64(cipheredText.toLatin1()), key, iv);
    QByteArray plainTextBytes = encryption.removePadding(decodedText);

    return QString::fromUtf8(plainTextBytes);
}

QString CryptoManager::generateRandomMasterKey() {
    const QString possibleChars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int keyLen = 24;
    QString randomStr;
    for (int i = 0; i < keyLen; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleChars.length());
        randomStr.append(possibleChars.at(index));
    }
    return randomStr;
}

QString CryptoManager::hashString(const QString& input) {
    QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(hash.toHex());
}