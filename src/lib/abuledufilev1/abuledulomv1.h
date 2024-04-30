/** Classe AbulEduLOMV1
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


#ifndef ABULEDULOMV1_H
#define ABULEDULOMV1_H

#include <QString>
#include <QMap>
#include <QDomElement>
#include <QDate>
#include <QDateTime>
#include <QFileInfo>
#include <QObject>
#include "vcard.h"
#include "abuleduloggerv1.h"

/** LOM LS est une paire langue / valeur */
class LomLS
{

public:
    LomLS() {}
    LomLS(QString lang, QString value) {lomLSValues.insert(lang,QStringList(value));}
    void set(QString lang, QString value) {lomLSValues.insert(lang,QStringList(value));}
    void set(QString lang, QStringList value) {lomLSValues.insert(lang,value);}
    void add(QString lang, QString value) {
        QStringList t = lomLSValues.take(lang);
        if(!t.contains(value)) {
            t << value;
        }
        lomLSValues.insert(lang,t);
    }
    QHash<QString, QStringList> toQHash();

    void clear() {lomLSValues.clear();}
    QStringList getList(QString lang) { if(lomLSValues.value(lang).size() > 0) return lomLSValues.value(lang); else return QStringList("");}
    QString get(QString lang) {if (lomLSValues.size() > 0 && lomLSValues.value(lang).size() > 0) return lomLSValues.value(lang).at(0); else return "";}
    inline QString toQString()
    {
        QString str;
        QMapIterator<QString, QStringList> iter(lomLSValues);
        while(iter.hasNext()) {
            iter.next();
            if (iter.hasNext())
                str.append(iter.key() + ";" + iter.value().join(";") + "\n");
            else
                str.append(iter.key() + ";" + iter.value().join(";"));
        }
//        str.remove(str.lastIndexOf("\n"),2);
//        qDebug() << " *******  toQString ******** " << str;
        return str;
    }
    inline void fromQString(QString str)
    {
//        qDebug() << " ******** fromQString ******* " << str;
        QStringList lignes = str.split("\n",QString::SkipEmptyParts);
        for(int i = 0; i < lignes.size(); i++) {
            QStringList ligne = lignes.at(i).split(";");
            lomLSValues.insert(ligne.takeFirst(),ligne);
        }
//        qDebug() << " ******** fromQString ******* " << lomLSValues;
    }

    QList<QString> keys() {return lomLSValues.keys();}
    bool operator== (const LomLS& param) const;

    inline uint qHash(const LomLS &key)
    {
        const QString k = key.lomLSValues.keys().first();
        const QString v = key.lomLSValues.value(k).first();

        int valeurKey;
        for(int i = 0; i < k.size(); i++) {
            valeurKey += k.at(i).toLatin1();
        }

        int valeurValeur;
        for(int i = 0; i < v.size(); i++) {
            valeurValeur += v.at(i).toLatin1();
        }

//        qDebug() << " *************** " << valeurKey << " et " << valeurValeur << " XOR : " << (valeurKey ^ valeurValeur);
        return valeurKey ^ valeurValeur;
    }

private:
    QMap<QString, QStringList> lomLSValues;
};

/**
  * Classe AbulEduLOMV1
  */
class AbulEduLOMV1: public QObject
{

    Q_OBJECT

public:
    AbulEduLOMV1();
    virtual ~AbulEduLOMV1();

    /**
      * Exporte la structure XML
      * @note fonction publique, les autres sont protected pour l'implémentation
      */
    virtual QByteArray abeLOMExportAsXML();
    /** Ouvre le fichier fileName et affecte son contenu a la m_domDocument */
    bool abeLOMsetFile(QString fileName);
    /** affecte directement du XML a la m_domDocument */
    bool abeLOMsetXML(QByteArray xml);

    /** Retourne une liste de vcard : toutes les vcard rencontrées dans le fichier XML */
    QList <vCard> abeLOMgetAllVcards();

    /** Retourne la vCard correspondant au nom fullName */
    vCard abeLOMgetVCardFromFullName(QString fullName);

    /** Passe la classe en mode debug ou pas */
    void abeLOMsetLocalDebug(int level);

    // =============================================================================== Code automatique
    // généré par le script autoSetGet.php

    /** Permet de faire la correspondance entre les langues et la traduction de leur nom
      * @return retourne une QMap dont la clef est le code d'une langue selon la norme ISO 639-2 et la valeur la traduction de son nom
      * @see http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
      */
    QMap<QString,QString> abeLOMgetAvailableLanguages();

    // ------------------- branche 1 : Général
    /**
      * 1.1.1: Le Catalogue
      * @attention cardinalité 1
      * @param value "URI" par défaut, mais peut-être modifié, aurait été un ARK si le système n'était pas trop centralisé à mon goût
      * @see http://www.bnf.fr/fr/professionnels/anx_isbn_issn_autres/a.faq_ark.html
      * @see Dublin Core  DC.Identifier
      * @note On utilisera le mot clé URI (ark:/ serait bien mais centralisé)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:identifier>
  <lom:catalog>URI</lom:catalog>
  <lom:entry>http://mediatheque.abuledu.org/URI/5343xdfg32</lom:entry>
</lom:identifier>
      * @endcode
      */
    void    abeLOMsetGeneralIdentifierCatalog(QString value = "URI");
    QString abeLOMgetGeneralIdentifierCatalog();

    /**
      * 1.1.2: L'entrée dans le catalogue
      * @attention cardinalité 1
      * @param value code fourni par le serveur de stockage (la médiatheque dans notre cas)
      * @see abeLOMsetGeneralIdentifierCatalog()
      * @note On utlisera http://mediatheque.abuledu.org/URI/code
      */
    void    abeLOMsetGeneralIdentifierEntry(QString value);
    QString abeLOMgetGeneralIdentifierEntry();
    /**
      * 1.2: Titre
      * @attention cardinalité 1
      * @see Dublin Core  DC.Title
      * @param lang langue du titre
      * @param value le titre du document
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:title>
  <lom:string language="fre">Le titre du document en français</lom:string>
</lom:title>
      * @endcode
      */
    void    abeLOMsetGeneralTitle(QString lang, QString value);
    QString abeLOMgetGeneralTitle(QString lang);
    QList<QString> abeLOMgetGeneralTitleKeys();

    /**
      * 1.3: Langue
      * @attention cardinalité n
      * @param value langue(s) du document
      * @see Dublin Core  DC.Language ISO639
      * @see http://www.loc.gov/standards/iso639-5/id.php
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:language>fre</lom:language>
<lom:language>eng</lom:language>
      * @endcode
      */
    void        abeLOMsetGeneralLanguage(QStringList lang);
    void        abeLOMsetGeneralLanguage(QString lang);
    void        abeLOMaddGeneralLanguage(QString lang);
    QStringList abeLOMgetGeneralLanguage();

    /**
      * 1.4: Description
      * @attention cardinalité n
      * @param lang langue de la description
      * @param value la description dans la langue en question
      * @see Dublin Core  DC.Description
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:description>
  <lom:string language="fre">Une description super intéressante de la ressource dans la langue 'fre'</lom:string>
</lom:description>
<lom:description>
  <lom:string language="eng">The same but in english</lom:string>
</lom:description>
      * @endcode
      */
    bool    abeLOMsetGeneralDescription(QString lang, QString value);
    bool    abeLOMaddGeneralDescription(QString lang, QString value);
    void    abeLOMclearGeneralDescription();
    QStringList abeLOMgetGeneralDescription(QString lang);
    QList<QString> abeLOMgetGeneralDescriptionKeys();

