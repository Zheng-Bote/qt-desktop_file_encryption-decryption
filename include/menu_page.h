#pragma once

/**
 * @file menu_page.h
 * @brief Main menu window of the application.
 *
 * This class handles the main menu UI, language selection, and navigation
 * to encryption and decryption dialogs.
 */

#include "decrypt_file_dialog.h"
#include "encrypt_file_dialog.h"
#include "template.h"
#include <QWidget>

#include <QDesktopServices>
#include <QUrl>

#include <QActionGroup>
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>

class QPushButton;
class QLabel;
class Template;
class QCloseEvent;
class QStatusBar;

/**
 * @class MenuPage
 * @brief The main menu widget.
 *
 * Inherits from Template and provides the main entry point UI for the
 * application. It allows users to switch languages and open encryption
 * or decryption tools.
 */
class MenuPage : public Template {
  Q_OBJECT

public:
  /**
   * @brief Constructs the MenuPage.
   * @param parent The parent template widget (default is 0).
   */
  MenuPage(Template *parent = 0);

  /**
   * @brief Destructor for MenuPage.
   */
  ~MenuPage();

  QTranslator m_translator;   // contains the translations for this application
  QTranslator m_translatorQt; // contains the translations for qt
  /**
   * @brief Loads a specific language translation.
   * @param rLanguage The language code (e.g., "en", "de") or filename part.
   */
  void loadLanguage(const QString &rLanguage);

protected:
  /**
   * @brief Handles language change events.
   * @param event The change event.
   */
  void changeEvent(QEvent *event);

protected slots:
  /**
   * @brief Slot called when a language action is triggered from the menu.
   * @param action The action attempting to change the language.
   */
  void slotLanguageChanged(QAction *action);

private:
  QMenu *languageMenu;
  void switchTranslator(QTranslator &translator, const QString &filename);
  // creates the language menu dynamically from the content of m_langPath
  void createLanguageMenu(void);

  QString m_currLang; // contains the currently loaded language
  QString m_langPath = ":/res/i18n";

  // menu page contents
  QLabel *label;
  QPushButton *encrypt_file_button;
  QPushButton *decrypt_file_button;
  QPushButton *exit_button;

  QMenu *infoMenu;
  QAction *helpAct;
  QAction *aboutAct;

  QStatusBar *bar;
  QPushButton *m_statusMiddle;

  // dialogs

  EncryptFileDialog *encrypt_file_dialog;
  DecryptFileDialog *decrypt_file_dialog;

private slots:
  void encrypt_dialog();
  void decrypt_dialog();

  void help();
  void about();
  void openGithub();

  void close();

private:
  void closeEvent(QCloseEvent *event);
};
