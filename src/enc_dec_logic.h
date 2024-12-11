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

public:
    EncryptionDecryption();

    std::tuple<int, QString> encrypt_data(const string &strFileName,
                                          const string &strPassword,
                                          const bool &overwriteSourceFile);
    std::tuple<int, QString> decrypt_data(const string &strFileName,
                                          const string &strPassword,
                                          const bool &overwriteSourceFile);

private:
    QByteArray readBinaryFile(const string &strFileName);
    void writeQArray(const string &strFileName, QByteArray &data);
};