    /**
      * 1.5: Mots clés libres
      * @attention cardinalité n
      * @param lang langue du mot clé
      * @param value le mot clé
      * @see Dublin Core  DC.Subject
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:keyword>
  <lom:string language="fre">bateau</lom:string>
</lom:keyword>
<lom:keyword>
  <lom:string language="fre">remorqueur</lom:string>
</lom:keyword>
<lom:keyword>
  <lom:string language="eng">tug</lom:string>
</lom:keyword>
      * @endcode
      */
    bool abeLOMsetGeneralKeyword(QString lang, QString value);
    bool abeLOMaddGeneralKeyword(QString lang, QString value);
    void abeLOMclearGeneralKeywords();
    QStringList    abeLOMgetGeneralKeywords(QString lang);
    QList<QString> abeLOMgetGeneralKeywordKeys();

    /**
      * 1.6: Couverture au sens large: géographique, époque (Lieu, Région, Époque)
      * @attention cardinalité n
      * @param lang langue de la couverture
      * @param value la couverture
      * @see Dublin Core  DC.Coverage
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:coverage>
 <lom:string language="fre">Empire musulman</lom:string>
</lom:coverage>
<lom:coverage>
 <lom:string language="fre">VIIIe-XVe siècle</lom:string>
</lom:coverage>
      * @endcode
      */
    bool abeLOMsetGeneralCoverage(QString lang, QString value);
    bool abeLOMaddGeneralCoverage(QString lang, QString value);
    QString abeLOMgetGeneralCoverage(QString lang);
    QList<QString> abeLOMgetGeneralCoverageKeys();

    /**
      * 1.7: Structure
      * @attention cardinalité 1
      * @attention utiliser le vocabulaire adapté : atomic, collection, hierarchical, linear, networked
      * @param value la structure (dans un vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:structure>
  <lom:source>LOMv1.0</lom:source>
  <lom:value>collection</lom:value>
</lom:structure>
      * @endcode
      */
    bool        abeLOMsetGeneralStructure(QString value);
    QString     abeLOMgetGeneralStructure();
    QMap<QString,QString> abeLOMgetGeneralStructureVocabLOM();

    /**
      * 1.8: Niveau d'agrégation
      * @attention cardinalité 1
      * @attention utiliser le vocabulaire adapté : 1,2,3,4  Attention si la ressource a été définie comme atomique alors son "aggregationLevel" doit être de 1
      * @param value de l'agrégation (dans un vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:aggregationLevel>
  <lom:source>LOMv1.0</lom:source>
  <lom:value>2</lom:value>
</lom:aggregationLevel>
      * @endcode
      */
    bool        abeLOMsetGeneralAggregationLevel(QString value);
    QString     abeLOMgetGeneralAggregationLevel();
    QMap<QString,QString> abeLOMgetGeneralAggregationLevelVocabLOM();


    // ------------------- branche 2 : Cycle de vie
    /**
      * 2.1: Version
      * @attention cardinalité 1
      * @param lang la langue
      * @param value la valeur
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:version>
  <lom:string language="fre">1.4.2</lom:string>
</lom:version>
      * @endcode
      */
    bool abeLOMsetLifeCycleVersion(QString lang, QString value);
    QString abeLOMgetLifeCycleVersion(QString lang);
    QList<QString> abeLOMgetLifeCycleVersionKeys();

    /**
      * 2.2: État
      * @attention cardinalité 1
      * @attention utiliser le vocabulaire adapté : draft, final, revised, unvailable
      * @param value valeur de l'état (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:status>
  <lom:source>LOMv1.0</lom:source>
  <lom:value>final</lom:value>
</lom:status>
      * @endcode
      */
    bool        abeLOMsetLifeCycleStatus(QString value);
    QString     abeLOMgetLifeCycleStatus();
    QMap<QString,QString> abeLOMgetLifeCycleStatusVocabLOM();

    /**
      * 2.3: Contribution (cardinalité n)
      * 2.3.1: Rôle (contribution)
      * @attention cardinalité 1
      * @attention utiliser le vocabulaire adapté : author, publisher, unknown, initiator, terminator, validator, editor, graphical designer, technical implementer, content provider, technical validator, educational validator, script writer, instructional designer, subject matter expert
      * @param value valeur de l'état (dans le vocabulaire)
      * @see Dublin Core  DC.Creator/DC.Publisher/DC.Contributor
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:contribute>
  <lom:role>
    <lom:source>LOMv1.0</lom:source>
    <lom:value>publisher</lom:value>
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
  <lom:dateTime>2012-04-09</lom:dateTime>
  </lom:date>
</lom:contribute>
      * @endcode
      */
    QMap<QString,QString> abeLOMgetLifeCycleContributionRoleVocabLOM();
    /**
      * Ajoute une nouvelle contribution
      */
    virtual bool          abeLOMaddLifeCycleContributionRole(QString role, vCard entity, QDate ladate);

    /**
      * Supprime une contribution
      */
    virtual bool          abeLOMremoveLifeCycleContributionRole(QString role, vCard entity, QDate ladate);
    //TODO

    /**
      * Retourne la date la plus récente des contributions
      * @param role le role en question (attention doit être dans la liste du vocabulaire accepté)
      */
    QDate abeLOMgetLifeCycleContributionLastDate(QString role);

    /**
      * Retourne les dates des contributions
      * @param role le role en question (attention doit être dans la liste du vocabulaire accepté)
      */
    virtual QList<QDate> abeLOMgetLifeCycleContributionDates(QString role);

    /**
      * Retourne une liste de vcard correspondantes au rôle ... role pour la date  ... date
      * @param role le role en question (attention doit être dans la liste du vocabulaire accepté)
      * @param date la date en question
      */
    QList<vCard> abeLOMgetLifeCycleContributionEntities(QString role, QDate date);

    /**
      * Retourne la liste des rôles utilisés
      * @note liste sans doublon
      */
    QStringList abeLOMgetLifeCycleContributionRoles();

    inline QMap<QString, QList<vCard> > abeLOMgetLifeCycleContributions(){return m_LifeCycleContributions;}


    // ------------------- branche 3: Meta MetaData
    /**
      * Branche 3: A FAIRE
      * @todo la branche 3 n'est pas du tout prioritaire et donc probablement pas implémentée, toute contribution est la bienvenue, merci d'avance
      */
    //3.1.1
    void    abeLOMsetMetaMetadataIdentifierCatalog(QString value);
    QString abeLOMgetMetaMetadataIdentifierCatalog();
    //3.1.2
    void    abeLOMsetMetaMetadataIdentifierEntry(QString value);
    QString abeLOMgetMetaMetadataIdentifierEntry();
    //3.2.1
    void    abeLOMsetMetaMetadataContributeRole(QString value);
    QString abeLOMgetMetaMetadataContributeRole();
    //3.2.2
    void    abeLOMsetMetaMetadataContributeEntity(QString value);
    QString abeLOMgetMetaMetadataContributeEntity();
    //3.2.3
    void    abeLOMsetMetaMetadataContributeDate(QString value);
    QString abeLOMgetMetaMetadataContributeDate();
    //3.3
    void    abeLOMsetMetaMetadataMetadataSchema(QString value);
    QString abeLOMgetMetaMetadataMetadataSchema();
    //3.4
    void    abeLOMsetMetaMetadataLanguage(QString value);
    QString abeLOMgetMetaMetadataLanguage();

