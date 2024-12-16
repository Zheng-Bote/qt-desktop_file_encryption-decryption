#include "menu_page.h"
#include "template.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <qdir.h>
#include <string>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLineEdit>
#include <QHBoxLayout>

#include <QStatusBar>
#include <QMenuBar>
#include "includes/rz_config.h"


using std::string;

MenuPage::MenuPage(Template *parent) : Template(parent)
{
    QString str_label = "Hello World 你好世界 Hola Mundo Привет мир Hallo Welt!\n\n"
                        + tr("With this little tool you can encrypt and decrypt "
                             "files.\nTo proceed, please select one of the following options:");
    label = new QLabel(this);
    label->setText(str_label);
    QFont font("Noto Serif", 13);
    QFontMetrics metrics(font);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    //label->setGeometry(190, 130, metrics.horizontalAdvance(str_label), metrics.height() * 2);
    label->setGeometry(20, 50, 900, 100);

    label->setStyleSheet("color: #7dfdfe");

    encrypt_file_button = new QPushButton(tr("&Encrypt File"), this);
    encrypt_file_button->setGeometry(370, 270, 150, 30);
    connect(encrypt_file_button, SIGNAL(clicked(bool)), this, SLOT(encrypt_dialog()));

    decrypt_file_button = new QPushButton(tr("&Decrypt File"), this);
    decrypt_file_button->setGeometry(570, 270, 150, 30);
    connect(decrypt_file_button, SIGNAL(clicked(bool)), this, SLOT(decrypt_dialog()));

    exit_button = new QPushButton(tr("E&xit"), this);
    exit_button->setGeometry(570, 330, 150, 30);
    connect(exit_button, SIGNAL(clicked(bool)), this, SLOT(close()));

    bar = new QStatusBar(this);
    QString version = "v";
    version.append(PROG_VERSION);
    m_statusMiddle = new QPushButton(version, this);
    m_statusMiddle->setFont(font);
    m_statusMiddle->setStyleSheet("font-size: 10px;"
                                  "border: none;");
    m_statusMiddle->setToolTip(
        tr("click to open your default Browser and go to the Github repository"));
    statusBar()->addWidget(m_statusMiddle, 1);
    connect(m_statusMiddle, SIGNAL(clicked(bool)), this, SLOT(openGithub()));

    encrypt_file_dialog = nullptr;
    decrypt_file_dialog = nullptr;
    createLanguageMenu();
}

void MenuPage::close()
{
    Template::close();
}

void MenuPage::encrypt_dialog()
{
    if (!encrypt_file_dialog) {
        encrypt_file_dialog = new EncryptFileDialog;
    }
    encrypt_file_dialog->file_name_textbox->setFocus();
    encrypt_file_dialog->show();
}

void MenuPage::decrypt_dialog()
{
    if (!decrypt_file_dialog) {
        decrypt_file_dialog = new DecryptFileDialog;
    }
    decrypt_file_dialog->file_name_textbox->setFocus();
    decrypt_file_dialog->show();
}

void MenuPage::help()
{
    QMessageBox::question(this,
                          tr("Help"),
                          tr("with some help from my friends?"),
                          QMessageBox::Ok);
}

void MenuPage::about()
{
    /*
    QMessageBox::information(this,
                             tr("About"),
                             tr("this is about.") + "\n\n <a href='http://www.trolltech.com'>Trolltech</a>",
                             QMessageBox::Ok);
*/
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Help / About");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("Mail: <a href='mailto:someone@somewhere.com?Subject=My%20Subject>Email me</a>");
    msgBox.exec();
}

void MenuPage::openGithub()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Zheng-Bote/qt_file_encryption-decryption"));
}

void MenuPage::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton response;

    response = QMessageBox::question(this,
                                     tr("Confirmation"),
                                     tr("Are you sure you want to exit?"),
                                     QMessageBox::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes)
    {

        encrypt_file_dialog->close();
        delete encrypt_file_dialog;

        decrypt_file_dialog->close();
        delete decrypt_file_dialog;

        event->accept();
    } else {
        event->ignore();
    }
}

