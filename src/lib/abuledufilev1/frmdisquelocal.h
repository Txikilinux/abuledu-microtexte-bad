/**
  * Classe
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef FRMDISQUELOCAL_H
#define FRMDISQUELOCAL_H

#include <QWidget>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDir>
#include <QFileInfo>
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"
#include "ui_frmdisquelocal.h"

namespace Ui {
class frmDisqueLocal;
}

class frmDisqueLocal : public QWidget
{
    Q_OBJECT

public:
    explicit frmDisqueLocal(QWidget *parent = 0);
    ~frmDisqueLocal();
    void setListeFichiers(QStringList liste);

private slots:
    void on_btnRecherche_clicked();

private:
    Ui::frmDisqueLocal *ui;
    QStringList m_listeFichiers;
};

#endif // FRMDISQUELOCAL_H