    // ------------------- branche 4 : Technique
    /**
      * 4.1: Format, le type mime du/des document(s) numériques ou autre pour documents physiques
      * @attention cardinalité n
      * @param value le type en question
      * @see Dublin Core DC.Format
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:format>application/pdf</lom:format>
<lom:format>application/abuledu</lom:format>
      * @endcode
      */
    bool abeLOMaddTechnicalFormat(QString value);
    QStringList abeLOMgetTechnicalFormat();
    void        abeLOMclearTechnicalFormat();

    /**
      * 4.2: Taille, la taille de la ressources en octets
      * @attention cardinalité 1
      * @param value la taille en octets
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:size>0</lom:size>
      * @endcode
      */
    void    abeLOMsetTechnicalSize(QString value);
    QString abeLOMgetTechnicalSize();

    /**
      * 4.3: Localisation, chaine de caractère utilisée pour accèder à la ressource: url, uri, ou une indication pour se procurer le document
      * @attention cardinalité n
      * @param value la localisation
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:location>http://raconte-moi.abuledu.org/w/tag/djataka</lom:location>
<lom:location>http://libre.pedagosite.net/voir_doc.php?DOC=274</lom:location>
      * @endcode
      */
    void    abeLOMaddTechnicalLocation(QString value);
    bool    abeLOMdelTechnicalLocation(QString value);
    QStringList abeLOMgetTechnicalLocation();

    /**
      * Branche 4: A FINIR
      * @todo la branche 4 n'est pas totalement implémentée, toute contribution est la bienvenue, merci d'avance
      */
    //4.4.1.1
    void    abeLOMsetTechnicalRequirementOrCompositeType(QString value);
    QString abeLOMgetTechnicalRequirementOrCompositeType();
    //4.4.1.2
    void    abeLOMsetTechnicalRequirementOrCompositeName(QString value);
    QString abeLOMgetTechnicalRequirementOrCompositeName();
    //4.4.1.3
    void    abeLOMsetTechnicalRequirementOrCompositeMinimumVersion(QString value);
    QString abeLOMgetTechnicalRequirementOrCompositeMinimumVersion();
    //4.4.1.4
    void    abeLOMsetTechnicalRequirementOrCompositeMaximumVersion(QString value);
    QString abeLOMgetTechnicalRequirementOrCompositeMaximumVersion();
    //4.5
    void    abeLOMsetTechnicalInstallationRemarks(QString lang, QString value);
    QString abeLOMgetTechnicalInstallationRemarks(QString lang);
    QList<QString> abeLOMgetTechnicalInstallationRemarksKeys();
    //4.6
    void    abeLOMsetTechnicalOtherPlatformRequirements(QString lang, QString value);
    QString abeLOMgetTechnicalOtherPlatformRequirements(QString lang);
    QList<QString> abeLOMgetTechnicalOtherPlatformRequirementsKeys();

    /**
      * 4.7: Durée Temps moyen nécessaire à une utilisation de la ressource pédagogique à présenter suivant le modèle ci-dessus
      * @attention cardinalité 1
      * @param value la durée PT suivi de hHmMsS, exemple PT1H15M30S
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:duration>
  <lom:duration>PT1H15M30S</lom:duration>
</lom:duration>
      * @endcode
      */
    void    abeLOMsetTechnicalDuration(QString value);
    QString abeLOMgetTechnicalDuration();

    // ------------------- branche 5 : Pédagogie
    /**
      * 5.1: Type d'interactivité: interactivityType
      * @attention attribut LOM non intégré au LOMFR
      * @attention utiliser le vocabulaire adapté :
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
      * @endcode
      */
    void    abeLOMsetEducationalInteractivityType(QString value);
    QString abeLOMgetEducationalInteractivityType();

    /**
      * 5.2: Type de ressource pédagogique dans l'idée de Nature pédagogique de la ressouce: learningResourceType
      * @attention cardinalité n
      * @attention vocabulaire LOM: exercise, simulation, questionnaire, diagram, figure, graph, index, slide, table, narrative text, exam, experiment, problem statement, self assessment, lecture
      * @attention vocabulaire LOMFR: démonstration, évaluation, animation, tutoriel, glossaire, guide, matériel de référence, méthodologie, outil, scénario pédagogique
      * @attention vocabulaire SCOLOMFR: étude de cas, jeu éducatif, manuel d'enseignement, production d'élève, annales, cyberquête, méthode de langues, témoignage pédagogique
      * @attention utiliser le vocabulaire adapté ... mais lequel ? la version française ou l'anglaise ? on aurait une préférence pour la version anglaise pour plus de "portabilité"
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:learningResourceType>
   <lom:source>LOMFRv1.0</lom:source>
   <lom:value>méthodologie</lom:value>
</lom:learningResourceType>
<lom:learningResourceType>
   <lom:source>LOMFRv1.0</lom:source>
   <lom:value>matériel de référence</lom:value>
</lom:learningResourceType>
<lom:learningResourceType>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>lecture</lom:value>
</lom:learningResourceType>
      * @endcode
      */
    virtual bool          abeLOMaddEducationalLearningResourceType(QString value);
    virtual QStringList   abeLOMgetEducationalLearningResourceType();
    QMap<QString,QString> abeLOMgetEducationalLearningResourceTypeVocabLOM();

    /**
      * 5.3: Niveau d'interactivité: interactivityLevel
      * @attention attribut LOM non intégré au LOMFR
      * @param value
      * @note Exemple de code XML qui sera généré:
      * @code
      * @endcode
      */
    void    abeLOMsetEducationalInteractivityLevel(QString value);
    QString abeLOMgetEducationalInteractivityLevel();


    /**
      * 5.4: Densité sémantique: semanticDensity
      * @attention attribut LOM non intégré au LOMFR
      * @param value
      * @note Exemple de code XML qui sera généré:
      * @code
      * @endcode
      */
    void    abeLOMsetEducationalSemanticDensity(QString value);
    QString abeLOMgetEducationalSemanticDensity();

    /**
      * 5.5: Rôle présumé de l'utilisateur final: intendedEndUserRole
      * @attention cardinalité n
      * @attention vocabulaire LOM: teacher, author, learner, manager
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:intendedEndUserRole>
   <lom:value>learner</lom:value>
</lom:intendedEndUserRole>
<lom:intendedEndUserRole>
   <lom:value>teacher</lom:value>
</lom:intendedEndUserRole>
      * @endcode
      */
    bool        abeLOMaddEducationalIntendedEndUserRole(QString value);
    QStringList abeLOMgetEducationalIntendedEndUserRole();
    QMap<QString,QString> abeLOMgetEducationalIntendedEndUserRoleVocabLOM();

    /**
      * 5.6: Contexte et niveau du public cible: context
      * @attention cardinalité n
      * @attention vocabulaire LOM: school, higher education, training, other
      * @attention vocabulaire LOMFR: enseignement primaire, enseignement secondaire, licence, master, doctorat, formation continue, formation en entreprise
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:context>
   <lom:source>LOMFRv1.0</lom:source>
   <lom:value>enseignement secondaire</lom:value>
</lom:context>
      * @endcode
      */
    virtual bool          abeLOMaddEducationalContext(QString value);
    QStringList           abeLOMgetEducationalContext();
    QMap<QString,QString> abeLOMgetEducationalContextVocabLOM();
    void                  abeLOMclearEducationalContext();

