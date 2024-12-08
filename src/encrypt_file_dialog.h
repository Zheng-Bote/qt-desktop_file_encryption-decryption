#pragma once

#include "enc_dec_logic.h"
#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;

class EncryptFileDialog : public QDialog, public EncryptionDecryption
{
    Q_OBJECT

public:
    EncryptFileDialog(QDialog *parent = 0);

private:
    QLabel *enter_file_name_label;
    QLabel *enter_password_label;
    QLineEdit *file_name_textbox;
    QLineEdit *password_textbox;
    QPushButton *chooseFile_btn;
    QPushButton *encrypt_button;
    QPushButton *cancel_button;

    QString getFileName(QString &pathTofile);

private slots:
    void encrypt_file_slot();
    void chooseFile();

    friend class MenuPage;
};
