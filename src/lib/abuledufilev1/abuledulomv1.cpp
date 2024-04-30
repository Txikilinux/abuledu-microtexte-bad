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

#include "abuledulomv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduLOMV1::AbulEduLOMV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //Le prefixe qui est ajouté un peu a la hache dans les fichiers XML
    m_xmlPrefix = "lom:";

    //Valeur par defaut abuledu*
    m_GeneralIdentifierCatalog = "URI";

    //Initialisation de la liste des langues
    m_availableLanguages.insert("",trUtf8("Non renseignée"));
    m_availableLanguages.insert("fre",trUtf8("Français"));
    m_availableLanguages.insert("eng",trUtf8("Anglais"));
    m_availableLanguages.insert("spa",trUtf8("Espagnol"));
    m_availableLanguages.insert("ita",trUtf8("Italien"));
    m_availableLanguages.insert("ger",trUtf8("Allemand"));
    m_availableLanguages.insert("ara",trUtf8("Arabe"));
    m_availableLanguages.insert("dut",trUtf8("Néerlandais"));
    m_availableLanguages.insert("oci",trUtf8("Occitan"));

    //Initialisation des listes du vocabublaire obligatoire
    //Vocabulaire de la structure 1.7
    m_GeneralStructureVocabLOM.insert("atomic", QObject::trUtf8("atomique"));
    m_GeneralStructureVocabLOM.insert("collection", QObject::trUtf8("collection"));
    m_GeneralStructureVocabLOM.insert("hierarchical", QObject::trUtf8("hiérarchique"));
    m_GeneralStructureVocabLOM.insert("linear", QObject::trUtf8("linéaire"));
    m_GeneralStructureVocabLOM.insert("networked", QObject::trUtf8("en réseau"));

    //structure 1.8
    m_GeneralAggregationLevelVocabLOM.insert("1",  QObject::trUtf8("1"));
    m_GeneralAggregationLevelVocabLOM.insert("2",  QObject::trUtf8("2"));
    m_GeneralAggregationLevelVocabLOM.insert("3",  QObject::trUtf8("3"));
    m_GeneralAggregationLevelVocabLOM.insert("4",  QObject::trUtf8("4"));

    //structure 2.2
    m_LifeCycleStatusVocabLOM.insert("draft",  QObject::trUtf8("brouillon"));
    m_LifeCycleStatusVocabLOM.insert("final",  QObject::trUtf8("définitive"));
    m_LifeCycleStatusVocabLOM.insert("revised",  QObject::trUtf8("relue"));
    m_LifeCycleStatusVocabLOM.insert("unvailable",  QObject::trUtf8("inconnue"));

    //structure 2.3.1
    m_LifeCycleContributionRoleVocabLOM.insert("author",  QObject::trUtf8("auteur"));
    m_LifeCycleContributionRoleVocabLOM.insert("publisher",  QObject::trUtf8("diffuseur"));
    m_LifeCycleContributionRoleVocabLOM.insert("unknown",  QObject::trUtf8("inconnu"));
    m_LifeCycleContributionRoleVocabLOM.insert("initiator",  QObject::trUtf8("créateur"));
    m_LifeCycleContributionRoleVocabLOM.insert("terminator",  QObject::trUtf8("modérateur"));
    m_LifeCycleContributionRoleVocabLOM.insert("validator",  QObject::trUtf8("contrôleur"));
    m_LifeCycleContributionRoleVocabLOM.insert("editor",  QObject::trUtf8("éditeur"));
    m_LifeCycleContributionRoleVocabLOM.insert("graphical designer",  QObject::trUtf8("concepteur graphique"));
    m_LifeCycleContributionRoleVocabLOM.insert("technical implementer",  QObject::trUtf8("développeur"));
    m_LifeCycleContributionRoleVocabLOM.insert("content provider",  QObject::trUtf8("fournisseur de contenu"));
    m_LifeCycleContributionRoleVocabLOM.insert("technical validator",  QObject::trUtf8("contrôleur technique"));
    m_LifeCycleContributionRoleVocabLOM.insert("educational validator",  QObject::trUtf8("contrôleur pédagogique"));
    m_LifeCycleContributionRoleVocabLOM.insert("script writer",  QObject::trUtf8("rédacteur"));
    m_LifeCycleContributionRoleVocabLOM.insert("instructional designer",  QObject::trUtf8("concepteur didactique"));
    m_LifeCycleContributionRoleVocabLOM.insert("subject matter expert",  QObject::trUtf8("expert conseil"));

    //structure 5.1
    m_EducationalInteractivityTypeVocabLOM.insert("active",  QObject::trUtf8("actif"));
    m_EducationalInteractivityTypeVocabLOM.insert("expositive",  QObject::trUtf8("explicatif"));
    m_EducationalInteractivityTypeVocabLOM.insert("mixed",  QObject::trUtf8("mixte"));

    //5.2
    m_EducationalLearningResourceTypeVocabLOM.insert("exercise",  QObject::trUtf8("exercice"));
    m_EducationalLearningResourceTypeVocabLOM.insert("simulation",  QObject::trUtf8("simulation"));
    m_EducationalLearningResourceTypeVocabLOM.insert("questionnaire",  QObject::trUtf8("questionnaire"));
    m_EducationalLearningResourceTypeVocabLOM.insert("diagram",  QObject::trUtf8("diagramme"));
    m_EducationalLearningResourceTypeVocabLOM.insert("figure",  QObject::trUtf8("figure"));
    m_EducationalLearningResourceTypeVocabLOM.insert("graph",  QObject::trUtf8("graphique"));
    m_EducationalLearningResourceTypeVocabLOM.insert("index",  QObject::trUtf8("index"));
    m_EducationalLearningResourceTypeVocabLOM.insert("slide",  QObject::trUtf8("diapositive"));
    m_EducationalLearningResourceTypeVocabLOM.insert("table",  QObject::trUtf8("tableau"));
    m_EducationalLearningResourceTypeVocabLOM.insert("narrative text",  QObject::trUtf8("texte narratif"));
    m_EducationalLearningResourceTypeVocabLOM.insert("exam",  QObject::trUtf8("examen"));
    m_EducationalLearningResourceTypeVocabLOM.insert("experiment",  QObject::trUtf8("expérience"));
    m_EducationalLearningResourceTypeVocabLOM.insert("problem statement",  QObject::trUtf8("énoncé"));
    m_EducationalLearningResourceTypeVocabLOM.insert("self assessment",  QObject::trUtf8("auto-évaluation"));
    m_EducationalLearningResourceTypeVocabLOM.insert("lecture",  QObject::trUtf8("lecture"));

    //structure 5.5
    m_EducationalIntendedEndUserRoleVocabLOM.insert("teacher",  QObject::trUtf8("enseignant"));
    m_EducationalIntendedEndUserRoleVocabLOM.insert("author",  QObject::trUtf8("auteur"));
    m_EducationalIntendedEndUserRoleVocabLOM.insert("learner",  QObject::trUtf8("apprenant"));
    m_EducationalIntendedEndUserRoleVocabLOM.insert("manager",  QObject::trUtf8("directeur"));

    // 5.6
    m_EducationalContextVocabLOM.insert("school",  QObject::trUtf8("école"));
    m_EducationalContextVocabLOM.insert("higher education",  QObject::trUtf8("enseignement supérieur"));
    m_EducationalContextVocabLOM.insert("training",  QObject::trUtf8("entraînement"));
    m_EducationalContextVocabLOM.insert("other",  QObject::trUtf8("autre"));

    //5.8
    m_EducationalDifficultyVocabLOM.insert("very easy",  QObject::trUtf8("très facile"));
    m_EducationalDifficultyVocabLOM.insert("easy",  QObject::trUtf8("facile"));
    m_EducationalDifficultyVocabLOM.insert("medium",  QObject::trUtf8("moyen"));
    m_EducationalDifficultyVocabLOM.insert("difficult",  QObject::trUtf8("difficile"));
    m_EducationalDifficultyVocabLOM.insert("very difficult",  QObject::trUtf8("très difficile"));

    //6.1
    m_RightsCostVocabLOM.insert(QString(""), QObject::trUtf8("Non renseigné"));
    m_RightsCostVocabLOM.insert("yes",  QObject::trUtf8("oui"));
    m_RightsCostVocabLOM.insert("no",  QObject::trUtf8("non"));

    //6.2
    m_RightsCopyrightAndOtherRestrictionsVocabLOM.insert(QString(""), QObject::trUtf8("Non renseigné"));
    m_RightsCopyrightAndOtherRestrictionsVocabLOM.insert("yes",  QObject::trUtf8("oui"));
    m_RightsCopyrightAndOtherRestrictionsVocabLOM.insert("no",  QObject::trUtf8("non"));

    //7.1
    m_RelationKindVocabLOM.insert("ispartof",  QObject::trUtf8("fait partie de"));
    m_RelationKindVocabLOM.insert("haspart",  QObject::trUtf8("a des parties"));
    m_RelationKindVocabLOM.insert("isversionof",  QObject::trUtf8("est une version de"));
    m_RelationKindVocabLOM.insert("hasversion",  QObject::trUtf8("est versionné"));
    m_RelationKindVocabLOM.insert("isformatof",  QObject::trUtf8("est le format de"));
    m_RelationKindVocabLOM.insert("hasformat",  QObject::trUtf8("a un format"));
    m_RelationKindVocabLOM.insert("references",  QObject::trUtf8("référence"));
    m_RelationKindVocabLOM.insert("isreferencedby",  QObject::trUtf8("est référencé par"));
    m_RelationKindVocabLOM.insert("isbasedon",  QObject::trUtf8("est fondé sur"));
    m_RelationKindVocabLOM.insert("isbasisfor",  QObject::trUtf8("est la base de"));
    m_RelationKindVocabLOM.insert("requires",  QObject::trUtf8("nécessite"));
    m_RelationKindVocabLOM.insert("isrequiredby",  QObject::trUtf8("est nécessaire pour"));

    //structure 9.1 Purpose

    /* Présents, avec intitulé correct */

    /* scolomfr-voc-028-num-010 */
    m_ClassificationPurposeVocabLOM.insert("prerequisite",  QObject::trUtf8("pré-requis"));

    /* scolomfr-voc-028-num-009 */
    m_ClassificationPurposeVocabLOM.insert("educational objective",  QObject::trUtf8("objectif pédagogique"));

    /* scolomfr-voc-028-num-005 */
    m_ClassificationPurposeVocabLOM.insert("skill level",  QObject::trUtf8("niveau de compétence"));

    /* scolomfr-voc-028-num-006 */
    m_ClassificationPurposeVocabLOM.insert("security level",  QObject::trUtf8("niveau de sécurité"));

    /* scolomfr-voc-028-num-002 */
    m_ClassificationPurposeVocabLOM.insert("competency",  QObject::trUtf8("compétence"));//Vocabulaire ScoLOMFR-voc-016

    /* scolomfr-voc-028-num-001 */
    /* "Le vocabulaire ScoLOMFR Champ disciplinaire (scolomfr-voc-014) correspond à l'objectif du LOMFR (élément 9.1, discipline)." */
    m_ClassificationPurposeVocabLOM.insert("discipline",  QObject::trUtf8("champ disciplinaire"));//Vocabulaire ScoLOMFR-voc-014

    /* scolomfr-voc-028-num-008 */
    m_ClassificationPurposeVocabLOM.insert("idea",  QObject::trUtf8("notion"));

    /* scolomfr-voc-028-num-012 */
    m_ClassificationPurposeVocabLOM.insert("accessibility restrictions",  QObject::trUtf8("restriction d'accessibilité"));

    /* scolomfr-voc-028-num-007 --> ScoLOMFR */
    m_ClassificationPurposeVocabLOM.insert("educational level",  QObject::trUtf8("niveau éducatif détaillé"));//Vocabulaire ScoLOMFR-voc-022

    /* Absents */

    /* scolomfr-voc-028-num-003 */
    // domaine d'enseignement, pas de TERME_TE, Vocabulaire ScoLOMFR-voc-015

    //Initialisation des variables
    m_EducationalTypicalLearningTime = QTime(0,0,0);
    m_AnnotationDate = QDate(0,0,0);
}

