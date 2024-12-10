#pragma once

#include <QString>

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

class EncryptionDecryption
{
    int m_iCountLines;
    const string m_strCheck;

public:
    EncryptionDecryption();
    enum class InfoLevel { INFORMATION = 1, WARNING, CRITICAL };

    int create_file(const string strFileName, const std::string &data);
    int write_file(const string strFileName,
                   const string &strData,
                   const int iAppend = 0,
                   const int iNewLine = 0);
    std::tuple<int, QString> encrypt_data(const string &strFileName,
                                          const string &strPassword,
                                          const bool &overwriteSourceFile);
    std::tuple<int, QString> decrypt_data(const string &strFileName,
                                          const string &strPassword,
                                          const bool &overwriteSourceFile);
    int delete_file(const string strFileName);
    bool is_exists(const string strFileName);

private:
    string set_password(string &strPassword);
    bool check_enc_dec(const string strFileName);
    bool check_password(const string strFileName, const string strPassword);
    QString read_file(const string &strFileName);

    QByteArray readBinaryFile(const string &strFileName);
    void writeQArray(const string &strFileName, QByteArray &data);
};

#pragma once
