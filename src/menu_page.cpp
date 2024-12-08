#include "menu_page.h"
#include "template.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <string>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLineEdit>
#include <QHBoxLayout>

#include <QAction>
#include <QMenu>
#include <QMenuBar>

using std::string;

MenuPage::MenuPage(Template *parent) : Template(parent)
{
    QString str_label = tr(
        "Hello and welcome!\nTo proceed, please select one of the following options:");
    label = new QLabel(this);
    label->setText(str_label);
    QFont font("Noto Serif", 13);
    QFontMetrics metrics(font);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(190, 130, metrics.horizontalAdvance(str_label), metrics.height() * 2);
    label->setStyleSheet("color: #7dfdfe");

    encrypt_file_button = new QPushButton(tr("&Encrypt File"), this);
    encrypt_file_button->setGeometry(350, 290, 150, 30);
    connect(encrypt_file_button, SIGNAL(clicked(bool)), this, SLOT(encrypt_dialog()));

    decrypt_file_button = new QPushButton(tr("&Decrypt File"), this);
    decrypt_file_button->setGeometry(550, 290, 150, 30);
    connect(decrypt_file_button, SIGNAL(clicked(bool)), this, SLOT(decrypt_dialog()));

    exit_button = new QPushButton(tr("E&xit"), this);
    exit_button->setGeometry(550, 360, 150, 30);
    connect(exit_button, SIGNAL(clicked(bool)), this, SLOT(close()));

    encrypt_file_dialog = nullptr;
    decrypt_file_dialog = nullptr;
}

void MenuPage::close()
{
    Template::close();
}

void MenuPage::encrypt_dialog()
{
    if (!encrypt_file_dialog) {
        encrypt_file_dialog = new EncryptFileDialog;
    }
    encrypt_file_dialog->file_name_textbox->setFocus();
    encrypt_file_dialog->show();
}

void MenuPage::decrypt_dialog()
{
    if (!decrypt_file_dialog) {
        decrypt_file_dialog = new DecryptFileDialog;
    }
    decrypt_file_dialog->file_name_textbox->setFocus();
    decrypt_file_dialog->show();
}

void MenuPage::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton response;

    response = QMessageBox::question(this,
                                     tr("Confirmation"),
                                     tr("Are you sure you want to exit?"),
                                     QMessageBox::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes)
    {

        encrypt_file_dialog->close();
        delete encrypt_file_dialog;

        decrypt_file_dialog->close();
        delete decrypt_file_dialog;

        event->accept();
    } else {
        event->ignore();
    }
}

MenuPage::~MenuPage() {}
