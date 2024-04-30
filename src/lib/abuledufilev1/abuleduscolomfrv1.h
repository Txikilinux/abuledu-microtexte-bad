/** Classe AbulEduSCOLOMFrV1
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_LOM_LOMFR_ScoLOMFR
  * @see http://www.lom-fr.fr/lomfr-unt/unisciel/modeles-de-fiche/LOMFRTemplate-Unisciel-v1.0-complet.xml/view
  *
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
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

#ifndef ABULEDUSCOLOMFRV1_H
#define ABULEDUSCOLOMFRV1_H

#include <QByteArray>
#include "abuledulomfrv1.h"
#include "abuleduloggerv1.h"

class AbulEduSCOLOMFrV1 : public AbulEduLOMFrV1
{
public:
    AbulEduSCOLOMFrV1();
    virtual ~AbulEduSCOLOMFrV1();
    QByteArray abeLOMExportAsXML(); /** Exporte comme structure XML */

    // ------------------- branche 1
    /**
      * 1.10: Typologie générale de documents
      * @attention cardinalité n
      * @attention utiliser le vocabulaire adapté : annuaire; article; atlas; banque de vidéos; banque d'images; base de données; bibliographie/sitographie; carte; carte heuristique et conceptuelle; chronologie; collection de documents; collection de données; compte rendu; conférence; diaporama; dossier technique; feuille de calcul; image numérique; livre numérique; maquette/prototype; norme; jeu de données; note; objet; objet 3D; ouvrage; partition; photographie; podcast; présentation multimédia; programme scolaire; rapport; référentiel de compétences; schéma/graphique; site web; tableau; web média; bande dessinée; biographie; dossier documentaire; exposition; film; objet physique; partition musicale; périodique; tableau (art)
      * @param value du type de document (dans un vocabulaire)
      * @see http://www.lom-fr.fr/scolomfr/outils/xsd/scolomfrv11/scolomfr/scolomfrvocabvaluesxsd.html
      * @note Exemple de code XML qui sera généré:
      * @code
<scolomfr:generalResourceType>
  <scolomfr:source>ScoLOMFRv1.1</scolomfr:source>
  <scolomfr:value>collection de documents</scolomfr:value>
</scolomfr:generalResourceType>
<scolomfr:generalResourceType>
  <scolomfr:source>ScoLOMFRv1.1</scolomfr:source>
  <scolomfr:value>programme scolaire</scolomfr:value>
</scolomfr:generalResourceType>
      * @endcode
      */
    bool                  abeLOMaddGeneralDocumentTypology(QString value);
    QStringList           abeLOMgetGeneralDocumentTypology();
    QMap<QString,QString> abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR();
    void                  abeLOMclearGeneralDocumentTypology();

    // ------------------- branche 2 : Cycle de vie
    /**
      * 2.3.1: Rôle (contribution)
      * @attention extention SCOLOMFR du vocabulaire adapté : contributeur
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
//    QString     abeLOMgetLifeCycleContributionRole();
//    QMap<QString,QString> abeLOMgetLifeCycleContributionRoleVocabLOM();

    // ------------------- branche 5
    /**
      * 5.2: Type de ressource pédagogique dans l'idée de Nature pédagogique de la ressouce: learningResourceType
      */
    bool                  abeLOMaddEducationalLearningResourceType(QString value);
    QMap<QString,QString> abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR();
    void                  abeLOMclearEducationalLearningResourceType();

    /**
      * 5.12: Activités induites: activity
      */
    bool                  abeLOMaddEducationalActivities(QString value);
    QMap<QString,QString> abeLOMgetEducationalActivitiesVocabSCOLOMFR();

    /**
      * 5.14: Lieu: place
      * @attention cardinalité n
      * @attention utiliser le vocabulaire adapté SCOLOMFR: en amphithéâtre, en atelier, en atelier de pédagogie personnalisée, en CDI, en salle de classe, en établissement, espace dédié à une pratique spécifique, en établissement socioculturel, en bibliothèque médiathèque, en musée, hors établissement, en installation de loisirs, en installation sportive, en laboratoire, en laboratoire de langues, en milieu familial, en milieu professionnel, en entreprise, non précisé, en salle informatique, en salle multimédia, en mobilité
      * @param value du type de document (dans un vocabulaire)
      * @see http://www.lom-fr.fr/scolomfr/outils/xsd/scolomfrv11/scolomfr/scolomfrvocabvaluesxsd.html
      * @note Exemple de code XML qui sera généré:
      * @code
<scolomfr:place>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en milieu familial</scolomfr:value>
</scolomfr:place>
<scolomfr:place>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en CDI</scolomfr:value>
</scolomfr:place>
      * @endcode
      */
    bool                  abeLOMaddEducationalLocation(QString value);
    QStringList           abeLOMgetEducationalLocation();
    QMap<QString,QString> abeLOMgetEducationalLocationVocabSCOLOMFR();
    void                  abeLOMclearEducationalLocation();


    /**
      * 5.15: Modalités pédagogiques: EducationalMethod
      * @attention cardinalité n
      * @attention utiliser le vocabulaire adapté SCOLOMFR: PAC, partenariat électronique interécoles, projets interétablissement, liaison primaire-collège, liaison collège-lycée, séjour pédagogique, sortie culturelle, spectacle, sortie pédagogique, TIPE, TPE, travail de recherche, travail en interdisciplinarité, IDD, PPCP, travaux dirigés, travaux pratiques, à distance, aide didactique, aide didactique individualisée, échange pédagogique de classes, en alternance, en autonomie, en classe entière, en collaboration, en entreprise, PFMP (CAP), PFE (BTS), visite d'entreprise, en groupe, en groupe de compétences, en ligne, étude dirigée, modalité mixte, en milieu professionel, en tutorat
      * @param value du type de document (dans un vocabulaire)
      * @see http://www.lom-fr.fr/scolomfr/outils/xsd/scolomfrv11/scolomfr/scolomfrvocabvaluesxsd.html
      * @note Exemple de code XML qui sera généré:
      * @code
<scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en autonomie</scolomfr:value>
</scolomfr:educationalMethod>
<scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en classe entière</scolomfr:value>
</scolomfr:educationalMethod>
<scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>etude dirigée</scolomfr:value>
</scolomfr:educationalMethod>
      * @endcode
      */
    bool                  abeLOMaddEducationalPedagogicModality(QString value);
    QStringList           abeLOMgetEducationalPedagogicModality();
    QMap<QString,QString> abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR();
    void                  abeLOMclearEducationalPedagogicModality();

    /**
      * 5.16: Outils: Tool
      * @attention cardinalité n
      * @attention utiliser le vocabulaire adapté SCOLOMFR: logiciels RIP, logiciel système et de programmation, système d'exploitation, logiciel de développement d'application, navigateurs Web, logiciel d'application, didacticiel, exerciseur, suite bureautique, logiciel d'aide à l'orientation, logiciel d'animation 2D, logiciel d'animation 3D, logiciel de communication et d'échange, logiciel de dessin assisté par ordinateur, logiciel de gestion de contenu, logiciel de gestion de projet, logiciel de messagerie, logiciel de montage vidéo, logiciel de présentation, logiciel de recherche documentaire, logiciel de traitement de son, logiciel de traitement de tableaux et calculs, logiciel de traitement de texte, logiciel de traitement d'enquête, logiciel de traitement d'image fixe, logiciel de traitement vidéo, logiciel de travail collaboratif, logiciel d'édition Web, logiciel d'image de synthèse, logiciel organisateur des idées, logiciel ou application de lecture, logiciel PAO, logiciel d’édition et de gestion d’exercices, logiciel text-to-speech, moteur de recherche, outil blogue, outil de mise en ligne, outil de modélisation, outil d'encodage, SGBD, SIG, matériel, appareil photographique, baladeur, borne informatique, calculatrice, camescope, classe mobile, écran tactile, machine à commande numérique, matériel de lecture livre numérique, ordinateur, outil de microscopie, PDA, rétroprojecteur, tablette informatique, TBI, vidéoprojecteur, outil de compensation, ressource d'information, atlas, base de données, carte heuristique, cédérom, dictionnaire, glossaire, documentation technique, encyclopédie, ENT, podcast
      * @param value du type de document (dans un vocabulaire)
      * @see http://www.lom-fr.fr/scolomfr/outils/xsd/scolomfrv11/scolomfr/scolomfrvocabvaluesxsd.html
      * @note Exemple de code XML qui sera généré:
      * @code
<scolomfr:tool>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>vidéoprojecteur</scolomfr:value>
</scolomfr:tool>
<scolomfr:tool>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>TBI</scolomfr:value>
</scolomfr:tool>
<scolomfr:tool>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>ENT</scolomfr:value>
</scolomfr:tool>
<scolomfr:tool>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>exerciseur</scolomfr:value>
</scolomfr:tool>
      * @endcode
      */
    bool                  abeLOMaddEducationalTools(QString value);
    QStringList           abeLOMgetEducationalTools();
    QMap<QString,QString> abeLOMgetEducationalToolsVocabSCOLOMFR();
    void                  abeLOMclearEducationalTools();

    // ------------------- branche 7 : relations
    /**
      * 7.1: Nature de la relation : kind
      */
    bool        abeLOMsetRelationKind(QString value);
    QMap<QString,QString> abeLOMgetRelationKindVocabSCLOMFR();

    // ------------------- branche 9 : Classification
    /**
      * 9: Classification, cette catégorie décrit où cette ressource se trouve dans un système de classification particulier: classification. Cardinalité n
      * 9.1: Objectif: purpose
      */
    bool abeLOMaddClassificationPurpose(QString purpose,LomLS sourceTaxonPath,QString idTaxon, LomLS entryTaxon);
    QMap<QString,QString> abeLOMgetClassificationPurposeVocabSCOLOMFR();

