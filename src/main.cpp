/**
 * @file main.cpp
 * @brief Main entry point with intelligent language loading logic.
 */

#include "menu_page.h"
#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QSettings>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // IMPORTANT: These settings are necessary so QSettings knows where to save
  // (On Linux mostly in ~/.config/ZhengBote/FileEncryption.conf,
  // Windows Registry)
  QCoreApplication::setOrganizationName("ZhengBote");
  QCoreApplication::setOrganizationDomain("robert.hase-zheng.net");
  QCoreApplication::setApplicationName("FileEncryption");

  app.setWindowIcon(QIcon(":/res/images/icon.png"));

  MenuPage menu_page;

  // --- LANGUAGE LOGIC START ---

  QSettings settings;
  // Try to load saved language
  QString langCode = settings.value("language", "").toString();

  if (langCode.isEmpty()) {
    // Case 1: First program start (no setting available)
    QString sysLang = QLocale::system().name();
    // sysLang is e.g. "de_DE", "en_US", "fr_FR"

    // Cut off country code (de_DE -> de)
    sysLang = sysLang.left(sysLang.indexOf('_'));

    if (sysLang.compare("de", Qt::CaseInsensitive) == 0) {
      langCode = "de";
    } else {
      // Case 2: System is not German -> Fallback to English
      langCode = "en";
    }
  }

  // Load the determined language (DE or EN)
  menu_page.loadLanguage(langCode);

  // --- LANGUAGE LOGIC END ---

  menu_page.show();

  return app.exec();
}