AbulEduLOMV1::~AbulEduLOMV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

// =============================================================================== Code automatique
// généré par le script autoSetGet.php
void AbulEduLOMV1::abeLOMsetGeneralIdentifierCatalog(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralIdentifierCatalog=value;
}
QString AbulEduLOMV1::abeLOMgetGeneralIdentifierCatalog()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralIdentifierCatalog;
}

void AbulEduLOMV1::abeLOMsetGeneralIdentifierEntry(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralIdentifierEntry=value;
}
QString AbulEduLOMV1::abeLOMgetGeneralIdentifierEntry()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralIdentifierEntry;
}

void AbulEduLOMV1::abeLOMsetGeneralTitle(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< lang << " : " << value;
    if(m_availableLanguages.keys().contains(lang))
        m_GeneralTitle.set(lang,value);
    else {
        if(m_availableLanguages.values().contains(lang)) {
            m_GeneralTitle.set(m_availableLanguages.key(lang),value);
        }
    }
}
QString AbulEduLOMV1::abeLOMgetGeneralTitle(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_GeneralTitle.keys().contains(lang)) {
        return m_GeneralTitle.get(lang);
    }
    if(m_availableLanguages.values().contains(lang)) {
        QString pivot(m_availableLanguages.key(lang));
        if(m_GeneralTitle.keys().contains(pivot)) {
            return m_GeneralTitle.get(pivot);
        }
    }
    return "";
}
QList<QString> AbulEduLOMV1::abeLOMgetGeneralTitleKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralTitle.keys();
}

void AbulEduLOMV1::abeLOMsetGeneralLanguage(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralLanguage.clear();
    if(m_availableLanguages.keys().contains(lang))
        m_GeneralLanguage << lang;
    else {
        if(m_availableLanguages.values().contains(lang)) {
            m_GeneralLanguage << m_availableLanguages.key(lang);
        }
    }
}

void AbulEduLOMV1::abeLOMaddGeneralLanguage(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_availableLanguages.keys().contains(lang)) {
        if(!m_GeneralLanguage.contains(lang)) {
            m_GeneralLanguage << lang;
        }
    }
    else {
        if(m_availableLanguages.values().contains(lang)) {
            QString pivot = m_availableLanguages.key(lang);
            //qDebug() << "AbulEduLOMV1::abeLOMaddGeneralLanguage (0b) : " << pivot;

            if(!m_GeneralLanguage.contains(pivot))
                m_GeneralLanguage << pivot;
        }
    }
}
void AbulEduLOMV1::abeLOMsetGeneralLanguage(QStringList lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralLanguage=lang;
}
QStringList AbulEduLOMV1::abeLOMgetGeneralLanguage()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_GeneralLanguage.size() == 0) {
        m_GeneralLanguage << "fre";
    }
    return m_GeneralLanguage;
}

bool AbulEduLOMV1::abeLOMsetGeneralDescription(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(value.trimmed().isEmpty()) {
        return false;
    }
    //qDebug() << "AbulEduLOMV1::abeLOMsetGeneralDescription(0) " << lang << " :: " << value;
    if(m_availableLanguages.keys().contains(lang))
    {
        m_GeneralDescription.set(lang,value);
        return true;
    }
    else
    {
        if(m_availableLanguages.values().contains(lang))
        {
            m_GeneralDescription.set(m_availableLanguages.key(lang),value);
            return true;
        }
        else return false;
    }
}

bool AbulEduLOMV1::abeLOMaddGeneralDescription(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG()<<"AbulEduLOMV1::abeLOMaddGeneralDescription("<<lang<<","<<value<<")";

    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (abeLOMgetAvailableLanguages().keys().contains(lang))
        {
            m_GeneralDescription.add(lang,value);
            return true;
        }
    }
    return false;
}

void AbulEduLOMV1::abeLOMclearGeneralDescription()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralDescription.clear();
    return;
}

QStringList AbulEduLOMV1::abeLOMgetGeneralDescription(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_availableLanguages.values().contains(lang))
    {
        lang = m_availableLanguages.key(lang);
    }
    else if (!m_availableLanguages.keys().contains(lang))
    {
        return QStringList("");
    }
    ABULEDU_LOG_DEBUG()  << m_GeneralDescription.getList(lang);
    //    if(m_GeneralDescription.keys().contains(lang)) {
    //        return m_GeneralDescription.get(lang);
    //    }
    //    if(m_availableLanguages.values().contains(lang)) {
    //        QString pivot(m_availableLanguages.key(lang));
    //        if(m_GeneralDescription.keys().contains(pivot)) {
    //            return m_GeneralDescription.get(pivot);
    //        }
    //    }
    //    return "";

    return m_GeneralDescription.getList(lang);
}
QList<QString> AbulEduLOMV1::abeLOMgetGeneralDescriptionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralDescription.keys();
}

bool AbulEduLOMV1::abeLOMsetGeneralKeyword(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG()<<"AbulEduLOMV1::abeLOMsetGeneralKeyword("<<lang<<","<<value<<")";

    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (abeLOMgetAvailableLanguages().keys().contains(lang))
        {
            m_GeneralKeyword.set(lang,value);
            return true;
        }
    }
    return false;
}

bool AbulEduLOMV1::abeLOMaddGeneralKeyword(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::abeLOMaddGeneralKeyword(" << lang << "," << value << ")";

    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (abeLOMgetAvailableLanguages().keys().contains(lang))
        {
            m_GeneralKeyword.add(lang,value);
            return true;
        }
    }
    return false;
}

void AbulEduLOMV1::abeLOMclearGeneralKeywords()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralKeyword.clear();
    return;
}

QStringList AbulEduLOMV1::abeLOMgetGeneralKeywords(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << lang << " " << m_GeneralKeyword.getList(lang);

    QStringList r;
    if(m_availableLanguages.keys().contains(lang))
        r = m_GeneralKeyword.getList(lang);
    else {
        if(m_availableLanguages.values().contains(lang)) {
            r = m_GeneralKeyword.getList(m_availableLanguages.key(lang));
        }
    }
    return r;
}
QList<QString> AbulEduLOMV1::abeLOMgetGeneralKeywordKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<m_GeneralKeyword.keys();
    return m_GeneralKeyword.keys();
}

bool AbulEduLOMV1::abeLOMsetGeneralCoverage(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_availableLanguages.values().contains(lang))
    {
        lang = m_availableLanguages.key(lang);
    }
    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (m_availableLanguages.keys().contains(lang))
        {
            m_GeneralCoverage.set(lang,value);
            return true;
        }
    }
    return false;
}

bool AbulEduLOMV1::abeLOMaddGeneralCoverage(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "AbulEduLOMV1::abeLOMaddGeneralCoverage(" << lang << "," << value << ")"
                        << "AbulEduLOMV1::abeLOMaddGeneralCoverage(QString lang, QString value)" << lang << ":" << value;

    if (m_availableLanguages.values().contains(lang))
    {
        lang = m_availableLanguages.key(lang);
    }
    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    if(!m_GeneralCoverage.getList(lang).contains(value) && m_availableLanguages.keys().contains(lang)) {
        QStringList t = m_GeneralCoverage.getList(lang);
        t.removeAll("");
        t << value;

        m_GeneralCoverage.set(lang,t);
        return true;
    }
    return false;
}

QString AbulEduLOMV1::abeLOMgetGeneralCoverage(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_availableLanguages.values().contains(lang))
    {
        lang = m_availableLanguages.key(lang);
    }
    if(m_GeneralCoverage.keys().contains(lang))
        return m_GeneralCoverage.get(lang);
    else
        return "";
}

QList<QString> AbulEduLOMV1::abeLOMgetGeneralCoverageKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralCoverage.keys();
}

bool AbulEduLOMV1::abeLOMsetGeneralStructure(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< value;
    bool retour;
    if(m_GeneralStructureVocabLOM.contains(value)) {
        m_GeneralStructure = "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QString AbulEduLOMV1::abeLOMgetGeneralStructure()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralStructure;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetGeneralStructureVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralStructureVocabLOM;
}

bool AbulEduLOMV1::abeLOMsetGeneralAggregationLevel(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_GeneralAggregationLevelVocabLOM.contains(value)) {
        m_GeneralAggregationLevel = "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}
QString AbulEduLOMV1::abeLOMgetGeneralAggregationLevel()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralAggregationLevel;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetGeneralAggregationLevelVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralAggregationLevelVocabLOM;
}

bool AbulEduLOMV1::abeLOMsetLifeCycleVersion(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< lang << ":" << value;
    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    if (m_availableLanguages.values().contains(lang))
    {
        lang = m_availableLanguages.key(lang);
    }
    if (m_availableLanguages.keys().contains(lang))
    {
        m_LifeCycleVersion.set(lang,value);
        return true;
    }
    return false;
}

QString AbulEduLOMV1::abeLOMgetLifeCycleVersion(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_availableLanguages.values().contains(lang))
    {
        lang = m_availableLanguages.key(lang);
    }
    if(m_LifeCycleVersion.keys().contains(lang))
        return m_LifeCycleVersion.get(lang);
    else
        return "";
}

QList<QString> AbulEduLOMV1::abeLOMgetLifeCycleVersionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_LifeCycleVersion.keys();
}

bool AbulEduLOMV1::abeLOMsetLifeCycleStatus(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_LifeCycleStatusVocabLOM.contains(value)) {
        m_LifeCycleStatus = "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QString AbulEduLOMV1::abeLOMgetLifeCycleStatus()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_LifeCycleStatus;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetLifeCycleStatusVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_LifeCycleStatusVocabLOM;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetLifeCycleContributionRoleVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_LifeCycleContributionRoleVocabLOM;
}

bool AbulEduLOMV1::abeLOMaddLifeCycleContributionRole(QString role, vCard entity, QDate ladate)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< role << ladate
                        << entity
                        << "entity : " << entity.getFullName();
    //Empecher qu'on ajoute un meme triplet
    if (!m_LifeCycleContributionRoleVocabLOM.keys().contains(role))
    {
        if (m_LifeCycleContributionRoleVocabLOM.values().contains(role))
        {
            role = m_LifeCycleContributionRoleVocabLOM.key(role);
        }
        else
        {
            ABULEDU_LOG_DEBUG()<<"Ce role n'existe pas...";
            return false;
        }
    }

    return p_abeLOMaddLifeCycleContributionRole(role, entity, ladate);
}

bool AbulEduLOMV1::abeLOMremoveLifeCycleContributionRole(QString role, vCard entity, QDate ladate)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour = false;

    QString recherche(role + ":" + ladate.toString("yyyy-MM-dd"));
    if(m_LifeCycleContributions.contains(recherche)) {
        //on cherche l'entité dans cette structure
        QList<vCard> listeVCardTemp = m_LifeCycleContributions.value(recherche);
        if(listeVCardTemp.contains(entity)) {
            listeVCardTemp.removeAll(entity);
            m_LifeCycleContributions.remove(recherche);
            m_LifeCycleContributions.insert(recherche,listeVCardTemp);
            retour = true;
        }

    }

    ABULEDU_LOG_DEBUG() << m_LifeCycleContributions
                        << "AbulEduLOMV1::abeLOMremoveLifeCycleContributionRole (2)";

    return retour;
}