protected:
    // ------------------- branche 1
    QStringList           m_GeneralDocumentTypology;
    QMap<QString,QString> m_GeneralDocumentTypologyVocabSCOLOMFR;

    // ------------------- branche 2
    QMap<QString,QString> m_LifeCycleContributionRoleVocabSCOLOMFR;

    // ------------------- branche 5
    QMap<QString,QString> m_EducationalLearningResourceTypeVocabSCOLOMFR;
    QStringList           m_EducationalLocation;
    QMap<QString,QString> m_EducationalLocationVocabSCOLOMFR;
    QStringList           m_EducationalPedagogicModality;
    QMap<QString,QString> m_EducationalPedagogicModalityVocabSCOLOMFR;
    QStringList           m_EducationalTools;
    QMap<QString,QString> m_EducationalToolsVocabSCOLOMFR;
    QMap<QString,QString> m_EducationalActivitiesVocabSCOLOMFR;

    // ------------------- branche 7
    QMap<QString,QString> m_RelationKindVocabSCOLOMFR;

    // ------------------- branche 9
    QMap<QString,QString> m_ClassificationPurposeVocabSCOLOMFR;

public:
    /**
      * Exporte l'entête du fichier XML
      *
      * @note Exemple de code XML qui sera généré:
      * @code
<?xml version="1.0" encoding="UTF-8"?>
<lomfr:lom xmlns:lomfr="http://www.lom-fr.fr/xsd/LOMFR"
           xmlns:lom="http://ltsc.ieee.org/xsd/LOM"
           xmlns:scolomfr="http://www.lom-fr.fr/xsd/SCOLOMFR">
      * @endcode
      */
    QByteArray p_abeLOMExportAsXML_header();


    /**
      * Exporte la section 1 Général
      * @see AbulEduLOMV1::p_abeLOMExportAsXML_section1()
      * @see AbulEduLOMFrV1::p_abeLOMExportAsXML_section1()
      *
      * @note Exemple de code XML qui sera généré:
      * @code
      <scolomfr:generalResourceType>
         <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
         <scolomfr:value>collection de données</scolomfr:value>
      </scolomfr:generalResourceType>
      <scolomfr:generalResourceType>
         <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
         <scolomfr:value>banque d'images</scolomfr:value>
      </scolomfr:generalResourceType>
      * @endcode
      */
    QByteArray p_abeLOMExportAsXML_section1();

    QByteArray p_abeLOMExportAsXML_section5();


    //import branche 1.1
    bool p_abeLOMImportXML_section1(QDomElement n);

    bool p_abeLOMImportXML_section5(QDomNode n);

private:
    QString      m_xmlPrefix; /** Pour prefixer tous les objets XML */

};

#endif // ABULEDUSCOLOMFRV1_H
