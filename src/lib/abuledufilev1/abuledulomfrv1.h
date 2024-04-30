/** Classe AbulEduLOMFrV1
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_LOM_LOMFR_ScoLOMFR
  * @see http://www.lom-fr.fr/lomfr-unt/unisciel/modeles-de-fiche/LOMFRTemplate-Unisciel-v1.0-complet.xml/view
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
#ifndef ABULEDULOMFRV1_H
#define ABULEDULOMFRV1_H

#ifndef DEBUG_ABULEDULOMFRV1
#define DEBUG_ABULEDULOMFRV1 0
#endif

#include <QByteArray>
#include "abuledulomv1.h"
#include "abuleduloggerv1.h"

class AbulEduLOMFrV1 : public AbulEduLOMV1
{
public:
    AbulEduLOMFrV1();
    virtual ~AbulEduLOMFrV1();
    QByteArray abeLOMExportAsXML(); /** Exporte comme structure XML */


    // ------------------- branche 1
    /**
      * 1.9: Type documentaire DC
      * @attention cardinalité n
      * @attention utiliser le vocabulaire adapté LOMFR: collection, ensemble de données, évènement, logiciel, objet physique, ressource interactive, service, son, texte,  image fixe, image en mouvement, image
      * @param value du type de document (dans un vocabulaire)
      * @return bool: true si l'affectation est correcte (la valeur fait partie du vocabulaire), false sinon
      * @see https://suplomfr.supelec.fr/mediawiki/index.php/1.9_Type_documentaire
      * @see Dublin Core  DC.Type
      * @note Exemple de code XML qui sera généré:
      * @code
<lomfr:documentType>
  <lomfr:source>LOMFRv1.0</lomfr:source>
  <lomfr:value>évènement</lomfr:value>
</lomfr:documentType>
      * @endcode
      */
    bool                  abeLOMaddGeneralDocumentType(QString value);
    QStringList           abeLOMgetGeneralDocumentType();
    QMap<QString,QString> abeLOMgetGeneralDocumentTypeVocabLOMFR();

    // ------------------- branche 2 : Cycle de vie
    /**
      * 2.3.1: Rôle (contribution)
      * @attention extention LOMFR du vocabulaire adapté : contributeur
      * @code
<lom:contribute>
  <lom:role>
    <lom:source>LOMFRv1.0</lom:source>
    <lom:value>contributeur</lom:value>
  </lom:role>
  <lom:entity>BEGIN:VCARD
VERSION:3.0
N:NOM;LePrénom;;;
FN:LePrénom NOM
TEL:01 02 03 04 05
LOGO:http://www.abuledu.org
EMAIL;TYPE=INTERNET:adresse@abuledu.org
NOTE;TYPE=SIREN:000 000 000
ORG:AbulÉdu
URL:
UID:login@abuledu.org
END:VCARD</lom:entity>
  <lom:date>
  <lom:dateTime>2012-04-17</lom:dateTime>
  </lom:date>
</lom:contribute>
      * @endcode
      */
    QMap<QString,QString> abeLOMgetLifeCycleContributionRoleVocabLOMFR();

    /**
      * Retourne les dates des contributions
      * @param role le role en question (attention doit être dans la liste du vocabulaire accepté)
      */
    QList<QDate> abeLOMgetLifeCycleContributionDates(QString role);

    /**
      * Ajoute une nouvelle contribution
      */
    bool          abeLOMaddLifeCycleContributionRole(QString role, vCard entity, QDate ladate);
    void          abeLOMclearLifeCycleContributionRole();

    // ------------------- branche 5
    /**
      * 5.2: Type de ressource pédagogique dans l'idée de Nature pédagogique de la ressouce: learningResourceType
      */
    bool abeLOMaddEducationalLearningResourceType(QString value);
    QMap<QString,QString> abeLOMgetEducationalLearningResourceTypeVocabLOMFR();

    /**
      * 5.6: Contexte et niveau du public cible: context
      */
    bool                  abeLOMaddEducationalContext(QString value);
    QMap<QString,QString> abeLOMgetEducationalContextVocabLOMFR();
    void                  abeLOMclearEducationalContext();

    /**
      * 5.12: Activités induites: activity
      * @attention cardinalité n
      * @attention vocabulaire LOMFR: animer, apprendre, collaborer, communiquer, coopérer, créer, échanger, lire, observer, organiser, produire, publier, rechercher, s'auto-former, s'exercer, s'informer, se former, simuler, s'évaluer
      * @attention vocabulaire SCOLOMFR: expérimenter
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lomfr:activity>
   <lomfr:source>LOMFRv1.0</lomfr:source>
   <lomfr:value>s'exercer</lomfr:value>
</lomfr:activity>
<lomfr:activity>
   <lomfr:source>LOMFRv1.0</lomfr:source>
   <lomfr:value>simuler</lomfr:value>
</lomfr:activity>
<lomfr:activity>
   <lomfr:source>LOMFRv1.0</lomfr:source>
   <lomfr:value>s'évaluer</lomfr:value>
</lomfr:activity>
      * @endcode
      */
    virtual bool          abeLOMaddEducationalActivities(QString value);
    QStringList           abeLOMgetEducationalActivities();
    QMap<QString,QString> abeLOMgetEducationalActivitiesVocabLOMFR();
    void                  abeLOMclearEducationalActivities();

    /**
      * 5.13: Validation des acquis, Quantité d'unités d'enseignement ou de crédits obtenues après avoir atteint les objectifs pédagogiques: credit
      * @attention cardinalité n
      * @param value value valeur de type chaine (localisée) de caractères
      * @note Exemple de code XML qui sera généré:
      * @code
<lomfr:credit>du texte</lomfr:credit>
      * @endcode
      */
        bool           abeLOMaddEducationalValidation(QString lang, QString value);
        void           abeLOMclearEducationalValidations();
        QStringList    abeLOMgetEducationalValidation(QString lang);
        QList<QString> abeLOMgetEducationalValidationKeys();

        // ------------------- branche 7
        /**
          * 7.1: Nature de la relation : kind
          */
        bool                  abeLOMsetRelationKind(QString value);
        QMap<QString,QString> abeLOMgetRelationKindVocabLOMFR();

        // ------------------- branche 9 : Classification
        /**
          * 9: Classification, ctte catégorie décrit où cette ressource se trouve dans un système de classification particulier: classification. Cardinalité n
          * 9.1: Objectif: purpose
          */