QDate AbulEduLOMV1::abeLOMgetLifeCycleContributionLastDate(QString role)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<role;
    //Retourne la date la plus "proche" pour un role donné
    //la c'est pas optimal du fait de la structure que j'utilise
    //je suis obligé de parcourir toute la structure ...
    /** @warning attention remarque générale, à vérifier pour tout, on pourrait avoir des roles provenant de LOMFR ... ou SCOLOM (selon les endroits)*/
    QString nomRole = abeLOMgetLifeCycleContributionRoleVocabLOM().key(role);
    QMapIterator<QString, QList<vCard> > i(m_LifeCycleContributions);
    QDate dateA, dateB;
    while (i.hasNext()) {
        i.next();
        QString ladate = i.key();
        if(ladate.contains(nomRole)) {
            ladate.remove(nomRole + ":");
            dateA = QDate::fromString(ladate,"yyyy-MM-dd");

            if(dateA > dateB)
                dateB = dateA;
            //            qDebug() << "****" << role << " -- " << ladate << "::" << dateA << "::" << dateB;
        }
    }
    //La date par défaut: maintenant
    if(dateB.isNull())
        dateB = QDate::currentDate();
    return dateB;
}

QList<QDate> AbulEduLOMV1::abeLOMgetLifeCycleContributionDates(QString role)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<role;
    //Retourne la liste des dates pour un role donné
    //la c'est pas optimal du fait de la structure que j'utilise
    //je suis obligé de parcourir toute la structure ...
    QString nomRole = abeLOMgetLifeCycleContributionRoleVocabLOM().key(role);
    //Peut-être qu'on a passé une clé ?
    if(nomRole.isEmpty()) {
        if(abeLOMgetLifeCycleContributionRoleVocabLOM().keys().contains(role)) {
            nomRole = role;
        }
    }

    QMapIterator<QString, QList<vCard> > i(m_LifeCycleContributions);
    QList<QDate> dates;
    while (i.hasNext()) {
        i.next();
        QString ladate = i.key();
        if(ladate.contains(nomRole)) {
            ladate.remove(nomRole + ":");
            dates << QDate::fromString(ladate,"yyyy-MM-dd");
        }
    }
    return dates;
}

QList<vCard> AbulEduLOMV1::abeLOMgetLifeCycleContributionEntities(QString role, QDate date)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QList<vCard> liste;

    if (!role.isEmpty())
    {
        QString recherche(role + ":" + date.toString("yyyy-MM-dd"));
        if(m_LifeCycleContributions.contains(recherche)) {
            liste << m_LifeCycleContributions.value(recherche);
        }
    }
    if (liste.isEmpty())
    {
        vCard empty = vCard();
        vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, trUtf8("Non renseigné"));
        empty.addProperty(fn_prop);
        liste << empty;
    }
    return liste;
}

QStringList AbulEduLOMV1::abeLOMgetLifeCycleContributionRoles()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QStringList retour;
    if(m_LifeCycleContributions.count() > 0) {
        QStringList liste = m_LifeCycleContributions.keys();
        for(int i = 0; i < liste.count(); i++) {
            if (!retour.contains(liste.at(i).split(":").at(0)))
                retour << liste.at(i).split(":").at(0);
        }
    }
    return retour;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataIdentifierCatalog(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_MetaMetadataIdentifierCatalog=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataIdentifierCatalog()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataIdentifierCatalog;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataIdentifierEntry(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_MetaMetadataIdentifierEntry=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataIdentifierEntry()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataIdentifierEntry;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataContributeRole(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_MetaMetadataContributeRole=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataContributeRole()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataContributeRole;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataContributeEntity(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_MetaMetadataContributeEntity=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataContributeEntity()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataContributeEntity;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataContributeDate(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_MetaMetadataContributeDate=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataContributeDate()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataContributeDate;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataMetadataSchema(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_MetaMetadataMetadataSchema=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataMetadataSchema()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataMetadataSchema;
}

void AbulEduLOMV1::abeLOMsetMetaMetadataLanguage(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier s'il est important que ça soit une langue de type "fre" */
    m_MetaMetadataLanguage=value;
}

QString AbulEduLOMV1::abeLOMgetMetaMetadataLanguage()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_MetaMetadataLanguage;
}

bool AbulEduLOMV1::abeLOMaddTechnicalFormat(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (value.isEmpty())
    {
        return false;
    }
    else
    {
        if(!m_TechnicalFormat.contains(value))
        {
            m_TechnicalFormat << value;
            return true;
        }
    }
    return false;
}

QStringList AbulEduLOMV1::abeLOMgetTechnicalFormat()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalFormat;
}

void AbulEduLOMV1::abeLOMclearTechnicalFormat()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_TechnicalFormat.clear();
}

void AbulEduLOMV1::abeLOMsetTechnicalSize(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_TechnicalSize=value;
}

QString AbulEduLOMV1::abeLOMgetTechnicalSize()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalSize;
}

void AbulEduLOMV1::abeLOMaddTechnicalLocation(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (value.trimmed().isEmpty())
        return;
    if(!m_TechnicalLocation.contains(value))
        m_TechnicalLocation << value;
}

bool AbulEduLOMV1::abeLOMdelTechnicalLocation(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (value.trimmed().isEmpty())
        return false;
    if(m_TechnicalLocation.contains(value)) {
        m_TechnicalLocation.removeAll(value);
        return true;
    }
    return false;
}

QStringList AbulEduLOMV1::abeLOMgetTechnicalLocation()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //On ajoute systématiquement l'URI comme 1ere localisation
    QString principale(abeLOMgetGeneralIdentifierEntry());
    QStringList retour;
    if(!m_TechnicalLocation.contains(principale)) {
        retour << principale;
    }
    retour << m_TechnicalLocation;
    return retour;
}

void AbulEduLOMV1::abeLOMsetTechnicalRequirementOrCompositeType(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_TechnicalRequirementOrCompositeType=value;
}

QString AbulEduLOMV1::abeLOMgetTechnicalRequirementOrCompositeType()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalRequirementOrCompositeType;
}

void AbulEduLOMV1::abeLOMsetTechnicalRequirementOrCompositeName(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_TechnicalRequirementOrCompositeName=value;
}

QString AbulEduLOMV1::abeLOMgetTechnicalRequirementOrCompositeName()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalRequirementOrCompositeName;
}

void AbulEduLOMV1::abeLOMsetTechnicalRequirementOrCompositeMinimumVersion(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_TechnicalRequirementOrCompositeMinimumVersion=value;
}

QString AbulEduLOMV1::abeLOMgetTechnicalRequirementOrCompositeMinimumVersion()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalRequirementOrCompositeMinimumVersion;
}

void AbulEduLOMV1::abeLOMsetTechnicalRequirementOrCompositeMaximumVersion(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_TechnicalRequirementOrCompositeMaximumVersion=value;
}

QString AbulEduLOMV1::abeLOMgetTechnicalRequirementOrCompositeMaximumVersion()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalRequirementOrCompositeMaximumVersion;
}

void AbulEduLOMV1::abeLOMsetTechnicalInstallationRemarks(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    m_TechnicalInstallationRemarks.set(lang,value);
}

QString AbulEduLOMV1::abeLOMgetTechnicalInstallationRemarks(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_TechnicalInstallationRemarks.get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetTechnicalInstallationRemarksKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalInstallationRemarks.keys();
}

void AbulEduLOMV1::abeLOMsetTechnicalOtherPlatformRequirements(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    m_TechnicalOtherPlatformRequirements.set(lang,value);
}

QString AbulEduLOMV1::abeLOMgetTechnicalOtherPlatformRequirements(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_TechnicalOtherPlatformRequirements.get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetTechnicalOtherPlatformRequirementsKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalOtherPlatformRequirements.keys();
}

void AbulEduLOMV1::abeLOMsetTechnicalDuration(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (value == "PT0H0M0S")
    {
        return;
    }
    else
    {
        m_TechnicalDuration=value;
    }
    return;
}

QString AbulEduLOMV1::abeLOMgetTechnicalDuration()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_TechnicalDuration;
}

void AbulEduLOMV1::abeLOMsetEducationalInteractivityType(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalInteractivityType=value;
}

QString AbulEduLOMV1::abeLOMgetEducationalInteractivityType()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalInteractivityType;
}

bool AbulEduLOMV1::abeLOMaddEducationalLearningResourceType(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
    bool retour;
    if(m_EducationalLearningResourceTypeVocabLOM.values().contains(value)) {
        QString key = m_EducationalLearningResourceTypeVocabLOM.key(value);
        if(!m_EducationalLearningResourceType.contains("LOMv1.0;" + key)) {
            m_EducationalLearningResourceType << "LOMv1.0;" + key;
        }
        retour = true;
    }
    //Si c'est une cle (mot en langue originale)
    else if(m_EducationalLearningResourceTypeVocabLOM.keys().contains(value)) {
        if(!m_EducationalLearningResourceType.contains("LOMv1.0;" + value)) {
            m_EducationalLearningResourceType << "LOMv1.0;" + value;
        }
        retour = true;
    }
    else {
        retour = false;
    }
    return retour;
}

QStringList AbulEduLOMV1::abeLOMgetEducationalLearningResourceType()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLearningResourceType;
}

QMap<QString,QString> AbulEduLOMV1::abeLOMgetEducationalLearningResourceTypeVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLearningResourceTypeVocabLOM;
}

void AbulEduLOMV1::abeLOMsetEducationalInteractivityLevel(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalInteractivityLevel=value;
}

QString AbulEduLOMV1::abeLOMgetEducationalInteractivityLevel()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalInteractivityLevel;
}

void AbulEduLOMV1::abeLOMsetEducationalSemanticDensity(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalSemanticDensity=value;
}

QString AbulEduLOMV1::abeLOMgetEducationalSemanticDensity()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalSemanticDensity;
}

bool AbulEduLOMV1::abeLOMaddEducationalIntendedEndUserRole(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;

    bool retour;
    if(m_EducationalIntendedEndUserRoleVocabLOM.contains(value)) {
        if(!m_EducationalIntendedEndUserRole.contains("LOMv1.0;" + value))
            m_EducationalIntendedEndUserRole << "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QStringList AbulEduLOMV1::abeLOMgetEducationalIntendedEndUserRole()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalIntendedEndUserRole;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetEducationalIntendedEndUserRoleVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalIntendedEndUserRoleVocabLOM;
}

bool AbulEduLOMV1::abeLOMaddEducationalContext(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_EducationalContextVocabLOM.values().contains(value)) {
        QString key = m_EducationalContextVocabLOM.key(value);
        if(!m_EducationalContext.contains("LOMv1.0;" + key)) {
            m_EducationalContext << "LOMv1.0;" + key;
        }
        retour = true;
    }
    /* Si c'est une cle (mot en langue originale) */
    else if(m_EducationalContextVocabLOM.keys().contains(value)) {
        if(!m_EducationalContext.contains("LOMv1.0;" + value)) {
            m_EducationalContext << "LOMv1.0;" + value;
        }
        retour = true;
    }
    else {
        retour = false;
    }
    return retour;
}

QStringList AbulEduLOMV1::abeLOMgetEducationalContext()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalContext;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetEducationalContextVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalContextVocabLOM;
}

void AbulEduLOMV1::abeLOMclearEducationalContext()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalContext.clear();
}

bool AbulEduLOMV1::abeLOMaddEducationalTypicalAgeRange(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG() << lang << " : " << value;
    if (lang.isEmpty() || value.isEmpty() || value == "0-0")
    {
        return false;
    }
    else
    {
        m_EducationalTypicalAgeRange.add(lang,value);
        return true;
    }
    return false;
}

void AbulEduLOMV1::abelomclearEducationalTypicalAgeRange()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalTypicalAgeRange.clear();
    return;
}

