#!/usr/bin/bash

# =============================================================================
# KONFIGURATION
# =============================================================================
APP_NAME="file_encryption-decryption"
PROJECT_DIR=$(pwd)
BUILD_DIR="$PROJECT_DIR/build_appimage"
APP_DIR="$BUILD_DIR/AppDir"
# Hier muss Ihr Quell-Icon liegen:
ICON_SOURCE="$PROJECT_DIR/resources/app_icon.png" 
DESKTOP_FILE="$PROJECT_DIR/linux/$APP_NAME.desktop"

# QMake Suche (Qt 6)
QMAKE_PATH="qmake6"
if ! command -v $QMAKE_PATH &> /dev/null; then
    QMAKE_PATH="qmake"
fi

# Prüfen ob Tools da sind
if ! command -v $QMAKE_PATH &> /dev/null; then
    echo "FEHLER: qmake nicht gefunden. Bitte Qt installieren oder Pfad anpassen."
    exit 1
fi

echo "--- Starte AppImage Erstellung für $APP_NAME ---"

# =============================================================================
# 1. BUILD UMGEBUNG VORBEREITEN
# =============================================================================
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# =============================================================================
# 2. KOMPILIEREN
# =============================================================================
echo "--- CMake Konfiguration ---"
cmake "$PROJECT_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr

echo "--- Build ---"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "FEHLER: Build fehlgeschlagen!"
    exit 1
fi

# =============================================================================
# 3. APPDIR STRUKTUR ERSTELLEN (KORRIGIERT)
# =============================================================================
echo "--- Installiere in AppDir ---"
make install DESTDIR="$APP_DIR"

# Ordner für Desktop-Datei und Icons anlegen (FHS Standard)
mkdir -p "$APP_DIR/usr/share/applications"
mkdir -p "$APP_DIR/usr/share/icons/hicolor/256x256/apps"

# Desktop Datei kopieren
cp "$DESKTOP_FILE" "$APP_DIR/usr/share/applications/"

# Icon kopieren und umbenennen, damit es zur .desktop Zeile "Icon=..." passt
cp "$ICON_SOURCE" "$APP_DIR/usr/share/icons/hicolor/256x256/apps/$APP_NAME.png"

# =============================================================================
# 4. LINUXDEPLOYQT
# =============================================================================
DEPLOY_TOOL="linuxdeployqt-continuous-x86_64.AppImage"

if [ ! -f "$PROJECT_DIR/$DEPLOY_TOOL" ]; then
    wget "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/$DEPLOY_TOOL" -O "$PROJECT_DIR/$DEPLOY_TOOL"
    chmod +x "$PROJECT_DIR/$DEPLOY_TOOL"
fi

# Wichtig: Environment Variablen unsetten, damit keine System-Libs reingezogen werden
unset LD_LIBRARY_PATH
unset QT_PLUGIN_PATH

# =============================================================================
# 5. GENERIERUNG
# =============================================================================
echo "--- Generiere AppImage ---"

# Hinweis: Wir rufen linuxdeployqt auf der Desktop-Datei im usr/share Pfad auf!
"$PROJECT_DIR/$DEPLOY_TOOL" "$APP_DIR/usr/share/applications/$APP_NAME.desktop" \
    -appimage \
    -qmake="$QMAKE_PATH" \
    -unsupported-allow-new-glibc \
    -verbose=2

if [ $? -eq 0 ]; then
    echo "------------------------------------------------"
    echo "ERFOLG! AppImage erstellt:"
    find "$BUILD_DIR" -name "*.AppImage"
    echo "------------------------------------------------"
else
    echo "FEHLER: linuxdeployqt ist fehlgeschlagen."
    exit 1
fi