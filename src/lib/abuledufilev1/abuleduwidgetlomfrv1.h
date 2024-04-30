/** Classe AbulEduWidgetLOMFrV1
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

#ifndef ABULEDUWIDGETLOMFRV1_H
#define ABULEDUWIDGETLOMFRV1_H

#include <QWidget>
#include <QPointer>
#include "abuleduscolomfrv1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "abuledussologinv1.h"
#include "ui_abuleduwidgetlomfrv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
    class AbulEduWidgetLOMFrV1;
}

class AbulEduWidgetLOMFrV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbulEduWidgetLOMFrV1(QWidget *parent = 0);
    ~AbulEduWidgetLOMFrV1();

    /** Donne comme valeur à la variable m_abeLomFrV1 la valeur passée en paramètre
      * @param le paramètre destiné à être passé ici est la AbulEduSCOLOMFrV1 de la mainwindow
      * @note je ne vais avoir qu'un seul objet de type AbulEduSCOLOMFrV1, les méthodes et attributs de AbulEduLOMFrV1 et AbulEduLOMV1 sont disponibles par héritage
      */
    void abeWLOMsetABELOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1);

    /** Enregistre les éléments présents dans l'interface (Catalogue, Entrée, Titre) */
      void saveOnExit();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AbulEduWidgetLOMFrV1 *ui;

    /** Copie du pointeur de la MainWindow vers l'attribut de type AbulEduSCOLOMFrV1 de son AbulEduFileV1 */
    QSharedPointer<AbulEduSCOLOMFrV1> m_abeLomFrV1;
    QString m_previousTitleLanguage;

private slots:
    /** Écrit dans le QLineEdit Titre le titre de la ressource dans la langue lang, s'il existe... */
    void abeWLOMchangeTitle(int indexLang);

    /** Définit les éléments de l'interface qui sont visibles ou pas selon que l'utilisateur est identifié auprès du portail captif
      @param le booléen passé en paramètre sera pour l'instant la property "isSSOAuthenticated" de l'AbulEduApplicationV1 */
    void abeWLOMFrDisplayAuthenticated(int returnAuthenticated);

    void on_cbLOMFR1_2Lang_currentIndexChanged(const QString &arg1);
    void on_btnAuthenticate_clicked();
};

#endif // ABULEDUWIDGETLOMFRV1_H