QStringList AbulEduLOMV1::abeLOMgetEducationalTypicalAgeRange(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalTypicalAgeRange.getList(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetEducationalTypicalAgeRangeKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalTypicalAgeRange.keys();
}

bool AbulEduLOMV1::abeLOMsetEducationalDifficulty(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_EducationalDifficultyVocabLOM.values().contains(value)) {
        QString key = m_EducationalDifficultyVocabLOM.key(value);
        if(!m_EducationalDifficulty.contains("LOMv1.0;" + key)) {
            m_EducationalDifficulty = "LOMv1.0;" + key;
        }
        retour = true;
    }
    /* Si c'est une cle (mot en langue originale) */
    else if(m_EducationalDifficultyVocabLOM.keys().contains(value)) {
        if(!m_EducationalDifficulty.contains("LOMv1.0;" + value)) {
            m_EducationalDifficulty = "LOMv1.0;" + value;
        }
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QString AbulEduLOMV1::abeLOMgetEducationalDifficulty()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalDifficulty;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetEducationalDifficultyLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalDifficultyVocabLOM;
}

void AbulEduLOMV1::abeLOMsetEducationalTypicalLearningTime(int minutes)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalTypicalLearningTime = QTime(0,minutes,0);
}

QTime AbulEduLOMV1::abeLOMgetEducationalTypicalLearningTime()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalTypicalLearningTime;
}

bool AbulEduLOMV1::abeLOMaddEducationalDescription(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG()<<lang<<","<<value<<")";
    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (abeLOMgetAvailableLanguages().keys().contains(lang))
        {
            m_EducationalDescription.add(lang,value);
            return true;
        }
    }
    return false;
}

void AbulEduLOMV1::abeLOMclearEducationalDescription()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalDescription.clear();
    return;
}

QStringList AbulEduLOMV1::abeLOMgetEducationalDescription(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalDescription.getList(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetEducationalDescriptionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalDescription.keys();
}

void AbulEduLOMV1::abeLOMaddEducationalLanguage(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    if(m_availableLanguages.contains(value))
        if(!m_EducationalLanguage.contains(value))
            m_EducationalLanguage << value;
}

QStringList AbulEduLOMV1::abeLOMgetEducationalLanguage()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLanguage;
}

bool AbulEduLOMV1::abeLOMsetRightsCost(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetRightsCostVocabLOM().values().contains(value))
    {
        value = abeLOMgetRightsCostVocabLOM().key(value);
    }
    ABULEDU_LOG_DEBUG() <<value;
    if (value.isEmpty())
    {
        return false;
    }
    bool retour;
    if(m_RightsCostVocabLOM.contains(value)) {
        m_RightsCost = "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QString AbulEduLOMV1::abeLOMgetRightsCost()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RightsCost;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetRightsCostVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RightsCostVocabLOM;
}

bool AbulEduLOMV1::abeLOMsetRightsCopyrightAndOtherRestrictions(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().values().contains(value))
    {
        value = abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().key(value);
    }
    ABULEDU_LOG_DEBUG() <<value;
    if (value.isEmpty())
    {
        return false;
    }
    bool retour;
    if(m_RightsCopyrightAndOtherRestrictionsVocabLOM.contains(value)) {
        m_RightsCopyrightAndOtherRestrictions = "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QString AbulEduLOMV1::abeLOMgetRightsCopyrightAndOtherRestrictions()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RightsCopyrightAndOtherRestrictions;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RightsCopyrightAndOtherRestrictionsVocabLOM;
}

void AbulEduLOMV1::abeLOMsetRightsDescription(QString lang, QString value)
{
    ABULEDU_LOG_DEBUG() << lang << " : " << value;
    if (value.isEmpty()){
        return;
    }
    else {
        if(m_availableLanguages.keys().contains(lang))
            m_RightsDescription.set(lang,value);
        else {
            if(m_availableLanguages.values().contains(lang)) {
                m_RightsDescription.set(m_availableLanguages.key(lang),value);
            }
        }
    }
    return;
}

QString AbulEduLOMV1::abeLOMgetRightsDescription(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_availableLanguages.keys().contains(lang))
        return m_RightsDescription.get(lang);
    else {
        if(m_availableLanguages.values().contains(lang)) {
            return m_RightsDescription.get(m_availableLanguages.key(lang));
        }
    }
    return QString();
}

QList<QString> AbulEduLOMV1::abeLOMgetRightsDescriptionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RightsDescription.keys();
}

bool AbulEduLOMV1::abeLOMsetRelationKind(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_RelationKindVocabLOM.contains(value)) {
        m_RelationKind = "LOMv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QString AbulEduLOMV1::abeLOMgetRelationKind()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RelationKind;
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetRelationKindVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RelationKindVocabLOM;
}

void AbulEduLOMV1::abeLOMaddRelationResourceIdentifier(QString catalog, QString entry)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_RelationResourceIdentifierEntry << QString(catalog + ";" + entry);
}

void AbulEduLOMV1::abeLOMdelRelationResourceIdentifier(QString catalog, QString entry)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString value = catalog + ";" + entry;
    if (value.trimmed().isEmpty())
        return;
    if(m_RelationResourceIdentifierEntry.contains(value)) {
        m_RelationResourceIdentifierEntry.removeAll(value);
        return;
    }
}

QStringList AbulEduLOMV1::abeLOMgetRelationResourceIdentifier()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RelationResourceIdentifierEntry;
}

void AbulEduLOMV1::abeLOMaddRelationResourceDescription(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    m_RelationResourceDescription.add(lang,value);
}

QString AbulEduLOMV1::abeLOMgetRelationResourceDescription(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_RelationResourceDescription.get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetRelationResourceDescriptionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RelationResourceDescription.keys();
}

void AbulEduLOMV1::abeLOMsetAnnotationEntity(vCard value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //On ajoute la vcard a la liste pour pouvoir retourner la liste des vcards plus tard ...
    if(!m_allVcards.contains(value))
        m_allVcards << value;

    m_AnnotationEntity=value;
}

vCard AbulEduLOMV1::abeLOMgetAnnotationEntity()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_AnnotationEntity;
}

void AbulEduLOMV1::abeLOMsetAnnotationDate(QDate value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_AnnotationDate=value;
}

QDate AbulEduLOMV1::abeLOMgetAnnotationDate()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_AnnotationDate;
}

void AbulEduLOMV1::abeLOMsetAnnotationDescription(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    m_AnnotationDescription.set(lang,value);
}

QString AbulEduLOMV1::abeLOMgetAnnotationDescription(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_AnnotationDescription.get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetAnnotationDescriptionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_AnnotationDescription.keys();
}

//bool AbulEduLOMV1::abeLOMsetClassificationPurpose(int racine, QString value)
//{
//    //    qDebug() << "AbulEduLOMV1::abeLOMsetClassificationPurpose (1) : " << racine << value;
//    bool retour;
//    if(m_ClassificationPurposeVocabLOM.contains(value)) {
//        m_ClassificationPurpose[racine] = "LOMv1.0;" + value;
//        retour = true;
//    }
//    else
//        retour = false;
//    return retour;
//}

QString AbulEduLOMV1::abeLOMgetClassificationPurpose(int racine)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationPurpose[racine];
}

int AbulEduLOMV1::abeLOMgetClassificationPurposeNumber()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationPurpose.size();
}

int AbulEduLOMV1::abeLOMgetClassificationPurposeIndex(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    int retour = -1;
    if (m_ClassificationPurpose.values().contains(value))
    {
        retour = m_ClassificationPurpose.key(value);
    }
    return retour;
}

QMap<QString,QString> AbulEduLOMV1::abeLOMgetClassificationPurposeVocabLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationPurposeVocabLOM;
}

void AbulEduLOMV1::abeLOMsetClassificationTaxonPathSource(int racine, QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    m_ClassificationTaxonPathSource.insert(racine,LomLS(lang,value));
}

QString AbulEduLOMV1::abeLOMgetClassificationTaxonPathSource(int racine, QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    LomLS l = m_ClassificationTaxonPathSource.at(racine);
    return l.get(lang);
}

int AbulEduLOMV1::abeLOMgetClassificationTaxonPathIndexForSource(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo chercher pourquoi la ligne ci-dessous est commentée */
    //    return m_ClassificationTaxonPathSource.indexOf(LomLS(lang,value));
    Q_UNUSED(lang);
    Q_UNUSED(value);
    ABULEDU_LOG_WARN() << "This method is not implemented. Return -1";
    return -1;
}

QList<QString> AbulEduLOMV1::abeLOMgetClassificationTaxonPathSourceKeys(int num)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    LomLS l = m_ClassificationTaxonPathSource.at(num);
    return l.keys();
}

int AbulEduLOMV1::abeLOMgetClassificationTaxonPathSourceSize()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationTaxonPathSource.size();
}

QHash<QString, QHash<QString, QHash<QString, QString> > > AbulEduLOMV1::abeLOMgetClassificationPurposes()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_classificationPurposes;
}

void AbulEduLOMV1::abeLOMclearClassificationPurposes()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_classificationPurposes.clear();
}

bool AbulEduLOMV1::abeLOMaddClassificationPurpose(QString purpose, LomLS sourceTaxonPath, QString idTaxon, LomLS entryTaxon)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString aocPurpose;
    if (m_ClassificationPurposeVocabLOM.contains(purpose))
    {
        aocPurpose = "LOMv1.0;" + purpose;
    }
    else if (m_ClassificationPurposeVocabLOM.contains(m_ClassificationPurposeVocabLOM.key(purpose)))
    {
        aocPurpose = "LOMv1.0;" + m_ClassificationPurposeVocabLOM.key(purpose);
    }
    else if(purpose.startsWith("ScoLOMFRv1.0"))
    {
        aocPurpose = purpose;
    }
    else
    {
        return false;
    }

    QHash<QString,QHash<QString,QString > > taxonPath;
    QHash<QString, QString > taxons;
    ABULEDU_LOG_DEBUG() << "On avait : "
                        << m_classificationPurposes
                        << "AbulEduLOMV1::abeLOMaddClassificationPurpose avec comme purpose " << aocPurpose;

    if (m_classificationPurposes.contains(aocPurpose))
    {
        taxonPath = m_classificationPurposes.value(aocPurpose);
        if (taxonPath.contains(sourceTaxonPath.toQString()))
        {
            taxons = taxonPath.value(sourceTaxonPath.toQString());
        }
    }

    /* Je n'ai pas besoin de vérifier si on avait déjà une entry pour cette id : si elle y était insert l'écrase, et l'ajoute sinon */
    taxons.insertMulti(idTaxon,entryTaxon.toQString());
    ABULEDU_LOG_DEBUG() <<" ---------------------------------- taxons ---------------------------------- "
                        << taxons
                        <<" ---------------------------------- ------ ---------------------------------- ";
    // LomLS (sourceTaxonPath)    , (             taxons                   )

    /* J'écrase l'ancienne value de la key sourceTaxonPath */
    taxonPath.insert(sourceTaxonPath.toQString(),taxons);
    ABULEDU_LOG_DEBUG() <<" ---------------------------------- taxonsPath ---------------------------------- "
                        << taxonPath
                        <<" ---------------------------------- ------ ---------------------------------- ";

    /* J'écrase l'ancienne value de la key purpose */
    m_classificationPurposes.insert(aocPurpose,taxonPath);
    ABULEDU_LOG_DEBUG() << " ====================== fin d'enquillage ================================ "
                        <<m_classificationPurposes;

    return true;
}

void AbulEduLOMV1::abeLOMaddClassificationTaxonPathTaxumID(int racine, int num, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< racine << ":" << num << ":" << value;
    if(m_ClassificationTaxonPathTaxumID.isEmpty()) {
        m_ClassificationTaxonPathTaxumID[racine].insert(num, value);
    }
    else {
        m_ClassificationTaxonPathTaxumID[racine][num].append(value);
    }
}

int AbulEduLOMV1::abeLOMgetClassificationTaxonPathNbTaxon(int racine)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationTaxonPathTaxumID[racine].count();
}

