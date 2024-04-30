/** Classe AbulEduFileAboutV1 : propriétés d'un fichier .abe
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  */
 /*
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

#ifndef ABULEDUFILEABOUTV1_H
#define ABULEDUFILEABOUTV1_H

#include <QWidget>
#include "abuledufilev1.h"
#include "abuleduwidgetlomv1.h"
#include "abuleduwidgetlomsuitev1.h"
#include "abuleduwidgetlomfrv1.h"
#include "abuleduwidgetscolomfrv1.h"
#include "abuleduwidgetcontribution.h"
#include "ui_abuledufileaboutv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduFileAboutV1;
}

class AbulEduFileAboutV1 : public QWidget
{
    Q_OBJECT

public:
    /** Cette classe permet d'avoir une "petite" popup "à propos de ce fichier .abe" qui listera donc
      * les champs lom, le titre, le résumé etc.
      */
    explicit AbulEduFileAboutV1(QWidget *parent = 0, const QString& fichierAbe="");
    ~AbulEduFileAboutV1();

public slots:
    bool abeSetFileName(const QString& fichierAbe="");

private :
    void p_afficheProprietes();

private:
    Ui::AbulEduFileAboutV1 *ui;
    QSharedPointer<AbulEduFileV1> m_abuleduFile;
};

#endif // ABULEDUFILEABOUTV1_H
