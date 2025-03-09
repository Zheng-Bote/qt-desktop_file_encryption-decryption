<div id="top" align="center">
<h1>file encryption/decryption</h1>

<h4>File encryption and decryption (Desktop version)</h4>
<h6>for Linux, MacOS, Windows</h6>

[Report Issue](https://github.com/Zheng-Bote/qt_file_encryption_decryption/issues) [Request Feature](https://github.com/Zheng-Bote/qt_file_encryption_decryption/pulls)

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/Zheng-Bote/qt_file_encryption_decryption?logo=GitHub)](https://github.com/Zheng-Bote/qt_file_encryption-decryption/releases)

</div>

<hr>

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

**Table of Contents**

- [Description](#description)
  - [Features](#features)
  - [Status](#status)
- [Documentation](#documentation)
  - [Encryption](#encryption)
  - [Decryption](#decryption)
  - [Screenshots](#screenshots)
    - [start page](#start-page)
    - [encryption dialog](#encryption-dialog)
    - [decryption dialog](#decryption-dialog)
    - [switch language](#switch-language)
    - [help page](#help-page)
    - [about page](#about-page)
- [Installation](#installation)
  - [Linux](#linux)
  - [MacOS](#macos)
  - [Windows](#windows)
    - [Set-Up](#set-up)
    - [Set-Up - De-Installation](#set-up---de-installation)
    - [without set-up](#without-set-up)
- [Development](#development)
  - [Dependencies](#dependencies)
    - [CMake](#cmake)
    - [QT](#qt)
    - [Qt-AES](#qt-aes)
  - [folder structure](#folder-structure)
- [Authors and License](#authors-and-license)
  - [License](#license)
  - [Authors](#authors)
    - [Code Contributors](#code-contributors)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

<hr>

# Description

![QT](https://img.shields.io/badge/Community-6-41CD52?logo=qt)
![CXX](https://img.shields.io/badge/C++-23-blue?logo=cplusplus)

Qt6 C++23 Desktop application to encrypt / decrypt the given file.

- Encryption: AES-256 CBC
- Password: SHA256, between 5 to 32 characters
- initialization vector: MD5

See folder `docs/img` for screenshots.

### Features

- [x] i18n: Desktop application supports German and English language
- [x] encrypt/decrypt every readable file (binary-mode, chunk size 4MB)

- [x] runs on DOS/Windows
- [x] runs on MacOS
- [x] runs on Linux
- [ ] runs on iOS
- [ ] runs on Android
- [ ] runs on HarmonyOS

- [x] OSS and license
- [x] works as designed
- [ ] no bugs

- [x] some more or less usefull Github Actions for GH-repo, GH-pages, GH-wiki, CI/CD-Pipelines
- [ ] Packagemanager
- [x] Buildsystem: CMake
- [x] Installation routine (no Adminstrator rights needed)
- [ ] portable application

<p align="right">(<a href="#top">back to top</a>)</p>

## Status

![GitHub Created At](https://img.shields.io/github/created-at/Zheng-Bote/qt_file_encryption_decryption)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/Zheng-Bote/qt_file_encryption_decryption?logo=GitHub)](https://github.com/Zheng-Bote/qt_file_encryption-decryption/releases)
![GitHub Release Date](https://img.shields.io/github/release-date/Zheng-Bote/qt_file_encryption_decryption)
![Status](https://img.shields.io/badge/Status-stable-green)
![Status](https://img.shields.io/badge/Status-works_as_designed-green)

![GitHub Issues](https://img.shields.io/github/issues/Zheng-Bote/qt_file_encryption_decryption)
![GitHub Pull Requests](https://img.shields.io/github/issues-pr/Zheng-Bote/qt_file_encryption_decryption)

### breaking Changes

> \[!CAUTION]
> breaking changes! Release v0.1.0 is **NOT** compatible to higher versions.

| Version | status                                  | Comment                                                            |
| ------- | --------------------------------------- | ------------------------------------------------------------------ |
| v0.1.0  | **NOT** compatible to higher versions.  | initial creation, text mode                                        |
| v1.0.0  | **NOT** compatible with lower versions. | optimized input/output, text mode limited to some text-based files |
| v2.0.0  | **NOT** compatible with lower versions. | binary-mode, works on every readable file. No filesize limit.      |

# Documentation

> \[!TIP]
> _see also_ [Screenshots](#screenshots)

## Encryption

- choose a file to encrypt
- activate the checkbox to encrypt the source file (original file will be replaced with the encrypted one)
- keep checkbox unchecked to encrypt the given file into a new (encrypted) file (encrypted file will be stored in your temp-folder `<filename>.aes`)

> \[!WARNING]
> don't loose your password. Decryption/Recovery without valid password is impossible!

## Decryption

- choose a file to decrypt (only files with extension ".aes")
- activate the checkbox to decrypt the source file (original file will be replaced with the decrypted one)
- keep checkbox unchecked to decrypt the given file into a new (decrypted) file (decrypted file will be stored in your temp-folder <originalfilename>)

## Test / Performance

**tested on**

- x86_64 Ubuntu 24.04.2 LTS
- Intel(R) Core(TM) i5-9600K CPU @ 3.70GHz with 6 cpu cores
- average SSD storage
- fallocate -l 1G test.img

**read / write binary mode of 1 GB file with a chunk size of 4 MB**
| type | val |
| ---- | --------- |
| real | 5m52,734s |
| user | 5m49,905s |
| sys | 0m2,391s |

**read / write binary mode of 1 GB file with a chunk size of 16 MB**
| type | val |
| ---- | --------- |
| real | 5m50,753s |
| user | 5m48,691s |
| sys | 0m2,049s |

> \[!INFO]
> so compiled chunk size is 4 MB to give better feedback about the process status.

## Screenshots

### start page

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/01a_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/01b_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/01_de.png)

### encryption dialog

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/02_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/02_de.png)

### decryption dialog

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/03_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/03_de.png)

### switch language

(Menu: `A/六)` -> `DE|EN`)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/04_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/04_de.png)

### help page

(Menu: `Info -> Help` or keyboard shortcut `F1`)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/05_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/05_de.png)

### about page

(Menu: `Info -> About` or keyboard shortcut `A`)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/06_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/06_de.png)

<p align="right">(<a href="#top">back to top</a>)</p>

# Installation

## Linux

- make AppImage executable (eg. `chmod 755 file_encryption-decryption-x86_64.AppImage` )
- execute / doubleclick AppImage

## MacOS

- make DiskImage executable (eg. `chmod 755 file_encryption-decryption.dmg`)
- execute / double-click on the `.dmg` file. A window should appear showing the program you are installing and the Applications Folder.
- drag the program into the Applications Folder; macOS will automatically install the software for you.

## Windows

### Set-Up

- doubleclick `setup__file_encryption-decryption__x86_amd64_v<VERSION>.exe` (eg.: `setup__file_encryption-decryption__x86_amd64_v1.1.0.exe`)
- choose "install for all users" (Administrator rights needed)
- choose "install for current user only" (no Administrator rights needed)
- choose "select language (DE|EN) to use during installation"

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/00_setup_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/00_setup_de.png)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/01_setup_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/01_setup_de.png)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/02_setup_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/02_setup_de.png)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/03_setup_de.png)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/04_setup_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/04_setup_de.png)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/05_setup_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/05_setup_de.png)

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/06_setup_en.png)
![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/06_setup_de.png)

### Set-Up - De-Installation

For de-installation, go to Windows `Settings -> Apps -> installed Apps` and choose within the hamburger menu "deinstallation"

![App Screenshot](https://github.com/Zheng-Bote/qt_file_encryption_decryption/blob/main/docs/img/deinstall_en.png)

### without set-up

- unzip `file_encryption-decryption__x86_amd64_v<VERSION>.zip` (eg.: `file_encryption-decryption__x86_amd64_v1.1.0.zip`) to a folder
- execute/doubleclick the executable `file_encryption-decryption_x86_amd64.exe`

# Development

> \[!NOTE]
> rename `CMakeLists.txt<OS-PLATFORM>` to `CMakeLists.txt`

## Dependencies

> \[!NOTE]
> All licenses are therefore Open Source.

### CMake

CMake: A Powerful Software Build System

[![CMake](https://img.shields.io/badge/CMake-v3.23+-064F8C?logo=cmake)](https://github.com/conan-io/conan)
[![BSD-3 License](https://img.shields.io/badge/License-BSD_3-green.svg)](https://choosealicense.com/licenses/bsd-3-clause/)

### QT

> \[!NOTE]
> _QT6 - Community Edition_ >= 6.7

The Qt framework contains a comprehensive set of highly intuitive and modularized C++ library classes and is loaded with APIs to simplify your application development. Qt produces highly readable, easily maintainable and reusable code with high runtime performance and small footprint – and it's cross-platform.

[![QT](https://img.shields.io/badge/Community-for_Open_Source_Development-black?logo=qt)](https://www.qt.io/download-open-source)
[![LGPL](https://img.shields.io/badge/License-LGPL_v3-green.svg)](https://choosealicense.com/licenses/lgpl-3.0/) _see also:_[Obligations of the GPL and LGPL](https://www.qt.io/licensing/open-source-lgpl-obligations)

### Qt-AES

Small and portable AES encryption class for Qt. Native support for all key sizes - 128/192/256 bits - ECB, CBC, CFB and OFB modes for all key sizes partial AES-NI support

[![Matt Bricke](https://img.shields.io/badge/Github-bricke-black?logo=github)](https://github.com/bricke/Qt-AES)
[![UNLICENSE](https://img.shields.io/badge/License-Unlicense-green.svg)](https://github.com/bricke/Qt-AES?tab=Unlicense-1-ov-file)

<p align="right">(<a href="#top">back to top</a>)</p>

## folder structure

<!-- readme-tree start -->

```
.
├── .github
│   ├── actions
│   │   └── doctoc
│   │       ├── README.md
│   │       ├── action.yml
│   │       └── dist
│   │           ├── index.js
│   │           ├── index.js.map
│   │           ├── licenses.txt
│   │           └── sourcemap-register.js
│   └── workflows
│       ├── ghp-call_Readme.yml
│       ├── ghp-create_doctoc.yml
│       ├── ghp-markdown_index.yml
│       ├── repo-actions_docu.yml
│       ├── repo-call_Readme.yml
│       ├── repo-create_doctoc.yml_
│       ├── repo-create_doctoc_md.yml
│       └── repo-create_tree_readme.yml
├── .gitignore
├── LICENSE
├── README.md
├── docs
│   └── img
│       ├── 00_setup_de.png
│       ├── 00_setup_en.png
│       ├── 01_de.png
│       ├── 01_setup_de.png
│       ├── 01_setup_en.png
│       ├── 01a_en.png
│       ├── 01b_en.png
│       ├── 02_de.png
│       ├── 02_en.png
│       ├── 02_setup_de.png
│       ├── 02_setup_en.png
│       ├── 03_de.png
│       ├── 03_en.png
│       ├── 03_setup_de.png
│       ├── 04_de.png
│       ├── 04_en.png
│       ├── 04_setup_de.png
│       ├── 04_setup_en.png
│       ├── 05_de.png
│       ├── 05_en.png
│       ├── 05_setup_de.png
│       ├── 05_setup_en.png
│       ├── 06_de.png
│       ├── 06_en.png
│       ├── 06_setup_de.png
│       ├── 06_setup_en.png
│       └── deinstall_en.png
├── src
│   ├── CMakeLists.txt
│   ├── CMakeLists.txt_Linux
│   ├── CMakeLists.txt_Windows
│   ├── configure
│   │   ├── ClangTidy.cmake
│   │   ├── Doxygen.cmake
│   │   ├── extra_headers
│   │   └── rz_config.h.in
│   ├── decrypt_file_dialog.cpp
│   ├── decrypt_file_dialog.h
│   ├── enc_dec_logic.cpp
│   ├── enc_dec_logic.h
│   ├── encrypt_file_dialog.cpp
│   ├── encrypt_file_dialog.h
│   ├── i18n
│   │   ├── de.png
│   │   ├── de_DE.qm
│   │   ├── en.png
│   │   ├── qt_file_encryption-decryption_de.qm
│   │   ├── qt_file_encryption-decryption_de.ts
│   │   ├── qt_file_encryption-decryption_en.qm
│   │   ├── qt_file_encryption-decryption_en.ts
│   │   ├── qtbase_de.qm
│   │   ├── qtbase_en.qm
│   │   └── translate.png
│   ├── images
│   │   ├── background.jpg
│   │   ├── icon-16x16.ico
│   │   ├── icon-256x256.ico
│   │   ├── icon-32x32.ico
│   │   └── icon.png
│   ├── includes
│   │   ├── qaesencryption.cpp
│   │   ├── qaesencryption.h
│   │   ├── rz_config.h
│   │   └── rz_snipptes.hpp
│   ├── main.cpp
│   ├── menu_page.cpp
│   ├── menu_page.h
│   ├── resources
│   │   ├── file_encryption-decryption.icns
│   │   ├── file_encryption-decryption.iconset
│   │   │   ├── icon_128x128.png
│   │   │   ├── icon_128x128@2.png
│   │   │   ├── icon_16x16.png
│   │   │   ├── icon_16x16@2.png
│   │   │   ├── icon_256x256.png
│   │   │   ├── icon_256x256@2.png
│   │   │   ├── icon_32x32.png
│   │   │   ├── icon_32x32@2.png
│   │   │   ├── icon_512x512.png
│   │   │   └── icon_512x512@2.png
│   │   └── file_encryption-decryption.png
│   ├── resources.qrc
│   ├── template.cpp
│   └── template.h
└── tree.bak

14 directories, 95 files
```

<!-- readme-tree end -->

<p align="right">(<a href="#top">back to top</a>)</p>

# Authors and License

## License

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)

Copyright (c) 2024 ZHENG Robert

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Authors

- [![Zheng Robert](https://img.shields.io/badge/Github-Zheng_Robert-black?logo=github)](https://www.github.com/Zheng-Bote)

### Code Contributors

![Contributors](https://img.shields.io/github/contributors/Zheng-Bote/qt_file_encryption_decryption?color=dark-green)

[![Zheng Robert](https://img.shields.io/badge/Github-Zheng_Robert-black?logo=github)](https://www.github.com/Zheng-Bote)

<hr>

:vulcan_salute:

<p align="right">(<a href="#top">back to top</a>)</p>
