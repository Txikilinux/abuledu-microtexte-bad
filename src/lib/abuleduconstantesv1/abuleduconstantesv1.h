/** Définition des constantes abuledu boite à outils v1
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduConstantesV1
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * Modification du .pro pour ajouter
  * #AbulEduConstantesV1
  * include(lib/abuleduconstantesv1/abuleduconstantesv1.pri)
  *
  */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef ABULEDUCONSTANTESV1_H
#define ABULEDUCONSTANTESV1_H


#include <QObject>
#include <QMap>
#include <QFont>

namespace abe {

    /** le nom du serveur local abuledu : servecole 1998->???? */
    static QString ABE_LAN_SERVERNAME         = "servecole.abuledu";
    static QString ABE_LAN_SERVERWEBADMINURI  = "http://servecole:8082/horizon-webadmin-xml/";
    static QString ABE_LAN_SERVERFILESURI     = "http://servecole/horizon-webadmin-files/";

#ifndef QT_NO_SSL
    static QString ABE_WAN_SERVERWEBADMINURI  = "https://webadminxml.abuledu.net/";
    static QString ABE_WAN_SERVERFILESURI     = "https://files.abuledu.net/";
#else
    //Il faut trouver des serveurs qui acceptent en mode non securise ... LAN ?
    static QString ABE_WAN_SERVERWEBADMINURI  = "http://webadminxml.abuledu.net/";
    static QString ABE_WAN_SERVERFILESURI     = "http://files.abuledu.net/";
#endif

    /** Les niveaux */
    enum ABE_LEVEL {Outil, CP, CE1, CE2, CM1, CM2};

    static inline QMap<abe::ABE_LEVEL,QString> ABE_DONNENIVEAU() {
        QMap<abe::ABE_LEVEL,QString> map;
        map.insert(Outil,QObject::trUtf8("Outil"));
        map.insert(CP,QObject::trUtf8("CP"));
        map.insert(CE1,QObject::trUtf8("CE1"));
        map.insert(CE2,QObject::trUtf8("CE2"));
        map.insert(CM1,QObject::trUtf8("CM1"));
        map.insert(CM2,QObject::trUtf8("CM2"));
        return map;
    }

    /** Les natures */
    enum ABE_NATURE {VER, NOM, ADJ, ADV, PRE, PRO, CON, DET};

    static inline QMap<abe::ABE_NATURE,QString> ABE_DONNENATURE() {QMap<abe::ABE_NATURE,QString> map;
                                                map.insert(VER,QObject::trUtf8("Verbe"));
                                                map.insert(NOM,QObject::trUtf8("Nom"));
                                                map.insert(ADJ,QObject::trUtf8("Adjectif"));
                                                map.insert(ADV,QObject::trUtf8("Adverbe"));
                                                map.insert(PRE,QObject::trUtf8("Préposition"));
                                                map.insert(PRO,QObject::trUtf8("Pronom"));
                                                map.insert(CON,QObject::trUtf8("Conjonction"));
                                                map.insert(DET,QObject::trUtf8("Déterminant"));
                                                return map;}
    /** Les types utilisateurs
      * - abuleduetiquettesv1
      */
    enum ABE_USERTYPE {
        abeEtiquettePiece,
        abeEtiquetteCellule,
        abeGraphicsEtiquette,
        abeContourPixmapItem
    };

    /** Les évènements
      * - abuleduexercicev1
      * - abuledulivret
      * - abuledustatemachine
      */
    enum ABE_EVENT {
        LogsPush, //plugin
        LogsDownload,
        UsersDownload,
        GroupsDownload,
        DomainsDownload,
        ApplicationsDownload,
        SkillsDownload,
        ExercicesDownload,
        LevelsDownload,
        LivretDownload,
        StateMachineStringEvent,
        LeTerrierStateMachineStringEvent
    };

    /** Les évaluations
      *   evalA : très bien
      *   evalB : bien
      *   evalC : moyen
      *   evalD : mal
      *   evalY : en attente, pas encore répondu -> attente
      *   evalZ : pas répondu, abandonné, a esquivé la réponse -> rien
      */
    enum ABE_EVAL { evalA, evalB, evalC, evalD, evalY, evalZ };

    static inline QMap<abe::ABE_EVAL,QString> ABE_DONNEEVAL() {
        QMap<abe::ABE_EVAL,QString> map;
        map.insert(evalA,QObject::trUtf8("Très bien"));
        map.insert(evalB,QObject::trUtf8("Bien"));
        map.insert(evalC,QObject::trUtf8("Moyen"));
        map.insert(evalD,QObject::trUtf8("Insuffisant"));
        map.insert(evalY,QObject::trUtf8("En attente"));
        map.insert(evalZ,QObject::trUtf8("Non répondu"));
        return map;
    }


    //a reflechir pour les mimecodes (dont on pourrait imaginer un type un peu générique qui serait manipulable entre les applications
    //abuledu)

    /** des codes couleurs */
    static QString couleurBienPlacee    = "background-color: #00ff00";
    static QString couleurMalPlacee     = "background-color: #ff0000";
    static QString STYLESELECTION       = "background-color: #FFDAB9";
    static QString STYLEERREUR          = "background-color: #FF7F50";
    static QString STYLENORMAL          = "background-color: #CCCCCC";
    static QString STYLEBLANC           = "background-color: #FFFFFF";
}

#endif // ABULEDUCONSTANTESV1_H
