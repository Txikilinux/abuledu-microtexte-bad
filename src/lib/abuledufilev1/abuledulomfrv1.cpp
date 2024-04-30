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

#include "abuledulomfrv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduLOMFrV1::AbulEduLOMFrV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduLOMV1();

    m_xmlPrefix = "lomfr:";

    //1.9
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("collection"),  QObject::trUtf8("collection"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("ensemble de données"),  QObject::trUtf8("ensemble de données"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("évènement"),  QObject::trUtf8("évènement"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("logiciel"),  QObject::trUtf8("logiciel"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("objet physique"),  QObject::trUtf8("objet physique"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("ressource interactive"),  QObject::trUtf8("ressource interactive"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("service"),  QObject::trUtf8("service"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("son"),  QObject::trUtf8("son"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("texte"),  QObject::trUtf8("texte"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("image fixe"),  QObject::trUtf8("image fixe"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("image en mouvement"),  QObject::trUtf8("image en mouvement"));
    m_GeneralDocumentTypeVocabLOMFR.insert(QString::fromUtf8("image"),  QObject::trUtf8("image"));

    //2.3
    m_LifeCycleContributionRoleVocabLOMFR.insert(QString(""),  QObject::trUtf8("Non renseigné"));
    m_LifeCycleContributionRoleVocabLOMFR.insert(QString::fromUtf8("contributeur"), "contributeur");

    //5.2
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("démonstration"),  QObject::trUtf8("démonstration"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("évaluation"),  QObject::trUtf8("évaluation"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("animation"),  QObject::trUtf8("animation"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("tutoriel"),  QObject::trUtf8("tutoriel"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("glossaire"),  QObject::trUtf8("glossaire"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("guide"),  QObject::trUtf8("guide"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("matériel de référence"),  QObject::trUtf8("matériel de référence"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("méthodologie"),  QObject::trUtf8("méthodologie"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("outil"),  QObject::trUtf8("outil"));
    m_EducationalLearningResourceTypeVocabLOMFR.insert(QString::fromUtf8("scénario pédagogique"),  QObject::trUtf8("scénario pédagogique"));

    //5.6
    m_EducationalContextVocabLOMFR.insert(QString(""),  QObject::trUtf8("Non renseigné"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("enseignement primaire"),  QObject::trUtf8("enseignement primaire"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("enseignement secondaire"),  QObject::trUtf8("enseignement secondaire"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("licence"),  QObject::trUtf8("licence"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("master"),  QObject::trUtf8("master"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("doctorat"),  QObject::trUtf8("doctorat"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("formation continue"),  QObject::trUtf8("formation continue"));
    m_EducationalContextVocabLOMFR.insert(QString::fromUtf8("formation en entreprise"),  QObject::trUtf8("formation en entreprise"));

    //5.12
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("animer"),  QObject::trUtf8("animer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("apprendre"),  QObject::trUtf8("apprendre"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("collaborer"),  QObject::trUtf8("collaborer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("communiquer"),  QObject::trUtf8("communiquer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("coopérer"),  QObject::trUtf8("coopérer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("créer"),  QObject::trUtf8("créer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("échanger"),  QObject::trUtf8("échanger"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("lire"),  QObject::trUtf8("lire"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("observer"),  QObject::trUtf8("observer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("organiser"),  QObject::trUtf8("organiser"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("produire"),  QObject::trUtf8("produire"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("publier"),  QObject::trUtf8("publier"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("rechercher"),  QObject::trUtf8("rechercher"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("s'auto-former"),  QObject::trUtf8("s'auto-former"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("s'exercer"),  QObject::trUtf8("s'exercer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("s'informer"),  QObject::trUtf8("s'informer"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("se former"),  QObject::trUtf8("se former"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("simuler"),  QObject::trUtf8("simuler"));
    m_EducationalActivitiesVocabLOMFR.insert(QString::fromUtf8("s'évaluer"),  QObject::trUtf8("s'évaluer"));

    //7.1?
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8("est associée à"),  QObject::trUtf8("est associée à"));
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8("est la traduction de"),  QObject::trUtf8("est la traduction de"));
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8("fait l'objet d'une traduction"),  QObject::trUtf8("fait l'objet d'une traduction"));
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8("est la localisation de"),  QObject::trUtf8("est la localisation de"));
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8("fait l'objet d'une localisation"),  QObject::trUtf8("fait l'objet d'une localisation"));
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8("est prérequis de"),  QObject::trUtf8("est prérequis de"));
    m_RelationKindVocabLOMFR.insert(QString::fromUtf8(" a pour prérequis"),  QObject::trUtf8(" a pour prérequis"));

    //9.1 -> aucune definition dans lomfrVocabValues.xsd
    //    m_ClassificationPurposeVocabLOMFR.insert(QString::fromUtf8(""),"");
}

AbulEduLOMFrV1::~AbulEduLOMFrV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

QByteArray AbulEduLOMFrV1::abeLOMExportAsXML()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QByteArray retour;
    if(m_GeneralIdentifierCatalog == "" || m_GeneralIdentifierEntry == "") {
        ABULEDU_LOG_DEBUG() << "La norme LOMFR n'est pas respectée: le GeneralIdentifierCatalog est vide";
        return retour;
    }

    p_abeLOMExportAsXML_header();
    p_abeLOMExportAsXML_section1();
    p_abeLOMExportAsXML_section2();
    p_abeLOMExportAsXML_section3();
    p_abeLOMExportAsXML_section4();
    p_abeLOMExportAsXML_section5();
    p_abeLOMExportAsXML_section6();
    p_abeLOMExportAsXML_section7();
    p_abeLOMExportAsXML_section8();
    p_abeLOMExportAsXML_section9();

    QByteArray XML  = m_domDocument.toByteArray();
    ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::abeLOMExportAsXML (2)";
    return XML;
}

QMap<QString,QString> AbulEduLOMFrV1::abeLOMgetGeneralDocumentTypeVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralDocumentTypeVocabLOMFR;
}

QMap<QString, QString> AbulEduLOMFrV1::abeLOMgetLifeCycleContributionRoleVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_LifeCycleContributionRoleVocabLOMFR;
}

QList<QDate> AbulEduLOMFrV1::abeLOMgetLifeCycleContributionDates(QString role)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << role;
    QList<QDate> dates;
    //Et on ajoute les dates dont le role correspond au vocabulaire local
    //On cherche si role est une valeur
    QString nomRole = abeLOMgetLifeCycleContributionRoleVocabLOMFR().key(role);
    //Peut-être qu'on a passé une clé ?
    if(nomRole.isEmpty()) {
        if(abeLOMgetLifeCycleContributionRoleVocabLOMFR().keys().contains(role)) {
            nomRole = role;
        }
        else {
            //On passera au parent via l'héritage
        }
    }

    //Si c'est pas dans le vocabulaire local, on passe a la classe au dessus
    if(nomRole.isEmpty()) {
        dates = AbulEduLOMV1::abeLOMgetLifeCycleContributionDates(role);
    }
    else {
        //ATTENTION, VIOLENT COPIER/COLLER DE AbulEduLOMV1::abeLOMgetLifeCycleContributionDates
        QMapIterator<QString, QList<vCard> > i(m_LifeCycleContributions);
        while (i.hasNext()) {
            i.next();
            QString ladate = i.key();
            if(ladate.contains(nomRole)) {
                ladate.remove(nomRole + ":");
                dates << QDate::fromString(ladate,"yyyy-MM-dd");
            }
        }
    }

    return dates;
}

bool AbulEduLOMFrV1::abeLOMaddLifeCycleContributionRole(QString role, vCard entity, QDate ladate)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << role;
    QString nomRole = abeLOMgetLifeCycleContributionRoleVocabLOMFR().key(role);
    //Peut-être qu'on a passé une clé ?
    if(nomRole.isEmpty()) {
        if(abeLOMgetLifeCycleContributionRoleVocabLOMFR().keys().contains(role)) {
            nomRole = role;
        }
        else {
            //On passera au parent via l'héritage
        }
    }

    //Si c'est pas dans notre vocabulaire, on passe au parent
    if(nomRole.isEmpty()) {
        return AbulEduLOMV1::abeLOMaddLifeCycleContributionRole(role, entity, ladate);
    }

    return p_abeLOMaddLifeCycleContributionRole(role, entity, ladate);
}

void AbulEduLOMFrV1::abeLOMclearLifeCycleContributionRole()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_LifeCycleContributions.clear();
}

bool AbulEduLOMFrV1::abeLOMaddGeneralDocumentType(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
    bool retour;
    //    qDebug() << "AbulEduLOMFrV1::abeLOMaddGeneralDocumentType(QString value)";
    //    qDebug() << value;
    //    qDebug() << "ensemble de données";
    //    qDebug() << m_GeneralDocumentTypeVocabLOMFR;

    if(m_GeneralDocumentTypeVocabLOMFR.contains(value)) {
        if(! m_GeneralDocumentType.contains("LOMFRv1.0;" + value)) {
            m_GeneralDocumentType << "LOMFRv1.0;" + value;
        }
        retour = true;
    }
    else
        retour = false;
    //    qDebug() << m_GeneralDocumentType;
    return retour;
}

QStringList AbulEduLOMFrV1::abeLOMgetGeneralDocumentType()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralDocumentType;
}


bool AbulEduLOMFrV1::abeLOMaddEducationalActivities(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
    bool retour;
    if(m_EducationalActivitiesVocabLOMFR.contains(value)) {
        if(!m_EducationalActivities.contains("LOMFRv1.0;" + value))
            m_EducationalActivities << "LOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QStringList AbulEduLOMFrV1::abeLOMgetEducationalActivities()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalActivities;
}

QMap<QString,QString> AbulEduLOMFrV1::abeLOMgetEducationalActivitiesVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalActivitiesVocabLOMFR;
}

void AbulEduLOMFrV1::abeLOMclearEducationalActivities()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalActivities.clear();
}

bool AbulEduLOMFrV1::abeLOMaddEducationalValidation(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG() <<"AbulEduLOMFrV1::abeLOMaddEducationalValidation("<<lang<<","<<value<<")";
    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (abeLOMgetAvailableLanguages().keys().contains(lang))
        {
            m_EducationalValidation.add(lang,value);
            return true;
        }
    }
    return false;
}

void AbulEduLOMFrV1::abeLOMclearEducationalValidations()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalValidation.clear();
    return;
}

QStringList AbulEduLOMFrV1::abeLOMgetEducationalValidation(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalValidation.getList(lang);
}

QList<QString> AbulEduLOMFrV1::abeLOMgetEducationalValidationKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalValidation.keys();
}

bool AbulEduLOMFrV1::abeLOMaddEducationalLearningResourceType(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
    bool retour;
    if(m_EducationalLearningResourceTypeVocabLOMFR.contains(value)) {
        if(!m_EducationalLearningResourceType.contains("LOMFRv1.0;" + value))
            m_EducationalLearningResourceType << "LOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = AbulEduLOMV1::abeLOMaddEducationalLearningResourceType(value);
    return retour;
}

QMap<QString, QString> AbulEduLOMFrV1::abeLOMgetEducationalLearningResourceTypeVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLearningResourceTypeVocabLOMFR;
}

bool AbulEduLOMFrV1::abeLOMaddEducationalContext(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
//    QString keyValue = m_EducationalContextVocabLOMFR.key(value);
//    if (keyValue.isEmpty())
//    {
//        return false;
//    }
    bool retour;
    if(m_EducationalContextVocabLOMFR.values().contains(value)) {
        QString key = m_EducationalContextVocabLOMFR.key(value);
        if(!m_EducationalContext.contains("LOMFRv1.0;" + key)) {
            m_EducationalContext << "LOMFRv1.0;" + key;
        }
        retour = true;
        ABULEDU_LOG_DEBUG()<<"----- "<<value<<" appartient à m_EducationalContextVocabLOMFR";
    }
    //Si c'est une cle (mot en langue originale)
    else if(m_EducationalContextVocabLOMFR.keys().contains(value)) {
        if(!m_EducationalContext.contains("LOMFRv1.0;" + value)) {
            m_EducationalContext << "LOMFRv1.0;" + value;
        }
        retour = true;
    }
    else {
        retour = AbulEduLOMV1::abeLOMaddEducationalContext(value);
    }
    return retour;
}

QMap<QString, QString> AbulEduLOMFrV1::abeLOMgetEducationalContextVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalContextVocabLOMFR;
}

void AbulEduLOMFrV1::abeLOMclearEducationalContext()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalContext.clear();
    return;
}

bool AbulEduLOMFrV1::abeLOMsetRelationKind(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
    bool retour;
    if(m_RelationKindVocabLOMFR.contains(value)) {
        m_RelationKind="LOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = AbulEduLOMV1::abeLOMsetRelationKind(value);
    return retour;
}

QMap<QString,QString> AbulEduLOMFrV1::abeLOMgetRelationKindVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RelationKindVocabLOMFR;
}

/* definition du vocabulaire vide ... on garde au cas ou
bool AbulEduLOMFrV1::abeLOMaddClassificationPurpose(QString purpose, QString source, QString id, QString entry)
{
    implémentation à écrire, mais l'idée c'est de regarder si le vocabulaire vient de ScoLOMFRV1, auquel cas on transmet,
    de regarder s'il vient d'ici, auquel cas on ajoute un préfixe LOMFRv1, de transmettre dans les deux autres cas
    dans tous les cas, on appelle la fonction éponyme de AbuleEduLOMv1
} */

QMap<QString,QString> AbulEduLOMFrV1::abeLOMgetClassificationPurposeVocabLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationPurposeVocabLOMFR;
}

QByteArray AbulEduLOMFrV1::p_abeLOMExportAsXML_header()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_domDocument.clear();
    QDomProcessingInstruction header = m_domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    m_domDocument.appendChild( header );

    QDomElement officeDocContent = m_domDocument.createElementNS("http://ltsc.ieee.org/xsd/LOM","lomfr:lom");
    m_domDocument.appendChild(officeDocContent);

    officeDocContent.setAttribute("xmlns:lomfr","http://www.lom-fr.fr/xsd/LOMFR");
    officeDocContent.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    officeDocContent.setAttribute("xsi:schemaLocation","http://ltsc.ieee.org/xsd/LOM http://ltsc.ieee.org/xsd/lomv1.0/lom.xsd");

    m_domRootElement = officeDocContent;

    QByteArray XML  = m_domDocument.toByteArray();
    ABULEDU_LOG_DEBUG() << "AbulEduLOMFrV1::abeLOMExportAsXML_header (2)";
    return XML;
}

QByteArray AbulEduLOMFrV1::p_abeLOMExportAsXML_section1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    AbulEduLOMV1::p_abeLOMExportAsXML_section1();

    e = m_domCurrentElement;

    // 1.9
    if(m_GeneralDocumentType.count()) {
        QListIterator<QString> i(abeLOMgetGeneralDocumentType());
        while (i.hasNext()) {
            QStringList contenu = i.next().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "documentType");
            e.appendChild(e_fils);
            {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(cle);
                e_fils2.appendChild(t);

                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(valeur);
                e_fils2.appendChild(t);
            }
        }
    }

    QByteArray XML  = m_domDocument.toByteArray();
    ABULEDU_LOG_DEBUG() << "AbulEduLOMFrV1::p_abeLOMExportAsXML_section1 (1)";
    return XML;
}

QByteArray AbulEduLOMFrV1::p_abeLOMExportAsXML_section5()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    AbulEduLOMV1::p_abeLOMExportAsXML_section5();

    e = m_domCurrentElement;

    //5.12
    if(abeLOMgetEducationalActivities().count()) {
        QListIterator<QString> i(abeLOMgetEducationalActivities());
        while (i.hasNext()) {
            QStringList contenu = i.next().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "activity");
            e.appendChild(e_fils);
            {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(cle);
                e_fils2.appendChild(t);

                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(valeur);
                e_fils2.appendChild(t);
            }
        }
    }

    // 5.13
    if(abeLOMgetEducationalValidationKeys().count()) {
        QListIterator<QString> i(abeLOMgetEducationalValidationKeys());
        while (i.hasNext()) {
            QString lang = i.next();

            QListIterator<QString> j(abeLOMgetEducationalValidation(lang));
            while (j.hasNext()) {
                QString valeur = j.next();
                e_fils = m_domDocument.createElement(m_xmlPrefix + "credit");
                e.appendChild(e_fils);

                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                e_fils2.setAttribute("language",lang);
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(valeur);
                e_fils2.appendChild(t);
            }
        }
    }

    QByteArray XML  = m_domDocument.toByteArray();
    ABULEDU_LOG_DEBUG() << "AbulEduLOMFrV1::p_abeLOMExportAsXML_section5 (1)";
    return XML;
}
bool AbulEduLOMFrV1::p_abeLOMImportXML_section1(QDomElement n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduLOMV1::p_abeLOMImportXML_section1(n);

    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();

            //1.1 Identifiant
            if(e.tagName() == m_xmlPrefix + "documentType") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                if(!e2.isNull()) {
                    abeLOMaddGeneralDocumentType(e2.text());
                }
            }
        }
        fils = fils.nextSibling();
    }
    return true;
}

bool AbulEduLOMFrV1::p_abeLOMImportXML_section5(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduLOMV1::p_abeLOMImportXML_section5(n);

    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();

            //5.12 activités induites
            if(e.tagName() == m_xmlPrefix + "activity") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalActivities(e2.text());
            }

            //TODO
            //5.13 validation des acquis ??? element perdu, quasi aucune doc !

            //SUITE DANS SCOLOMFR
        }
        fils = fils.nextSibling();
    }
    ABULEDU_LOG_DEBUG() << "AbulEduLOMFrV1::p_abeLOMImportXML_section5() (2)";
    return true;
}

