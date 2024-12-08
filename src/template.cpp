#include "template.h"
#include <QWidget>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QRect>
#include <QScreen>

#include "includes/rz_config.h"

Template::Template(QMainWindow *parent) : QMainWindow(parent)
{
    const int WIDTH = 1000;
    const int HEIGHT = 600;

    setFixedSize(WIDTH, HEIGHT);
    setWindowTitle(tr("File Encryption and Decryption") + " v" + PROG_VERSION);
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
    background->setStyleSheet("background-image: url(:/res/images/background.jpg);"
                              "background-position: center; /* Center the image */"
                              "background-repeat: no-repeat; /* Prevent image repetition */");
}
