#pragma once

/**
 * @file encrypt_file_dialog.h
 * @brief Dialog window for file encryption.
 *
 * This class provides a graphical interface for the user to select a file
 * and a password for encryption. It includes password verification.
 */

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
class EncryptionManager; // Forward Declaration

/**
 * @class EncryptFileDialog
 * @brief A dialog class to handle file encryption.
 *
 * This class inherits from QDialog and manages the UI for selecting a file
 * to encrypt, setting a password (with verification), and triggering the
 * encryption process via the EncryptionManager.
 */
class EncryptFileDialog : public QDialog {
  Q_OBJECT

public:
  /**
   * @brief Constructs the EncryptFileDialog.
   * @param parent The parent widget (default is nullptr).
   */
  explicit EncryptFileDialog(QWidget *parent = nullptr);
  /**
   * @brief Destructor for EncryptFileDialog.
   */
  ~EncryptFileDialog(); // Important for cleanup if needed

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
  /**
   * @brief Slot called when the encrypt button is clicked.
   *
   * Validates the inputs (password match, file selection) and initiates
   * the encryption process.
   */
  void encrypt_file_slot();

  /**
   * @brief Slot called when the choose file button is clicked.
   *
   * Opens a file dialog to let the user select a file to encrypt.
   */
  void chooseFile();

  // Erlaubt der MenuPage Zugriff auf private Member (Fix für den Fehler)
  friend class MenuPage;
};