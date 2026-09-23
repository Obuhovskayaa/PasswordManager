#include "CryptoManager.hpp"
#include "qaesencryption.h"

QString CryptoManager::encrypt(const QString &plainText, const QString &masterKey) {
    if (plainText.isEmpty()) {
        return "";
    }
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray key = QCryptographicHash::hash(masterKey.toUtf8(), QCryptographicHash::Sha256);
    QByteArray iv = QCryptographicHash::hash(key, QCryptographicHash::Md5);

    QByteArray encodedText = encryption.encode(plainText.toUtf8(), key, iv);

    return QString::fromLatin1(encodedText.toBase64());
}

QString CryptoManager::decrypt(const QString &cipheredText, const QString &masterKey) {
    if (cipheredText.isEmpty()) {
        return "";
    }
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray key = QCryptographicHash::hash(masterKey.toUtf8(), QCryptograpghicHash::Sha256);
    QByteArray iv = QCryptographicHash::hash(key, QCryptographicHash::Md5);

    QByteArray decodedText = encryption.decode(QByteArray::fromBase64(cipheredText.toLatin1()), key, iv);
    QByteArray plainTextBytes = QAESEncryption::removePadding(decodedText);


    return QString::fromUtf8(decodedText);
}