QString AbulEduLOMV1::abeLOMgetClassificationTaxonPathTaxumID(int racine, int num)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    qDebug() << "AbulEduLOMV1::abeLOMgetClassificationTaxonPathTaxumID (1) : " << num;
    if(m_ClassificationTaxonPathTaxumID.isEmpty())
        return QString();
    else
        return m_ClassificationTaxonPathTaxumID[racine][num];
}

void AbulEduLOMV1::abeLOMsetClassificationTaxonPathTaxumEntry(int racine, int num, QString lang, QString value)
{
    /** @todo vérifier blindage langue */
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << racine << ":" << num << " " << lang << ":" << value;

    if(m_ClassificationTaxonPathTaxumEntry.isEmpty()) {
        m_ClassificationTaxonPathTaxumEntry[racine].insert(num, LomLS(lang,value));
    }
    else {
        m_ClassificationTaxonPathTaxumEntry[racine][num].set(lang,value);
    }
}

QString AbulEduLOMV1::abeLOMgetClassificationTaxonPathTaxumEntry(int racine, int num, QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_ClassificationTaxonPathTaxumEntry[racine][num].get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetClassificationTaxonPathTaxumEntryKeys(int racine, int num)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationTaxonPathTaxumEntry[racine][num].keys();
}

void AbulEduLOMV1::abeLOMsetClassificationDescription(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    m_ClassificationDescription.set(lang,value);
}

QString AbulEduLOMV1::abeLOMgetClassificationDescription(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_ClassificationDescription.get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetClassificationDescriptionKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationDescription.keys();
}

bool AbulEduLOMV1::abeLOMsetClassificationKeyword(QString lang, QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeLOMgetAvailableLanguages().values().contains(lang))
    {
        lang = abeLOMgetAvailableLanguages().key(lang);
    }
    ABULEDU_LOG_DEBUG()<<lang<<","<<value<<")";
    if (lang.isEmpty() || value.isEmpty())
    {
        return false;
    }
    else
    {
        if (abeLOMgetAvailableLanguages().keys().contains(lang))
        {
            m_ClassificationKeyword.set(lang,value);
            return true;
        }
    }
    return false;
}

QString AbulEduLOMV1::abeLOMgetClassificationKeyword(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @todo vérifier blindage langue */
    return m_ClassificationKeyword.get(lang);
}

QList<QString> AbulEduLOMV1::abeLOMgetClassificationKeywordKeys()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationKeyword.keys();
}

