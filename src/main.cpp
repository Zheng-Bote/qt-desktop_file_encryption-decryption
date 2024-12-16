#include <QApplication>
#include <QLibraryInfo>
#include <QMainWindow>
#include <QTranslator>
#include "menu_page.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /*
    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                              QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        qDebug() << "qtBaseTranslator ok: " << QLocale::system().name() << " " << QLibraryInfo::path(QLibraryInfo::TranslationsPath);
        app.installTranslator(&qtBaseTranslator);
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qt_file_encryption-decryption_" + QLocale(locale).name();
        if (translator.load(":/res/i18n/" + baseName)) {
            qDebug() << "qtAppTranslator ok: " << baseName;
            app.installTranslator(&translator);
            break;
        } else {
            qDebug() << "qtAppTranslator NOK: " << baseName;
        }
    }
*/
    MenuPage menu_page;
    menu_page.show();

    return app.exec();
}
