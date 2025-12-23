#include "decrypt_file_dialog.h"
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
#include <QVBoxLayout>

DecryptFileDialog::DecryptFileDialog(QWidget *parent) : QDialog(parent) {
  m_logic = new EncryptionManager(this);
  setupUI();
}

DecryptFileDialog::~DecryptFileDialog() {}

void DecryptFileDialog::setupUI() {
  setWindowTitle(tr("Decrypt File"));
  setWindowIcon(QIcon(":/res/images/icon.png"));
  resize(400, 200);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  chooseFile_btn = new QPushButton(tr("Choose encrypted file (*.aes)"), this);
  connect(chooseFile_btn, &QPushButton::clicked, this,
          &DecryptFileDialog::chooseFile);

  enter_file_name_label = new QLabel(tr("No file selected"), this);
  enter_file_name_label->setAlignment(Qt::AlignCenter);

  file_name_textbox = new QLineEdit(this);
  file_name_textbox->setPlaceholderText(tr("Path to file..."));

  enter_password_label = new QLabel(tr("Enter Password:"), this);
  password_textbox = new QLineEdit(this);
  password_textbox->setEchoMode(QLineEdit::Password);

  overwriteFile_checkbox =
      new QCheckBox(tr("Decrypt Sourcefile (overwrite)"), this);

  QHBoxLayout *btnLayout = new QHBoxLayout();
  decrypt_button = new QPushButton(tr("&Decrypt"), this);
  cancel_button = new QPushButton(tr("&Cancel"), this);
  btnLayout->addWidget(decrypt_button);
  btnLayout->addWidget(cancel_button);

  connect(decrypt_button, &QPushButton::clicked, this,
          &DecryptFileDialog::decrypt_file_slot);
  connect(cancel_button, &QPushButton::clicked, this, &QDialog::close);

  mainLayout->addWidget(chooseFile_btn);
  mainLayout->addWidget(enter_file_name_label);
  mainLayout->addWidget(file_name_textbox);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(enter_password_label);
  mainLayout->addWidget(password_textbox);
  mainLayout->addWidget(overwriteFile_checkbox);
  mainLayout->addStretch();
  mainLayout->addLayout(btnLayout);
}

void DecryptFileDialog::chooseFile() {
  // ANFORDERUNG 3: Startverzeichnis immer Home
  QString startDir = QDir::homePath();

  QString file = QFileDialog::getOpenFileName(
      this, tr("Open Encrypted File"), startDir, tr("Encrypted files (*.aes)"));
  if (!file.isEmpty()) {
    file_name_textbox->setText(file);
    QFileInfo fi(file);
    enter_file_name_label->setText(fi.fileName());
  }
}

void DecryptFileDialog::decrypt_file_slot() {
  QString sourceFile = file_name_textbox->text();
  if (sourceFile.isEmpty() || !QFile::exists(sourceFile)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Please select a valid file."));
    return;
  }

  bool overwrite = overwriteFile_checkbox->isChecked();

  if (!overwrite) {
    QFileInfo sourceInfo(sourceFile);
    QString suggestedName = sourceInfo.fileName();
    if (suggestedName.endsWith(".aes"))
      suggestedName.chop(4);
    else
      suggestedName += ".dec";

    // ANFORDERUNG: Save Dialog startet im Verzeichnis der Quelle
    QString targetFile = QFileDialog::getSaveFileName(
        this, tr("Save Decrypted File"),
        sourceInfo.absolutePath() + "/" + suggestedName);
    if (targetFile.isEmpty())
      return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto [code, msg] =
        m_logic->decryptFile(sourceFile, password_textbox->text(), false);
    QApplication::restoreOverrideCursor();

    if (code == 1) {
      // Umbenennen zum gewünschten Ziel
      // Logic erstellt [Source ohne .aes]
      QString defaultOut = sourceFile;
      if (defaultOut.endsWith(".aes"))
        defaultOut.chop(4);
      else
        defaultOut += ".dec";

      if (targetFile != defaultOut) {
        QFile::remove(targetFile);
        QFile::rename(defaultOut, targetFile);
      }
      QMessageBox::information(this, tr("Success"),
                               tr("File saved to:\n") + targetFile);
      close();
    } else {
      QMessageBox::critical(this, tr("Error"), msg);
    }

  } else {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto [code, msg] =
        m_logic->decryptFile(sourceFile, password_textbox->text(), true);
    QApplication::restoreOverrideCursor();

    if (code == 1) {
      QMessageBox::information(this, tr("Success"), msg);
      close();
    } else {
      QMessageBox::critical(this, tr("Error"), msg);
    }
  }
}