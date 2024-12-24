/**
 * @file enc_dec_logic.cpp
 * @author ZHENG Bote (www.robert.hase-zheng.net)
 * @brief model for encryption / decryption
 * @version 1.1.0
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2024 ZHENG Robert
 * 
 */
#include "enc_dec_logic.h"
#include <QFile>

#include <QtWidgets/qmessagebox.h>
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
    std::string newFile{""};

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
    QByteArray data;
    std::tie(oknok, data) = readTextFile(pathToFile);
    if (oknok == false) {
        return std::make_tuple(3, QString(data));
    }

    QByteArray encodeText = encryption.encode(data, hashKey, hashIV);

    if (overwriteSourceFile == true) {
        newFile = pathToFile + ".aes";
        std::tie(oknok, msg) = writeText(newFile, encodeText);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("encrypted file:");
        msg.append("\n" + newFile);
    } else {
        std::tie(oknok, msg) = chooseFilePathToSave(qt_pathToFile);
        std::string newPathToFile = msg.toStdString();

        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        newPathToFile.append(msg.toStdString() + ".aes");

        std::tie(oknok, msg) = writeText(newPathToFile, encodeText);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
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
    std::string newFile{""};
    qDebug() << "decrypt_data: " << qt_pathToFile;
    std::tie(oknok, msg) = rz_snipptes::checkFile(qt_pathToFile, overwriteSourceFile, true);
    const int passwordMinLength = 5;

    QString key = strPassword.c_str();
    if (key.length() < passwordMinLength) {
        return std::make_tuple(2, QObject::tr("Password should have more than 5 characters"));
    }

    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray data;
    std::tie(oknok, data) = readTextFile(pathToFile);
    if (oknok == false) {
        return std::make_tuple(3, QString(data));
    }

    QByteArray decodeText = encryption.decode(data, hashKey, hashIV);

    QString decodedString = QString(encryption.removePadding(decodeText));

    if (overwriteSourceFile == true) {
        std::string newPathToFile = pathToFile.substr(0, pathToFile.size() - 4);
        std::tie(oknok, msg) = writeQArray(newPathToFile, decodeText);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("decrypted file:");
        msg.append("\n" + newPathToFile);
    } else {
        std::tie(oknok, msg) = chooseFilePathToSave(qt_pathToFile);
        std::string newPathToFile = msg.toStdString();

        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        newFile = msg.toStdString();
        newPathToFile.append(newFile.substr(0, newFile.size() - 4));
        std::tie(oknok, msg) = writeQArray(newPathToFile, decodeText);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        msg = QObject::tr("decrypted file:");
        msg.append("\n" + newPathToFile);
    }

    return std::make_tuple(1, msg);
}

std::tuple<bool, QByteArray> EncryptionDecryption::readTextFile(const string &strFileName)
{
    QFile file(strFileName.c_str());
    if (!file.open(QIODeviceBase::ReadOnly)) {
        QString msg = QObject::tr("Can not read file") + "\n" + strFileName.c_str();
        return std::make_tuple(false, msg.toUtf8());
    }
    QByteArray blob = file.readAll();
    file.close();
    return std::make_tuple(true, blob);
}

std::tuple<bool, QString> EncryptionDecryption::writeText(const string &strFileName,
                                                          QByteArray &data)
{

    QFile file(strFileName.c_str());
    if (!file.open(QIODeviceBase::WriteOnly)) {
        QString msg = QObject::tr("Can not write file") + "\n" + strFileName.c_str();
        return std::make_tuple(false, msg);
    }
    file.write(data);
    file.close();
    return std::make_tuple(true, QObject::tr("successful"));
}

std::tuple<bool, QString> EncryptionDecryption::writeQArray(const string &strFileName,
                                                            QByteArray &data)
{
    QFile file(strFileName.c_str());
    if (!file.open(QIODeviceBase::WriteOnly)) {
        QString msg = QObject::tr("Can not write file") + "\n" + strFileName.c_str();
        return std::make_tuple(false, msg);
    }
    data.replace('\0',"");
    data.chop(1);
    file.write(data);
    file.close();
    return std::make_tuple(true, QObject::tr("successful"));
}

std::tuple<bool, QString> EncryptionDecryption::chooseFilePathToSave(QString &pathToFile)
{
    bool oknok{false};
    QString msg{""};
    std::tie(oknok, msg) = rz_snipptes::getFilePath(pathToFile);
    QString homeDir = msg; // QDir::homePath();
    QString origFileName;
    std::tie(oknok, origFileName) = rz_snipptes::getFileName(pathToFile);
    QString newPathToFile;

    newPathToFile = QFileDialog::getExistingDirectory(NULL,
                                                      QObject::tr("choose Directory"),
                                                      homeDir,
                                                      QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);

    if (newPathToFile.isEmpty() == true) {
        QString msgTxt = QObject::tr("Missing location to store the file");
        QString msgInfoText = QObject::tr("Please choose a location to store the file");

        QMessageBox msgBox;
        msgBox.setWindowTitle(QObject::tr("Warning"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(msgTxt);
        msgBox.setInformativeText(msgInfoText);
        msgBox.setFixedWidth(700);
        msgBox.exec();

        return std::make_tuple(false, msgTxt);
    }
    return std::make_tuple(true, newPathToFile + "/");
}
