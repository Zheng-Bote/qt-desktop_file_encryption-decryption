/**
 * @file encryption_manager.cpp
 * @brief Implementation of the EncryptionManager class.
 */

#include "encryption_manager.h"
#include "qaesencryption.h"

#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

// Static IV (Note: From a security perspective, a random IV per file is better,
// but we keep this for compatibility with existing logic)
static const QString STATIC_IV = "0x0c2ad4a4acb9f023";
static const int CHUNK_SIZE = 4 * 1024 * 1024; // 4 MB Chunks
static const int MIN_PASS_LEN = 5;

EncryptionManager::EncryptionManager(QObject *parent) : QObject(parent) {}

std::tuple<int, QString>
EncryptionManager::encryptFile(const QString &sourcePath,
                               const QString &password, bool overwriteSource) {
  if (password.length() < MIN_PASS_LEN) {
    return {2, tr("Password should have more than 5 characters")};
  }

  // Define temporary destination
  QString destPath = sourcePath + ".tmp_enc";

  // Execute process
  auto result = processFile(sourcePath, destPath, password, true);

  // On success and overwrite request: Replace original
  if (std::get<0>(result) == 1) {
    if (overwriteSource) {
      QFile::remove(sourcePath);
      QFile::rename(destPath, sourcePath + ".aes");
      return {1, tr("Encrypted file:\n") + sourcePath + ".aes"};
    } else {
      // If not overwrite, we rename the temp file to final name
      // Standard case: User did not choose path -> .aes
      // Note: The dialog (UI) should ideally pass the target path.
      // Here we generate the standard path if the dialog uses logic 1b.
      QString finalPath = sourcePath + ".aes";
      QFile::remove(finalPath); // If exists
      QFile::rename(destPath, finalPath);
      return {1, tr("Encrypted file:\n") + finalPath};
    }
  } else {
    // Cleanup on error
    QFile::remove(destPath);
  }

  return result;
}

std::tuple<int, QString>
EncryptionManager::decryptFile(const QString &sourcePath,
                               const QString &password, bool overwriteSource) {
  if (password.length() < MIN_PASS_LEN) {
    return {2, tr("Password should have more than 5 characters")};
  }

  QString destPath = sourcePath + ".tmp_dec";

  auto result = processFile(sourcePath, destPath, password, false);

  if (std::get<0>(result) == 1) {
    if (overwriteSource) {
      QFile::remove(sourcePath);
      QString finalName = sourcePath;
      if (finalName.endsWith(".aes"))
        finalName.chop(4);
      QFile::rename(destPath, finalName);
      return {1, tr("Decrypted file:\n") + finalName};
    } else {
      QString finalName = sourcePath;
      if (finalName.endsWith(".aes"))
        finalName.chop(4);
      else
        finalName += ".dec";

      QFile::remove(finalName);
      QFile::rename(destPath, finalName);
      return {1, tr("Decrypted file:\n") + finalName};
    }
  } else {
    QFile::remove(destPath);
  }

  return result;
}

std::tuple<int, QString>
EncryptionManager::processFile(const QString &sourcePath,
                               const QString &destPath, const QString &password,
                               bool isEncrypting) {
  QFile inFile(sourcePath);
  if (!inFile.open(QIODevice::ReadOnly)) {
    return {3, tr("Error opening source file:\n") + sourcePath};
  }

  QFile outFile(destPath);
  if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    inFile.close();
    return {3, tr("Error creating target file:\n") + destPath};
  }

  // Crypto Initialization
  QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
  QByteArray hashKey = QCryptographicHash::hash(password.toLocal8Bit(),
                                                QCryptographicHash::Sha256);
  QByteArray hashIV = QCryptographicHash::hash(STATIC_IV.toLocal8Bit(),
                                               QCryptographicHash::Md5);

  // qint64 totalSize = inFile.size();
  qint64 processedBytes = 0;

  // Stream Loop
  while (!inFile.atEnd()) {
    QByteArray chunk = inFile.read(CHUNK_SIZE);
    QByteArray processedChunk;

    if (isEncrypting) {
      processedChunk = encryption.encode(chunk, hashKey, hashIV);
    } else {
      processedChunk = encryption.decode(chunk, hashKey, hashIV);
      // Remove padding at end of file
      if (inFile.atEnd()) {
        while (processedChunk.endsWith('\0')) {
          processedChunk.chop(1);
        }
      }
    }

    if (outFile.write(processedChunk) == -1) {
      inFile.close();
      outFile.close();
      return {3, tr("Error writing to file.")};
    }

    processedBytes += chunk.size();

    // Maintain UI Responsiveness (simple method)
    if (processedBytes % (CHUNK_SIZE * 2) == 0) {
      QApplication::processEvents();
    }
  }

  inFile.close();
  outFile.close();

  return {1, "OK"};
}