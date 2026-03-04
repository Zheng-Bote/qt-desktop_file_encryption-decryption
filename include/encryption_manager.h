/**
 * SPDX-FileComment: Logic class for handling file encryption and decryption.
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file encryption_manager.h
 * @brief Logic class for handling file encryption and decryption.
 * @version <2.1.0>
 * @date <2026-03-04>
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

#include <QObject>
#include <QString>
#include <tuple>

/**
 * @brief Manager class handling the core AES-256 CBC operations.
 * Optimized for streaming large files without high memory consumption.
 */
class EncryptionManager : public QObject {
  Q_OBJECT

public:
  explicit EncryptionManager(QObject *parent = nullptr);

  /**
   * @brief Encrypts the source file.
   * @param sourcePath Path to existing file.
   * @param password User password.
   * @param overwriteSource If true, replaces original file after success.
   * @return Tuple {StatusCode (1=OK, other=Error), Message}.
   */
  std::tuple<int, QString> encryptFile(const QString &sourcePath,
                                       const QString &password,
                                       bool overwriteSource);

  /**
   * @brief Decrypts the source file.
   * @param sourcePath Path to existing encoded file.
   * @param password User password.
   * @param overwriteSource If true, replaces original file after success.
   * @return Tuple {StatusCode (1=OK, other=Error), Message}.
   */
  std::tuple<int, QString> decryptFile(const QString &sourcePath,
                                       const QString &password,
                                       bool overwriteSource);

private:
  /**
   * @brief Internal helper to stream file data through AES engine.
   */
  std::tuple<int, QString> processFile(const QString &sourcePath,
                                       const QString &destPath,
                                       const QString &password,
                                       bool isEncrypting);
};