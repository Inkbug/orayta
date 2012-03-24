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

#ifndef SEARCH_H
#define SEARCH_H

#include <QProgressBar>

extern bool stopSearchFlag;

//Searches for the given phrase in all books that have SearchMe set to true.
// Returns the html contents of the results
QString SearchInBooks (const QRegExp& regexp, QString disp, vector<Book*> searchList, QProgressBar *pbar);

QString createSearchPattern (QString userInput, bool allWords = true, bool fullWords = false, int spacing = 0);

#endif // SEARCH_H