bool AbulEduLOMV1::clear()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_allVcards.clear();
    m_domDocument.clear();
    m_domRootElement.clear();
    m_domCurrentElement.clear();

    // ------------------- branche 1
    m_GeneralIdentifierCatalog.clear();
    m_GeneralIdentifierEntry.clear();
    m_GeneralTitle.clear();
    m_GeneralLanguage.clear();
    m_GeneralDescription.clear();
    m_GeneralKeyword.clear();
    m_GeneralCoverage.clear();
    m_GeneralStructure.clear();
    m_GeneralAggregationLevel.clear();

    // ------------------- branche 2
    m_LifeCycleVersion.clear();
    m_LifeCycleStatus.clear();
    m_LifeCycleContributions.clear();

    // ------------------- branche 3
    m_MetaMetadataIdentifierCatalog.clear();
    m_MetaMetadataIdentifierEntry.clear();
    m_MetaMetadataContributeRole.clear();
    m_MetaMetadataContributeEntity.clear();
    m_MetaMetadataContributeDate.clear();
    m_MetaMetadataMetadataSchema.clear();
    m_MetaMetadataLanguage.clear();

    // ------------------- branche 4
    m_TechnicalFormat.clear();
    m_TechnicalSize.clear();
    m_TechnicalLocation.clear();
    m_TechnicalRequirementOrCompositeType.clear();
    m_TechnicalRequirementOrCompositeName.clear();
    m_TechnicalRequirementOrCompositeMinimumVersion.clear();
    m_TechnicalRequirementOrCompositeMaximumVersion.clear();
    m_TechnicalInstallationRemarks.clear();
    m_TechnicalOtherPlatformRequirements.clear();
    m_TechnicalDuration.clear();

    // ------------------- branche 5
    m_EducationalInteractivityType.clear();
    m_EducationalLearningResourceType.clear();
    m_EducationalInteractivityLevel.clear();
    m_EducationalSemanticDensity.clear();
    m_EducationalIntendedEndUserRole.clear();
    m_EducationalContext.clear();
    m_EducationalTypicalAgeRange.clear();
    m_EducationalDifficulty.clear();
    //m_EducationalTypicalLearningTime;
    m_EducationalDescription.clear();
    m_EducationalLanguage.clear();

    // ------------------- branche 6
    m_RightsCost.clear();
    m_RightsCopyrightAndOtherRestrictions.clear();
    m_RightsDescription.clear();

    // ------------------- branche 7
    m_RelationKind.clear();
    m_RelationResourceIdentifierEntry.clear();
    m_RelationResourceDescription.clear();

    // ------------------- branche 8
    //    m_AnnotationEntity
    //    m_AnnotationDate
    m_AnnotationDescription.clear();

    // ------------------- branche 9
    m_ClassificationPurpose.clear();
    m_ClassificationTaxonPathSource.clear();
    m_ClassificationTaxonPathTaxumID.clear();
    m_ClassificationTaxonPathTaxumEntry.clear();

    m_ClassificationDescription.clear();
    m_ClassificationKeyword.clear();

    return true;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_header()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_domDocument.clear();
    QDomProcessingInstruction header = m_domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    m_domDocument.appendChild( header );

    QDomElement officeDocContent = m_domDocument.createElementNS("http://ltsc.ieee.org/xsd/LOM", m_xmlPrefix);
    m_domDocument.appendChild(officeDocContent);

    officeDocContent.setAttribute("xsi:schemaLocation","http://ltsc.ieee.org/xsd/LOM http://ltsc.ieee.org/xsd/lomv1.0/lom.xsd");
    officeDocContent.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");

    m_domRootElement = officeDocContent;

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    //============= General 1
    e = m_domDocument.createElement(m_xmlPrefix + "general");
    m_domRootElement.appendChild(e);
    {
        // 1.1
        e_fils = m_domDocument.createElement(m_xmlPrefix + "identifier");
        e.appendChild(e_fils);
        {
            e_fils2 = m_domDocument.createElement(m_xmlPrefix + "catalog");
            e_fils.appendChild(e_fils2);
            t = m_domDocument.createTextNode(m_GeneralIdentifierCatalog);
            e_fils2.appendChild(t);

            e_fils2 = m_domDocument.createElement(m_xmlPrefix + "entry");
            e_fils.appendChild(e_fils2);
            t = m_domDocument.createTextNode(m_GeneralIdentifierEntry);
            e_fils2.appendChild(t);
        }

        // 1.2
        if(m_GeneralTitle.keys().count()) {
            e_fils = m_domDocument.createElement(m_xmlPrefix + "title");
            e.appendChild(e_fils);
            {
                QListIterator<QString> i(abeLOMgetGeneralTitleKeys());
                while (i.hasNext()) {
                    QString lang = i.next();
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                    e_fils2.setAttribute("language",lang);
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(m_GeneralTitle.get(lang));
                    e_fils2.appendChild(t);
                }
            }
        }

        // 1.3
        if(m_GeneralLanguage.count()) {
            QListIterator<QString> i(abeLOMgetGeneralLanguage());
            while (i.hasNext()) {
                QString lang = i.next();
                e_fils = m_domDocument.createElement(m_xmlPrefix + "language");
                e.appendChild(e_fils);
                t = m_domDocument.createTextNode(lang);
                e_fils.appendChild(t);
            }
        }

        // 1.4
        if(m_GeneralDescription.keys().count()) {
            {
                QListIterator<QString> i(m_GeneralDescription.keys());
                //1ere boucle pour les langues
                while (i.hasNext()) {
                    QString lang = i.next();
                    //Seconde boucle pour les mots cles
                    QListIterator<QString> j(m_GeneralDescription.getList(lang));
                    while (j.hasNext()) {
                        e_fils = m_domDocument.createElement(m_xmlPrefix + "description");
                        e.appendChild(e_fils);
                        e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                        e_fils2.setAttribute("language",lang);
                        e_fils.appendChild(e_fils2);

                        t = m_domDocument.createTextNode(j.next());

                        e_fils2.appendChild(t);
                    }
                }
            }
        }

        // 1.5
        if(m_GeneralKeyword.keys().count()) {
            {
                QListIterator<QString> i(m_GeneralKeyword.keys());
                //1ere boucle pour les langues
                while (i.hasNext()) {
                    QString lang = i.next();
                    //Seconde boucle pour les mots cles
                    QListIterator<QString> j(m_GeneralKeyword.getList(lang));
                    while (j.hasNext()) {
                        e_fils = m_domDocument.createElement(m_xmlPrefix + "keyword");
                        e.appendChild(e_fils);
                        e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                        e_fils2.setAttribute("language",lang);
                        e_fils.appendChild(e_fils2);

                        t = m_domDocument.createTextNode(j.next());

                        e_fils2.appendChild(t);
                    }
                }
            }
        }

        // 1.6
        if(m_GeneralCoverage.keys().count()) {
            e_fils = m_domDocument.createElement(m_xmlPrefix + "coverage");
            e.appendChild(e_fils);
            {
                QListIterator<QString> i(abeLOMgetGeneralCoverageKeys());
                while (i.hasNext()) {
                    QString lang = i.next();
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                    e_fils2.setAttribute("language",lang);
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(m_GeneralCoverage.get(lang));
                    e_fils2.appendChild(t);
                }
            }
        }

        // 1.7
        if(!m_GeneralStructure.isEmpty()) {
            e_fils = m_domDocument.createElement(m_xmlPrefix + "structure");
            e.appendChild(e_fils);
            {
                QStringList contenu = m_GeneralStructure.split(";");
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

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

        // 1.8
        if(!m_GeneralAggregationLevel.isEmpty()) {
            e_fils = m_domDocument.createElement(m_xmlPrefix + "aggregationLevel");
            e.appendChild(e_fils);
            {
                QStringList contenu = m_GeneralAggregationLevel.split(";");
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

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
    m_domCurrentElement = e;

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section2()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool vide=true;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomElement e_fils3;
    QDomText t;

    //============= Cycle de vie 2 -- lifeCycle
    e = m_domDocument.createElement(m_xmlPrefix + "lifeCycle");
    {
        if(abeLOMgetLifeCycleVersionKeys().count()) {
            vide=false;
            e_fils = m_domDocument.createElement(m_xmlPrefix + "version");
            e.appendChild(e_fils);
            {
                QListIterator<QString> i(abeLOMgetLifeCycleVersionKeys());
                while (i.hasNext()) {
                    QString lang = i.next();
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                    e_fils2.setAttribute("language",lang);
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(m_LifeCycleVersion.get(lang));
                    e_fils2.appendChild(t);
                }
            }

            //2.2
            e_fils = m_domDocument.createElement(m_xmlPrefix + "status");
            e.appendChild(e_fils);
            {
                QStringList contenu = m_LifeCycleStatus.split(";");
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

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

        //Les contributions, il faut juste faire une boucle de la structure et générer le code
        QMapIterator<QString, QList<vCard> > iLifeCycleContributions(m_LifeCycleContributions);
        while (iLifeCycleContributions.hasNext()) {
            vide=false;
            iLifeCycleContributions.next();
            QString roleEtDate = iLifeCycleContributions.key();
            QList<vCard> entitees = iLifeCycleContributions.value();
            QString role = roleEtDate.split(":").at(0);
            QString ladate = roleEtDate.split(":").at(1);

            e_fils = m_domDocument.createElement(m_xmlPrefix + "contribute");
            e.appendChild(e_fils);
            //Le Role
            {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "role");
                e_fils.appendChild(e_fils2);
                {
                    e_fils3 = m_domDocument.createElement(m_xmlPrefix + "source");
                    e_fils2.appendChild(e_fils3);
                    t = m_domDocument.createTextNode("LOMv1.0");
                    e_fils3.appendChild(t);

                    e_fils3 = m_domDocument.createElement(m_xmlPrefix + "value");
                    e_fils2.appendChild(e_fils3);
                    t = m_domDocument.createTextNode(role);
                    e_fils3.appendChild(t);
                }

            }

            //Les entités, 2° itérateur / boucle
            for(int entity_i = 0; entity_i < entitees.count() ; entity_i++) {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "entity");
                e_fils.appendChild(e_fils2);
                {
                    vCard v = entitees.at(entity_i);
                    t = m_domDocument.createTextNode(v.toByteArray());
                    e_fils2.appendChild(t);
                }
            }

            //La date
            e_fils2 = m_domDocument.createElement(m_xmlPrefix + "date");
            e_fils.appendChild(e_fils2);
            {
                e_fils3 = m_domDocument.createElement(m_xmlPrefix + "dateTime");
                e_fils2.appendChild(e_fils3);
                t = m_domDocument.createTextNode(ladate);
                e_fils3.appendChild(t);
            }
        }

    }
    //Si non vide
    if(!vide) {
        m_domRootElement.appendChild(e);
        m_domCurrentElement = e;
    }
    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section4()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool vide=true;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    //============= Technique - technical

    //4.1 Format
    e = m_domDocument.createElement(m_xmlPrefix + "technical");
    {
        if(abeLOMgetTechnicalFormat().count()) {
            vide=false;
            QListIterator<QString> i(abeLOMgetTechnicalFormat());
            while (i.hasNext()) {
                QString valeur = i.next();
                e_fils = m_domDocument.createElement(m_xmlPrefix + "format");
                e.appendChild(e_fils);
                t = m_domDocument.createTextNode(valeur);
                e_fils.appendChild(t);
            }
        }
    }

    //4.2 Taille
    {
        if(! abeLOMgetTechnicalSize().isNull()) {
            vide=false;
            QString valeur = abeLOMgetTechnicalSize();
            e_fils = m_domDocument.createElement(m_xmlPrefix + "size");
            e.appendChild(e_fils);
            t = m_domDocument.createTextNode(valeur);
            e_fils.appendChild(t);
        }
    }

    //4.3 Location
    {
        if(abeLOMgetTechnicalLocation().count() > 0) {
            vide=false;
            QListIterator<QString> i(abeLOMgetTechnicalLocation());
            while (i.hasNext()) {
                QString valeur = i.next();
                e_fils = m_domDocument.createElement(m_xmlPrefix + "location");
                e.appendChild(e_fils);
                t = m_domDocument.createTextNode(valeur);
                e_fils.appendChild(t);
            }
        }
    }

    //TODO

    //Si non vide
    if(!vide) {
        m_domRootElement.appendChild(e);
        m_domCurrentElement = e;
    }

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

/**
  * @todo: a faire !
  */
QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section3()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< "NOT YET IMPLEMENTED";
    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}


QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section5()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool vide = true;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    //============= classification
    e = m_domDocument.createElement(m_xmlPrefix + "educational");
    {
        //5.1
        if(abeLOMgetEducationalInteractivityType().trimmed() != "") {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.1)";
            vide=false;
            QStringList contenu = abeLOMgetEducationalInteractivityType().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "interactivityType");
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

        //5.2
        if(abeLOMgetEducationalLearningResourceType().count()) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.2)";
            vide=false;
            QListIterator<QString> i(abeLOMgetEducationalLearningResourceType());
            while (i.hasNext()) {
                QStringList contenu = i.next().split(";");
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

                e_fils = m_domDocument.createElement(m_xmlPrefix + "learningResourceType");
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

        //5.3
        if(abeLOMgetEducationalInteractivityLevel() != "") {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.3)";
            vide=false;
            QString valeur = abeLOMgetEducationalInteractivityLevel();
            e_fils = m_domDocument.createElement(m_xmlPrefix + "interactivityLevel");
            e.appendChild(e_fils);
            {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode("LOMv1.0");
                e_fils2.appendChild(t);

                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(valeur);
                e_fils2.appendChild(t);
            }
        }

        //5.4
        if(abeLOMgetEducationalSemanticDensity() != "") {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.4)";
            vide=false;
            QString valeur = abeLOMgetEducationalSemanticDensity();
            e_fils = m_domDocument.createElement(m_xmlPrefix + "semanticDensity");
            e.appendChild(e_fils);
            {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode("LOMv1.0");
                e_fils2.appendChild(t);

                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(valeur);
                e_fils2.appendChild(t);
            }
        }

        //5.5
        if(abeLOMgetEducationalIntendedEndUserRole().count()) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.5)";
            vide=false;
            QListIterator<QString> i(abeLOMgetEducationalIntendedEndUserRole());
            while (i.hasNext()) {
                QStringList contenu = i.next().split(";");
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

                e_fils = m_domDocument.createElement(m_xmlPrefix + "intendedEndUserRole");
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

        //5.6
        if(abeLOMgetEducationalContext().count()) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.6)";
            vide=false;
            QListIterator<QString> i(abeLOMgetEducationalContext());
            while (i.hasNext()) {
                QStringList contenu = i.next().split(";");
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

                e_fils = m_domDocument.createElement(m_xmlPrefix + "context");
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

        //5.7
        if(abeLOMgetEducationalTypicalAgeRangeKeys().count()) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.7)";
            vide=false;
            QListIterator<QString> i(abeLOMgetEducationalTypicalAgeRangeKeys());
            while (i.hasNext()) {
                QString lang = i.next();

                QListIterator<QString> j(abeLOMgetEducationalTypicalAgeRange(lang));
                while (j.hasNext()) {
                    QString value = j.next();

                    e_fils = m_domDocument.createElement(m_xmlPrefix + "typicalAgeRange");
                    e.appendChild(e_fils);

                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                    e_fils2.setAttribute("language",lang);
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(value);
                    e_fils2.appendChild(t);
                }
            }
        }

        //5.8
        if(abeLOMgetEducationalDifficulty() != "") {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.8)";
            vide=false;
            QStringList contenu = abeLOMgetEducationalDifficulty().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "difficulty");
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

        //5.9
        if(abeLOMgetEducationalTypicalLearningTime() != QTime(0,0,0)) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.9)";
            vide=false;
            QString valeur = abeLOMgetEducationalTypicalLearningTime().toString("'PT'hh'H'mm'M'ss'S'");
            if(! valeur.isEmpty()) {
                vide=false;
                e_fils = m_domDocument.createElement(m_xmlPrefix + "typicalLearningTime");
                e.appendChild(e_fils);
                {
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode("LOMv1.0");
                    e_fils2.appendChild(t);

                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(valeur);
                    e_fils2.appendChild(t);
                }
            }
        }

        //5.10
        if(abeLOMgetEducationalDescriptionKeys().count()) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.10)";
            vide=false;
            QListIterator<QString> i(abeLOMgetEducationalDescriptionKeys());
            while (i.hasNext()) {
                QString lang = i.next();
                QListIterator<QString> j(abeLOMgetEducationalDescription(lang));
                while (j.hasNext()) {
                    QString value = j.next();
                    e_fils = m_domDocument.createElement(m_xmlPrefix + "description");
                    e.appendChild(e_fils);

                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                    e_fils2.setAttribute("language",lang);
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(value);
                    e_fils2.appendChild(t);
                }
            }
        }

        //5.11
        if(abeLOMgetEducationalLanguage().count()) {
            ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (5.11)";
            vide=false;
            QListIterator<QString> i(abeLOMgetEducationalLanguage());
            while (i.hasNext()) {
                QString valeur = i.next();
                e_fils = m_domDocument.createElement(m_xmlPrefix + "language");
                t = m_domDocument.createTextNode(valeur);
                e_fils.appendChild(t);
                e.appendChild(e_fils);

                /*
                {
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode("LOMv1.0");
                    e_fils2.appendChild(t);

                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
                    e_fils.appendChild(e_fils2);
                    t = m_domDocument.createTextNode(valeur);
                    e_fils2.appendChild(t);
                }
                */
            }
        }
    }
    if(!vide) {
        m_domRootElement.appendChild(e);
        m_domCurrentElement = e;
    }

    QByteArray XML  = m_domDocument.toByteArray();
    ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMExportAsXML_section5 (2)";
    return XML;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section6()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool vide = true;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    //============= rights
    e = m_domDocument.createElement(m_xmlPrefix + "rights");
    {
        //6.1
        if(abeLOMgetRightsCost() != "") {
            vide=false;
            QStringList contenu = abeLOMgetRightsCost().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "cost");
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

        //6.2
        if(abeLOMgetRightsCopyrightAndOtherRestrictions() != "") {
            vide=false;
            QStringList contenu = abeLOMgetRightsCopyrightAndOtherRestrictions().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "copyrightAndOtherRestrictions");
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

        //6.3
        if(abeLOMgetRightsDescriptionKeys().count()) {
            vide=false;
            QListIterator<QString> i(abeLOMgetRightsDescriptionKeys());
            while (i.hasNext()) {
                QString lang = i.next();

                QString value = abeLOMgetRightsDescription(lang);
                e_fils = m_domDocument.createElement(m_xmlPrefix + "description");
                e.appendChild(e_fils);

                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
                e_fils2.setAttribute("language",lang);
                e_fils.appendChild(e_fils2);
                t = m_domDocument.createTextNode(value);
                e_fils2.appendChild(t);
            }
        }
    }

    if(!vide) {
        m_domRootElement.appendChild(e);
        m_domCurrentElement = e;
    }

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section7()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool vide = true;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomElement e_fils3;
    QDomText t;

    //============= 7: relation
    e = m_domDocument.createElement(m_xmlPrefix + "relation");
    {
        //7.1 kind
        if(abeLOMgetRelationKind() != "") {
            vide=false;
            QStringList contenu = abeLOMgetRelationKind().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "kind");
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

        //7.2 ressources
        e_fils = m_domDocument.createElement(m_xmlPrefix + "resource");
        e.appendChild(e_fils);
        {

            QStringList liste(abeLOMgetRelationResourceIdentifier());
            for(int i = 0; i < liste.size(); i++) {
                e_fils2 = m_domDocument.createElement(m_xmlPrefix + "identifier");
                e_fils.appendChild(e_fils2);

                QString key = liste.at(i).split(";").first();
                QString value = liste.at(i).split(";").last();
                e_fils3 = m_domDocument.createElement(m_xmlPrefix + "catalog");
                t = m_domDocument.createTextNode(key);
                e_fils3.appendChild(t);
                e_fils2.appendChild(e_fils3);

                e_fils3 = m_domDocument.createElement(m_xmlPrefix + "entry");
                t = m_domDocument.createTextNode(value);
                e_fils3.appendChild(t);
                e_fils2.appendChild(e_fils3);
            }

            //boucle pour les differentes langues
            if(abeLOMgetRelationResourceDescriptionKeys().count()) {
                vide=false;
                QListIterator<QString> i(abeLOMgetRelationResourceDescriptionKeys());
                while (i.hasNext()) {
                    QString lang = i.next();

                    QString value = abeLOMgetRelationResourceDescription(lang);
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "description");
                    e_fils.appendChild(e_fils2);

                    e_fils3 = m_domDocument.createElement(m_xmlPrefix + "string");
                    e_fils3.setAttribute("language",lang);
                    e_fils2.appendChild(e_fils3);
                    t = m_domDocument.createTextNode(value);
                    e_fils3.appendChild(t);
                }
            }

        }
    }

    if(!vide) {
        m_domRootElement.appendChild(e);
        m_domCurrentElement = e;
    }

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section8()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}
QByteArray AbulEduLOMV1::p_abeLOMExportAsXML_section9()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool vide = true;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomElement e_fils3;
    QDomElement e_fils4;
    QDomText t;

    ABULEDU_LOG_DEBUG() << "On essaye d'exporter " << m_classificationPurposes;

    QHashIterator<QString, QHash <QString, QHash <QString,QString > > > iter1(m_classificationPurposes);
    while (iter1.hasNext()) {
        // --> pour chaque purpose différent
        iter1.next();
        //9.1
        e = m_domDocument.createElement(m_xmlPrefix + "classification");
        {
            if(!m_classificationPurposes.isEmpty()) {
                vide = false;

                QStringList contenu = iter1.key().split(";"); //la première QString est "source;objectif"
                QString cle = contenu.takeFirst();
                QString valeur = contenu.join(";");

                e_fils = m_domDocument.createElement(m_xmlPrefix + "purpose");
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

        //9.2 ->
        //        qDebug() << "AbulEduLOMV1::p_abeLOMExportAsXML_section9 (2) : " << racine;
        QHashIterator<QString, QHash <QString,QString > > iter2(iter1.value());
        ABULEDU_LOG_DEBUG()<< " ~~~~~~~~~~~~~~ export 9.2 ~~~~~~~~~~~~~~ ";

        while (iter2.hasNext()) {
            // --> pour chaque source différente d'un purpose donné
            iter2.next();
            //            qDebug() << "AbulEduLOMV1::p_abeLOMExportAsXML_section9 (3) : " << lang;

            e_fils = m_domDocument.createElement(m_xmlPrefix + "taxonPath");
            e.appendChild(e_fils);
            {
                LomLS source;
                source.fromQString(iter2.key()); //était LomLS

                ABULEDU_LOG_DEBUG()<< "########################"<<source.keys().size() ;
                for(int i = 0;i <  source.keys().size(); i++)
                {
                    QString lang = source.keys().at(i);
                    QStringList value = source.getList(lang);

                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
                    e_fils.appendChild(e_fils2);

                    foreach(QString val,value)
                    {
                        e_fils3 = m_domDocument.createElement(m_xmlPrefix + "string");
                        e_fils3.setAttribute("language",lang);
                        e_fils2.appendChild(e_fils3);
                        t = m_domDocument.createTextNode(val); //on ne prend qu'une seule source
                        e_fils3.appendChild(t);
                    }
                }
            }

            //            qDebug() << "AbulEduLOMV1::p_abeLOMExportAsXML_section9 (4) : ";
            //autre boucle pour tous les taxon

            QHashIterator<QString,QString >  iter3(iter2.value());
            while(iter3.hasNext()) {
                iter3.next();
                //                qDebug() << "AbulEduLOMV1::p_abeLOMExportAsXML_section9 (4b) : ";
                //20130218
                QString valeurID =iter3.key();
                LomLS valeurEntry;
                valeurEntry.fromQString(iter3.value());
                for (int k = 0; k < valeurEntry.keys().size(); k++){
                    QString langue = valeurEntry.keys().at(k);
                    QStringList entry = valeurEntry.getList(langue);
                    e_fils2 = m_domDocument.createElement(m_xmlPrefix + "taxon");
                    e_fils.appendChild(e_fils2);
                    {
                        e_fils3 = m_domDocument.createElement(m_xmlPrefix + "id");
                        e_fils2.appendChild(e_fils3);
                        t = m_domDocument.createTextNode(valeurID);
                        e_fils3.appendChild(t);

                        foreach(QString ent,entry)
                        {
                            e_fils3 = m_domDocument.createElement(m_xmlPrefix + "entry");
                            e_fils2.appendChild(e_fils3);

                            e_fils4 = m_domDocument.createElement(m_xmlPrefix + "string");
                            e_fils4.setAttribute("language",langue);
                            e_fils3.appendChild(e_fils4);
                            t = m_domDocument.createTextNode(ent);
                            e_fils4.appendChild(t);
                        }
                    }
                }
            }

        }
        if(!vide) {
            m_domRootElement.appendChild(e);
            m_domCurrentElement = e;
        }
    }

    //    qDebug() << "AbulEduLOMV1::p_abeLOMExportAsXML_section9 (5) : ";

    //9.3
    if(abeLOMgetClassificationDescriptionKeys().count()) {
        QListIterator<QString> i(abeLOMgetClassificationDescriptionKeys());
        while (i.hasNext()) {
            QString lang = i.next();

            QString value = abeLOMgetClassificationDescription(lang);
            e_fils = m_domDocument.createElement(m_xmlPrefix + "description");
            e.appendChild(e_fils);

            e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
            e_fils2.setAttribute("language",lang);
            e_fils.appendChild(e_fils2);
            t = m_domDocument.createTextNode(value);
            e_fils2.appendChild(t);
        }
    }

    //9.4
    if(abeLOMgetClassificationKeywordKeys().count()) {
        QListIterator<QString> i(abeLOMgetClassificationKeywordKeys());
        while (i.hasNext()) {
            QString lang = i.next();

            QString value = abeLOMgetClassificationKeyword(lang);
            e_fils = m_domDocument.createElement(m_xmlPrefix + "keyword");
            e.appendChild(e_fils);

            e_fils2 = m_domDocument.createElement(m_xmlPrefix + "string");
            e_fils2.setAttribute("language",lang);
            e_fils.appendChild(e_fils2);
            t = m_domDocument.createTextNode(value);
            e_fils2.appendChild(t);
        }
    }

    if(!vide) {
        m_domRootElement.appendChild(e);
        m_domCurrentElement = e;
    }

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section1(QDomElement n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    qDebug() << "===========================================================";
    //    qDebug() << n.tagName();
    //    qDebug() << "===========================================================";

    //Debut du parcours de l'arbre a partir de e
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        //        qDebug() << "on est sur le section1 de LOMV1.cpp ..." << fils.toElement().tagName();
        if (fils.isElement()) {
            QDomElement e = fils.toElement();

            //1.1 Identifiant
            if(e.tagName() == m_xmlPrefix + "identifier") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "catalog");
                if(! e2.isNull()) {
                    abeLOMsetGeneralIdentifierCatalog(e2.text());
                }
                e2 = e.firstChildElement(m_xmlPrefix + "entry");
                if(! e2.isNull()) {
                    abeLOMsetGeneralIdentifierEntry(e2.text());
                }
            }

            //1.2 Titre
            if(e.tagName() == m_xmlPrefix + "title") {
                QDomElement e2 = e.firstChild().toElement();
                ABULEDU_LOG_DEBUG() << "on est sur le titre ... : " << e2.text();
                while (!e2.isNull()) {
                    abeLOMsetGeneralTitle(e2.attribute("language"), e2.text());
                    e2 = e2.nextSibling().toElement();
                }
            }

            //1.3 Langues
            if(e.tagName() == m_xmlPrefix + "language") {
                abeLOMaddGeneralLanguage(e.text());
            }

            //1.4 Description
            if(e.tagName() == m_xmlPrefix + "description") {
                QDomElement e2 = e.firstChild().toElement();
                while (!e2.isNull()) {
                    abeLOMsetGeneralDescription(e2.attribute("language"), e2.text());
                    e2 = e2.nextSibling().toElement();
                }
            }

            //1.5 Mots clés
            if(e.tagName() == m_xmlPrefix + "keyword") {
                QDomElement e2 = e.firstChild().toElement();
                while (!e2.isNull()) {
                    abeLOMaddGeneralKeyword(e2.attribute("language"), e2.text());
                    e2 = e2.nextSibling().toElement();
                }
            }

            //1.6 Couverture
            if(e.tagName() == m_xmlPrefix + "coverage") {
                QDomElement e2 = e.firstChild().toElement();
                while (!e2.isNull()) {
                    abeLOMaddGeneralCoverage(e2.attribute("language"), e2.text());
                    e2 = e2.nextSibling().toElement();
                }
            }

            //1.7 Structure
            if(e.tagName() == m_xmlPrefix + "structure") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                if(!e2.isNull()) {
                    abeLOMsetGeneralStructure(e2.text());
                }
            }

            //1.8 Niveau d'agrégation
            if(e.tagName() == m_xmlPrefix + "aggregationLevel") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                if(!e2.isNull()) {
                    abeLOMsetGeneralAggregationLevel(e2.text());
                }
            }

        }
        fils = fils.nextSibling();
        //        qDebug() << "on est sur le section1 de LOMV1.cpp ..." << fils.toElement().tagName();
    }

    return true;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section2(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Debut du parcours de l'arbre a partir de e
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();
            ABULEDU_LOG_DEBUG() << "Element name section2: " << e.tagName() << endl;

            //2.1 Version
            if(e.tagName() == m_xmlPrefix + "version") {
                QDomElement e2 = e.firstChild().toElement();
                if(!e2.isNull()) {
                    abeLOMsetLifeCycleVersion(e2.attribute("language"), e2.text());
                }
            }

            //2.2 Status
            if(e.tagName() == m_xmlPrefix + "status") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                if(!e2.isNull()) {
                    abeLOMsetLifeCycleStatus(e2.text());
                }
            }

            //2.3 Contributions (n)
            if(e.tagName() == m_xmlPrefix + "contribute") {
                QString role;
                QDate   ladate;
                vCard   entity;
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "role");
                if(!e2.isNull()) {
                    QDomElement e3 = e2.firstChildElement(m_xmlPrefix + "value");
                    if(!e3.isNull()) {
                        role = e3.text();
                    }
                }

                e2 = e.firstChildElement(m_xmlPrefix + "entity");
                //Une liste temporaire des entités
                QList<vCard> entityTemp;
                while(!e2.isNull()) {
                    QByteArray relais;
                    relais.append(e2.text());
                    entity = vCard::fromByteArray(relais).at(0);
                    entityTemp << entity;
                    e2 = e2.nextSiblingElement(m_xmlPrefix + "entity");
                }

                e2 = e.firstChildElement(m_xmlPrefix + "date");
                if(!e2.isNull()) {
                    QDomElement e3 = e2.firstChildElement(m_xmlPrefix + "dateTime");
                    if(!e3.isNull()) {
                        ladate = QDate::fromString(e3.text(),"yyyy-MM-dd");
                    }
                }

                //On ajoute toute les entités une par une
                for(int i = 0; i < entityTemp.count(); i++)
                    abeLOMaddLifeCycleContributionRole(role, entityTemp.at(i), ladate);
            }
        }
        fils = fils.nextSibling();
    }

    return true;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section3(QDomNode n)
{
    Q_UNUSED(n)
    //TODO
    return false;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section4(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();
            ABULEDU_LOG_DEBUG() << "Element name section4: " << e.tagName() << endl;

            //4.1 format
            if(e.tagName() == m_xmlPrefix + "format") {
                abeLOMaddTechnicalFormat(e.text());
            }

            //4.2 taille
            if(e.tagName() == m_xmlPrefix + "size") {
                abeLOMsetTechnicalSize(e.text());
            }

            //4.3 localisation
            if(e.tagName() == m_xmlPrefix + "location") {
                abeLOMaddTechnicalLocation(e.text());
            }

            //TODO
        }
        fils = fils.nextSibling();
    }

    return true;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section5(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();
            ABULEDU_LOG_DEBUG() << "Element name section5: " << e.tagName() << "(" << m_xmlPrefix << ")";
            //5.1 type d'interactivité : uniquement dans lom ... et pas utilisé !
            if(e.tagName() == m_xmlPrefix + "interactivityTypeValue") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetEducationalInteractivityType(e2.text());
            }

            //5.2 type de ressource
            if(e.tagName() == m_xmlPrefix + "learningResourceType") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalLearningResourceType(e2.text());
            }

            //5.3 niveau d'interactivité - pas utilisé
            if(e.tagName() == m_xmlPrefix + "interactivityLevel") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetEducationalInteractivityLevel(e2.text());
            }

            //5.4 densité sémantique - pas utilisé
            if(e.tagName() == m_xmlPrefix + "semanticDensity") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetEducationalSemanticDensity(e2.text());
            }

            //5.5 role de l'utilisateur final
            if(e.tagName() == m_xmlPrefix + "intendedEndUserRole") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalIntendedEndUserRole(e2.text());
            }

            //5.6 contexte
            if(e.tagName() == m_xmlPrefix + "context") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalContext(e2.text());
            }

            //5.7 tranche d'age
            if(e.tagName() == m_xmlPrefix + "typicalAgeRange") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "string");
                abeLOMaddEducationalTypicalAgeRange(e2.attribute("language"), e2.text());
            }

            //5.8 difficulté
            if(e.tagName() == m_xmlPrefix + "difficulty") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetEducationalDifficulty(e2.text());
            }

            //5.9 temps d'apprentissage moyen
            if(e.tagName() == m_xmlPrefix + "typicalLearningTime") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                QTime duree = QTime::fromString(e2.text(), "'PT'hh'H'mm'M'ss'S'");
                abeLOMsetEducationalTypicalLearningTime(duree.hour()*60+duree.minute());
            }

            //5.10 description / proposition d'utilisation
            if(e.tagName() == m_xmlPrefix + "description") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "string");
                abeLOMaddEducationalDescription(e2.attribute("language"), e2.text());
            }

            //5.11 langue
            if(e.tagName() == m_xmlPrefix + "language") {
                //QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalLanguage(e.text());
            }

            //fin de lom -> suite dans lomfr
        }
        fils = fils.nextSibling();
    }

    return true;

}

