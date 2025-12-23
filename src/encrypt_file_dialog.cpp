/**
 * @file encrypt_file_dialog.cpp
 * @brief Implementation of the EncryptFileDialog class.
 */

#include "encrypt_file_dialog.h"
#include "encryption_manager.h"

#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

EncryptFileDialog::EncryptFileDialog(QWidget *parent) : QDialog(parent) {
  m_logic = new EncryptionManager(this);
  setupUI();
}

EncryptFileDialog::~EncryptFileDialog() {
  // m_logic is automatically deleted as it is a child of QDialog
}

void EncryptFileDialog::setupUI() {
  setWindowTitle(tr("Encrypt File"));
  setWindowIcon(QIcon(":/res/images/icon.png"));
  resize(400, 250);

  // Create layouts and widgets
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // File Selection
  chooseFile_btn = new QPushButton(tr("Choose &file "), this);
  connect(chooseFile_btn, &QPushButton::clicked, this,
          &EncryptFileDialog::chooseFile);

  enter_file_name_label = new QLabel(tr("No file selected"), this);
  enter_file_name_label->setAlignment(Qt::AlignCenter);

  file_name_textbox = new QLineEdit(this);
  file_name_textbox->setPlaceholderText(tr("Path to file..."));
  // Make readonly so user chooses via button (avoids errors)
  // file_name_textbox->setReadOnly(true);

  // Password
  enter_password_label = new QLabel(tr("Enter Password:"), this);
  password_textbox = new QLineEdit(this);
  password_textbox->setEchoMode(QLineEdit::Password);

  enter_password_label_verify = new QLabel(tr("Verify Password:"), this);
  password_textbox_verify = new QLineEdit(this);
  password_textbox_verify->setEchoMode(QLineEdit::Password);

  // Checkbox
  overwriteFile_checkbox =
      new QCheckBox(tr("Encrypt Sourcefile (overwrite)"), this);
  overwriteFile_checkbox->setToolTip(
      tr("Checked: Replaces original file.\nUnchecked: Creates a new file."));

  // Action Buttons
  QHBoxLayout *btnLayout = new QHBoxLayout();
  encrypt_button = new QPushButton(tr("&Encrypt"), this);
  cancel_button = new QPushButton(tr("&Cancel"), this);

  btnLayout->addWidget(encrypt_button);
  btnLayout->addWidget(cancel_button);

  connect(encrypt_button, &QPushButton::clicked, this,
          &EncryptFileDialog::encrypt_file_slot);
  connect(cancel_button, &QPushButton::clicked, this, &QDialog::close);

  // Add to Main Layout
  mainLayout->addWidget(chooseFile_btn);
  mainLayout->addWidget(enter_file_name_label);
  mainLayout->addWidget(file_name_textbox);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(enter_password_label);
  mainLayout->addWidget(password_textbox);
  mainLayout->addWidget(enter_password_label_verify);
  mainLayout->addWidget(password_textbox_verify);
  mainLayout->addWidget(overwriteFile_checkbox);
  mainLayout->addStretch();
  mainLayout->addLayout(btnLayout);
}

void EncryptFileDialog::chooseFile() {
  // Start directory is always Home
  QString startDir = QDir::homePath();

  QString file = QFileDialog::getOpenFileName(
      this, tr("Select File to Encrypt"), startDir);

  if (!file.isEmpty()) {
    file_name_textbox->setText(file);
    QFileInfo fi(file);
    enter_file_name_label->setText(fi.fileName());
    chooseFile_btn->setText(tr("Change File"));
  }
}

void EncryptFileDialog::encrypt_file_slot() {
  // Validation
  QString sourceFile = file_name_textbox->text();
  if (sourceFile.isEmpty() || !QFile::exists(sourceFile)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Please select a valid file first."));
    return;
  }

  if (password_textbox->text() != password_textbox_verify->text()) {
    QMessageBox::critical(this, tr("Error"), tr("Passwords do not match!"));
    return;
  }

  bool overwrite = overwriteFile_checkbox->isChecked();

  // Target directory same as source file (implicit)
  // If NOT overwriting, we ask for save location,
  // but start in source file directory.
  if (!overwrite) {
    QFileInfo sourceInfo(sourceFile);
    QString suggestedName = sourceInfo.fileName() + ".aes";

    QString targetFile = QFileDialog::getSaveFileName(
        this, tr("Save Encrypted File"),
        sourceInfo.absolutePath() + "/" + suggestedName,
        tr("AES Encrypted (*.aes)"));

    if (targetFile.isEmpty())
      return; // Abbruch

    // Since our logic class currenty builds paths internally (to save code),
    // we use a trick: We let logic encrypt (to .aes next to source)
    // and then move it to the desired 'targetFile'. Alternative:
    // Extend Logic with 'targetPath' parameter. Here: Logic extension
    // simulated by move.

    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto [code, msg] =
        m_logic->encryptFile(sourceFile, password_textbox->text(), false);
    QApplication::restoreOverrideCursor();

    if (code == 1) {
      // The logic created [Source].aes. But we want [targetFile].
      QString defaultOut = sourceFile + ".aes";
      if (targetFile != defaultOut) {
        QFile::remove(targetFile); // If exists
        QFile::rename(defaultOut, targetFile);
      }
      QMessageBox::information(this, tr("Success"),
                               tr("File saved to:\n") + targetFile);
      close();
    } else {
      QMessageBox::critical(this, tr("Error"), msg);
    }
  } else {
    // Overwrite Case
    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto [code, msg] =
        m_logic->encryptFile(sourceFile, password_textbox->text(), true);
    QApplication::restoreOverrideCursor();

    if (code == 1) {
      QMessageBox::information(this, tr("Success"), msg);
      close();
    } else {
      QMessageBox::critical(this, tr("Error"), msg);
    }
  }
}