#pragma once

#include <QFileDialog>
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
    std::tuple<int, QString> doEncryptData(const string &pathToFile,
                                           const string &strPassword,
                                           const QByteArray &buffer,
                                           const bool &overwriteSourceFile,
                                           const string &targetPathToFile);
    std::tuple<int, QString> decrypt_data(const string &strFileName,
                                          const string &strPassword,
                                          const bool &overwriteSourceFile);
    std::tuple<int, QString> doDecryptData(const string &pathToFile,
                                           const string &strPassword,
                                           const QByteArray &buffer,
                                           const bool &overwriteSourceFile,
                                           const string &targetPathToFile);

private:
    std::tuple<bool, QByteArray> readTextFile(const string &strFileName);
    std::tuple<bool, QString> readBinaryFile(const string &sourcePathToFile,
                                             const string &strPassword,
                                             const bool &do_encrypt,
                                             const bool &overwriteSourceFile,
                                             const string &targetPathToFile);
    std::tuple<bool, QString> writeText(const string &strFileName, QByteArray &data);
    std::tuple<bool, QString> writeQArray(const string &strFileName, QByteArray &data);

    std::tuple<bool, QString> chooseFilePathToSave(QString &pathToFile);
};