    /**
      * 5.7: Tranche d'âge typicalAgeRange
      * @attention cardinalité n
      * @param value valeur de type chaine (localisée) de caractère de type "ageMini-ageMaxi"
      * @note Exemple de code XML qui sera généré:
      * @todo Ajouter une regExp qui interdise d'enregistrer une chaine de caractère de structure différente
      * @code
<lom:typicalAgeRange>
   <lom:string language="fre">15-16</lom:string>
</lom:typicalAgeRange>
      * @endcode
      */
    bool           abeLOMaddEducationalTypicalAgeRange(QString lang, QString value);
    void           abelomclearEducationalTypicalAgeRange();
    QStringList    abeLOMgetEducationalTypicalAgeRange(QString lang);
    QList<QString> abeLOMgetEducationalTypicalAgeRangeKeys();

    /**
      * 5.8: Difficulté: Difficulty
      * @attention cardinalité 1
      * @attention vocabulaire LOM: very easy, easy, medium, difficult, very difficult
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:difficulty>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>medium</lom:value>
</lom:difficulty>
      * @endcode
      */
    bool        abeLOMsetEducationalDifficulty(QString value);
    QString     abeLOMgetEducationalDifficulty();
    QMap<QString,QString> abeLOMgetEducationalDifficultyLOM();

    /**
      * 5.9: Temps d'apprentissage moyen: typicalLearningTime
      * @attention cardinalité 1
      * @param value valeur de type Date
      * @note Exemple de code XML qui sera généré:
      * @code
<typicallearningtime>
  <datetime>PT1H20M</datetime>
</typicallearningtime>
      * @endcode
      */
    void    abeLOMsetEducationalTypicalLearningTime(int minutes);
    QTime abeLOMgetEducationalTypicalLearningTime();

    /**
      * 5.10: Description et proposition d'utilisation: description
      * @attention cardinalité n
      * @param value valeur de type chaine (localisée) de caractère
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:description>
  <lom:string language="fre">Cette ressource peut s'utiliser en vidéoprojection, en séance avec les élèves sur les postes informatiques ou à domicile.</lom:string>
</lom:description>
      * @endcode
      */
    bool             abeLOMaddEducationalDescription(QString lang, QString value);
    void             abeLOMclearEducationalDescription();
    QStringList      abeLOMgetEducationalDescription(QString lang);
    QList<QString>   abeLOMgetEducationalDescriptionKeys();

    /**
      * 5.11: Langue de l'utilisateur
      * @attention cardinalité n
      * @param value valeur de type chaine (localisée) de caractères au format ISO639
      * @see http://www.loc.gov/standards/iso639-5/id.php
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:language>fre</lom:language>
<lom:language>eng</lom:language>
<lom:language>ara</lom:language>
<lom:language>spa</lom:language>
<lom:language>dut</lom:language>
<lom:language>ger</lom:language>
      * @endcode
      */
    void        abeLOMaddEducationalLanguage(QString value);
    QStringList abeLOMgetEducationalLanguage();


    // ------------------- branche 6 : Droits & Coûts
    /**
      * 6.1: Coût, permet d'indiquer si la ressource est payante / a un coût: cost
      * @attention cardinalité 1
      * @attention vocabulaire LOM: yes, no
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:cost>
  <lom:source>LOMv1.0</lom:source>
  <lom:value>yes</lom:value>
</lom:cost>
      * @endcode
      */
    bool        abeLOMsetRightsCost(QString value);
    QString     abeLOMgetRightsCost();
    QMap<QString,QString> abeLOMgetRightsCostVocabLOM();

    /**
      * 6.2: Droit d'auteur, permet d'indiquer si la ressource a un droit d'auteur ou si elle est complètement libre de droits: copyrightAndOtherRestrictions
      * @attention cardinalité 1
      * @attention vocabulaire LOM: yes, no
      * @param value valeur (dans le vocabulaire), sera généralement à yes, on a un droit d'auteur qui s'applique et c'est justement grâce à lui qu'on applique ensuite une licence libre de type cc-by-sa
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:copyrightAndOtherRestrictions>
  <lom:source>LOMv1.0</lom:source>
  <lom:value>yes</lom:value>
</lom:copyrightAndOtherRestrictions>
      * @endcode
      */
    bool        abeLOMsetRightsCopyrightAndOtherRestrictions(QString value);
    QString     abeLOMgetRightsCopyrightAndOtherRestrictions();
    QMap<QString,QString> abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM();

    /**
      * 6.3: Description, conditions légales d'utilisation de la ressource ou du document: description
      * @attention cardinalité 1
      * @param valeur de type chaine (localisée) de caractère
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:description>
  <lom:string language="fre">cc-by-sa</lom:string>
</lom:description>
      * @endcode
      */
    void    abeLOMsetRightsDescription(QString lang, QString value);
    QString abeLOMgetRightsDescription(QString lang);
    QList<QString> abeLOMgetRightsDescriptionKeys();


    // ------------------- branche 7 : relations
    /**
      * 7.1: Nature de la relation : kind
      * @attention cardinalité 1
      * @attention vocabulaire LOM: ispartof, haspart, isversionof, hasversion, isformatof, hasformat, references, isreferencedby, isbasedon, isbasisfor, requires, isrequiredby
      * @attention vocabulaire LOMFR: est associée à, est la traduction de, fait l'objet d'une traduction, est la localisation de, fait l'objet d'une localisation, est prérequis de,  a pour prérequis
      * @attention vocabulaire SCOLOMFR: a pour vignette, a pour logo, a pour aperçu, est l'aperçu de
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:relation>
  <lom:kind>
    <lom:source>LOMv1.0</lom:source>
    <lom:value>ispartof</lom:value>
  </lom:kind>
  <lom:resource>
    <lom:identifier>
      <lom:catalog>nom du catalogue où la ressource est stockée</lom:catalog>
      <lom:entry> Valeur de la ressource destination avec lequel est fait le lien. Cela peut être url du document ou du chapitre ou autre moyen qui permet de retrouver le document</lom:entry>
    </lom:identifier>
    <lom:description>
      <lom:string language="fre">Description de la ressource "destination"</lom:string>
    </lom:description>
  </lom:resource>
</lom:relation>

<lom:relation>
  <lom:kind>
    <lom:source>LOMFRv1.0</lom:source>
    <lom:value>est la traduction de</lom:value>
  </lom:kind>
  <lom:resource>
    <lom:identifier>
      <lom:catalog>nom du catalogue où la ressource est stockée</lom:catalog>
      <lom:entry> Valeur de la ressource destination avec lequel est fait le lien. Cela peut être url du document ou du chapitre ou autre moyen qui permet de retrouver le document</lom:entry>
    </lom:identifier>
    <lom:description>
      <lom:string language="fre">Description de la ressource "destination"</lom:string>
    </lom:description>
  </lom:resource>
</lom:relation>
      * @endcode
      */
    virtual bool          abeLOMsetRelationKind(QString value);
    QString               abeLOMgetRelationKind();
    QMap<QString,QString> abeLOMgetRelationKindVocabLOM();