bool AbulEduLOMV1::p_abeLOMImportXML_section6(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();
            //6.1 cout
            if(e.tagName() == m_xmlPrefix + "cost") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetRightsCost(e2.text());
            }

            //6.2 copyright
            if(e.tagName() == m_xmlPrefix + "copyrightAndOtherRestrictions") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetRightsCopyrightAndOtherRestrictions(e2.text());
            }

            //6.3 description
            if(e.tagName() == m_xmlPrefix + "description") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "string");
                abeLOMsetRightsDescription(e2.attribute("language"), e2.text());
            }

        }
        fils = fils.nextSibling();
    }

    return true;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section7(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();
            //7.1 kind
            if(e.tagName() == m_xmlPrefix + "kind") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMsetRelationKind(e2.text());
            }

            //7.2 ressource
            if(e.tagName() == m_xmlPrefix + "resource") {

                //7.2.1 identifiant
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "identifier");
                while (!e2.isNull()) {
                    QDomElement e3 = e2.firstChildElement(m_xmlPrefix + "catalog");
                    QString catalog = e3.text();

                    e3 = e2.firstChildElement(m_xmlPrefix + "entry");
                    QString entry = e3.text();
                    if(catalog.trimmed() != "") {
                        abeLOMaddRelationResourceIdentifier(catalog, entry);
                    }
                    e2 = e2.nextSibling().toElement();
                }

                //7.2.2 Description, LomLS
                e2 = e.firstChildElement(m_xmlPrefix + "description");
                QDomElement e3 = e2.firstChild().toElement();
                while (!e3.isNull()) {
                    abeLOMaddRelationResourceDescription(e3.attribute("language"), e3.text());
                    e3 = e3.nextSibling().toElement();
                }
            }
        }
        fils = fils.nextSibling();
    }

    return true;
}