MenuPage::~MenuPage() {}

void MenuPage::changeEvent(QEvent *event)
{
    if(0 != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
        {
            //retranslateUi(this);
            setWindowTitle(tr("File Encryption and Decryption"));
            encrypt_file_button->setText(tr("&Encrypt File"));
            decrypt_file_button->setText(tr("&Decrypt File"));
            exit_button->setText(tr("E&xit"));

            QString str_label = "Hello World 你好世界 Hola Mundo Привет мир Hallo Welt!\n\n"
                                + tr("With this little tool you can encrypt and decrypt "
                                     "files.\nTo proceed, please select one of the following options:");

            label->setText(str_label);
            break;
        }
        // this event is send, if the system, language changes
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
        break;
        }
    }
    QMainWindow::changeEvent(event);
}

void MenuPage::slotLanguageChanged(QAction *action)
{
    if(0 != action) {
        // load the language dependant on the action content
        loadLanguage(action->data().toString());
        //setWindowIcon(action->icon());
    }
}

void MenuPage::switchTranslator(QTranslator& translator, const QString& filename) {
    // remove the old translator
    qApp->removeTranslator(&translator);
    qDebug() << "switchTranslator:  " << filename;

    // load the new translator
    QString path =  ":/res/i18n/";
    //path.append("/languages/");
    if(translator.load(path + filename)){ //Here Path and Filename has to be entered because the system didn't find the QM Files else
        qApp->installTranslator(&translator);
    }

    qDebug() << "switchTranslator: " << filename;
}

void MenuPage::loadLanguage(const QString& rLanguage) {
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("qt_file_encryption-decryption_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qtbase_%1.qm").arg(rLanguage));
        //statusBar()->showMessage(tr("Current Language changed to %1").arg(languageName));
    }
}

void MenuPage::createLanguageMenu()
{
    QActionGroup* langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    // format systems language
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
    qDebug() << "defaultLocale: " << defaultLocale;

   // m_langPath = ":/res/i18n";
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("qt_file_encryption-decryption_*.qm"));

    if(fileNames.size() > 0){
        //languageMenu = menuBar()->addMenu(tr("&Language"));
        QIcon langIco(":/res/i18n/translate.png");
        languageMenu = menuBar()->addMenu("A / 六");
    }

    for (int i = 0; i < fileNames.size(); ++i) {
        qDebug() << "name: " << fileNames[i];
    }

    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale;
        locale = fileNames[i]; // "qt_file_encryption-decryption_de.qm"
        locale.truncate(locale.lastIndexOf('.')); // "qt_file_encryption-decryption_de"
        locale.remove(0, locale.lastIndexOf('_') + 1); // "de"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QString language;
        if(lang.compare("German") == 0) {
            language = "DE";
        } else if(lang.compare("English") == 0) {
            language = "EN";
        } else {
            language = lang;
        }

        qDebug() << "locale: " << locale << " lang: " << lang;
        qDebug() << "flag: " << QString("%1/%2.png").arg(m_langPath).arg(locale);
        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

        QAction *action = new QAction(ico, language, this);
        action->setCheckable(true);
        action->setData(locale);

        languageMenu->addAction(action);
        langGroup->addAction(action);

        // set default translators and language checked
        if (defaultLocale == locale) {
            action->setChecked(true);
        }
    }

    helpAct=new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpFaq), tr("&Help"), this);
    helpAct->setShortcuts(QKeySequence::HelpContents);
    connect(helpAct, &QAction::triggered, this, &MenuPage::help);
    aboutAct=new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout), tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::New);
    connect(aboutAct, &QAction::triggered, this, &MenuPage::about);
    infoMenu = menuBar()->addMenu(tr("&Info"));
    infoMenu->addAction(helpAct);
    infoMenu->addAction(aboutAct);
}
