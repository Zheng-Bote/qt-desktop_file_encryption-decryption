/**
 * @file rz_snipptes.hpp
 * @author ZHENG Bote (www.robert.hase-zheng.net)
 * @brief some c++23 snippets
 * @version 0.1.0
 * @date 2024-12-16
 * 
 * @copyright Copyright (c) 2024 ZHENG Robert
 * 
 */
#pragma once

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <filesystem>
#include <format>
#include <print>
#include <qdir.h>
#include <string>
#include <tuple>

namespace rz_snipptes {

std::tuple<bool, QString> isFile(const std::filesystem::path &pathToFile)
{
    if (std::filesystem::is_regular_file(pathToFile)) {
        return std::make_tuple(true, QObject::tr("is a regular file"));
    }
    return std::make_tuple(false, QObject::tr("not a regular file"));
}

std::tuple<bool, QString> fileIsWriteable(QString &pathTofile)
{
    QFileInfo fileInfo(pathTofile);
    return std::make_tuple(fileInfo.isWritable(), QObject::tr("File isn't writeable"));
}

std::tuple<bool, QString> getFileName(QString &pathTofile)
{
    QFile file(pathTofile);
    QFileInfo fileInfo(file.fileName());
    return std::make_tuple(true, fileInfo.fileName());
}

std::tuple<bool, QString> getFileExtension(QString &pathTofile)
{
    QFileInfo fi(pathTofile);
    QString ext = fi.suffix();

    return std::make_tuple(true,  ext);
}

std::uintmax_t getFileSize(const std::filesystem::path &file)
{
    return std::filesystem::file_size(file);
}

std::tuple<bool, QString> copyFile(const std::filesystem::path &pathToFileSrc,
                                   const std::filesystem::path &pathToFileDest) noexcept
{
    const auto copyOptions = std::filesystem::copy_options::update_existing;

    std::filesystem::copy(pathToFileSrc, pathToFileDest, copyOptions);
    try {
        std::filesystem::copy(pathToFileSrc, pathToFileDest, copyOptions);
        qDebug() << "copy file successfull to: " << pathToFileDest.c_str() << "\n";
        return std::make_tuple(true, QObject::tr("not a regular file"));
    } catch (std::filesystem::filesystem_error &e) {
        qDebug() << "copy file error: " << e.what() << "\n";
        return std::make_tuple(false, e.what());
    }

}

std::tuple<bool, QString> getFilePath(QString &pathTofile)
{
    QFileInfo fileInfo(pathTofile);
    QString path = fileInfo.absolutePath() + "/";
    //path.append(QDir::separator());
    qDebug() << "getFilePath: " << path << "\n";
    return std::make_tuple(true, path);
}

std::tuple<bool, QString> getTempDir()
{
    QString dir = QDir::tempPath() + "/";
    //dir.append(QDir::separator());

    return std::make_tuple(true, dir);
}

std::tuple<bool, QString> checkFile(QString &pathTofile, bool overwrite, bool isEncrypted)
{
    const auto fileSizeLimit = 4000000000; // 4GB
    const QString fileExtensionEncrypted = ".aes";
    bool oknok{false};
    QString msg{""};
    QString fileName{""};
    QString fileExtension{""};

    std::tie(oknok, msg) = isFile(pathTofile.toStdString());
    if (oknok == false) {
        return std::make_tuple(oknok, msg);
    }

    std::tie(oknok, fileName) = getFileName(pathTofile);
    std::tie(oknok, fileExtension) = getFileExtension(pathTofile);

    auto fileSize = getFileSize(pathTofile.toStdString());
    if (static_cast<int>(fileSize) > fileSizeLimit) {
        msg = fileName + " ";
        msg.append(QObject::tr("is too big."));
        msg.append(" " + QObject::tr("Filesize limit is"));
        msg.append(" 4GB");
        return std::make_tuple(false, msg);
    }
    if (overwrite) {
        std::tie(oknok, msg) = fileIsWriteable(pathTofile);
        if (oknok == false) {
            return std::make_tuple(oknok, msg);
        }
    }
    if (isEncrypted == true && fileExtension.compare(fileExtensionEncrypted) == 0) {
        msg = std::format("{} is encrypted", fileName.toStdString()).c_str();
        return std::make_tuple(false, msg);
    }
    return std::make_tuple(1, "");
}

} // namespace rz_snipptes
