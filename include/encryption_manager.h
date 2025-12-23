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