//        virtual void abeLOMaddClassificationPurpose(QString purpose,QString source,QString id, QString entry);
        QMap<QString,QString> abeLOMgetClassificationPurposeVocabLOMFR();


protected:
    // ------------------- branche 1
    QStringList           m_GeneralDocumentType;
    QMap<QString,QString> m_GeneralDocumentTypeVocabLOMFR;

    // ------------------- branche 2
    QMap<QString,QString> m_LifeCycleContributionRoleVocabLOMFR;


    // ------------------- branche 5
    QStringList           m_EducationalActivities;
    QMap<QString,QString> m_EducationalActivitiesVocabLOMFR;
    LomLS                 m_EducationalValidation;
    QMap<QString,QString> m_EducationalLearningResourceTypeVocabLOMFR;
    QMap<QString,QString> m_EducationalContextVocabLOMFR;

    // ------------------- branche 7
    QMap<QString,QString> m_RelationKindVocabLOMFR;

    // ------------------- branche 9
    QMap<QString,QString> m_ClassificationPurposeVocabLOMFR;

    /**
      * Exporte l'entête du fichier XML
      *
      * @note Exemple de code XML qui sera généré:
      * @code
<?xml version="1.0" encoding="UTF-8"?>

<lom xmlns="http://ltsc.ieee.org/xsd/LOM" xmlns:lomfr="http://www.lom-fr.fr/xsd/LOMFR">
      * @endcode
      */
    QByteArray p_abeLOMExportAsXML_header();

    /**
      * Exporte la section 1 Général
      * @see AbulEduLOMV1::p_abeLOMExportAsXML_section1()
      * @note Exemple de code XML qui sera généré:
      * @code
      <lomfr:documentType>
         <lomfr:source>LOMFRv1.0</lomfr:source>
         <lomfr:value>ensemble de données</lomfr:value>
      </lomfr:documentType>
      <lomfr:genTypologie>
         <lomfr:source>LOMFRv1.0</lomfr:source>
         <lomfr:value/>
      </lomfr:genTypologie>
      <lomfr:documentType>
         <lomfr:source>LOMFRv1.0</lomfr:source>
         <lomfr:value>image</lomfr:value>
      </lomfr:documentType>
      <lomfr:genTypologie>
         <lomfr:source>LOMFRv1.0</lomfr:source>
         <lomfr:value/>
      </lomfr:genTypologie>
      * @endcode
      */
    QByteArray p_abeLOMExportAsXML_section1();

    QByteArray p_abeLOMExportAsXML_section5();

    /*
    <lom:contribute>
        <lom:role>
            <lom:source>LOMFRv1.0</lom:source>
            <lom:value>contributeur</lom:value>
        </lom:role>
        <lom:entity>BEGIN:VCARD
VERSION:3.0
N:Nom; Prenom;;;
FN:Nom Prenom
ORG:Nom de l'organisme du "contributeur";;
EMAIL;INTERNET:Nom@NomOrganisme.fr
END:VCARD
        </lom:entity>
        <lom:date>
            <lom:dateTime>2012-04-17</lom:dateTime>
            <lom:description>
                <lom:string language="fre">commentaire</lom:string>
            </lom:description>
        </lom:date>
    </lom:contribute>
    */

    //import branche 1.1
    bool p_abeLOMImportXML_section1(QDomElement n);

    //import branche 5.12
    bool p_abeLOMImportXML_section5(QDomNode n);

private:
    QString      m_xmlPrefix; /** Pour prefixer tous les objets XML */
};

#endif // ABULEDULOMFRV1_H
