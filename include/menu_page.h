#pragma once

#include <QWidget>
#include "template.h"
#include "encrypt_file_dialog.h"
#include "decrypt_file_dialog.h"

#include <QDesktopServices>
#include <QUrl>

#include <QTranslator>
#include <QActionGroup>
#include <QApplication>
#include <QLibraryInfo>

class QPushButton;
class QLabel;
class Template;
class QCloseEvent;
class QStatusBar;

class MenuPage : public Template
{
    Q_OBJECT

public:
    MenuPage(Template *parent = 0);
    ~MenuPage();

    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt
    void loadLanguage(const QString& rLanguage);

protected:
    void changeEvent(QEvent *event);

protected slots:
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction* action);

private:
    QMenu *languageMenu;
    void switchTranslator(QTranslator& translator, const QString& filename);
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
