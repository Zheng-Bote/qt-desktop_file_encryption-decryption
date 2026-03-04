#!/bin/bash

# =============================================================================
# KONFIGURATION
# =============================================================================
APP_NAME="file_encryption-decryption"
PROJECT_DIR=$(pwd)
BUILD_DIR="$PROJECT_DIR/build_macos"

echo "--- Starte macOS DMG Erstellung ---"

# =============================================================================
# 1. QT PFAD FINDEN
# =============================================================================
if [ -d "$HOME/Qt/6.10.2/macos" ]; then
    QT_PATH="$HOME/Qt/6.10.2/macos"
elif command -v brew &> /dev/null && [ -d "$(brew --prefix qt)" ]; then
    QT_PATH="$(brew --prefix qt)"
else
    echo "FEHLER: Qt6 Installation nicht gefunden. Bitte CMAKE_PREFIX_PATH manuell setzen."
    exit 1
fi

echo "Verwende Qt Pfad: $QT_PATH"

# =============================================================================
# 2. BUILD UMGEBUNG VORBEREITEN
# =============================================================================
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# =============================================================================
# 3. KOMPILIEREN
# =============================================================================
echo "--- CMake Konfiguration ---"
cmake "$PROJECT_DIR" \
    -DCMAKE_PREFIX_PATH="$QT_PATH" \
    -DCMAKE_BUILD_TYPE=Release

echo "--- Build ---"
cmake --build . -j$(sysctl -n hw.ncpu)

if [ $? -ne 0 ]; then
    echo "FEHLER: Build fehlgeschlagen!"
    exit 1
fi

# =============================================================================
# 4. MACDEPLOYQT (ABHÄNGIGKEITEN HINZUFÜGEN)
# =============================================================================
echo "--- Sammle Qt-Abhängigkeiten ---"

MACDEPLOYQT="$QT_PATH/bin/macdeployqt"

if [ ! -f "$MACDEPLOYQT" ]; then
    echo "FEHLER: macdeployqt nicht gefunden unter $MACDEPLOYQT"
    exit 1
fi

"$MACDEPLOYQT" "$APP_NAME.app"

if [ $? -ne 0 ]; then
    echo "FEHLER: macdeployqt ist fehlgeschlagen."
    exit 1
fi

# =============================================================================
# 5. DMG GENERIEREN (MIT CREATE-DMG)
# =============================================================================
echo "--- Generiere Drag-and-Drop DMG ---"

if ! command -v create-dmg &> /dev/null; then
    echo "FEHLER: create-dmg nicht gefunden. Bitte mit 'brew install create-dmg' installieren."
    exit 1
fi

rm -f "$APP_NAME.dmg"

create-dmg \
  --volname "$APP_NAME Installer" \
  --window-pos 200 120 \
  --window-size 600 400 \
  --icon-size 100 \
  --icon "$APP_NAME.app" 150 190 \
  --hide-extension "$APP_NAME.app" \
  --app-drop-link 450 190 \
  "$APP_NAME.dmg" \
  "$APP_NAME.app/"

if [ $? -eq 0 ]; then
    echo "------------------------------------------------"
    echo "ERFOLG! Drag-and-Drop DMG erstellt:"
    ls -lh *.dmg
    echo "------------------------------------------------"
else
    echo "FEHLER: create-dmg ist fehlgeschlagen."
    exit 1
fi
