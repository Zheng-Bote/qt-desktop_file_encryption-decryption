#include "decrypt_file_dialog.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

DecryptFileDialog::DecryptFileDialog(QDialog *parent) : QDialog(parent)
{
    chooseFile_btn = new QPushButton(tr("Choose a &file "));
    connect(chooseFile_btn, SIGNAL(clicked(bool)), this, SLOT(chooseFile()));

    enter_file_name_label = new QLabel;
    enter_file_name_label->setText(tr("Choose a file: "));
    enter_file_name_label->setVisible(false);

    file_name_textbox = new QLineEdit;
    connect(file_name_textbox, SIGNAL(clicked(bool)), this, SLOT(chooseFile()));

    enter_password_label = new QLabel;
    enter_password_label->setText(tr("Enter the password: "));

    password_textbox = new QLineEdit;
    password_textbox->setEchoMode(QLineEdit::Password);

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    // adding widgets to the vertical layout
    vertical_layout->addWidget(chooseFile_btn);
    vertical_layout->addWidget(enter_file_name_label);
    vertical_layout->addWidget(enter_password_label);
    vertical_layout->addWidget(password_textbox);

    encrypt_button = new QPushButton(tr("&Decrypt"));
    connect(encrypt_button, SIGNAL(clicked(bool)), this, SLOT(decrypt_file_slot()));

    cancel_button = new QPushButton(tr("&Cancel"));
    connect(cancel_button, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *horizontal_layout = new QHBoxLayout;
    // adding the widgets to horizontal layout
    horizontal_layout->addWidget(encrypt_button);
    horizontal_layout->addWidget(cancel_button);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(vertical_layout);
    main_layout->addLayout(horizontal_layout);

    resize(300, 100);
    setLayout(main_layout);
    setWindowTitle(tr("Decrypt File"));
}

QString DecryptFileDialog::getFileName(QString &pathTofile)
{
    QFile file = pathTofile;
    QFileInfo fileInfo(file.fileName());
    return fileInfo.fileName();
}

void DecryptFileDialog::decrypt_file_slot()
{
    int status = decrypt_data(file_name_textbox->text().toStdString(),
                              password_textbox->text().toStdString());

    if (status == -1) {
        QMessageBox::warning(this, tr("Warning"), tr("Enter the file name."));
    } else if (status == -2) {
        QMessageBox::warning(this, tr("Warning"), tr("Enter the password."));
    } else if (status == -3) {
        QString message = tr("File") + "\"" + file_name_textbox->text() + "\""
                          + tr("does not exists!");
        QMessageBox::critical(this, tr("Error"), message);
    } else if (status == -4) {
        QMessageBox::critical(this, tr("Error"), tr("File allready decrypted!"));
    } else if (status == -5) {
        QMessageBox::critical(this, tr("Error"), tr("Incorrect password. Please try again."));
    } else {
        QMessageBox::information(this, tr("Success"), tr("Data decrypted successfully."));
        this->close();
        file_name_textbox->clear();
        password_textbox->clear();
    }
}

void DecryptFileDialog::chooseFile()
{
    QString file;
    chooseFile_btn->setText("choose file");
    file = QFileDialog::getOpenFileName(this,
                                        tr("Open File"),
                                        QDir::currentPath(),
                                        tr("Textfiles (*.aes)"));

    if (file.isEmpty() == false)
    {
        file_name_textbox->setText(file);
        chooseFile_btn->setText(tr("choosed &file:"));
        enter_file_name_label->setText(getFileName(file));
        enter_file_name_label->setAlignment(Qt::AlignCenter);
        enter_file_name_label->setVisible(true);
    }
}
