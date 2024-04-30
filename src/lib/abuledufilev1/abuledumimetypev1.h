/** Classe AbulEduMimeTypeV1
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program. If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef ABULEDUMIMETYPEV1_H
#define ABULEDUMIMETYPEV1_H

#include <QStringList>
#include <QHash>
#include "abuleduloggerv1.h"

class AbulEduMimeTypeV1
{
public:
    AbulEduMimeTypeV1();

    static QHash<QString,QString> initialise();
    /**
      * Cette fonction retourne le mime-type correspondant à l'extension du fichier
      * par exemple get("jpg") retournera image/jpeg
      * @param qstring str : l'extension du fichier
      * @return qstring : le mime-type du fichier
      */
    static QString get(QString str);
    static QStringList getAll();
private:
};

#endif // ABULEDUMIMETYPEV1_H
