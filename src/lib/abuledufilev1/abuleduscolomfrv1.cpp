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

#include "abuleduscolomfrv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduSCOLOMFrV1::AbulEduSCOLOMFrV1()
{
    AbulEduLOMFrV1();
    m_xmlPrefix = "scolomfr:";

    //1.10
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString(""), QObject::trUtf8("Non renseignée"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("annuaire"),  QObject::trUtf8("annuaire"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("article"),  QObject::trUtf8("article"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("atlas"),  QObject::trUtf8("atlas"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("banque de vidéos"),  QObject::trUtf8("banque de vidéos"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("banque d'images"),  QObject::trUtf8("banque d'images"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("base de données"),  QObject::trUtf8("base de données"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("bibliographie/sitographie"),  QObject::trUtf8("bibliographie/sitographie"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("carte"),  QObject::trUtf8("carte"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("carte heuristique et conceptuelle"),  QObject::trUtf8("carte heuristique et conceptuelle"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("chronologie"),  QObject::trUtf8("chronologie"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("collection de documents"),  QObject::trUtf8("collection de documents"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("collection de données"),  QObject::trUtf8("collection de données"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("compte rendu"),  QObject::trUtf8("compte rendu"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("conférence"),  QObject::trUtf8("conférence"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("diaporama"),  QObject::trUtf8("diaporama"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("dossier technique"),  QObject::trUtf8("dossier technique"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("feuille de calcul"),  QObject::trUtf8("feuille de calcul"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("image numérique"),  QObject::trUtf8("image numérique"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("livre numérique"),  QObject::trUtf8("livre numérique"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("maquette/prototype"),  QObject::trUtf8("maquette/prototype"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("norme"),  QObject::trUtf8("norme"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("jeu de données"),  QObject::trUtf8("jeu de données"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("note"),  QObject::trUtf8("note"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("objet"),  QObject::trUtf8("objet"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("objet 3D"),  QObject::trUtf8("objet 3D"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("ouvrage"),  QObject::trUtf8("ouvrage"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("partition"),  QObject::trUtf8("partition"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("photographie"),  QObject::trUtf8("photographie"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("podcast"),  QObject::trUtf8("podcast"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("présentation multimédia"),  QObject::trUtf8("présentation multimédia"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("programme scolaire"),  QObject::trUtf8("programme scolaire"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("rapport"),  QObject::trUtf8("rapport"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("référentiel de compétences"),  QObject::trUtf8("référentiel de compétences"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("schéma/graphique"),  QObject::trUtf8("schéma/graphique"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("site web"),  QObject::trUtf8("site web"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("tableau"),  QObject::trUtf8("tableau"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("web média"),  QObject::trUtf8("web média"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("bande dessinée"),  QObject::trUtf8("bande dessinée"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("biographie"),  QObject::trUtf8("biographie"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("dossier documentaire"),  QObject::trUtf8("dossier documentaire"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("exposition"),  QObject::trUtf8("exposition"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("film"),  QObject::trUtf8("film"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("objet physique"),  QObject::trUtf8("objet physique"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("partition musicale"),  QObject::trUtf8("partition musicale"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("périodique"),  QObject::trUtf8("périodique"));
    m_GeneralDocumentTypologyVocabSCOLOMFR.insert(QString::fromUtf8("tableau (art)"),  QObject::trUtf8("tableau (art)"));

    //2.3.1
    m_LifeCycleContributionRoleVocabLOM.insert("diffuseur/distributeur",  QObject::trUtf8("diffuseur/distributeur"));

    //5.2
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString(""), QObject::trUtf8("Non renseigné"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("étude de cas"),  QObject::trUtf8("étude de cas"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("jeu éducatif"),  QObject::trUtf8("jeu éducatif"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("manuel d'enseignement"),  QObject::trUtf8("manuel d'enseignement"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("production d'élève"),  QObject::trUtf8("production d'élève"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("annales"),  QObject::trUtf8("annales"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("cyberquête"),  QObject::trUtf8("cyberquête"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("méthode de langues"),  QObject::trUtf8("méthode de langues"));
    m_EducationalLearningResourceTypeVocabSCOLOMFR.insert(QString::fromUtf8("témoignage pédagogique"),  QObject::trUtf8("témoignage pédagogique"));



    //5.12
    m_EducationalActivitiesVocabSCOLOMFR.insert(QString(""), QObject::trUtf8("Non renseignée"));
    m_EducationalActivitiesVocabSCOLOMFR.insert(QString::fromUtf8("expérimenter"),  QObject::trUtf8("expérimenter"));

    //5.14
    m_EducationalLocationVocabSCOLOMFR.insert(QString(""),  QObject::trUtf8("Non renseigné"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en amphithéâtre"),  QObject::trUtf8("en amphithéâtre"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en atelier"),  QObject::trUtf8("en atelier"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en atelier de pédagogie personnalisée"),  QObject::trUtf8("en atelier de pédagogie personnalisée"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en CDI"),  QObject::trUtf8("en CDI"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en salle de classe"),  QObject::trUtf8("en salle de classe"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en établissement"),  QObject::trUtf8("en établissement"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("espace dédié à une pratique spécifique"),  QObject::trUtf8("espace dédié à une pratique spécifique"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en établissement socioculturel"),  QObject::trUtf8("en établissement socioculturel"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en bibliothèque médiathèque"),  QObject::trUtf8("en bibliothèque médiathèque"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en musée"),  QObject::trUtf8("en musée"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("hors établissement"),  QObject::trUtf8("hors établissement"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en installation de loisirs"),  QObject::trUtf8("en installation de loisirs"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en installation sportive"),  QObject::trUtf8("en installation sportive"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en laboratoire"),  QObject::trUtf8("en laboratoire"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en laboratoire de langues"),  QObject::trUtf8("en laboratoire de langues"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en milieu familial"),  QObject::trUtf8("en milieu familial"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en milieu professionnel"),  QObject::trUtf8("en milieu professionnel"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en entreprise"),  QObject::trUtf8("en entreprise"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("non précisé"),  QObject::trUtf8("non précisé"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en salle informatique"),  QObject::trUtf8("en salle informatique"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en salle multimédia"),  QObject::trUtf8("en salle multimédia"));
    m_EducationalLocationVocabSCOLOMFR.insert(QString::fromUtf8("en mobilité"),  QObject::trUtf8("en mobilité"));

    //5.15
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString(""),  QObject::trUtf8("Non renseignée"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("PAC"),  QObject::trUtf8("PAC"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("partenariat électronique interécoles"),  QObject::trUtf8("partenariat électronique interécoles"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("projets interétablissement"),  QObject::trUtf8("projets interétablissement"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("liaison primaire-collège"),  QObject::trUtf8("liaison primaire-collège"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("liaison collège-lycée"),  QObject::trUtf8("liaison collège-lycée"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("séjour pédagogique"),  QObject::trUtf8("séjour pédagogique"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("sortie culturelle"),  QObject::trUtf8("sortie culturelle"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("spectacle"),  QObject::trUtf8("spectacle"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("sortie pédagogique"),  QObject::trUtf8("sortie pédagogique"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("TIPE"),  QObject::trUtf8("TIPE"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("TPE"),  QObject::trUtf8("TPE"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("travail de recherche"),  QObject::trUtf8("travail de recherche"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("travail en interdisciplinarité"),  QObject::trUtf8("travail en interdisciplinarité"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("IDD"),  QObject::trUtf8("IDD"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("PPCP"),  QObject::trUtf8("PPCP"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("travaux dirigés"),  QObject::trUtf8("travaux dirigés"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("travaux pratiques"),  QObject::trUtf8("travaux pratiques"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("à distance"),  QObject::trUtf8("à distance"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("aide didactique"),  QObject::trUtf8("aide didactique"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("aide didactique individualisée"),  QObject::trUtf8("aide didactique individualisée"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("échange pédagogique de classes"),  QObject::trUtf8("échange pédagogique de classes"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en alternance"),  QObject::trUtf8("en alternance"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en autonomie"),  QObject::trUtf8("en autonomie"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en classe entière"),  QObject::trUtf8("en classe entière"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en collaboration"),  QObject::trUtf8("en collaboration"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en entreprise"),  QObject::trUtf8("en entreprise"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("PFMP (CAP)"),  QObject::trUtf8("PFMP (CAP)"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("PFE (BTS)"),  QObject::trUtf8("PFE (BTS)"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("visite d'entreprise"),  QObject::trUtf8("visite d'entreprise"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en groupe"),  QObject::trUtf8("en groupe"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en groupe de compétences"),  QObject::trUtf8("en groupe de compétences"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en ligne"),  QObject::trUtf8("en ligne"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("étude dirigée"),  QObject::trUtf8("étude dirigée"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("modalité mixte"),  QObject::trUtf8("modalité mixte"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en milieu professionel"),  QObject::trUtf8("en milieu professionel"));
    m_EducationalPedagogicModalityVocabSCOLOMFR.insert(QString::fromUtf8("en tutorat"),  QObject::trUtf8("en tutorat"));

    // 5.16
    m_EducationalToolsVocabSCOLOMFR.insert(QString(""),  QObject::trUtf8("Non renseigné"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciels RIP"),  QObject::trUtf8("logiciels RIP"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel système et de programmation"),  QObject::trUtf8("logiciel système et de programmation"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("système d'exploitation"),  QObject::trUtf8("système d'exploitation"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de développement d'application"),  QObject::trUtf8("logiciel de développement d'application"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("navigateurs Web"),  QObject::trUtf8("navigateurs Web"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d'application"),  QObject::trUtf8("logiciel d'application"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("didacticiel"),  QObject::trUtf8("didacticiel"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("exerciseur"),  QObject::trUtf8("exerciseur"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("suite bureautique"),  QObject::trUtf8("suite bureautique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d'aide à l'orientation"),  QObject::trUtf8("logiciel d'aide à l'orientation"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d'animation 2D"),  QObject::trUtf8("logiciel d'animation 2D"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d'animation 3D"),  QObject::trUtf8("logiciel d'animation 3D"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de communication et d'échange"),  QObject::trUtf8("logiciel de communication et d'échange"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de dessin assisté par ordinateur"),  QObject::trUtf8("logiciel de dessin assisté par ordinateur"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de gestion de contenu"),  QObject::trUtf8("logiciel de gestion de contenu"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de gestion de projet"),  QObject::trUtf8("logiciel de gestion de projet"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de messagerie"),  QObject::trUtf8("logiciel de messagerie"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de montage vidéo"),  QObject::trUtf8("logiciel de montage vidéo"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de présentation"),  QObject::trUtf8("logiciel de présentation"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de recherche documentaire"),  QObject::trUtf8("logiciel de recherche documentaire"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de traitement de son"),  QObject::trUtf8("logiciel de traitement de son"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de traitement de tableaux et calculs"),  QObject::trUtf8("logiciel de traitement de tableaux et calculs"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de traitement de texte"),  QObject::trUtf8("logiciel de traitement de texte"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de traitement d'enquête"),  QObject::trUtf8("logiciel de traitement d'enquête"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de traitement d'image fixe"),  QObject::trUtf8("logiciel de traitement d'image fixe"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de traitement vidéo"),  QObject::trUtf8("logiciel de traitement vidéo"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel de travail collaboratif"),  QObject::trUtf8("logiciel de travail collaboratif"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d'édition Web"),  QObject::trUtf8("logiciel d'édition Web"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d'image de synthèse"),  QObject::trUtf8("logiciel d'image de synthèse"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel organisateur des idées"),  QObject::trUtf8("logiciel organisateur des idées"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel ou application de lecture"),  QObject::trUtf8("logiciel ou application de lecture"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel PAO"),  QObject::trUtf8("logiciel PAO"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel d’édition et de gestion d’exercices"),  QObject::trUtf8("logiciel d’édition et de gestion d’exercices"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("logiciel text-to-speech"),  QObject::trUtf8("logiciel text-to-speech"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("moteur de recherche"),  QObject::trUtf8("moteur de recherche"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("outil blogue"),  QObject::trUtf8("outil blogue"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("outil de mise en ligne"),  QObject::trUtf8("outil de mise en ligne"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("outil de modélisation"),  QObject::trUtf8("outil de modélisation"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("outil d'encodage"),  QObject::trUtf8("outil d'encodage"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("SGBD"),  QObject::trUtf8("SGBD"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("SIG"),  QObject::trUtf8("SIG"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("matériel"),  QObject::trUtf8("matériel"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("appareil photographique"),  QObject::trUtf8("appareil photographique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("baladeur"),  QObject::trUtf8("baladeur"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("borne informatique"),  QObject::trUtf8("borne informatique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("calculatrice"),  QObject::trUtf8("calculatrice"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("camescope"),  QObject::trUtf8("camescope"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("classe mobile"),  QObject::trUtf8("classe mobile"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("écran tactile"),  QObject::trUtf8("écran tactile"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("machine à commande numérique"),  QObject::trUtf8("machine à commande numérique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("matériel de lecture livre numérique"),  QObject::trUtf8("matériel de lecture livre numérique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("ordinateur"),  QObject::trUtf8("ordinateur"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("outil de microscopie"),  QObject::trUtf8("outil de microscopie"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("PDA"),  QObject::trUtf8("PDA"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("rétroprojecteur"),  QObject::trUtf8("rétroprojecteur"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("tablette informatique"),  QObject::trUtf8("tablette informatique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("TBI"),  QObject::trUtf8("TBI"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("vidéoprojecteur"),  QObject::trUtf8("vidéoprojecteur"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("outil de compensation"),  QObject::trUtf8("outil de compensation"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("ressource d'information"),  QObject::trUtf8("ressource d'information"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("atlas"),  QObject::trUtf8("atlas"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("base de données"),  QObject::trUtf8("base de données"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("carte heuristique"),  QObject::trUtf8("carte heuristique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("cédérom"),  QObject::trUtf8("cédérom"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("dictionnaire"),  QObject::trUtf8("dictionnaire"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("glossaire"),  QObject::trUtf8("glossaire"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("documentation technique"),  QObject::trUtf8("documentation technique"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("encyclopédie"),  QObject::trUtf8("encyclopédie"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("ENT"),  QObject::trUtf8("ENT"));
    m_EducationalToolsVocabSCOLOMFR.insert(QString::fromUtf8("podcast"),  QObject::trUtf8("podcast"));

    // 7.1
    m_RelationKindVocabSCOLOMFR.insert(QString::fromUtf8("a pour vignette"),  QObject::trUtf8("a pour vignette"));
    m_RelationKindVocabSCOLOMFR.insert(QString::fromUtf8("a pour logo"),  QObject::trUtf8("a pour logo"));
    m_RelationKindVocabSCOLOMFR.insert(QString::fromUtf8("a pour aperçu"),  QObject::trUtf8("a pour aperçu"));
    m_RelationKindVocabSCOLOMFR.insert(QString::fromUtf8("est l'aperçu de"),  QObject::trUtf8("est l'aperçu de"));

    //9.1
    /* scolomfr-voc-028-num-013 */
    m_ClassificationPurposeVocabSCOLOMFR.insert(QString::fromUtf8("label"),  QObject::trUtf8("label"));

    /* scolomfr-voc-028-num-014 */
    m_ClassificationPurposeVocabSCOLOMFR.insert(QString::fromUtf8("type de diffusion"),  QObject::trUtf8("type de diffusion"));

    /* scolomfr-voc-028-num-004 */
    m_ClassificationPurposeVocabSCOLOMFR.insert(QString::fromUtf8("enseignement"),  QObject::trUtf8("enseignement"));//Vocabulaire ScoLOMFR-voc-030, pour les référentiels définissant les programmes et thèmes des enseignements officiels. Il y a un vocabulaire par domaine d'enseignement (construit sur la base 030, 031, 032, etc.).

    /* d'où ça sort, ça ... est-ce "competency" scolomfr-voc-028-num-002 ? */
    m_ClassificationPurposeVocabSCOLOMFR.insert(QString::fromUtf8("compétences du socle commun"),  QObject::trUtf8("compétences du socle commun"));

    /* scolomfr-voc-028-num-011 */
    m_ClassificationPurposeVocabSCOLOMFR.insert(QString::fromUtf8("public cible détaillé"),  QObject::trUtf8("public cible détaillé"));

    /* scolomfr-voc-028-num-003 */
    m_ClassificationPurposeVocabSCOLOMFR.insert(QString::fromUtf8("domaine d'enseignement"),  QObject::trUtf8("domaine d'enseignement"));

    //Attention vocabulaires actualises le 9.12.2012 ... exemple
    //http://www.lom-fr.fr/scolomfr/fileadmin/user_upload/fiches_vocabulaire/scolomfr-voc-022_VDEX.xml
}

AbulEduSCOLOMFrV1::~AbulEduSCOLOMFrV1()
{
}

QByteArray AbulEduSCOLOMFrV1::abeLOMExportAsXML()
{
    //    QByteArray retour;
    //    if(m_GeneralIdentifierCatalog == "" || m_GeneralIdentifierEntry == "") {
    //        qDebug() << "La norme LOMFR n'est pas respectée: le GeneralIdentifierCatalog est vide";
    //        return retour;
    //    }

    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
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
    return XML;
}

bool AbulEduSCOLOMFrV1::abeLOMaddEducationalLearningResourceType(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_EducationalLearningResourceTypeVocabSCOLOMFR.contains(value)) {
        if(!m_EducationalLearningResourceType.contains("ScoLOMFRv1.0;" + value))
            m_EducationalLearningResourceType << "ScoLOMFRv1.0;" + value;;
        retour = true;
    }
    else {
        retour = AbulEduLOMFrV1::abeLOMaddEducationalLearningResourceType(value);
    }
    return retour;
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLearningResourceTypeVocabSCOLOMFR;
}

void AbulEduSCOLOMFrV1::abeLOMclearEducationalLearningResourceType()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalLearningResourceType.clear();
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralDocumentTypologyVocabSCOLOMFR;
}

void AbulEduSCOLOMFrV1::abeLOMclearGeneralDocumentTypology()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_GeneralDocumentTypology.clear();
    return;
}


bool AbulEduSCOLOMFrV1::abeLOMaddGeneralDocumentTypology(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString keyValue = m_GeneralDocumentTypologyVocabSCOLOMFR.key(value);
    if (keyValue.isEmpty())
    {
        return false;
    }
    bool retour;
    //    qDebug() << " ---------------------------- " << m_GeneralDocumentTypology;
    if(m_GeneralDocumentTypologyVocabSCOLOMFR.contains(value)) {
        if(!m_GeneralDocumentTypology.contains("ScoLOMFRv1.0;" + value)) {
            m_GeneralDocumentTypology << "ScoLOMFRv1.0;" + value;;
        }
        retour = true;
    }
    else
        retour = false;
    //    qDebug() << " ---------------------------- " << m_GeneralDocumentTypology;
    return retour;
}

QStringList AbulEduSCOLOMFrV1::abeLOMgetGeneralDocumentTypology()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_GeneralDocumentTypology;
}

bool AbulEduSCOLOMFrV1::abeLOMaddEducationalLocation(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<value ;

    QString keyValue = m_EducationalLocationVocabSCOLOMFR.key(value);
    if (keyValue.isEmpty())
    {
        return false;
    }
    bool retour;
    if(m_EducationalLocationVocabSCOLOMFR.contains(value)) {
        if(!m_EducationalLocation.contains("ScoLOMFRv1.0;" + value)) {
            m_EducationalLocation << "ScoLOMFRv1.0;" + value;
        }
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QStringList AbulEduSCOLOMFrV1::abeLOMgetEducationalLocation()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLocation;
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetEducationalLocationVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalLocationVocabSCOLOMFR;
}

void AbulEduSCOLOMFrV1::abeLOMclearEducationalLocation()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalLocation.clear();
    return;
}

bool AbulEduSCOLOMFrV1::abeLOMaddEducationalPedagogicModality(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<value;
    QString keyValue = m_EducationalPedagogicModalityVocabSCOLOMFR.key(value);
    if (keyValue.isEmpty())
    {
        return false;
    }
    bool retour;
    if(m_EducationalPedagogicModalityVocabSCOLOMFR.contains(value)) {
        if(!m_EducationalPedagogicModality.contains("ScoLOMFRv1.0;" + value))
            m_EducationalPedagogicModality << "ScoLOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QStringList AbulEduSCOLOMFrV1::abeLOMgetEducationalPedagogicModality()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalPedagogicModality;
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalPedagogicModalityVocabSCOLOMFR;
}

void AbulEduSCOLOMFrV1::abeLOMclearEducationalPedagogicModality()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalPedagogicModality.clear();
    return;
}

bool AbulEduSCOLOMFrV1::abeLOMaddEducationalTools(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<value;
    QString keyValue = m_EducationalToolsVocabSCOLOMFR.key(value);
    if (keyValue.isEmpty())
    {
        return false;
    }
    bool retour;
    if(m_EducationalToolsVocabSCOLOMFR.contains(value)) {
        if(!m_EducationalTools.contains("ScoLOMFRv1.0;" + value))
            m_EducationalTools << "ScoLOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = false;
    return retour;
}

QStringList AbulEduSCOLOMFrV1::abeLOMgetEducationalTools()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalTools;
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetEducationalToolsVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalToolsVocabSCOLOMFR;
}

void AbulEduSCOLOMFrV1::abeLOMclearEducationalTools()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_EducationalTools.clear();
    return;
}

bool AbulEduSCOLOMFrV1::abeLOMsetRelationKind(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_RelationKindVocabSCOLOMFR.contains(value)) {
        if(!m_RelationKind.contains("ScoLOMFRv1.0;" + value))
            m_RelationKind="ScoLOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = AbulEduLOMFrV1::abeLOMsetRelationKind(value);
    return retour;
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetRelationKindVocabSCLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_RelationKindVocabSCOLOMFR;
}

bool AbulEduSCOLOMFrV1::abeLOMaddClassificationPurpose(QString purpose,LomLS sourceTaxonPath,QString idTaxon, LomLS entryTaxon)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << purpose;

    QString aocPurpose = purpose;
    if(m_ClassificationPurposeVocabSCOLOMFR.contains(purpose))
    {
        aocPurpose = "ScoLOMFRv1.0;" + purpose;
    }
    return AbulEduLOMFrV1::abeLOMaddClassificationPurpose(aocPurpose,sourceTaxonPath,idTaxon,entryTaxon);
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetClassificationPurposeVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_ClassificationPurposeVocabSCOLOMFR;
}

QMap<QString,QString> AbulEduSCOLOMFrV1::abeLOMgetEducationalActivitiesVocabSCOLOMFR()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_EducationalActivitiesVocabSCOLOMFR;
}

bool AbulEduSCOLOMFrV1::abeLOMaddEducationalActivities(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< m_EducationalActivities;
//    QString keyValue = m_EducationalActivitiesVocabSCOLOMFR.key(value);
//    if (keyValue.isEmpty())
//    {
//        return false;
//    }
    bool retour;
    if(m_EducationalActivitiesVocabSCOLOMFR.contains(value)) {
        if(!m_EducationalActivities.contains("ScoLOMFRv1.0;" + value))
            m_EducationalActivities << "ScoLOMFRv1.0;" + value;
        retour = true;
    }
    else
        retour = AbulEduLOMFrV1::abeLOMaddEducationalActivities(value);

    ABULEDU_LOG_DEBUG() << m_EducationalActivities;
    return retour;
}

QByteArray AbulEduSCOLOMFrV1::p_abeLOMExportAsXML_header()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_domDocument.clear();
    QDomProcessingInstruction header = m_domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    m_domDocument.appendChild( header );

    QDomElement officeDocContent = m_domDocument.createElement("lomfr:lom");
    m_domDocument.appendChild(officeDocContent);

    officeDocContent.setAttribute("xmlns:lom","http://ltsc.ieee.org/xsd/LOM");
    officeDocContent.setAttribute("xmlns:lomfr","http://www.lom-fr.fr/xsd/LOMFR");
    officeDocContent.setAttribute("xmlns:scolomfr","http://www.lom-fr.fr/xsd/SCOLOMFR");
    officeDocContent.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    officeDocContent.setAttribute("xsi:schemaLocation","http://ltsc.ieee.org/xsd/LOM http://ltsc.ieee.org/xsd/lomv1.0/lom.xsd");

    m_domRootElement = officeDocContent;

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduSCOLOMFrV1::p_abeLOMExportAsXML_section1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    AbulEduLOMFrV1::p_abeLOMExportAsXML_section1();

    e = m_domCurrentElement;

    // 1.10
    for(int i = 0; i < m_GeneralDocumentTypology.count(); i++)
    {
        QStringList contenu = m_GeneralDocumentTypology.at(i).split(";");
        QString cle = contenu.takeFirst();
        QString valeur = contenu.join(";");

        e_fils = m_domDocument.createElement(m_xmlPrefix + "generalResourceType");
        e_fils2 = m_domDocument.createElement(m_xmlPrefix + "source");
        e_fils.appendChild(e_fils2);
        t = m_domDocument.createTextNode(cle);
        e_fils2.appendChild(t);

        e_fils2 = m_domDocument.createElement(m_xmlPrefix + "value");
        e_fils.appendChild(e_fils2);
        //TODO
        t = m_domDocument.createTextNode(valeur);
        e_fils2.appendChild(t);
        e.appendChild(e_fils);
    }
    //    if(!vide)

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

QByteArray AbulEduSCOLOMFrV1::p_abeLOMExportAsXML_section5()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement e;
    QDomElement e_fils;
    QDomElement e_fils2;
    QDomText t;

    AbulEduLOMFrV1::p_abeLOMExportAsXML_section5();

    e = m_domCurrentElement;

    // 5.14
    if(abeLOMgetEducationalLocation().count()) {
        QListIterator<QString> i(abeLOMgetEducationalLocation());
        while (i.hasNext()) {
            QStringList contenu = i.next().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "place");
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

    // 5.15
    if(abeLOMgetEducationalPedagogicModality().count()) {
        QListIterator<QString> i(abeLOMgetEducationalPedagogicModality());
        while (i.hasNext()) {
            QStringList contenu = i.next().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");
            e_fils = m_domDocument.createElement(m_xmlPrefix + "educationalMethod");
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

    // 5.16
    if(abeLOMgetEducationalTools().count()) {
        QListIterator<QString> i(abeLOMgetEducationalTools());
        while (i.hasNext()) {
            QStringList contenu = i.next().split(";");
            QString cle = contenu.takeFirst();
            QString valeur = contenu.join(";");

            e_fils = m_domDocument.createElement(m_xmlPrefix + "tool");
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
    return XML;
}

bool AbulEduSCOLOMFrV1::p_abeLOMImportXML_section1(QDomElement n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduLOMFrV1::p_abeLOMImportXML_section1(n);
    //Debut du parcours de l'arbre a partir de e
    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();

            //1.1 Identifiant
            if(e.tagName() == m_xmlPrefix + "generalResourceType") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                if(!e2.isNull()) {
                    abeLOMaddGeneralDocumentTypology(e2.text());
                }
            }
        }
        fils = fils.nextSibling();
    }
    return true;

}

bool AbulEduSCOLOMFrV1::p_abeLOMImportXML_section5(QDomNode n)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduLOMFrV1::p_abeLOMImportXML_section5(n);

    QDomNode fils = n.firstChild();
    while (!fils.isNull()) {
        if (fils.isElement()) {
            QDomElement e = fils.toElement();

            //5.14 Lieu: place
            if(e.tagName() == m_xmlPrefix + "place") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalLocation(e2.text());
            }

            //5.15 Modalités pedagogiques
            if(e.tagName() == m_xmlPrefix + "educationalMethod") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalPedagogicModality(e2.text());
            }

            //5.16 Outils
            if(e.tagName() == m_xmlPrefix + "tool") {
                QDomElement e2 = e.firstChildElement(m_xmlPrefix + "value");
                abeLOMaddEducationalTools(e2.text());
            }

        }
        fils = fils.nextSibling();
    }
    return true;
}

//TODO
//QDate AbulEduSCOLOMFrV1::abeLOMgetLifeCycleContributionLastDate(QString role)
//{
//               QDate date;//initialisation à la date du jour

////       parcours des <lom:contribute> où <lom:role><lom:value> est m_LifeCycleContributionRoleVocabLOM.key(role)
////       en gardant la valeur trouvee dans <lom:date><lom:dateTime> si elle est plus grande que ...
//               return date;
//}

//TODO
//QList<vCard> AbulEduSCOLOMFrV1::abeLOMgetLifeCycleContributionEntities(QString role, QDate date)
//{
//       QList<vCard> entitiesList;
////       parcours des <lom:contribute> où <lom:role><lom:value> est role et <lom:date><lom:dateTime> est date
////       avec append dans entitiesList quand on en trouve
//       return entitiesList;
//}
