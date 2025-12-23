/**
 * @file template.cpp
 * @author ZHENG Bote (www.robert.hase-zheng.net)
 * @brief stupid template 4 main window object
 * @version 1.1.0
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024 ZHENG Robert
 *
 */

#include "template.h"
#include "rz_config.h"
#include <QGuiApplication>
#include <QMessageBox>
#include <QRect>
#include <QResizeEvent>
#include <QScreen>
#include <QSizePolicy>
#include <QWidget>

Template::Template(QMainWindow *parent) : QMainWindow(parent) {
  const int WIDTH = 1000;
  const int HEIGHT = 600;

  setFixedSize(WIDTH, HEIGHT);
  setWindowTitle(tr(PROG_NAME));
  setWindowIcon(QIcon(":/res/images/icon.png"));

  QScreen *screen = QGuiApplication::primaryScreen();
  int screenWidth = screen->size().width();
  int screenHeight = screen->size().height();
  int windowWidth = WIDTH;
  int windowHeight = HEIGHT;
  int x = (screenWidth - windowWidth) / 2;
  int y = (screenHeight - windowHeight) / 2;

  setGeometry(x, y, windowWidth, windowHeight);

  background = new QWidget(this);
  setCentralWidget(background);
  background->setStyleSheet(
      "background-image: url(:/res/images/background.jpg);"
      "background-position: center; /* Center the image */"
      "background-repeat: no-repeat; /* Prevent image repetition */");
}
