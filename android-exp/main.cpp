/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include <QApplication>
#include "mobileapp.h"
#include "desktopapp.h"
#include "functions.h"
#include "about.h"
#include <QDebug>
#include <QTranslator>
#include <iostream>

#include <QFontDatabase>

//For test only
//#define MOBILE


//Define location for program dirs
void initPaths()
{

    QString defPath;

#ifdef Q_OS_ANDROID
    defPath = "/sdcard/Orayta/Books/";
//    defPath = "Orayta/Books/"; //IZAR: try to access books directly in assets. this didn't work. waiting for answer from bogdan.

//    MAINPATH = "/sdcard/Orayta/";
    //set terget to assets folder
    MAINPATH = "/Orayta/";
#elif defined Q_OS_LINUX
    defPath = "/usr/share/Orayta/Books/";
    MAINPATH = "/usr/share/Orayta/";
#elif defined Q_OS_WIN
    defPath = QDir::rootPath() + "\\program files\\orayta\\books\\"; //TODO: ask yoch to currect this.
    MAINPATH = QDir::rootPath() + "\\program files\\orayta\\";
#endif

    QDir dir("Books/");
    if (dir.exists()) BOOKPATH = dir.absolutePath() + "/" ;
    else BOOKPATH = defPath;
    //qDebug() << "bookpath:" << BOOKPATH ;

    dir.setPath("UserData/");
    if (dir.exists()) USERPATH =  dir.absolutePath() + "/";
    else
    {
        dir.mkdir(QDir::homePath() + "/.Orayta/");
        USERPATH = QDir::homePath() + "/.Orayta/";
    }

    dir.setPath("Htmltmp/");
    if (dir.exists()) TMPPATH = dir.absolutePath() + "/";
    else
    {
        //Improved by Yoch. Thanks again.
        dir.mkdir(QDir::tempPath() + "/Orayta/");
        dir.mkdir(QDir::tempPath() + "/Orayta/Htmltmp/");

        TMPPATH = QDir::tempPath() + "/Orayta/Htmltmp/";
    }

    dir.setPath("Pics/");
    if ( !dir.exists() )
    {
        //UNIX ONLY:
        #ifndef Q_WS_WIN    //Yoch's idea. Thank you very much :-)
            //Create symbolic link to the folder

            //TODO: this should be solved in a different way. the html files should be corrected so the link is to the right place

            system("ln -s -T /tmp/Orayta/Pics /usr/share/Orayta/Books/Pics 2> /dev/null");
        #endif
    }
}


//IZAR: adds a font to the application
void addFont(const QString &font)
{
    int fontId = QFontDatabase::addApplicationFont(font);
    if (fontId >= 0)
        qDebug()<< "installed font successfuly: " << font;
    else
        qDebug()<< "cant add font "<< font;
}

//IZAR: define location for fonts
void initFonts()
{
    QList<QString> fonts;
//    QString fontpath (":/fonts/");
    QString fontpath (MAINPATH + "fonts/");
#ifdef MOBILE_TEST
    fontpath = "fonts/";
#endif
    fonts.append(fontpath + "DejaVuSans.ttf");
    fonts.append(fontpath + "DejaVuSerif.ttf");
//    fonts.append(fontpath + "DroidSansHebrew.ttf");
//     fonts.append(fontpath + "DroidSansFallbackFull.ttf");
//     fonts.append(fontpath + "DroidSansHebrew-Regular.ttf");
//     fonts.append(fontpath + "DroidSansHebrew-Bold.ttf");
    fonts.append(fontpath + "DroidSansHebrewOrayta.ttf");
//    fonts.append(fontpath + "DavidCLM-Medium.ttf");
    fonts.append(fontpath + "MiriamCLM-Book.ttf");
    fonts.append(fontpath + "SILEOTSR.ttf");

    //add all fonts to the application
    foreach(const QString &font, fonts){
        addFont(font);
    }
}


void initLang(QApplication *app)
{
    //Read lang conf
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    bool systemlang = false;
#ifndef MOBILE
    //system language disabled in mobile because it doesn't work.
    systemlang = settings.value("systemLang",true).toBool();
#endif

    if (systemlang) LANG = QLocale::languageToString(QLocale::system().language());
    else (LANG = settings.value("lang","Hebrew").toString());

    settings.endGroup();


    extern QTranslator *translator;
    translator = new QTranslator();

    //Update path for translator installation
    QString transPath (MAINPATH);
    if (!translator->load(LANG + ".qm", ".")) translator->load(LANG + ".qm", transPath);
    //Install as the app's translator
    app->installTranslator(translator);
}

int main(int argc, char *argv[])
{
    //Start the App
    QApplication app(argc, argv);

    //Deal with command line options
    //TODO: add more if it's needed
    QStringList args = app.arguments();
    if (args.contains("-v") || args.contains("--version"))
    {
        cout << "Orayta, Hebrew books program, ";
        cout << "Version " << VERSION << endl;

        exit(0);
    }

    if (args.contains("-i") || args.contains("--import"))
    {

    }


    //Define location for program dirs
    initPaths();


    //IZAR: add fonts to our app.
    initFonts();


    //Define the program's language
    initLang(&app);

#ifndef MOBILE


    //Show splash screen:
    QPixmap pixmap(":/Images/Orayta.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    app.processEvents();

    QApplication::restoreOverrideCursor();


    DesktopApp d;
    d.show();
    app.processEvents();

    //Hide slpash screen
    splash->finish(&d);
#endif

#ifdef MOBILE
    // fix for certain devices which don't support hebrew chars well.
    app.setFont(QFont("DejaVu Sans"));

    MobileApp m;

    m.show();

    app.processEvents();
#endif

    return app.exec();
}
