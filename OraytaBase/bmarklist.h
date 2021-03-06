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
* Author: Izar <izar00@gmail.com>
*/

/**
  written by abe.izar <izar00@gmail.com>
  this class holds a list of the most recent locations the user has opened.
  these locations are set as 'mini book marks' that actually hold a book and bookIter.
  */

#ifndef BMARKLIST_H
#define BMARKLIST_H

#include "../OraytaBase/booklist.h"
#include "../OraytaBase/functions.h"
#include <QSettings>
#include <QDate>
#include <QObject>
#include <QListWidget>
#include <QWidget>

class MiniBMark;

class BMarkList : public QListWidget
{

    Q_OBJECT

public:
    explicit BMarkList(QWidget *parent = 0);
    ~BMarkList();
    void saveSettings();
    MiniBMark* addBookMark(Book* book, BookIter iter, int viewPosition =0);

signals:
    void shortPress(QListWidgetItem * item);
    void longPress(QListWidgetItem * item);

public slots:
    void loadHistory(BookList booklist);

    // add daf yomi to this list
    void addDafYomi(BookList booklist);

    void addHalachaYomit(BookList booklist);
    void addMishnaYomit(BookList booklist);

    void eraseBookMark(MiniBMark *bm);

private:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *e);

    // max number of bookmarks to remember
    // NOTE: currently this is a const, but later we may want to let the user set this manually
   static const int limit = 30;

   //Holds the time of the last click, so we can calculate the delta
   QTime clickT;

   // true if chagend since last save
   bool changed;

   // does the user want to show daf yomi in this list
//   bool dafYomiActive;
//   bool parshaYomiActive;
//   bool mishnaYomiActive;
//   //etc...

//   bool isDafYomiActive();
//   bool isParshaYomiActive();
//   bool isMishnaYomiActive();

//   void setDafYomiActive(bool);
//   void setParshaYomiActive(bool);
//   void setMishnaYomiActive(bool);




};

#endif // BMARKLIST_H
