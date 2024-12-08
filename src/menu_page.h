#pragma once

#include <QWidget>
#include "template.h"
#include "encrypt_file_dialog.h"
#include "decrypt_file_dialog.h"

class QPushButton;
class QLabel;
class Template;
class QCloseEvent;

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

    // dialogs

    EncryptFileDialog *encrypt_file_dialog;
    DecryptFileDialog *decrypt_file_dialog;

private slots:
    void encrypt_dialog();
    void decrypt_dialog();

    void close();

private:
    void closeEvent(QCloseEvent *event);
};
