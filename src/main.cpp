/**
 * @file main.cpp
 * @brief Main entry point with intelligent language loading logic
 */

#include "menu_page.h"
#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QSettings>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // WICHTIG: Diese Angaben sind notwendig, damit QSettings weiß, wo gespeichert
  // werden soll (Unter Linux meist in ~/.config/ZhengBote/FileEncryption.conf,
  // Windows Registry)
  QCoreApplication::setOrganizationName("ZhengBote");
  QCoreApplication::setOrganizationDomain("robert.hase-zheng.net");
  QCoreApplication::setApplicationName("FileEncryption");

  app.setWindowIcon(QIcon(":/res/images/icon.png"));

  MenuPage menu_page;

  // --- SPRACH-LOGIK START ---

  QSettings settings;
  // Versuche, gespeicherte Sprache zu laden
  QString langCode = settings.value("language", "").toString();

  if (langCode.isEmpty()) {
    // Fall 1: Erster Programmstart (keine Einstellung vorhanden)
    QString sysLang = QLocale::system().name();
    // sysLang ist z.B. "de_DE", "en_US", "fr_FR"

    // Schneide Ländercode ab (de_DE -> de)
    sysLang = sysLang.left(sysLang.indexOf('_'));

    if (sysLang.compare("de", Qt::CaseInsensitive) == 0) {
      langCode = "de";
    } else {
      // Fall 2: System ist nicht Deutsch -> Fallback auf Englisch
      langCode = "en";
    }
  }

  // Lade die ermittelte Sprache (DE oder EN)
  menu_page.loadLanguage(langCode);

  // --- SPRACH-LOGIK ENDE ---

  menu_page.show();

  return app.exec();
}