#pragma once

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
class EncryptionManager; // Forward Declaration

class EncryptFileDialog : public QDialog {
  Q_OBJECT

public:
  explicit EncryptFileDialog(QWidget *parent = nullptr);
  ~EncryptFileDialog(); // Wichtig für Cleanup falls nötig

private:
  // UI Elements
  QLabel *enter_file_name_label;
  QLabel *enter_password_label;
  QLabel *enter_password_label_verify;
  QLineEdit *file_name_textbox;
  QLineEdit *password_textbox;
  QLineEdit *password_textbox_verify;
  QPushButton *chooseFile_btn;
  QPushButton *encrypt_button;
  QPushButton *cancel_button;
  QCheckBox *overwriteFile_checkbox;

  // Logic Component
  EncryptionManager *m_logic;

  void setupUI();

private slots:
  void encrypt_file_slot();
  void chooseFile();

  // Erlaubt der MenuPage Zugriff auf private Member (Fix für den Fehler)
  friend class MenuPage;
};