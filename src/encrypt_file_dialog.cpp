/**
 * @file encrypt_file_dialog.cpp
 * @author ZHENG Bote (www.robert.hase-zheng.net)
 * @brief ui for encryption dialog
 * @version 1.1.0
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2024 ZHENMG Robert
 * 
 */

#include "encrypt_file_dialog.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

EncryptFileDialog::EncryptFileDialog(QDialog *parent) : QDialog(parent)
{
    chooseFile_btn = new QPushButton(tr("Choose &file "));
    connect(chooseFile_btn, SIGNAL(clicked(bool)), this, SLOT(chooseFile()));

    enter_file_name_label = new QLabel;
    enter_file_name_label->setText(tr("Choose a &file: "));
    enter_file_name_label->setVisible(false);

    file_name_textbox = new QLineEdit;
    file_name_textbox->clear();

    enter_password_label = new QLabel;
    enter_password_label->setText(tr("Enter the password: "));
    enter_password_label_verify = new QLabel;
    enter_password_label_verify->setText(tr("Verify password: "));

    password_textbox = new QLineEdit;
    password_textbox->setEchoMode(QLineEdit::Password);
    password_textbox->setToolTip(tr("don't lose your password, recovery is impossible!"));
    password_textbox_verify = new QLineEdit;
    password_textbox_verify->setEchoMode(QLineEdit::Password);

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(chooseFile_btn);
    vertical_layout->addWidget(enter_file_name_label);

    vertical_layout->addWidget(enter_password_label);
    vertical_layout->addWidget(password_textbox);
    vertical_layout->addWidget(enter_password_label_verify);
    vertical_layout->addWidget(password_textbox_verify);

    overwriteFile_checkbox = new QCheckBox(tr("encrypt &Sourcefile"), this);
    overwriteFile_checkbox->setToolTip(
        tr("checked: encrypt the original file.\nunchecked: encrypt a file copy."));
    vertical_layout->addWidget(overwriteFile_checkbox);

    encrypt_button = new QPushButton(tr("&Encrypt"));
    connect(encrypt_button, SIGNAL(clicked(bool)), this, SLOT(encrypt_file_slot()));

    cancel_button = new QPushButton(tr("&Cancel"));
    connect(cancel_button, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *horizontal_layout = new QHBoxLayout;
    horizontal_layout->addWidget(encrypt_button);
    horizontal_layout->addWidget(cancel_button);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(vertical_layout);
    main_layout->addLayout(horizontal_layout);

    resize(300, 200);
    setFixedHeight(200);
    setMaximumWidth(600);
    setLayout(main_layout);
    setWindowTitle(tr("Encrypt File"));
    setWindowIcon(QIcon(":/res/images/icon.png"));
}

QString EncryptFileDialog::getFileName(QString &pathTofile)
{
    QFile file(pathTofile);
    QFileInfo fileInfo(file.fileName());
    return fileInfo.fileName();
}

void EncryptFileDialog::encrypt_file_slot()
{
    QString msg{""};
    int oknok{0};

    if (QString::compare(password_textbox->text(),
                         password_textbox_verify->text(),
                         Qt::CaseSensitive)) {
        msg = tr("password doesn't match");
        QMessageBox::critical(this, tr("Error"), msg);
        return;
    }

    std::tie(oknok, msg) = encrypt_data(file_name_textbox->text().toStdString(),
                                        password_textbox->text().toStdString(),
                                        overwriteFile_checkbox->isChecked());

    switch (oknok) {
    case 1: {
        QMessageBox::information(this, tr("Success"), msg);
        //this->close();
        file_name_textbox->clear();
        password_textbox->clear();
        password_textbox_verify->clear();
        enter_file_name_label->clear();
        chooseFile_btn->setText(tr("Choose &file "));
        this->close();
        break;
    }
    case 2: {
        QMessageBox::warning(this, tr("Warning"), msg);
        break;
    }
    case 3: {
        QMessageBox::critical(this, tr("Error"), msg);
        break;
    }
    }
}

void EncryptFileDialog::chooseFile()
{
    QString file;
    chooseFile_btn->setText("choose file");
    file_name_textbox->clear();
    //file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),tr("files (*.txt *.md *.html *.sql)"));
    file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());

    if (file.endsWith(".aes")) {
        chooseFile();
    }
    if (file.isEmpty() == false) {
        file_name_textbox->setText(file);
        chooseFile_btn->setText(tr("choosed &file:"));
        enter_file_name_label->setText(getFileName(file));
        enter_file_name_label->setAlignment(Qt::AlignCenter);
        enter_file_name_label->setVisible(true);
    }
}
