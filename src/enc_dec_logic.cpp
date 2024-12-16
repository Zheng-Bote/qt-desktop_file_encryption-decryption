#include "enc_dec_logic.h"
#include <QFile>

#include <cstdio>
#include <filesystem>
#include <fstream>

using std::ifstream;
using std::ios;
using std::ofstream;
using std::streampos;

#include <QCryptographicHash>
#include "includes/qaesencryption.h"
#include "includes/rz_snipptes.hpp"

QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
QString iv("0x0c2ad4a4acb9f023");

EncryptionDecryption::EncryptionDecryption() {}

std::tuple<int, QString> EncryptionDecryption::encrypt_data(const string &pathToFile,
                                                            const std::string &strPassword,
                                                            const bool &overwriteSourceFile)
{
    bool oknok{false};
    QString msg{""};
    QString qt_pathToFile = pathToFile.c_str();
    qDebug() << "encrypt_data: " << qt_pathToFile;
    std::tie(oknok, msg) = rz_snipptes::checkFile(qt_pathToFile, overwriteSourceFile, false);
    const int passwordMinLength = 5;

    if (oknok == false) {
        return std::make_tuple(3, msg);
    }

    QString key = strPassword.c_str();
    if (key.length() < passwordMinLength) {
        return std::make_tuple(2, QObject::tr("Password should have more than 5 characters"));
    }
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    /*
    QString strData = read_file(pathToFile);

    QByteArray encodeText = encryption.encode(strData.toLocal8Bit(), hashKey, hashIV);
*/
    QByteArray encodeText = encryption.encode(readBinaryFile(pathToFile), hashKey, hashIV);

    if (overwriteSourceFile == true) {
        std::string newFile = pathToFile + ".aes";
        writeBinary(newFile, encodeText);
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("encrypted file:");
        msg.append("\n" + newFile);
    } else {
        std::tie(oknok, msg) = rz_snipptes::getTempDir();
        std::string newPath = msg.toStdString();
        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        std::string newFile = msg.toStdString() + ".aes";
        std::string newPathToFile = newPath + newFile;
        writeBinary(newPathToFile, encodeText);
        msg = QObject::tr("encrypted file:");
        msg.append("\n" + newPathToFile);
    }

    return std::make_tuple(1, msg);
}

std::tuple<int, QString> EncryptionDecryption::decrypt_data(const string &pathToFile,
                                                            const string &strPassword,
                                                            const bool &overwriteSourceFile)
{
    bool oknok{false};
    QString msg{""};
    QString qt_pathToFile = pathToFile.c_str();
    qDebug() << "decrypt_data: " << qt_pathToFile;
    std::tie(oknok, msg) = rz_snipptes::checkFile(qt_pathToFile, overwriteSourceFile, true);
    const int passwordMinLength = 5;

    QString key = strPassword.c_str();
    if (key.length() < passwordMinLength) {
        return std::make_tuple(2, QObject::tr("Password should have more than 5 characters"));
    }

    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray decodeText = encryption.decode(readBinaryFile(pathToFile), hashKey, hashIV);

    QString decodedString = QString(encryption.removePadding(decodeText));

    if (overwriteSourceFile == true) {
        std::string newPathToFile = pathToFile.substr(0, pathToFile.size() - 4);
        writeQArray(newPathToFile, decodeText);
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("decrypted file:");
        msg.append("\n" + newPathToFile);
    } else {
        std::tie(oknok, msg) = rz_snipptes::getTempDir();
        std::string newPath = msg.toStdString();
        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        std::string newFile = newPath + msg.toStdString();
        std::string newPathToFile = newFile.substr(0, newFile.size() - 4);
        writeQArray(newPathToFile, decodeText);
        msg = QObject::tr("decrypted file:");
        msg.append("\n" + newPathToFile);
    }

    return std::make_tuple(1, msg);
}

QByteArray EncryptionDecryption::readBinaryFile(const string &strFileName)
{
    QFile file(strFileName.c_str());
    file.open(QIODeviceBase::ReadOnly);
    QByteArray blob = file.readAll();
    file.close();
    return blob;
}

void EncryptionDecryption::writeBinary(const string &strFileName, QByteArray &data)
{

    QFile file(strFileName.c_str());
    file.open(QIODeviceBase::WriteOnly);
    file.write(data);
    file.close();
}

void EncryptionDecryption::writeQArray(const string &strFileName, QByteArray &data)
{
    QFile file(strFileName.c_str());
    file.open(QIODeviceBase::WriteOnly);
    data.replace('\0',"");
    data.chop(1);
    file.write(data);
    file.close();
}


