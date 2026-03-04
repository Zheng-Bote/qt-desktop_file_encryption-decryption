<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Use Cases & Process Views](#use-cases--process-views)
  - [Use Case Diagram](#use-case-diagram)
  - [Sequence Diagram (Encryption Flow)](#sequence-diagram-encryption-flow)
  - [Activity Diagram (Decryption)](#activity-diagram-decryption)
  - [Flowchart](#flowchart)
  - [Information Flow Diagram](#information-flow-diagram)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Use Cases & Process Views

## Use Case Diagram
```mermaid
graph TD
  User((User))
  
  subgraph App[File Encryption App]
    UC1(Encrypt a File)
    UC2(Decrypt a File)
    UC3(Configure App)
    
    UC1a(Select Source File)
    UC1b(Enter Password)
    UC1c(Save Destination File)
  end
  
  User --> UC1
  User --> UC2
  User --> UC3
  
  UC1 -.->|include| UC1a
  UC1 -.->|include| UC1b
  UC1 -.->|include| UC1c
  
  UC2 -.->|include| UC1a
  UC2 -.->|include| UC1b
  UC2 -.->|include| UC1c
```

## Sequence Diagram (Encryption Flow)
```mermaid
sequenceDiagram
    actor User
    participant UI as EncryptFileDialog
    participant EM as EncryptionManager
    participant AES as QAESEncryption
    participant FS as FileSystem
    
    User->>UI: Select File
    UI->>FS: check File Access
    User->>UI: Enter Password
    User->>UI: Click "Encrypt"
    
    UI->>EM: encryptFile(src, dest, pwd)
    EM->>EM: generateRandomSalt()
    EM->>EM: generateRandomIV()
    EM->>EM: deriveKey(pwd, salt)
    
    EM->>FS: read source file chunk
    FS-->>EM: data
    
    EM->>AES: encode(data, key, iv)
    AES-->>EM: cipher_data
    
    EM->>FS: write header(salt, iv) + cipher_data
    
    EM-->>UI: return true (success)
    UI-->>User: Show Success Notification
```

## Activity Diagram (Decryption)
```mermaid
stateDiagram-v2
  [*] --> SelectFile
  SelectFile: User selects encrypted file
  SelectFile --> EnterPassword
  EnterPassword: User enters password
  EnterPassword --> StartDecryption
  StartDecryption: User starts Decryption
  
  state if_valid <<choice>>
  StartDecryption --> if_valid: File Valid?
  
  if_valid --> ReadHeader: yes
  ReadHeader: Read File Header
  ReadHeader --> ExtractSaltIV
  ExtractSaltIV: Extract Salt & IV
  ExtractSaltIV --> DeriveKey
  DeriveKey: Derive Key (Password + Salt)
  DeriveKey --> ReadChunk
  ReadChunk: Read Payload Chunk
  ReadChunk --> DecryptChunk
  DecryptChunk: Decrypt Chunk (AES-256-CBC)
  
  state if_mac_valid <<choice>>
  DecryptChunk --> if_mac_valid: MAC/Padding Valid?
  
  if_mac_valid --> WriteOutput: yes
  WriteOutput: Write to Output File
  WriteOutput --> ShowSuccess
  ShowSuccess: Show Success
  ShowSuccess --> [*]
  
  if_mac_valid --> ShowErrorMac: no
  ShowErrorMac: Show "Invalid Password or corrupted file"
  ShowErrorMac --> [*]
  
  if_valid --> ShowErrorFile: no
  ShowErrorFile: Show "File Not Found"
  ShowErrorFile --> [*]
```

## Flowchart
```mermaid
flowchart TD
    A[Start App] --> B{Choose Action}
    B -->|Click Encrypt| C[Open Encrypt Dialog]
    B -->|Click Decrypt| D[Open Decrypt Dialog]
    
    C --> E[Select File & Enter Password]
    E --> F["Generate Salt/IV & Key"]
    F --> G[AES Encrypt Data]
    G --> H[Save .enc File]
    H --> I[Success]
    
    D --> J[Select .enc File & Enter Password]
    J --> K["Read Header (Salt/IV)"]
    K --> L[Derive Key]
    L --> M[AES Decrypt Data]
    M --> N[Save original File]
    N --> I
```

## Information Flow Diagram
```mermaid
flowchart LR
    SourceFile[Raw File] --> |Bytes| Reader[File Reader]
    Reader --> |Plain Text Blocks| CryptoEngine((QAES Crypto))
    
    Password["User Input: Password"] --> |String| KDF["Key Derivation (PBKDF2)"]
    Salt[Random Salt] --> KDF
    KDF --> |256-bit Key| CryptoEngine
    
    IV[Random IV] --> CryptoEngine
    
    CryptoEngine --> |Cipher Text Blocks| Writer[File Writer]
    Writer --> OutputFile[.enc File]
```