    /**
      * 7.2: Ressource : resource
      * 7.2.1: Identifiant : identifier
      *
      * 7.2.1.1: catalogue
      * 7.2.1.2: Entrée : entry
      * @attention cardinalité n
      * @param value valeur chaîne de caractères
      * @see abeLOMsetRelationKind()
      * Comme on peut avoir plusieurs URI,http://bnmablablabla une QMap n'est pas adaptée,
      * choix d'avoir une qstringlist de type URI;http://jgdklgd << URI;http://glgegre
      */
    void        abeLOMaddRelationResourceIdentifier(QString catalog, QString entry);
    void        abeLOMdelRelationResourceIdentifier(QString catalog, QString entry);
    QStringList abeLOMgetRelationResourceIdentifier();

    /**
      * 7.2: Ressource : resource
      * 7.2.2: Description de la ressource "destination": description
      * @attention cardinalité n
      * @param value valeur de type chaine (localisée) de caractère
      * @see abeLOMsetRelationKind()
      */
    void    abeLOMaddRelationResourceDescription(QString lang, QString value);
    QString abeLOMgetRelationResourceDescription(QString lang);
    QList<QString> abeLOMgetRelationResourceDescriptionKeys();

    // ------------------- branche 8 : Annotations
    /**
      * 8: Annotations, cette catégorie permet de stocker des remarques libres sur la ressource: annotation. Cardinalité n
      * 8.1: Entité au format vcard: entity
      * @attention cardinalité 1
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:annotation>
  <lom:entity>[BEGIN:VCARD VERSION:3.0 N:Nom; Prenom;;; FN:Nom Prenom ORG:Nom de l'organisme du "contributeur" EMAIL;INTERNET:Nom@adresse.fr END:VCARD]</lom:entity>
  <lom:date>
    <lom:dateTime>2012-04-15</lom:dateTime>
  </lom:date>
  <lom:description>
    <lom:string language="fre">Contenu du commentaire sur l'utilisation de la ressource</lom:string>
  </lom:description>
</lom:annotation>
      * @endcode
      */
    void    abeLOMsetAnnotationEntity(vCard value);
    vCard   abeLOMgetAnnotationEntity();

    /**
      * 8.2: La date : date
      * @attention cardinalité 1
      * @param value valeur de type QDate
      * @see abeLOMsetAnnotationEntity();
      */
    void    abeLOMsetAnnotationDate(QDate value);
    QDate   abeLOMgetAnnotationDate();

    /**
      * 8.3: La description: description
      * @attention cardinalité 1
      * @param value valeur de type chaine (localisée) de caractère
      * @see abeLOMsetAnnotationEntity();
      */
    void    abeLOMsetAnnotationDescription(QString lang, QString value);
    QString abeLOMgetAnnotationDescription(QString lang);
    QList<QString> abeLOMgetAnnotationDescriptionKeys();


    // ------------------- branche 9 : Classification
    /**
      * 9: Classification, ctte catégorie décrit où cette ressource se trouve dans un système de classification particulier: classification. Cardinalité n
      * 9.1: Objectif: purpose
      * @attention cardinalité 1
      * @attention vocabulaire LOM: discipline, idea, prerequisite, educational objective, accessibility restrictions, educational level, skill level, security level, competency
      * @attention vocabulaire LOMFR:
      * @attention vocabulaire SCOLOMFR: enseignement, public cible détaillé, label, type de diffusion
      * @attention vocabulaire SCOLOMFR decembre 2012 beaucoup plus détaillé: http://www.lom-fr.fr/scolomfr/vocabulaires/consultation-des-vocabulaires.html
      * @param value valeur (dans le vocabulaire)
      * @note Exemple de code XML qui sera généré:
      * @code
<lom:classification>
  <lom:purpose>
    <lom:source>LOMv1.0</lom:source>
    <lom:value>discipline</lom:value>
  </lom:purpose>
  <lom:taxonPath>
    <lom:source>
      <lom:string language="fre">CDD 22e éd.</lom:string>
      <lom:string language="eng">DDC 22nd ed</lom:string>
    </lom:source>
    <lom:taxon>
      <lom:id>identifiant du taxon si vous utilisez la DDC: mettre l'indice</lom:id>
      <lom:entry>
        <lom:string language="fre">"Etiquette" texte du taxon</lom:string>
      </lom:entry>
    </lom:taxon>
  </lom:taxonPath>
  <lom:description>
    <lom:string language="fre">Description textuelle relative à la classification</lom:string>
  </lom:description>
  <lom:keyword>
    <lom:string language="fre">Mots clés relatifs à l'objectif de la classification</lom:string>
  </lom:keyword>
</lom:classification>
      * @endcode
      */
//    virtual bool        abeLOMsetClassificationPurpose(int racine, QString value);
    QString     abeLOMgetClassificationPurpose(int racine);
    /** Retourne le nombre d'objectifs présents dans la structure de données */
    int         abeLOMgetClassificationPurposeNumber();
    /** Retourne l'index correspondant à la value, ou -1 si la value n'est pas dans la QHash */
    int         abeLOMgetClassificationPurposeIndex(QString value);
    QMap<QString,QString> abeLOMgetClassificationPurposeVocabLOM();

    /**
      * 9.2 : Chemin taxum / taxon : taxonPath. Cardinalité n
      * 9.2.1 : Source: source
      * @attention cardinalité 1
      * @param num le numero (entier) de ce Taxon
      * @param lang la langue
      * @param value valeur de type chaine (localisée) de caractère
      * @see abeLOMsetClassificationPurpose();
      */
    void    abeLOMsetClassificationTaxonPathSource(int racine, QString lang, QString value);
    QString abeLOMgetClassificationTaxonPathSource(int racine, QString lang);
    /** Retourne l'index correspondant au LomLS(lang,value), ou -1 s'il n'existe pas dans la QHash */
    int     abeLOMgetClassificationTaxonPathIndexForSource(QString lang, QString value);
    QList<QString> abeLOMgetClassificationTaxonPathSourceKeys(int num);
    int abeLOMgetClassificationTaxonPathSourceSize();

    /* Fonctions liées à ma nouvelle structure de données (attention : je mets "purpose" au lieu d'"objectif" pour ne pas tout renommer si on part comme ça */
    QHash<QString, QHash<QString, QHash<QString, QString> > > abeLOMgetClassificationPurposes();
    /** Efface le contenu de la structure de données. Appelée avant de faire les "add", de façon à éviter les doublons */
    void abeLOMclearClassificationPurposes();
    /** Ajoute les éléments d'un nouveau bloc <classification> dans le XML */
    virtual bool abeLOMaddClassificationPurpose(QString purpose,LomLS sourceTaxonPath,QString idTaxon, LomLS entryTaxon);


    /**
      * 9.2.2 : Taxum / Taxon: taxon. Cardinalité n
      * 9.2.2.1 : ID: id
      * @attention cardinalité 1
      * @param value valeur de type chaine de caractère
      * @see abeLOMsetClassificationPurpose();
      */
    void    abeLOMaddClassificationTaxonPathTaxumID(int racine, int num, QString value);
    /** retourne le nombre de taxons dans cette racine */
    int     abeLOMgetClassificationTaxonPathNbTaxon(int racine);
    QString abeLOMgetClassificationTaxonPathTaxumID(int racine, int num);

