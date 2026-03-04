/**
 * SPDX-FileComment: Base template window class.
 * SPDX-FileType: SOURCE
 * SPDX-FileContributor: ZHENG Robert
 * SPDX-FileCopyrightText: 2026 ZHENG Robert
 * SPDX-License-Identifier: MIT
 *
 * @file template.h
 * @brief Base template window class.
 * @version <2.1.0>
 * @date <2026-03-04>
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 *
 * @license MIT License
 */

#pragma once

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
