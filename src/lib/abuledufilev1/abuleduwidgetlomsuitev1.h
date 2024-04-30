/** Classe AbulEduWidgetLOMSuiteV1
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

#ifndef ABULEDUWIDGETLOMSUITEV1_H
#define ABULEDUWIDGETLOMSUITEV1_H

#include <QWidget>
#include "abuleduscolomfrv1.h"
#include "abuledussologinv1.h"
#include "abuleduwidgettaxon.h"
#include "abuleduapplicationv1.h"
#include "ui_abuleduwidgetlomsuitev1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduWidgetLOMSuiteV1;
}

class AbulEduWidgetLOMSuiteV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbulEduWidgetLOMSuiteV1(QWidget *parent = 0);
    ~AbulEduWidgetLOMSuiteV1();

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
    void abeWLOMSuiteDisplayAuthenticated(int returnAuthenticated);

    void on_btnPlusLOM9_2_clicked();

    void addGBtaxon(QString taxon = QString());

    void on_btnPlusLOM9_4_clicked();

    void abeWLOMChangeLang5_7(int index);

    void abeWLOMChangeLang5_10(int index);

    void abeWLOMChangeLang5_13(int index);

    void abeWLOMChangeLang6_3(int index);

    void addCBLearningRessourceType(QString type = QString());

    void addCBEducationalContext(QString context = QString());

    void addEducationalTypicalAgeRange(QString lang = QString(), QString range = QString());

    void addEducationalDescription(QString lang = QString(), QString description = QString());

    void addCBEducationalActivities(QString activity = QString());

    void addEducationalValidation(QString lang = QString(), QString validation = QString());

    void addCBEducationalLocation(QString location = QString());

    void addCBEducationalPedagogicModality(QString modality = QString());

    void addCBEducationalTool(QString tool = QString());

    void on_btnPlusLOM5_2_clicked();

    void on_btnPlusLOM5_6_clicked();

    void on_btnPlusLOM5_12_clicked();

    void on_btnPlusLOM5_14_clicked();

    void on_btnPlusLOM5_15_clicked();

    void on_btnPlusLOM5_16_clicked();

    void on_sbMinLOM5_7_valueChanged(int arg1);

    void on_sbMaxLOM5_7_valueChanged(int arg1);

    void on_teLOM5_10_textChanged();

    void on_leLOM5_13_textChanged(const QString &arg1);

    void on_btnPlusLOM5_10_clicked();

    void on_btnPlusLOM5_7_clicked();

    void on_btnPlusLOM5_13_clicked();

private:
    Ui::AbulEduWidgetLOMSuiteV1 *ui;

    /** Copie du pointeur de la MainWindow vers l'attribut de type AbulEduSCOLOMFrV1 de son AbulEduFileV1 */
    QSharedPointer<AbulEduSCOLOMFrV1> m_abeLomFrV1;

    int m_numberCheminTaxon;

    int m_numberLearningRessourceType;

    int m_numberEducationalContext;

    int m_numberEducationalTypicalAgeRange;

    int m_numberEducationalDescriptions;

    int m_numberEducationalActivities;

    int m_numberEducationalValidations;

    int m_numberEducationalLocations;

    int m_numberEducationalPedagogicModalities;

    int m_numberEducationalTools;

    QString m_previousEducationalTypicalAgeRange;
    QString m_previousEducationalTypicalAgeRangeLang;

    QString m_previousEducationalDescription;
    QString m_previousEducationalDescriptionLang;

    QString m_previousEducationalValidation;
    QString m_previousEducationalValidationLang;

    int m_numberKeyWord;

    /** Liste des objets créés à volée pour stocker les informations quand les objets de l'interface graphique sont en nombre insuffisant
      * Chaque QWidget* est stocké avec un descriptif qui permet de les détruire par catégorie */
    QList<QPair<QString,QWidget*> > m_addedWidgets;

    /** Cache des éléments correspondant à des méta-données non demandées à ce jour */
    void hideUnused(bool yesNo);
};

#endif // ABULEDUWIDGETLOMSUITEV1_H
