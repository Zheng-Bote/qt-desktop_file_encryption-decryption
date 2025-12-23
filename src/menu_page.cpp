/**
 * @file menu_page.cpp
 * @brief UI start page implementation
 */

#include "menu_page.h"
#include "decrypt_file_dialog.h"
#include "encrypt_file_dialog.h"
#include "template.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QDir>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QUrl>
#include <QWidget>

#include <string>

#include "rz_config.h"

using std::string;

MenuPage::MenuPage(Template *parent) : Template(parent) {
#ifdef Q_OS_MACOS
  QFont font("Times New Roman", 13);
#else
  QFont font("Noto Serif", 13);
#endif
  // QFontMetrics metrics(font); // Unused warning fix

  QString str_label =
      "Hello World 你好世界 Hola Mundo Привет мир Hallo Welt!\n\n" +
      tr("With this little tool you can encrypt and decrypt "
         "files.\nTo proceed, please select one of the following options:");
  label = new QLabel(this);
  label->setText(str_label);
  label->setFont(font);

  label->setAlignment(Qt::AlignCenter);
  label->setGeometry(20, 50, 900, 100);

  label->setStyleSheet("color: #7dfdfe");

  encrypt_file_button = new QPushButton(tr("&Encrypt File"), this);
  encrypt_file_button->setGeometry(370, 270, 150, 30);
  connect(encrypt_file_button, SIGNAL(clicked(bool)), this,
          SLOT(encrypt_dialog()));

  decrypt_file_button = new QPushButton(tr("&Decrypt File"), this);
  decrypt_file_button->setGeometry(570, 270, 150, 30);
  connect(decrypt_file_button, SIGNAL(clicked(bool)), this,
          SLOT(decrypt_dialog()));

  exit_button = new QPushButton(tr("E&xit"), this);
  exit_button->setGeometry(570, 330, 150, 30);
  connect(exit_button, SIGNAL(clicked(bool)), this, SLOT(close()));

  QString version = "v";
  version.append(PROG_VERSION);
  m_statusMiddle = new QPushButton(version, this);

  m_statusMiddle->setFont(font);
  m_statusMiddle->setStyleSheet("font-size: 10px; border: none;");
  m_statusMiddle->setToolTip(
      tr("click to open your default Browser and go to the Github repository"));

  statusBar()->addWidget(m_statusMiddle, 1);
  connect(m_statusMiddle, SIGNAL(clicked(bool)), this, SLOT(openGithub()));

  encrypt_file_dialog = nullptr;
  decrypt_file_dialog = nullptr;

  createLanguageMenu();
}

MenuPage::~MenuPage() {
  // Clean up dialogs if they assume ownership wasn't taken by Qt parent system
  // (Though with parent=this they are auto-deleted, explicit check is safe)
  if (encrypt_file_dialog)
    delete encrypt_file_dialog;
  if (decrypt_file_dialog)
    delete decrypt_file_dialog;
}

void MenuPage::close() { Template::close(); }

void MenuPage::encrypt_dialog() {
  if (!encrypt_file_dialog) {
    encrypt_file_dialog = new EncryptFileDialog(this);
  }
  encrypt_file_dialog->file_name_textbox->setFocus();
  encrypt_file_dialog->show();
}

void MenuPage::decrypt_dialog() {
  if (!decrypt_file_dialog) {
    decrypt_file_dialog = new DecryptFileDialog(this);
  }
  decrypt_file_dialog->file_name_textbox->setFocus();
  decrypt_file_dialog->show();
}

