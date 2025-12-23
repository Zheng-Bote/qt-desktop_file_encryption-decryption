#include "encryption_manager.h"
#include "qaesencryption.h" // Liegt jetzt in include/ via CMake include_directories

#include <QApplication> // Für processEvents
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

// Statischer IV (Hinweis: Aus Security-Sicht ist random IV pro Datei besser,
// aber wir behalten dies für Kompatibilität mit Ihrer bestehenden Logik bei)
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

  // Temporäres Ziel definieren
  QString destPath = sourcePath + ".tmp_enc";

  // Prozess ausführen
  auto result = processFile(sourcePath, destPath, password, true);

  // Bei Erfolg und Overwrite-Wunsch: Original ersetzen
  if (std::get<0>(result) == 1) {
    if (overwriteSource) {
      QFile::remove(sourcePath);
      QFile::rename(destPath, sourcePath + ".aes");
      return {1, tr("Encrypted file:\n") + sourcePath + ".aes"};
    } else {
      // Wenn nicht overwrite, benennen wir das temp file final um
      // Standardfall: User hat Pfad nicht gewählt -> .aes
      // Hinweis: Der Dialog (UI) sollte idealerweise den Zielpfad übergeben.
      // Hier generieren wir den Standardpfad, falls der Dialog Logik 1b nutzt.
      QString finalPath = sourcePath + ".aes";
      QFile::remove(finalPath); // Falls existiert
      QFile::rename(destPath, finalPath);
      return {1, tr("Encrypted file:\n") + finalPath};
    }
  } else {
    // Aufräumen bei Fehler
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

  // Crypto Initialisierung
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
      // Padding-Entfernung am Dateiende
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

    // UI Responsiveness erhalten (einfache Methode)
    if (processedBytes % (CHUNK_SIZE * 2) == 0) {
      QApplication::processEvents();
    }
  }

  inFile.close();
  outFile.close();

  return {1, "OK"};
}