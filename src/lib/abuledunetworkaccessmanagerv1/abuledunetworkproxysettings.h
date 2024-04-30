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

#ifndef ABULEDUNETWORKPROXYSETTINGS_H
#define ABULEDUNETWORKPROXYSETTINGS_H

#include <QWidget>
#include "abuleduapplicationv1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "ui_abuledunetworkproxysettings.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduNetworkProxySettings;
}

class AbulEduNetworkProxySettings : public QWidget
{
    Q_OBJECT

public:
    explicit AbulEduNetworkProxySettings(QWidget *parent = 0);
    virtual ~AbulEduNetworkProxySettings();

public slots:
    void slotRefresh();

private slots:
    void on_btnAnnuler_clicked();
    void on_btnOK_clicked();
    void on_rbProxyDesactive_toggled(bool checked);
    void on_rbProxyAutomatique_toggled(bool checked);
    void on_rbProxyManuel_toggled(bool checked);

    void on_rbProxyAutoURI_toggled(bool checked);

    void on_rbProxySysteme_toggled(bool checked);

private:
    Ui::AbulEduNetworkProxySettings *ui;
    AbulEduNetworkAccessManagerV1 *m_nam;

signals:
    void signalBtnOk();
    void signalBtnCancel();
};

#endif // ABULEDUNETWORKPROXYSETTINGS_H
