/** Classe AbulEduWidgetLOMV1
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_LOM_LOMFR_ScoLOMFR
  * @see http://www.lom-fr.fr/lomfr-unt/unisciel/modeles-de-fiche/LOMFRTemplate-Unisciel-v1.0-complet.xml/view
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#ifndef ABULEDUWIDGETLOMV1_H
#define ABULEDUWIDGETLOMV1_H

#include <QWidget>
#include "abuleduscolomfrv1.h"
#include "abuledussologinv1.h"
#include "abuleduapplicationv1.h"
#include "ui_abuleduwidgetlomv1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduWidgetLOMV1;
}

class AbulEduWidgetLOMV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbulEduWidgetLOMV1(QWidget *parent = 0);
    ~AbulEduWidgetLOMV1();

    /** Donne comme valeur à la variable m_abeLomFrV1 la valeur passée en paramètre
      * @param le paramètre destiné à être passé ici est la AbulEduSCOLOMFrV1 de la mainwindow
      * @note je ne vais avoir qu'un seul objet de type AbulEduSCOLOMFrV1, les méthodes et attributs de AbulEduLOMFrV1 et AbulEduLOMV1 sont disponibles par héritage
      */
    void abeWLOMsetABELOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1);

    /** Enregistre en quittant le widget les valeurs affichées */
    void saveOnExit();

private slots:
    void on_btnAuthenticate_clicked();

    /** Définit les éléments de l'interface qui sont visibles ou pas selon que l'utilisateur est identifié auprès du portail captif
      @param le booléen passé en paramètre sera pour l'instant la property "isSSOAuthenticated" de l'AbulEduApplicationV1 */
    void abeWLOMDisplayAuthenticated(int returnAuthenticated);

    void addFreeKeyword(QString lang = QString(), QString keyword = QString());

    void addCBGeneralDocumentTypology(QString type = QString());

    void addLEtechnicalFormat(QString format = QString());

    void on_btnPlusLOM1_10_clicked();

    void on_btnPlusLOM1_5_clicked();

    void on_btnPlusLOM4_1_clicked();

private:
    Ui::AbulEduWidgetLOMV1 *ui;

    /** Copie du pointeur de la MainWindow vers l'attribut de type AbulEduSCOLOMFrV1 de son AbulEduFileV1 */
    QSharedPointer<AbulEduSCOLOMFrV1> m_abeLomFrV1;

    int m_numberFreeKeyword;

    int m_numberGeneralRessourceType;

    int m_numberTechnicalFormat;

    /** Liste des objets créés à volée pour stocker les informations quand les objets de l'interface graphique sont en nombre insuffisant
      * Chaque QWidget* est stocké avec un descriptif qui permet de les détruire par catégorie */
    QList<QPair<QString,QWidget*> > m_addedWidgets;
};

#endif // ABULEDUWIDGETLOMV1_H
