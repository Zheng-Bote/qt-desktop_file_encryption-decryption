#include "enc_dec_logic.h"
#include <QFile>

#include <cstdio>
#include <filesystem>
#include <fstream>

#define MAXLINES 1000
using std::ifstream;
using std::ios;
using std::ofstream;
using std::streampos;

#include <QCryptographicHash>
#include "includes/qaesencryption.h"
#include "includes/rz_snipptes.hpp"

QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
QString iv("0x0c2ad4a4acb9f023");

EncryptionDecryption::EncryptionDecryption() : m_strCheck("`i^]jn`o"), m_iCountLines(0) {}

int EncryptionDecryption::create_file(const string strFileName, const std::string &data)
{
    if (strFileName == "") {
        return -1;
    }

    if (is_exists(strFileName)) {
        return -2;
    }

    ofstream out(strFileName);
    out << data;
    out.close();

    return 0;
}

int EncryptionDecryption::write_file(const string strFileName,
                                     const string &strData,
                                     const int iAppend,
                                     const int iNewLine)
{
    if (strFileName == "") {
        return -1;
    }
    if (strData == "") {
        return -2;
    }
    if (!is_exists(strFileName)) {
        return -3;
    }

    ofstream write;

    if (iAppend == 0) {
        write.open(strFileName);
    } else {
        write.open(strFileName, ios::app);
    }

    if (!write.is_open())
    {
        write.close();
        return -4;
    }

    streampos currentpos = write.tellp();

    if (iNewLine == 0) {
        write << strData + " ";
    } else {
        write << strData << endl;
    }

    streampos finalpos = write.tellp();

    write.flush();
    write.close();

    return static_cast<int>(finalpos - currentpos - 1);
}

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
        writeQArray(newFile, encodeText);
        std::filesystem::remove(pathToFile);
        msg = QObject::tr("encrypted file:");
        msg.append("\n" + newFile);
    } else {
        std::tie(oknok, msg) = rz_snipptes::getTempDir();
        std::string newPath = msg.toStdString();
        std::tie(oknok, msg) = rz_snipptes::getFileName(qt_pathToFile);
        std::string newFile = msg.toStdString() + ".aes";
        std::string newPathToFile = newPath + newFile;
        writeQArray(newPathToFile, encodeText);
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
    //std::cout << "decoded:\n\n" << decodedString.toStdString() << std::endl;

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

string EncryptionDecryption::set_password(string &strPassword)
{
    int iCnt = 0;

    for (iCnt = 0; iCnt < strPassword.length(); iCnt += 2) {
        strPassword[iCnt] += 3;
    }
    for (iCnt = 1; iCnt < strPassword.length(); iCnt += 2) {
        strPassword[iCnt] -= 3;
    }

    return strPassword;
}

bool EncryptionDecryption::check_password(const string strFileName, const string strPassword)
{
    int iCnt = 0;
    string strCheckPass = "";

    ifstream checkpass(strFileName);
    getline(checkpass, strCheckPass);
    checkpass.close();

    strCheckPass = strCheckPass.substr(10, 8);

    for (iCnt = 0; iCnt < strCheckPass.length(); iCnt += 2) {
        strCheckPass[iCnt] -= 3;
    }
    for (iCnt = 1; iCnt < strCheckPass.length(); iCnt += 2) {
        strCheckPass[iCnt] += 3;
    }

    if (strCheckPass == strPassword) {
        return true;
    }
    return false;
}

bool EncryptionDecryption::check_enc_dec(const string strFileName)
{
    string strCheckEncDec = "";

    ifstream checkencdec(strFileName);
    getline(checkencdec, strCheckEncDec);
    checkencdec.close();

    strCheckEncDec = strCheckEncDec.substr(0, 10);

    if (strCheckEncDec == m_strCheck) {
        return true;
    }
    return false;
}

QString EncryptionDecryption::read_file(const string &strFileName)
{
    qDebug() << "read_file: " << strFileName;
    ifstream infile{strFileName};
    static string file_contents{std::istreambuf_iterator<char>(infile),
                                std::istreambuf_iterator<char>()};

    infile.close();
    return file_contents.c_str();
}

QByteArray EncryptionDecryption::readBinaryFile(const string &strFileName)
{
    qDebug() << "readBinaryFile: " << strFileName;
    QFile file(strFileName.c_str());
    file.open(QIODeviceBase::ReadOnly);
    QByteArray blob = file.readAll();

    file.close();
    return blob;
}

void EncryptionDecryption::writeQArray(const string &strFileName, QByteArray &data)
{
    QFile file(strFileName.c_str());
    file.open(QIODeviceBase::WriteOnly);
    file.write(data);
    file.close();
}

bool EncryptionDecryption::is_exists(const string strFileName)
{
    ifstream fcheck(strFileName);

    if (fcheck.is_open())
    {
        fcheck.close();
        return true;
    }
    fcheck.close();
    return false;
}

int EncryptionDecryption::delete_file(const string strFileName)
{
    if (strFileName == "") {
        return -1;
    }

    if (!is_exists(strFileName)) {
        return -2;
    }

    if (remove(strFileName.c_str())) {
        return -3;
    }
    return 0;
}
