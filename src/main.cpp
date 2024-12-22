/**
 * @file main.cpp
 * @author ZHENG Bote (www.robert.hase-zheng.net)
 * @brief file encryption / decryption
 * @version 1.1.0
 * @date 2024-11-17
 * 
 * @copyright Copyright (c) 2024 ZHENG Robert
 * 
 */

#include <QApplication>
#include <QLibraryInfo>
#include <QMainWindow>
#include <QTranslator>
#include "menu_page.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/file_encryption-decryption.png"));

    MenuPage menu_page;

    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    menu_page.loadLanguage(locale);

    menu_page.show();

    return app.exec();
}
