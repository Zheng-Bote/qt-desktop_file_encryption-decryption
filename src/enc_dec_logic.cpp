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
    bool do_encrypt = true;
    QString msg{""};
    QString qt_pathToFile = pathToFile.c_str();
    std::string newPathToFile{""};
    const int passwordMinLength = 5;

    QString key = strPassword.c_str();
    if (key.length() < passwordMinLength) {
        return std::make_tuple(2, QObject::tr("Password should have more than 5 characters"));
    }

    if (overwriteSourceFile == true) {
        newPathToFile = pathToFile + ".aes";
        std::filesystem::remove(newPathToFile);
        std::tie(oknok, msg) = readBinaryFile(pathToFile,
                                              strPassword,
                                              do_encrypt,
                                              overwriteSourceFile,
                                              newPathToFile);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("encrypted file:");
        msg.append("\n" + newPathToFile);
    } else {
        std::tie(oknok, msg) = chooseFilePathToSave(qt_pathToFile);
        newPathToFile = msg.toStdString();

        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        newPathToFile.append(msg.toStdString() + ".aes");

        std::filesystem::remove(newPathToFile);
        std::tie(oknok, msg) = readBinaryFile(pathToFile,
                                              strPassword,
                                              do_encrypt,
                                              overwriteSourceFile,
                                              newPathToFile);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        msg = QObject::tr("encrypted file:");
        msg.append("\n" + newPathToFile);
    }

    return std::make_tuple(1, msg);
}

std::tuple<int, QString> EncryptionDecryption::doEncryptData(const string &pathToFile,
                                                             const string &strPassword,
                                                             const QByteArray &buffer,
                                                             const bool &overwriteSourceFile,
                                                             const string &targetPathToFile)
{
    bool oknok{false};
    QString msg{""};
    QString qt_pathToFile = pathToFile.c_str();

    std::tie(oknok, msg) = rz_snipptes::checkFile(qt_pathToFile, overwriteSourceFile, false);
    if (oknok == false) {
        return std::make_tuple(3, msg);
    }

    QString key = QString::fromStdString(strPassword);
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encodeText = encryption.encode(buffer, hashKey, hashIV);

    std::ofstream file(targetPathToFile, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Error opening file: " << targetPathToFile << std::endl;
        msg = "Error opening file: ";
        msg.append(targetPathToFile);
        return std::make_tuple(3, msg);
    }

    file.write(encodeText.data(), encodeText.size());

    // Check for write errors
    if (!file) {
        std::cerr << "Error writing data to the file." << std::endl;
        msg = "Error writing data to the file: ";
        msg.append(targetPathToFile);
        return std::make_tuple(3, msg);
    }

    file.close();
    msg = "ok";
    return std::make_tuple(1, msg);
}

std::tuple<int, QString> EncryptionDecryption::decrypt_data(const string &pathToFile,
                                                            const string &strPassword,
                                                            const bool &overwriteSourceFile)
{
    bool oknok{false};
    bool do_encrypt = false;
    QString msg{""};
    QString qt_pathToFile = pathToFile.c_str();
    std::string newPathToFile{""};
    qDebug() << "decrypt_data: " << qt_pathToFile;
    std::tie(oknok, msg) = rz_snipptes::checkFile(qt_pathToFile, overwriteSourceFile, true);
    const int passwordMinLength = 5;

    QString key = strPassword.c_str();
    if (key.length() < passwordMinLength) {
        return std::make_tuple(2, QObject::tr("Password should have more than 5 characters"));
    }

    if (overwriteSourceFile == true) {
        newPathToFile = pathToFile.substr(0, pathToFile.size() - 4);
        //std::filesystem::remove(newPathToFile);
        std::tie(oknok, msg) = readBinaryFile(pathToFile,
                                              strPassword,
                                              do_encrypt,
                                              overwriteSourceFile,
                                              newPathToFile);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("decrypted file:");
        msg.append("\n" + newPathToFile);
    } else {
        std::tie(oknok, msg) = chooseFilePathToSave(qt_pathToFile);
        newPathToFile = msg.toStdString();

        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        std::string newFile = msg.toStdString();
        newPathToFile.append(newFile.substr(0, newFile.size() - 4));
        std::tie(oknok, msg) = readBinaryFile(pathToFile,
                                              strPassword,
                                              do_encrypt,
                                              overwriteSourceFile,
                                              newPathToFile);
        if (oknok == false) {
            return std::make_tuple(3, msg);
        }
        msg = QObject::tr("decrypted file:");
        msg.append("\n" + newPathToFile);
    }

    return std::make_tuple(1, msg);
}

std::tuple<int, QString> EncryptionDecryption::doDecryptData(const string &pathToFile,
                                                             const string &strPassword,
                                                             const QByteArray &buffer,
                                                             const bool &overwriteSourceFile,
                                                             const string &targetPathToFile)
{
    bool oknok{false};
    QString msg{""};

    QString key = QString::fromStdString(strPassword);
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    // Decrypt the data
    QByteArray decodeText = encryption.decode(buffer, hashKey, hashIV);

    std::ofstream file(targetPathToFile, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Error opening file: " << pathToFile << std::endl;
        msg = "Error opening file: ";
        msg.append(pathToFile);
        return std::make_tuple(3, msg);
    }

    // Write the binary data to the file
    file.write(decodeText.data(), decodeText.size());

    // Check for write errors
    if (!file) {
        std::cerr << "Error writing data to the file." << std::endl;
        msg = "Error writing data to the file: ";
        msg.append(targetPathToFile);
        return std::make_tuple(3, msg);
    }
    file.close();
    msg = "ok";
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

std::tuple<bool, QString> EncryptionDecryption::readBinaryFile(const string &sourcePathToFile,
                                                               const string &strPassword,
                                                               const bool &do_encrypt,
                                                               const bool &overwriteSourceFile,
                                                               const string &targetPathToFile)
{
    std::size_t chunkSize = 1024 * 1024; // 1 MB chunk size

    std::ifstream file(sourcePathToFile, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file: " << sourcePathToFile << std::endl;
        QString msg = "Error opening file: ";
        msg.append(sourcePathToFile);
        return std::make_tuple(false, msg);
    }

    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(chunkSize); // Buffer to hold chunks

    while (file) {
        // Read a chunk of the file into the buffer
        file.read(buffer.data(), chunkSize);
        size_t bytesRead = file.gcount(); // Number of bytes read

        std::cout << "Read " << bytesRead << " bytes" << std::endl;

        // Create a QByteArray from the buffer
        QByteArray chunkData(buffer.data(), bytesRead);

        if (do_encrypt) {
            std::cout << "Encrypting..." << std::endl;
            doEncryptData(sourcePathToFile,
                          strPassword,
                          chunkData,
                          overwriteSourceFile,
                          targetPathToFile);
        } else {
            std::cout << "Decrypting..." << std::endl;
            doDecryptData(sourcePathToFile,
                          strPassword,
                          chunkData,
                          overwriteSourceFile,
                          targetPathToFile);
        }
    }
    file.close();
    return std::make_tuple(true, "ok");
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