    /**
      * 9.2.2.2 : Entrée: entry
      * @attention cardinalité 1
      * @param value valeur de type chaine (localisée) de caractère
      * @see abeLOMsetClassificationPurpose();
      */
    void    abeLOMsetClassificationTaxonPathTaxumEntry(int racine, int num, QString lang, QString value);
    QString abeLOMgetClassificationTaxonPathTaxumEntry(int racine, int num, QString lang);
    QList<QString> abeLOMgetClassificationTaxonPathTaxumEntryKeys(int racine, int num);

    /**
      * 9.3 : Description : description
      * @attention cardinalité 1
      * @param value valeur de type chaine (localisée) de caractère
      * @see abeLOMsetClassificationPurpose();
      */
    void    abeLOMsetClassificationDescription(QString lang, QString value);
    QString abeLOMgetClassificationDescription(QString lang);
    QList<QString> abeLOMgetClassificationDescriptionKeys();

    /**
      * 9.4 : Mot clé: keyword: les mots clés de type "texte libre"
      * @attention cardinalité n
      * @param value valeur de type chaine (localisée) de caractère
      * @see abeLOMsetClassificationPurpose();
      */
    bool abeLOMsetClassificationKeyword(QString lang, QString value);
    QString abeLOMgetClassificationKeyword(QString lang);
    QList<QString> abeLOMgetClassificationKeywordKeys();
    // =============================================================================== Code automatique (fin)

    /**
      * Nettoyage de la structure de données
      */
    bool clear();

protected:
    /** Correspondance entre les noms des langues et leur code ISO 639-2 */
    QMap<QString,QString> m_availableLanguages;
    QList<vCard>          m_allVcards;

    // ------------------- branche 1
    QString               m_GeneralIdentifierCatalog;
    QString               m_GeneralIdentifierEntry;
    LomLS                 m_GeneralTitle;
    QStringList           m_GeneralLanguage;
    LomLS                 m_GeneralDescription;
    LomLS                 m_GeneralKeyword;
    LomLS                 m_GeneralCoverage;
    QString               m_GeneralStructure;
    QMap<QString,QString> m_GeneralStructureVocabLOM;
    QString               m_GeneralAggregationLevel;
    QMap<QString,QString> m_GeneralAggregationLevelVocabLOM;

    // ------------------- branche 2
    LomLS                 m_LifeCycleVersion;
    QString               m_LifeCycleStatus;
    QMap<QString,QString> m_LifeCycleStatusVocabLOM;
    /**
      * Structure permettant de stocker toutes les contributions de type
      *  contrib1
      *  role: auteur
      *    entité1 : vcard
      *    entité2 : vcard
      *    entité3 : vcard
      *  date : ladate
      *  contrib2
      *  role: éditeur
      *    entité1 : vcard
      *    entité2 : vcard
      *  date : ladate
      *
      * Finalement je simplifie beaucoup, vu qu'une clé ne peut être composée de deux champs je fais une QString
      * role:2012-04-17 et c'est tout !
      */
    QMap<QString, QList<vCard> > m_LifeCycleContributions;
    QMap<QString,QString>        m_LifeCycleContributionRoleVocabLOM;

    // ------------------- branche 3
    QString m_MetaMetadataIdentifierCatalog;
    QString m_MetaMetadataIdentifierEntry;
    QString m_MetaMetadataContributeRole;
    QString m_MetaMetadataContributeEntity;
    QString m_MetaMetadataContributeDate;
    QString m_MetaMetadataMetadataSchema;
    QString m_MetaMetadataLanguage;

    // ------------------- branche 4
    QStringList m_TechnicalFormat;
    QString     m_TechnicalSize;
    QStringList m_TechnicalLocation;
    QString     m_TechnicalRequirementOrCompositeType;
    QString     m_TechnicalRequirementOrCompositeName;
    QString     m_TechnicalRequirementOrCompositeMinimumVersion;
    QString     m_TechnicalRequirementOrCompositeMaximumVersion;
    LomLS       m_TechnicalInstallationRemarks;
    LomLS       m_TechnicalOtherPlatformRequirements;
    QString     m_TechnicalDuration;

    // ------------------- branche 5
    QString               m_EducationalInteractivityType;
    QMap<QString,QString> m_EducationalInteractivityTypeVocabLOM;
    QStringList           m_EducationalLearningResourceType;
    QMap<QString,QString> m_EducationalLearningResourceTypeVocabLOM;
    QString               m_EducationalInteractivityLevel;
    QString               m_EducationalSemanticDensity;
    QStringList           m_EducationalIntendedEndUserRole;
    QMap<QString,QString> m_EducationalIntendedEndUserRoleVocabLOM;
    QStringList           m_EducationalContext;
    QMap<QString,QString> m_EducationalContextVocabLOM;
    LomLS                 m_EducationalTypicalAgeRange;
    QString               m_EducationalDifficulty;
    QMap<QString,QString> m_EducationalDifficultyVocabLOM;
    QTime                 m_EducationalTypicalLearningTime;
    LomLS                 m_EducationalDescription;
    QStringList           m_EducationalLanguage;

    // ------------------- branche 6
    QString               m_RightsCost;
    QMap<QString,QString> m_RightsCostVocabLOM;
    QString               m_RightsCopyrightAndOtherRestrictions;
    QMap<QString,QString> m_RightsCopyrightAndOtherRestrictionsVocabLOM;
    LomLS                 m_RightsDescription;

    // ------------------- branche 7
    QString                m_RelationKind;
    QMap<QString,QString>  m_RelationKindVocabLOM;
    QStringList            m_RelationResourceIdentifierEntry;
    LomLS                  m_RelationResourceDescription;

    // ------------------- branche 8
    vCard                 m_AnnotationEntity;
    QDate                 m_AnnotationDate;
    LomLS                 m_AnnotationDescription;

    // ------------------- branche 9
    QHash<int, QString>        m_ClassificationPurpose;//inutile (fonctions qui l'utilisent idem) ?
    QMap<QString,QString>      m_ClassificationPurposeVocabLOM;
    QList<LomLS>               m_ClassificationTaxonPathSource;//inutile (fonctions qui l'utilisent idem) ?
    QHash <int, QHash <int, QString> > m_ClassificationTaxonPathTaxumID;//inutile (fonctions qui l'utilisent idem) ?
    QHash <int, QHash <int,LomLS> >    m_ClassificationTaxonPathTaxumEntry;//inutile (fonctions qui l'utilisent idem) ?

    /* Eventuellement, on pourrait faire ça
        Attention, vérifier que entry est bien une LomLS, et si source n'en est pas une
    */
    QHash <QString, QHash <QString, QHash <QString,QString > > >  m_classificationPurposes;

    LomLS                 m_ClassificationDescription;
    LomLS                 m_ClassificationKeyword;

    QDomDocument m_domDocument; /** Pour generer le XML */
    QDomElement  m_domRootElement;    /** Pour revenir à l'élément racine quand on change d'élément (passage aux classes héritées) */
    QDomElement  m_domCurrentElement; /** Pour se "passer" l'element en cours entre les différentes classes héritées */


