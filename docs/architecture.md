# Architecture & Design Views

---

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Bounded Contexts](#bounded-contexts)
- [Class Diagram](#class-diagram)
- [Component Diagram](#component-diagram)
- [Entity Relationship Diagram](#entity-relationship-diagram)
- [Logical Decomposition Diagram](#logical-decomposition-diagram)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

---

## Bounded Contexts
```mermaid
mindmap
  root((File Encryption App))
    UI_Context
      MainWindow
      EncryptDialog
      DecryptDialog
      MenuRouter
    Security_Context
      AESEncryption
      KeyDerivation
      FileIO_Secure
    Config_Context
      AppConfig
      ThemeConfig
```

## Class Diagram
```mermaid
classDiagram
  class QMainWindow {
    <<Framework>>
  }
  
  class Template {
    +Template(parent)
    +initUI()
    -openEncryptDialog()
    -openDecryptDialog()
  }
  
  class EncryptionManager {
    +encryptFile(source, dest, password) bool
    +decryptFile(source, dest, password) bool
    -generateKey(password, salt) QByteArray
  }
  
  class QAESEncryption {
    +encode(rawText, key, iv) QByteArray
    +decode(rawText, key, iv) QByteArray
  }
  
  class EncryptFileDialog {
    +EncryptFileDialog(parent)
    -onEncryptClicked()
    -onSelectFileClicked()
  }
  
  class DecryptFileDialog {
    +DecryptFileDialog(parent)
    -onDecryptClicked()
    -onSelectFileClicked()
  }
  
  QMainWindow <|-- Template
  Template --> EncryptFileDialog : opens
  Template --> DecryptFileDialog : opens
  EncryptFileDialog --> EncryptionManager : uses
  DecryptFileDialog --> EncryptionManager : uses
  EncryptionManager --> QAESEncryption : delegates
```

## Component Diagram
```mermaid
graph TD
  subgraph UserInterface[User Interface]
    MainWindow[Main Window]
    EncryptDialog[Encrypt Dialog]
    DecryptDialog[Decrypt Dialog]
  end
  
  subgraph BusinessLogic[Business Logic]
    EncryptionManager[Encryption Manager]
    ConfigLoader[Config Loader]
  end
  
  subgraph CoreCryptography[Core Cryptography]
    QAESEncryption[QAESEncryption]
    OpenSSLCrypto[OpenSSL/QtCrypto]
  end
  
  MainWindow --> EncryptDialog
  MainWindow --> DecryptDialog
  EncryptDialog --> EncryptionManager
  DecryptDialog --> EncryptionManager
  EncryptionManager --> QAESEncryption
```

## Entity Relationship Diagram
```mermaid
erDiagram
    FILE ||--|| ENCRYPTED_FILE : becomes
    FILE {
        string path
        int size
        date modified
    }
    ENCRYPTED_FILE {
        string path
        string salt
        string iv
        byte[] payload
    }
    USER ||--o{ ENCRYPTED_FILE : "encrypts/decrypts with password"
    USER {
        string password
    }
```

## Logical Decomposition Diagram
```mermaid
graph TD
  App[File Encryption App]
  
  App --> PresentationLayer[Presentation Layer]
  App --> ApplicationLayer[Application Layer]
  App --> InfrastructureLayer[Infrastructure Layer]
  
  PresentationLayer --> UI[Qt Widgets UI]
  PresentationLayer --> Assets[Resources & Styling]
  
  ApplicationLayer --> FileMan[File Management]
  ApplicationLayer --> CryptMan[Cryptographic Management]
  
  InfrastructureLayer --> OSBridge[OS File System Bridge]
  InfrastructureLayer --> CryptLib[AES-256-CBC Implementation]
```