bool AbulEduLOMV1::p_abeLOMImportXML_section8(QDomNode n)
{
    Q_UNUSED(n)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return true;
}

/**
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
bool AbulEduLOMV1::p_abeLOMImportXML_section9(QDomNode n, int numSection9)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< numSection9;

    QString objectif;
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            LomLS sourceTaxonPath;
            QString idTaxon;
            LomLS entryTaxon;
            QDomElement e = fils.toElement();
            //9.1 purpose
            if(e.tagName() == m_xmlPrefix + "purpose") {

                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                QDomElement e2bis = e.firstChildElement(m_xmlPrefix + "source");
                objectif = e2.text(); //e2bis.text()+";"+
//                qDebug() << "   AbulEduLOMV1::p_abeLOMImportXML_section9() (007)  : " << objectif;
                //                abeLOMsetClassificationPurpose(numSection9,e2.text());

            }

            //9.2 taxonPath
            if(e.tagName() == m_xmlPrefix + "taxonPath") {
                //9.2.1 source
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "source");
                QDomElement e2b = e2.firstChild().toElement();
                sourceTaxonPath =  LomLS(e2b.attribute("language"),e2b.text());
                QDomElement e3 = e.firstChildElement(m_xmlPrefix + "taxon");
                while(!e3.isNull())
                {
                    QDomElement e3b = e3.firstChildElement(m_xmlPrefix + "id");
                    idTaxon = e3b.text();
                    QDomElement e3c = e3.firstChildElement(m_xmlPrefix + "entry");
                    entryTaxon = LomLS(e3c.firstChild().toElement().attribute("language"),e3c.text());

                    ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMImportXML_section9() (009)  : " << e3c.firstChild().toElement().attribute("language");

                    abeLOMaddClassificationPurpose(objectif,sourceTaxonPath,idTaxon,entryTaxon);

                    e3 = e3.nextSibling().toElement();
                }
            }
            //9.3 description
            if(e.tagName() == m_xmlPrefix + "description") {
                QDomElement e2 = e.firstChild().toElement();
                abeLOMsetClassificationDescription(e2.attribute("language"), e2.text());
            }

            //9.4 keyword
            if(e.tagName() == m_xmlPrefix + "keyword") {
                QDomElement e2 = e.firstChild().toElement();
                abeLOMsetClassificationKeyword(e2.attribute("language"), e2.text());
            }
        }
        fils = fils.nextSibling();
    }

    return true;
}


bool AbulEduLOMV1::p_abeLOMaddLifeCycleContributionRole(QString role, vCard entity, QDate ladate)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< role << ladate;

    /* Si l'entité est "vide" on dégage tout de suite */
    if(entity.getFullName() == "") {
        ABULEDU_LOG_DEBUG() << "entity vide on droppe";
        return false;
    }

    ABULEDU_LOG_DEBUG() << "enregistrement d'une contribution de "<<entity.getFullName();

    /* On ajoute la vcard a la liste pour pouvoir retourner la liste des vcards plus tard ... */
    if(!m_allVcards.contains(entity)) {
        m_allVcards << entity;
    }

    bool retour = false;
    /* Si par malheur la date n'est pas bonne on passe a aujourd'hui */
    if(!ladate.isValid()) {
        ladate = QDate::currentDate();
    }
    QString recherche(role + ":" + ladate.toString("yyyy-MM-dd"));
    /* On ajoute une entity a la liste qui peut déjà exister, pour le savoir, on cherche la cle "recherche" */
    /* On cherche une clé qui est composée de role:date */
    if(m_LifeCycleContributions.contains(recherche)) {
        QList<vCard> listeVCardTemp = m_LifeCycleContributions.value(recherche);
        if(! listeVCardTemp.contains(entity)) {
            listeVCardTemp << entity;
            m_LifeCycleContributions.remove(recherche);
            m_LifeCycleContributions.insert(recherche,listeVCardTemp);
            retour = true;
        }
    }
    else {
        /* La clé n'existe pas, on ajoute tout */
        QList<vCard> listeVCardTemp;
        listeVCardTemp << entity;
        m_LifeCycleContributions.insert(recherche,listeVCardTemp);
        retour = true;
    }

    ABULEDU_LOG_DEBUG() << "AbulEduLOMV1::p_abeLOMaddLifeCycleContributionRole (2) : "<<retour
                        << m_LifeCycleContributions;
    return retour;
}

// =============================================================================== Code automatique (fin)

// =============================================================================== Code manuel
QByteArray AbulEduLOMV1::abeLOMExportAsXML()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    p_abeLOMExportAsXML_header();
    p_abeLOMExportAsXML_section1();
    p_abeLOMExportAsXML_section2();
    p_abeLOMExportAsXML_section4();
    p_abeLOMExportAsXML_section5();
    p_abeLOMExportAsXML_section6();
    p_abeLOMExportAsXML_section7();
    p_abeLOMExportAsXML_section8();
    p_abeLOMExportAsXML_section9();

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

bool AbulEduLOMV1::abeLOMsetFile(QString fileName)
{
 ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< fileName;

    QFile fi(fileName);
    if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fi.seek(0);
        return abeLOMsetXML(fi.readAll());
        fi.close();
    }
    return true;
}

bool AbulEduLOMV1::abeLOMsetXML(QByteArray xml)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    this->clear();

    QString errorMessage;
    if(!m_domDocument.setContent(xml,&errorMessage)) {
        ABULEDU_LOG_ERROR() << "=================================="
                            << "Erreur de lecture XML: " << errorMessage
                            << "==================================";
        return false;
    }
    //qDebug() << m_domDocument.toDocument().toString();

    //Ensuite il faut "parcourir" le fichier XML
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = m_domDocument.documentElement();

    QDomNode n = docElem.firstChild();
    int numSection9 = 0;
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(e.tagName() == m_xmlPrefix + "general") {
            p_abeLOMImportXML_section1(e);
        }
        if(e.tagName() == m_xmlPrefix + "lifeCycle") {
            p_abeLOMImportXML_section2(n);
        }
        if(e.tagName() == m_xmlPrefix + "metaMetadata") {
            p_abeLOMImportXML_section3(n);
        }
        if(e.tagName() == m_xmlPrefix + "technical") {
            p_abeLOMImportXML_section4(n);
        }
        if(e.tagName() == m_xmlPrefix + "educational") {
            p_abeLOMImportXML_section5(n);
        }
        if(e.tagName() == m_xmlPrefix + "rights") {
            p_abeLOMImportXML_section6(n);
        }
        if(e.tagName() == m_xmlPrefix + "relation") {
            p_abeLOMImportXML_section7(n);
        }
        if(e.tagName() == m_xmlPrefix + "annotation") {
            p_abeLOMImportXML_section8(n);
        }
        if(e.tagName() == m_xmlPrefix + "classification") {
            p_abeLOMImportXML_section9(n,numSection9);
            numSection9++;
        }
        else {
            if(!e.isNull()) {
                ABULEDU_LOG_DEBUG() << qPrintable(e.tagName()) << endl; // the node really is an element.
            }
        }
        n = n.nextSibling();
    }
    return true;
}

QList<vCard> AbulEduLOMV1::abeLOMgetAllVcards()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    vCard empty = vCard();
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, trUtf8("Non renseigné"));
    empty.addProperty(fn_prop);

    QList<vCard> liste;
    liste << empty << m_allVcards;
    return liste;
}

vCard AbulEduLOMV1::abeLOMgetVCardFromFullName(QString fullName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << fullName;
    int i = 0;
    while (i<m_allVcards.count())
    {
        if (m_allVcards[i].getFullName() == fullName)
            return m_allVcards[i];
        else i++;
    }
    return vCard();
}

QMap<QString, QString> AbulEduLOMV1::abeLOMgetAvailableLanguages()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_availableLanguages;
}

QHash<QString, QStringList> LomLS::toQHash()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QHash<QString, QStringList> hash;
    QMapIterator<QString, QStringList> it(lomLSValues);
    while(it.hasNext())
    {
        it.next();
        hash.insert(it.key(),it.value());
    }
    return hash;
}

bool LomLS::operator ==(const LomLS &param) const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return (lomLSValues.keys() == param.lomLSValues.keys() && lomLSValues.values() == param.lomLSValues.values());
}