void MenuPage::help() {
  QString intro = tr("File Encryption and Decryption") + "<br>";
  intro.append(tr("Desktop application for Linux, MacOS and Windows") + "\n\n");
  QString encrypt =
      "<b>" + tr("Encryption") + ":</b><br>1) " +
      tr("choose a file to encrypt.") + "<br>1a) " +
      tr("activate the checkbox to") + ":<br>- " +
      tr("encrypt the source file") + "<br>-> " +
      tr("original file will be replaced with the encrypted one") + "<br>1b) " +
      tr("keep checkbox unchecked to") + ":<br>- " +
      tr("encrypt the given file into a new (encrypted) file") + "<br><br><b>" +
      tr("Warning") + ":</b><br>" + tr("Don't loose your password.") + "<br>" +
      tr("Decryption/Recovery without a valid password is impossible!");
  QString decrypt =
      "<b>" + tr("Decryption") + ":</b><br>1) " +
      tr("choose a file to decrypt (only files with extension '.aes').") +
      "<br>1a) " + tr("activate the checkbox to") + ":<br>- " +
      tr("decrypt the source file") + "<br>-> " +
      tr("original file will be replaced with the decrypted one.") +
      "<br>1b) " + tr("keep checkbox unchecked to") + ":<br>- " +
      tr("decrypt the given file into a new (decrypted) file");
  QString text = encrypt + "<br><br>" + decrypt;
  QString detailedText =
      tr("File Encryption and Decryption") + "\n- " + tr("Encryption") +
      ": AES-256 CBC\n- " + tr("Password") + ": SHA256, " +
      tr("5 to 32 characters") + "\n- initialization vector: MD5";

  QMessageBox msgBox(this);
  msgBox.setWindowTitle(tr("Help"));
  msgBox.setIcon(QMessageBox::Question);
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText(intro);
  msgBox.setInformativeText(text);
  msgBox.setDetailedText(detailedText);
  msgBox.setFixedWidth(700);
  msgBox.exec();
}

void MenuPage::about() {
  QString text = tr("File Encryption and Decryption") + "\n\n";
  QString setInformativeText = "<p><ul><li>" + tr("Encryption") +
                               ": AES-256 CBC</li><li>" + tr("Password") +
                               ": SHA256, " + tr("5 to 32 characters") +
                               "</li><li>initialization vector: "
                               "MD5</li><li>chunk size: 4 MB</li></ul></p>";
  setInformativeText.append(
      "<p><ul><li>" + QString(PROG_EXEC_NAME) + " v " + QString(PROG_VERSION) +
      "</li><li>" + tr("Desktop application for Linux, MacOS and Windows") +
      "</li><li>Copyright (c) 2024 ZHENG Robert</li><li>OSS MIT " +
      tr("license") + "</li></ul></p>");
  setInformativeText.append("<br><a "
                            "href=\"https://github.com/Zheng-Bote/"
                            "qt-desktop_file_encryption-decryption\" "
                            "alt=\"Github repository\">");
  setInformativeText.append(QString(PROG_EXEC_NAME) + " v" +
                            QString(PROG_VERSION) + " " + tr("at") +
                            " Github</a>");

  QMessageBox msgBox(this);
  msgBox.setWindowTitle(tr("About") + " " + tr(PROG_NAME));
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText(text);
  msgBox.setInformativeText(setInformativeText);
  msgBox.setFixedWidth(900);
  msgBox.exec();
}

void MenuPage::openGithub() { QDesktopServices::openUrl(QUrl(PROG_HOMEPAGE)); }

void MenuPage::closeEvent(QCloseEvent *event) {
  QMessageBox::StandardButton response;

  response = QMessageBox::question(this, tr("Confirmation"),
                                   tr("Are you sure you want to exit?"),
                                   QMessageBox::Yes | QMessageBox::No);

  if (response == QMessageBox::Yes) {
    // Dialogs are cleaned up by Qt parent/child mechanism or destructor
    event->accept();
  } else {
    event->ignore();
  }
}

void MenuPage::changeEvent(QEvent *event) {
  if (0 != event) {
    switch (event->type()) {
    case QEvent::LanguageChange: {
      setWindowTitle(tr("File Encryption and Decryption"));
      encrypt_file_button->setText(tr("&Encrypt File"));
      decrypt_file_button->setText(tr("&Decrypt File"));
      exit_button->setText(tr("E&xit"));

      QString str_label =
          "Hello World 你好世界 Hola Mundo Привет мир Hallo Welt!\n\n" +
          tr("With this little tool you can encrypt and decrypt "
             "files.\nTo proceed, please select one of the following options:");

      label->setText(str_label);
      break;
    }
    case QEvent::LocaleChange: {
      QString locale = QLocale::system().name();
      locale.truncate(locale.lastIndexOf('_'));
      loadLanguage(locale);
      break;
    }

    default:
      break;
    }
  }
  QMainWindow::changeEvent(event);
}

