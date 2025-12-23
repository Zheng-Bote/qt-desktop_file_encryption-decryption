# File Encryption and Decryption

# Description

A desktop application to encrypt and decrypt files using AES encryption.

# Struktur-Übersicht:

```Bash
Projektordner/
├── CMakeLists.txt           (Einstiegspunkt für Build-System)
├── include/                 (Alle Header)
│   ├── encryption_manager.h (Ehemals enc_dec_logic.h)
│   ├── encrypt_file_dialog.h
│   ├── decrypt_file_dialog.h
│   ├── menu_page.h
│   ├── template.h
│   ├── qaesencryption.h     (3rd Party - bitte hierher verschieben)
│   └── rz_config.h          (Konfiguration)
└── src/                     (Alle Quellen)
    ├── main.cpp
    ├── encryption_manager.cpp
    ├── encrypt_file_dialog.cpp
    ├── decrypt_file_dialog.cpp
    ├── menu_page.cpp
    ├── template.cpp
    └── qaesencryption.cpp   (3rd Party - bitte hierher verschieben)
```

# changes

1. Performance & Logic Fix (Kritisch): In Ihrer ursprünglichen Logik wurde die Zieldatei für jeden Chunk (alle 4MB) neu geöffnet, ans Ende gesprungen, geschrieben und wieder geschlossen. Das ist extrem ineffizient. Die Datei wird nun einmal geöffnet und offen gehalten.

2. Architektur: Die Vererbung (DecryptFileDialog : public EncryptionDecryption) ist ein Design-Fehler ("Composition over Inheritance"). Dialoge sollten Logik benutzen, nicht Logik sein. Ich habe die Logik in eine eigenständige Klasse gekapselt.

3. Code-Bereinigung: Entfernung von unnötigen C-Style Casts, Vereinheitlichung auf Qt-Klassen (QFile statt Mix aus std::fstream und QFile für bessere Unicode/Pfad-Unterstützung unter Windows/macOS) und Modern C++.

4. Anforderung Verzeichnisse: Die Logik für Quell- und Zielverzeichnisse wurde gemäß Ihren Wünschen angepasst.

# i18n

```Bash
/opt/Qt/6.10.0/gcc_64/bin/lupdate -no-obsolete src/ -ts i18n/qt_file_encryption-decryption_en.ts
```

```Bash
/opt/Qt/6.10.0/gcc_64/bin/lrelease i18n/qt_file_encryption-decryption_en.ts -qm i18n/qt_file_encryption-decryption_en.qm
```

## Sprachauswahl

1. Erster Start (Clean Install):

- main.cpp: settings.value("language") ist leer.
- Systemsprache wird geprüft.
- Ist System = Deutsch? -> Lade "de".
- Ist System = Spanisch/Russisch/etc.? -> Lade "en".
- menu_page.loadLanguage("en") wird aufgerufen.
- Einstellung "language" = "en" wird gespeichert.

2. Benutzer ändert Sprache:

- Benutzer klickt im Menü auf "DE".
- slotLanguageChanged ruft loadLanguage("de").
- loadLanguage lädt die Übersetzer UND schreibt "language" = "de" in die Settings.

3. Nächster Start:

- main.cpp: settings.value("language") liefert "de".
- sysLang Logik wird übersprungen.
- menu_page.loadLanguage("de") wird aufgerufen.
- Anwendung startet direkt auf Deutsch.

# AppImage

```Bash
./create_appimage.sh
```

## Mögliche Fehlerursache: Fehlendes FUSE

AppImage benötigt FUSE, um erstellt oder ausgeführt zu werden. Auf neueren Systemen (z.B. Ubuntu 22.04+) fehlt oft libfuse2.

Wenn der Fehler weiterhin besteht, prüfen Sie bitte:

1. Ist FUSE installiert?

Versuchen Sie das linuxdeployqt Tool manuell zu starten:

```Bash
./linuxdeployqt-continuous-x86_64.AppImage --version
```

Wenn hier ein Fehler kommt (z.B. dlopen), installieren Sie FUSE:

**Ubuntu/Debian/Mint:**

```Bash
sudo apt update
sudo apt install libfuse2 file
```

**Fedora:**

```Bash
sudo dnf install fuse-libs
```

**Arch Linux:**

```Bash
sudo pacman -S fuse2
```

2. Dateiname Icon vs Desktop:

Im Skript oben benenne ich das Icon beim Kopieren explizit in $APP_NAME.png um. Das entspricht dem Eintrag Icon=file_encryption-decryption in der .desktop Datei. Das ist der häufigste Fehlergrund bei linuxdeployqt.
