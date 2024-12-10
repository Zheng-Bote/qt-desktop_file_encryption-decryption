#include "decrypt_file_dialog.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <qcheckbox.h>

DecryptFileDialog::DecryptFileDialog(QDialog *parent) : QDialog(parent)
{
    chooseFile_btn = new QPushButton(tr("Choose a &file "));
    connect(chooseFile_btn, SIGNAL(clicked(bool)), this, SLOT(chooseFile()));

    enter_file_name_label = new QLabel;
    enter_file_name_label->setText(tr("Choose a file: "));
    enter_file_name_label->setVisible(false);

    file_name_textbox = new QLineEdit;
    file_name_textbox->clear();
    //connect(file_name_textbox, SIGNAL(clicked(bool)), this, SLOT(chooseFile()));

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

    overwriteFile_checkbox = new QCheckBox(tr("decrypt &Sourcefile"), this);
    vertical_layout->addWidget(overwriteFile_checkbox);

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

    //resize(300, 100);
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
    QString msg{""};
    int oknok{0};

    std::tie(oknok, msg) = decrypt_data(file_name_textbox->text().toStdString(),
                                        password_textbox->text().toStdString(),
                                        overwriteFile_checkbox->isChecked());

    switch (oknok) {
    case 1: {
        QMessageBox::information(this, tr("Success"), msg);
        file_name_textbox->clear();
        password_textbox->clear();
        enter_file_name_label->clear();
        chooseFile_btn->setText(tr("Choose a &file "));
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

void DecryptFileDialog::chooseFile()
{
    QString file;
    chooseFile_btn->setText("choose file");
    file_name_textbox->clear();
    file = QFileDialog::getOpenFileName(this,
                                        tr("Open File"),
                                        QDir::currentPath(),
                                        tr("encoded files (*.aes)"));

    if (file.isEmpty() == false)
    {
        file_name_textbox->setText(file);
        chooseFile_btn->setText(tr("choosed &file:"));
        enter_file_name_label->setText(getFileName(file));
        enter_file_name_label->setAlignment(Qt::AlignCenter);
        enter_file_name_label->setVisible(true);
    }
}
