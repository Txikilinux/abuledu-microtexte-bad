/** Classe AbulEduWidgetSCOLOMFrV1
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

#ifndef ABULEDUWIDGETSCOLOMFRV1_H
#define ABULEDUWIDGETSCOLOMFRV1_H

#include <QWidget>
#include <QPointer>
#include "abuleduscolomfrv1.h"
#include "vcard.h"
#include "abuledussologinv1.h"
#include "ui_abuleduwidgetscolomfrv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
    class AbulEduWidgetSCOLOMFrV1;
}

class AbulEduWidgetSCOLOMFrV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbulEduWidgetSCOLOMFrV1(QWidget *parent = 0);
    ~AbulEduWidgetSCOLOMFrV1();

    /** Donne comme valeur à la variable m_abeScoLomFrV1 la valeur passée en paramètre
      * @param le paramètre destiné à être passé ici est la AbulEduSCOLOMFrV1 de la mainwindow
      * @note je ne vais avoir qu'un seul objet de type AbulEduSCOLOMFrV1, les méthodes et attributs de AbulEduLOMFrV1 et AbulEduLOMV1 sont disponibles par héritage
      */
    void abeWLOMsetABESCOLOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1);

    /** Enregistre en quittant le widget les valeurs affichées de langue, description, contribution et localisation */
    void saveOnExit();

protected:
    void changeEvent(QEvent *e);

private slots:
    /** Ajoute un QLineEdit destiné à recevoir une autre langue */
    void on_pushBbtnPlusSCOLOMFR1_3utton_clicked();

    /** Ajoute un QTextEdit destiné à recevoir une autre description */
    void on_btnPlusSCOLOMFR1_4_clicked();

    /** Ajoute un QLineEdit destiné à recevoir une autre localisation */
    void on_btnPlusScoLOMFR4_3_clicked();

    /** Renseigne la comboBox Langue, et ajoute une autre comboBox Langue par langue supplémentaire trouvée
      * @param lang est le nom de la langue éventuellement passée à la fonction
      * @note la fonction est appelée sans paramètre dans le cas d'une création ou d'un ajout, avec paramètre pour un affichage d'informations existantes
      */
    void addCBLang(QString lang = QString());

    void addDescription(QString lang = QString(), QString description = QString());

    void addContribution(QString role = QString(), QList<vCard> contributeurs = QList<vCard>(), QDate date = QDate::currentDate());

    /** Ajoute une QLineEdit location
      * @param location est la localisation éventuellement passée à la fonction
      * @note la fonction est appelée sans paramètre dans le cas d'une création ou d'un ajout, avec paramètre pour un affichage d'informations existantes
      */
    void addLELocation(QString location = QString());

    /** Provoque la lecture dans le fichier XML de la description associée à une langue
      @param lang est la QString envoyée par le changement de valeur dans la comboBox Langues */
    void abeWLOMchangeDescription(int indexLang);

    /** Définit les éléments de l'interface qui sont visibles ou pas selon que l'utilisateur est identifié auprès du portail captif
      @param le booléen passé en paramètre sera pour l'instant la property "isSSOAuthenticated" de l'AbulEduApplicationV1 */
    void abeWScoLOMDisplayAuthenticated(int returnAuthenticated);

    void on_cbEntityChanged(QString arg);

    void on_btnAuthenticate_clicked();

    void on_btnPlus2_3_clicked();

private:
    Ui::AbulEduWidgetSCOLOMFrV1 *ui;

    /** Copie du pointeur de la MainWindow vers l'attribut de type AbulEduSCOLOMFrV1 de son AbulEduFileV1 */
    QSharedPointer<AbulEduSCOLOMFrV1> m_abeScoLomFrV1;

    /** variable vCard membre, stocke les informations sur l'utilisateur
      * @deprecated Elle n'est même pas instanciée dans le code */
    vCard* m_currentEntity;

    QString m_previousRole;
    QString m_previousDescription;
    QDate m_previousDate;
    QString m_previousLangDescription;
    QString m_currentLocation;
    int m_numberLangs;
    int m_numberWContrib;
    int m_numberDescription;
    int m_numberLocation;
    bool m_isCbEntityChanged;

    /** Liste des objets créés à volée pour stocker les informations quand les objets de l'interface graphique sont en nombre insuffisant
      * Chaque QWidget* est stocké avec un descriptif qui permet de les détruire par catégorie */
    QList<QPair<QString,QWidget*> > m_addedWidgets;
};

#endif // ABULEDUWIDGETSCOLOMFRV1_H