    //============================================================================= EXPORT XML
    /**
      * Exporte l'entête du fichier XML
      *
      * @note Exemple de code XML qui sera généré:
      * @code
<?xml version="1.0" encoding="UTF-8"?>
<lom:lom xmlns:lom="http://ltsc.ieee.org/xsd/LOM">
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_header();


    /**
      * Exporte la section 1 Général
      *
      * @note Exemple de code XML qui sera généré:
      * @code
   <lom:general>
      <lom:identifier>
         <lom:catalog>URI</lom:catalog>
         <lom:entry>http://mediatheque.abuledu.org/URI/5343xdfg32</lom:entry>
      </lom:identifier>
      <lom:title>
         <lom:string language="fre">Le titre de la ressource</lom:string>
      </lom:title>
      <lom:language>fre</lom:language>
      <lom:description>
         <lom:string language="fre">Résumé de la ressource.</lom:string>
      </lom:description>
      <lom:keyword>
         <lom:string language="fre">19e-20e siècle</lom:string>
      </lom:keyword>
      <lom:keyword>
         <lom:string language="fre">lutte politique, résistance : politique</lom:string>
      </lom:keyword>
      <lom:structure>
         <lom:source>LOMv1.0</lom:source>
         <lom:value>networked</lom:value>
      </lom:structure>
      <lom:aggregationLevel>
         <lom:source>LOMv1.0</lom:source>
         <lom:value>2</lom:value>
      </lom:aggregationLevel>
   </lom:general>
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section1();

    /**
      * Exporte la section 2 Cycle de vie
      *
      * @note Exemple de code XML qui sera généré:
      * @code
    <lom:lifeCycle>
        <lom:version>
            <lom:string language="fre">Un intitulé libre de version</lom:string>
        </lom:version>
        <lom:status>
            <lom:source>LOMv1.0</lom:source>
            <lom:value>final</lom:value>
        </lom:status>
        <lom:contribute>
            <lom:role>
                <lom:source>LOMv1.0</lom:source>
                <lom:value>author</lom:value>
            </lom:role>
            <lom:entity>BEGIN:VCARD
VERSION:3.0
N:Nom; Prenom;;;
FN:Nom Prenom
ORG:Nom de l'organisme du "contributeur";;
EMAIL;INTERNET:adresse.email@domaine.fr
END:VCARD
            </lom:entity>
            <lom:date>
                <lom:dateTime>2012-04-17</lom:dateTime>
                <lom:description>
                    <lom:string language="fre">un petit commentaire</lom:string>
                </lom:description>
            </lom:date>
        </lom:contribute>
    </lom:lifeCycle>
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section2();


    /**
      * Exporte la section 3 Méta Métadonnées
      *
      * @note Exemple de code XML qui sera généré:
      * @code
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section3();


    /**
      * Exporte la section 4 Technique
      *
      * @note Exemple de code XML qui sera généré:
      * @code
   <lom:technical>
      <lom:format>text/html</lom:format>
      <lom:size>125</lom:size>
      <lom:location>http://www.ryxeo.com/</lom:location>
      <lom:format>image/jpeg</lom:format>
      <lom:format>video/msvideo</lom:format>
   </lom:technical>
      * @endcode
      * @todo
      */
    virtual QByteArray p_abeLOMExportAsXML_section4();

    /**
      * Exporte la section 5 Pédagogie
      *
      * @note Exemple de code XML qui sera généré:
      * @code
 <lom:educational>
  <lom:learningResourceType>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>simulation</lom:value>
  </lom:learningResourceType>
  <lom:learningResourceType>
   <lom:source>LOMFRv1.0</lom:source>
   <lom:value>animation</lom:value>
  </lom:learningResourceType>
  <lom:learningResourceType>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>lecture</lom:value>
  </lom:learningResourceType>
  <lom:intendedEndUserRole>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>learner</lom:value>
  </lom:intendedEndUserRole>
  <lom:intendedEndUserRole>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>teacher</lom:value>
  </lom:intendedEndUserRole>
  <lom:context>
   <lom:source>LOMFRv1.0</lom:source>
   <lom:value>enseignement secondaire</lom:value>
  </lom:context>
  <lom:context>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>other</lom:value>
  </lom:context>
  <lom:typicalAgeRange>
   <lom:string language="eng">kids</lom:string>
  </lom:typicalAgeRange>
  <lom:typicalAgeRange>
   <lom:string language="fre">13-18 ans</lom:string>
  </lom:typicalAgeRange>
  <lom:typicalAgeRange>
   <lom:string language="fre">ado</lom:string>
  </lom:typicalAgeRange>
  <lom:typicalAgeRange>
   <lom:string language="fre">jeunes adultes</lom:string>
  </lom:typicalAgeRange>
  <lom:difficulty>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>easy</lom:value>
  </lom:difficulty>
  <lom:typicalLearningTime>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>PT00H15M00S</lom:value>
  </lom:typicalLearningTime>
  <lom:description>
   <lom:string language="fre">Description de l'utilisation de cette ressource ...</lom:string>
  </lom:description>
  <lom:language>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>fre</lom:value>
  </lom:language>
  <lomfr:activity>
   <lomfr:source>LOMFRv1.0</lomfr:source>
   <lomfr:value>apprendre</lomfr:value>
  </lomfr:activity>
  <lomfr:activity>
   <lomfr:source>LOMFRv1.0</lomfr:source>
   <lomfr:value>observer</lomfr:value>
  </lomfr:activity>
  <lomfr:activity>
   <lomfr:source>LOMFRv1.0</lomfr:source>
   <lomfr:value>s'informer</lomfr:value>
  </lomfr:activity>
  <scolomfr:place>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en salle de classe</scolomfr:value>
  </scolomfr:place>
  <scolomfr:place>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en CDI</scolomfr:value>
  </scolomfr:place>
  <scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>aide didactique individualisée</scolomfr:value>
  </scolomfr:educationalMethod>
  <scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en autonomie</scolomfr:value>
  </scolomfr:educationalMethod>
  <scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en groupe de compétences</scolomfr:value>
  </scolomfr:educationalMethod>
  <scolomfr:educationalMethod>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>en classe entière</scolomfr:value>
  </scolomfr:educationalMethod>
  <scolomfr:tool>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>vidéoprojecteur</scolomfr:value>
  </scolomfr:tool>
  <scolomfr:tool>
   <scolomfr:source>ScoLOMFRv1.0</scolomfr:source>
   <scolomfr:value>logiciels RIP</scolomfr:value>
  </scolomfr:tool>
 </lom:educational>
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section5();

    /**
      * Exporte la section 6 Droits
      *
      * @note Exemple de code XML qui sera généré:
      * @code
 <lom:rights>
  <lom:cost>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>yes</lom:value>
  </lom:cost>
  <lom:copyrightAndOtherRestrictions>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>yes</lom:value>
  </lom:copyrightAndOtherRestrictions>
  <lom:description>
   <lom:string language="fre">cc-by-sa</lom:string>
  </lom:description>
 </lom:rights>
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section6();

    /**
      * Exporte la section 7 Relation
      *
      * @note Exemple de code XML qui sera généré:
      * @code
 <lom:relation>
  <lom:kind>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>references</lom:value>
  </lom:kind>
  <lom:resource>
   <lom:identifier>
    <lom:catalog>URI</lom:catalog>
    <lom:entry>http://www.ryxeo.com/</lom:entry>
   </lom:identifier>
   <lom:description>
    <lom:string language="fre">Présentation de la ressource.
Là c'est un test avec un lien vers le site de la société.</lom:string>
   </lom:description>
  </lom:resource>
 </lom:relation>
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section7();

    /**
      * Exporte la section 8 Annotation
      *
      * @note Exemple de code XML qui sera généré:
      * @code
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section8();

    /**
      * Exporte la section 9 Classification
      *
      * @note Exemple de code XML qui sera généré:
      * @code
 <lom:classification>
  <lom:purpose>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>educational level</lom:value>
  </lom:purpose>
  <lom:taxonPath>
   <lom:source>
    <lom:string language="fre">nomenclature niveaux Sceren V 2.1</lom:string>
   </lom:source>
   <lom:taxon>
    <lom:id></lom:id>
    <lom:entry>
     <lom:string language="fre">voies générale et technologique collège cycle d'orientation 3e</lom:string>
    </lom:entry>
   </lom:taxon>
   <lom:taxon>
    <lom:id></lom:id>
    <lom:entry>
     <lom:string language="fre">voies générale et technologique lycée</lom:string>
    </lom:entry>
   </lom:taxon>
   <lom:taxon>
    <lom:id></lom:id>
    <lom:entry>
     <lom:string language="fre">voie professionnelle lycée CAP CAP Santé-Social</lom:string>
    </lom:entry>
   </lom:taxon>
  </lom:taxonPath>
 </lom:classification>
      * @endcode
      */
    virtual QByteArray p_abeLOMExportAsXML_section9();

