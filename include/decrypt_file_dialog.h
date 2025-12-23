#pragma once

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
class EncryptionManager;

class DecryptFileDialog : public QDialog {
  Q_OBJECT

public:
  explicit DecryptFileDialog(QWidget *parent = nullptr);
  ~DecryptFileDialog();

private:
  QLabel *enter_file_name_label;
  QLabel *enter_password_label;
  QLineEdit *file_name_textbox;
  QLineEdit *password_textbox;
  QPushButton *chooseFile_btn;
  QPushButton *decrypt_button;
  QPushButton *cancel_button;
  QCheckBox *overwriteFile_checkbox;

  EncryptionManager *m_logic;
  void setupUI();

private slots:
  void decrypt_file_slot();
  void chooseFile();

  // Erlaubt der MenuPage Zugriff auf private Member (Fix für den Fehler)
  friend class MenuPage;
};