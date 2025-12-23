#pragma once

/**
 * @file template.h
 * @brief Base template window class.
 *
 * A base QMainWindow class that serves as a template for other windows
 * in the application, mostly setting up a common background.
 */

#include <QMainWindow>

class QWidget;
class QResizeEvent;
class QCloseEvent;

/**
 * @class Template
 * @brief Base class for application windows.
 *
 * This class inherits from QMainWindow and allows setting a common
 * look and feel (e.g., background) for the application's windows.
 */
class Template : public QMainWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructs the Template window.
   * @param parent The parent window (default is 0).
   */
  Template(QMainWindow *parent = 0);

private:
  QWidget *background;
};

#pragma once
