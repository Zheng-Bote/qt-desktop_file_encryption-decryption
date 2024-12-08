#include "enc_dec_logic.h"
#include <QFile>
#include <cstdio>
#include <fstream>

#define MAXLINES 1000
using std::ifstream;
using std::ios;
using std::ofstream;
using std::streampos;

#include <QCryptographicHash>
#include "includes/qaesencryption.h"

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

int EncryptionDecryption::write_file(const string strFileName, const string strData, const int iAppend, const int iNewLine)
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

int EncryptionDecryption::encrypt_data(const string strFileName, std::string strPassword)
{
    int iCnt1 = 0, iCnt2 = 0;
    string strLine = "";

    if (strFileName == "") {
        return -1;
    }

    if (strPassword == "") {
        return -2;
    }

    if (strPassword.length() != 8) {
        return -3;
    }

    if (!is_exists(strFileName)) {
        return -4;
    }

    if (check_enc_dec(strFileName)) {
        return -5;
    }

    QString key = strPassword.c_str();
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    //set_password(strPassword);

    QString strData = read_file(strFileName);

    QByteArray encodeText = encryption.encode(strData.toLocal8Bit(), hashKey, hashIV);

    //    write_file(strFileName, strLine, 1, 1);

    std::string newFile = strFileName + ".aes";
    writeQArray(strFileName, encodeText);

    return 0;
}

int EncryptionDecryption::decrypt_data(const string strFileName, const string strPassword)
{
    /*
    int iCnt1 = 0, iCnt2 = 0;
    string strLine = "";

    if (strFileName == "") {
        return -1;
    }

    if (strPassword == "") {
        return -2;
    }

    if (!is_exists(strFileName)) {
        return -3;
    }

    if (!check_enc_dec(strFileName)) {
        return -4;
    }

    if (!check_password(strFileName, strPassword)) {
        return -5;
    }
*/

    QString key = strPassword.c_str();
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    //QByteArray encodeText = readBinaryFile(strFileName);
    QByteArray decodeText = encryption.decode(readBinaryFile(strFileName), hashKey, hashIV);

    QString decodedString = QString(encryption.removePadding(decodeText));
    std::cout << "decoded:\n\n" << decodedString.toStdString() << std::endl;

    return 0;
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

QString EncryptionDecryption::read_file(const string strFileName)
{
    ifstream infile{strFileName};
    static string file_contents{std::istreambuf_iterator<char>(infile),
                                std::istreambuf_iterator<char>()};

    return file_contents.c_str();
}

QByteArray EncryptionDecryption::readBinaryFile(const string strFileName)
{
    std::cout << "readBinary " << strFileName << std::endl;

    QFile file(strFileName.c_str());
    file.open(QIODeviceBase::ReadOnly);
    QByteArray blob = file.readAll();

    return blob;
}

void EncryptionDecryption::writeQArray(const string strFileName, QByteArray &data)
{
    std::string newFile = strFileName + ".aes";
    QFile file(newFile.c_str());
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
