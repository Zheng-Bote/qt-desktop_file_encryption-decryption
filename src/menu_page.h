#pragma once

#include <QWidget>
#include "template.h"
#include "encrypt_file_dialog.h"
#include "decrypt_file_dialog.h"

#include <QDesktopServices>
#include <QUrl>

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

private:
    // menu page contents
    QLabel *label;
    QPushButton *encrypt_file_button;
    QPushButton *decrypt_file_button;
    QPushButton *exit_button;

    QStatusBar *bar;
    //QLabel *m_statusMiddle;
    QPushButton *m_statusMiddle;

    // dialogs

    EncryptFileDialog *encrypt_file_dialog;
    DecryptFileDialog *decrypt_file_dialog;

private slots:
    void encrypt_dialog();
    void decrypt_dialog();

    void openGithub();

    void close();

private:
    void closeEvent(QCloseEvent *event);
};