    //============================================================================= IMPORT XML
    /**
      * importe la section 1 Général
      *
      * @note Exemple de code XML source
      * @param e noeud XML de la branche 1: general
      * @code
   <lom:general>
      <lom:identifier>
         <lom:catalog>URI</lom:catalog>
         <lom:entry>http://mediatheque.abuledu.org/URI/5343xdfg32</lom:entry>
      </lom:identifier>
      <lom:title>
         <lom:string language="fre">Le titre de la ressource</lom:string>
      </lom:title>
      <lom:language>fre</lom:language>
      <lom:description>
         <lom:string language="fre">Résumé de la ressource.</lom:string>
      </lom:description>
      <lom:keyword>
         <lom:string language="fre">19e-20e siècle</lom:string>
      </lom:keyword>
      <lom:keyword>
         <lom:string language="fre">lutte politique, résistance : politique</lom:string>
      </lom:keyword>
      <lom:structure>
         <lom:source>LOMv1.0</lom:source>
         <lom:value>networked</lom:value>
      </lom:structure>
      <lom:aggregationLevel>
         <lom:source>LOMv1.0</lom:source>
         <lom:value>2</lom:value>
      </lom:aggregationLevel>
   </lom:general>
      * @endcode
      */
    virtual bool p_abeLOMImportXML_section1(QDomElement n);

    /**
      * Import la section 2 Cycle de vie
      * @param e noeud XML de la branche 2: lifeCycle
      * @code
    <lom:lifeCycle>
        <lom:version>
            <lom:string language="fre">Un intitulé libre de version</lom:string>
        </lom:version>
        <lom:status>
            <lom:source>LOMv1.0</lom:source>
            <lom:value>final</lom:value>
        </lom:status>
        <lom:contribute>
            <lom:role>
                <lom:source>LOMv1.0</lom:source>
                <lom:value>author</lom:value>
            </lom:role>
            <lom:entity>BEGIN:VCARD
VERSION:3.0
N:Nom; Prenom;;;
FN:Nom Prenom
ORG:Nom de l'organisme du "contributeur";;
EMAIL;INTERNET:adresse.email@domaine.fr
END:VCARD
            </lom:entity>
            <lom:date>
                <lom:dateTime>2012-04-17</lom:dateTime>
                <lom:description>
                    <lom:string language="fre">un petit commentaire</lom:string>
                </lom:description>
            </lom:date>
        </lom:contribute>
    </lom:lifeCycle>
      * @endcode
      */
    virtual bool p_abeLOMImportXML_section2(QDomNode n);

    virtual bool p_abeLOMImportXML_section3(QDomNode n);
    virtual bool p_abeLOMImportXML_section4(QDomNode n);
    virtual bool p_abeLOMImportXML_section5(QDomNode n);
    virtual bool p_abeLOMImportXML_section6(QDomNode n);

    /**
      * Import la section 7 relations
      * @param e noeud XML de la branche 7: relation
      * @code
    <lom:relation>
        <lom:kind>
            <lom:source>LOMv1.0</lom:source>
            <lom:value>ispartof</lom:value>
            <!--ispartof/haspart/isversionof/hasversion/isformatof/hasformat/isreferencedby/references/isbasedon/isbasisfor/requires/isrequiredby -->
        </lom:kind>
        <lom:resource>
            <lom:identifier>
                <lom:catalog>nom du catalogue où la ressource destination est stockée</lom:catalog>
                <lom:entry> Valeur de la ressource destination avec lequel est fait le lien. Cela
                    peut être url du document ou du chapitre ou autre moyen qui permet de retrouver
                    le document</lom:entry>
            </lom:identifier>
            <lom:description>
                <lom:string language="fre">Description de la ressource "destination"</lom:string>
            </lom:description>
        </lom:resource>
    </lom:relation>
      */
    virtual bool p_abeLOMImportXML_section7(QDomNode n);


    /**
    <lom:annotation>
        <!-- L'usage des VCard est préconisée pour être en conformité avec la politique de modernisation de l'état-->
        <lom:entity>[BEGIN:VCARD VERSION:3.0 N:Nom; Prenom;;; FN:Nom Prenom ORG:Nom de l'organisme
            du "contributeur" EMAIL;INTERNET:Nom@NomOrganisme.fr END:VCARD]</lom:entity>
        <lom:date>
            <lom:dateTime>2008</lom:dateTime>
            <!-- Date de la contribution -->
            <!-- La date doit être présentée sous cette forme: AAAA-MM-JJ -->
            <lom:description>
                <lom:string language="fre">commentaire</lom:string>
            </lom:description>
        </lom:date>
        <lom:description>
            <lom:string language="fre">Contenu du commentaire sur l'utilisation de la
            ressource</lom:string>
        </lom:description>
    </lom:annotation>
      */
    virtual bool p_abeLOMImportXML_section8(QDomNode n);

    /**
      * Import la section 9 classification
      * @param e noeud XML de la branche 9: classification
      * @code
 <lom:classification>
  <lom:purpose>
   <lom:source>LOMv1.0</lom:source>
   <lom:value>educational level</lom:value>
  </lom:purpose>
  <lom:taxonPath>
   <lom:source>
    <lom:string language="fre">nomenclature niveaux Sceren V 2.1</lom:string>
   </lom:source>
   <lom:taxon>
    <lom:id></lom:id>
    <lom:entry>
     <lom:string language="fre">voies générale et technologique collège cycle d'orientation 3e</lom:string>
    </lom:entry>
   </lom:taxon>
   <lom:taxon>
    <lom:id></lom:id>
    <lom:entry>
     <lom:string language="fre">voies générale et technologique lycée</lom:string>
    </lom:entry>
   </lom:taxon>
   <lom:taxon>
    <lom:id></lom:id>
    <lom:entry>
     <lom:string language="fre">voie professionnelle lycée CAP CAP Santé-Social</lom:string>
    </lom:entry>
   </lom:taxon>
  </lom:taxonPath>
 </lom:classification>
      */
    virtual bool p_abeLOMImportXML_section9(QDomNode n, int numSection9);


    //============================================================================= PROTECTED (Générales)
    bool p_abeLOMaddLifeCycleContributionRole(QString role, vCard entity, QDate ladate);


private:
    QString      m_xmlPrefix;  /** Pour prefixer tous les objets XML */
  };

#endif // ABULEDULOMV1_H
