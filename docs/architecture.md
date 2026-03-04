# Architecture & Design Views

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
componentDiagram
  package "User Interface" {
    [Main Window]
    [Encrypt Dialog]
    [Decrypt Dialog]
  }
  
  package "Business Logic" {
    [Encryption Manager]
    [Config Loader]
  }
  
  package "Core Cryptography" {
    [QAESEncryption]
    [OpenSSL/QtCrypto]
  }
  
  [Main Window] --> [Encrypt Dialog]
  [Main Window] --> [Decrypt Dialog]
  [Encrypt Dialog] --> [Encryption Manager]
  [Decrypt Dialog] --> [Encryption Manager]
  [Encryption Manager] --> [QAESEncryption]
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
