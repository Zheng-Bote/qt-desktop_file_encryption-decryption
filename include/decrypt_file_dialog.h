#pragma once

/**
 * @file decrypt_file_dialog.h
 * @brief Dialog window for file decryption.
 *
 * This class provides a graphical interface for the user to select a file
 * and a password for decryption.
 */

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
class EncryptionManager;

/**
 * @class DecryptFileDialog
 * @brief A dialog generic class to handle file decryption.
 *
 * This class inherits from QDialog and sets up the user interface for
 * selecting a file to decrypt, entering the password, and executing the
 * decryption process via the EncryptionManager.
 */
class DecryptFileDialog : public QDialog {
  Q_OBJECT

public:
  /**
   * @brief Constructs the DecryptFileDialog.
   * @param parent The parent widget (default is nullptr).
   */
  explicit DecryptFileDialog(QWidget *parent = nullptr);
  /**
   * @brief Destructor for DecryptFileDialog.
   */
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
  /**
   * @brief Slot called when the decrypt button is clicked.
   *
   * Initiates the decryption process using the provided file and password.
   */
  void decrypt_file_slot();

  /**
   * @brief Slot called when the choose file button is clicked.
   *
   * Opens a file dialog to let the user select a file to decrypt.
   */
  void chooseFile();

  // Erlaubt der MenuPage Zugriff auf private Member (Fix für den Fehler)
  friend class MenuPage;
};