void MenuPage::slotLanguageChanged(QAction *action) {
  if (0 != action) {
    loadLanguage(action->data().toString());
  }
}

void MenuPage::switchTranslator(QTranslator &translator,
                                const QString &filename) {
  qApp->removeTranslator(&translator);
  QString path = ":/res/i18n/";
  if (translator.load(path + filename)) {
    qApp->installTranslator(&translator);
  }
}

void MenuPage::loadLanguage(const QString &rLanguage) {
  if (m_currLang != rLanguage) {
    m_currLang = rLanguage;
    QLocale locale = QLocale(m_currLang);
    QLocale::setDefault(locale);
    QString languageName = QLocale::languageToString(locale.language());

    // Load translator
    switchTranslator(
        m_translator,
        QString("qt_file_encryption-decryption_%1.qm").arg(rLanguage));
    switchTranslator(m_translatorQt, QString("qtbase_%1.qm").arg(rLanguage));

    statusBar()->showMessage(
        tr("Current Language changed to %1").arg(languageName), 2000);

    // Save selection for next start
    QSettings settings;
    settings.setValue("language", m_currLang);

    // Optional: Update menu so checkmark updates immediately
    // (This is complex because createLanguageMenu rebuilds menu.
    // Easier needed: adjust createLanguageMenu to check m_currLang)

    // Since createLanguageMenu was called in constructor, we need to
    // ensure checkmarks are correct. Simple way: Loop through actions
    // and set checked.
    if (languageMenu) {
      QList<QAction *> actions = languageMenu->actions();
      for (QAction *action : actions) {
        if (action->data().toString() == m_currLang) {
          action->setChecked(true);
        }
      }
    }
  }
}

void MenuPage::createLanguageMenu() {
  menuBar()->setNativeMenuBar(false);
#ifdef Q_OS_MACOS
  menuBar()->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
#endif

  QActionGroup *langGroup = new QActionGroup(this);
  langGroup->setExclusive(true);

  connect(langGroup, SIGNAL(triggered(QAction *)), this,
          SLOT(slotLanguageChanged(QAction *)));

  QString defaultLocale = QLocale::system().name();
  defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

  QDir dir(m_langPath);
  QStringList fileNames =
      dir.entryList(QStringList("qt_file_encryption-decryption_*.qm"));

  if (fileNames.size() > 0) {
    languageMenu = menuBar()->addMenu("A / 六");
  }

  // Loop through found language files
  for (int i = 0; i < fileNames.size(); ++i) {
    QString locale;
    locale = fileNames[i];
    locale.truncate(locale.lastIndexOf('.'));
    locale.remove(0, locale.lastIndexOf('_') + 1);

    QString lang = QLocale::languageToString(QLocale(locale).language());
    QString language;
    if (lang.compare("German") == 0) {
      language = "DE";
    } else if (lang.compare("English") == 0) {
      language = "EN";
    } else {
      language = lang;
    }

    QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

    QAction *action = new QAction(ico, language, this);
    action->setCheckable(true);
    action->setData(locale);

    languageMenu->addAction(action);
    langGroup->addAction(action);

    // Set checked based on m_currLang (set by main.cpp)
    // instead of QLocale::system(). Note: On very first call in constructor
    // m_currLang might be empty, but loadLanguage is called in main.cpp
    // directly after and corrects the checkmark.
    if (m_currLang == locale) {
      action->setChecked(true);
    }
  }

  helpAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpFaq),
                        tr("&Help"), this);
  helpAct->setShortcuts(QKeySequence::HelpContents);
  connect(helpAct, &QAction::triggered, this, &MenuPage::help);
  aboutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                         tr("&About"), this);
  aboutAct->setShortcuts(QKeySequence::WhatsThis);
  connect(aboutAct, &QAction::triggered, this, &MenuPage::about);
  infoMenu = menuBar()->addMenu(tr("&Info"));
  infoMenu->addAction(helpAct);
  infoMenu->addAction(aboutAct);
}