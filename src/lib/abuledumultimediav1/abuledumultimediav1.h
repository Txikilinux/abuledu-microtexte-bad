/** Classe AbulEduMultiMediaV1
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
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
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef ABULEDUMULTIMEDIAV1_H
#define ABULEDUMULTIMEDIAV1_H

#include <QWidget>
#include <QPainter>
#include <QStateMachine>
#include <QFinalState>
#include <QAudioDeviceInfo>
#include "abuleducontrolaudiov1.h"
#include "abuledumediaviewerinterface.h"
#include "abuleduanimatedgifviewerv1.h"
#include "abuleduvideoviewerv1.h"
#include "abuledusoundplayerv1.h"
#include "abuledusoundrecorderv1.h"
#include "abuleduslidesrecorderv1.h"
#include "abuleduslidesviewerv1.h"
#include "abuledupicottsv1.h"
#include "abuledusounddevices.h"
#include "abuledumultimediasettingsv1.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"
#include "abuledumediamedias.h"

namespace Ui {
class AbulEduMultiMediaV1;
}
/** @brief Cette classe implémente un widget (QWidget) permettant la visualisation de divers documents multimédia.
 *  \li Elle implémente un controleur qui installe une télécommande (AbulEduControlAudio) à trois boutons Play Pause Stop
 *  (et d'autre aussi : Next, Previous, Record, Hide)
 *  \li Elle fournit aussi une interface (AbulEduMediaViewerInterface) permettant l'accès aux classes de mediaViewer dont la responsabilité est d'afficher les médias passés en paramètre
 */
class AbulEduMultiMediaV1 : public QWidget
{
    Q_OBJECT
    Q_ENUMS(enumPlayerState enumMediaViewerType enumControlButton)
    Q_FLAGS(flagControlButtons)
public:
    /** Précise l'état dans lequel est le player
     * \li Playing : le player est entrain de jouer.
     * \li Paused  : le player est stoppé, il repartira où il s'est arrêté.
     * \li Stopped : le played est arrêté, il reprendra au début.
     * \li Waiting : état d'attente entre deux medias (mode manuel).
     * \li Playing : le "player" enregistre.

     */
    enum enumPlayerState{Playing, Paused, Stopped, Waiting, Recording};

    /** Type de médias à jouer
     *  Cette liste est destinée à s'allonger grâce au polymorphisme
     *  \li Diapos : une série d'images avec du son et du texte.
     *  \li AnimatedGif : une série de gif animés avec du son et du texte.
     *  \li  Video : une série de vidéo avec du son et du texte.
     *  \li  Sound : une série de sons avec du texte.
     *  \li  Recorder : un enregistreur de son.
     */
    enum enumMediaViewerType{Full, Diapos, AnimatedGif, Video, Sound, Recorder};

    /// Énumère les boutons de la télécommande
    enum enumControlButton{BtnPlay = 1, BtnPause = 2, BtnStop = 4, BtnRecord = 8, BtnPrevious = 16, BtnNext = 32, BtnMagnifyingGlass = 64, BtnHide = 128, BtnToggleOutput = 256};


    Q_DECLARE_FLAGS(flagControlButtons, enumControlButton)

    /** Crée un player avec le diaporama passé en paramètre
     *  @param medias La liste des diapos à jouer
     *  @note La liste est composée d'instances d'AbulEduMediaMedias : chemin image, chemin son, text
     */
    explicit AbulEduMultiMediaV1(QList<AbulEduMediaMedias> medias, enumMediaViewerType type = Full, AbulEduPicottsV1::AbulEduLang language = AbulEduPicottsV1::None, bool textIsVisible = true, AbulEduMultiMediaSettingsV1::enumPlayMode mode = AbulEduMultiMediaSettingsV1::Automatic,  QWidget *parent = 0);

        /** Crée un player "vide"
         * @param type le type de media player voir AbulEduMultiMediaV1::MediaViewerType
         */
    explicit AbulEduMultiMediaV1(enumMediaViewerType type = Full, QWidget *parent = 0);

    /** Crée un player "vide" et promouvable depuis le designer
     * @param parent l'objet parent de l'interface
     * @note le type par défaut est Full
     */
    explicit AbulEduMultiMediaV1(QWidget *parent = 0);

    ~AbulEduMultiMediaV1();

    inline QList<AbulEduMediaMedias> abeMultiMediaGetListMedia(){return m_listMedias;}

    /** Stoppe le diaporama en cours s'il y en a un et charge le diaporama en paramètre
     *  @param medias La liste des diapos à jouer
     *  @note La liste est composée d'instances d'AbulEduMediaMedias : chemin image, chemin son, text
     */
    void abeMultiMediaSetNewMedia(QList<AbulEduMediaMedias> medias);

    /** Réimplémentation qui permet de ne passer qu'un seul AbulEduMediaMedias
     *  @param media Le média à partir duquel on va construire une liste à une seule entrée
     */
    void abeMultiMediaSetNewMedia(AbulEduMediaMedias media = AbulEduMediaMedias(QString(),QString(),QString()));

    /** Précise le mode de fonctionnement du player
     *  \li Autostart : démarrage automatique + enchainement automatique
     *  \li Automatic : démarage manuel       + enchainement automatique
     *  \li Manuel    : démarrage et défilement manuel
     */
    void abeMultiMediaSetPlayMode(AbulEduMultiMediaSettingsV1::enumPlayMode mode);

    /** Change le type de viewer du MultiMedia
     * @param mediaType Le type de viewer
     * \li Diapos : un viewer pour images fixes
     * \li AnimatedGif : un viewer pour les gif animés
     * \li Video : un viewer pour les vidéos
     * \li Sound : un player de sons sans image
     * \li Recorder : permet d'enregistrer un son au micro et de l'écouter
     */
    void abeMultiMediaSetMediaViewerType(enumMediaViewerType mediaType);

    /** Retourne un pointeur sur le widget de visualisation
     *  @return Pointeur sur le viewerWidget (AbulEduMediaViewerInterface *)
     */
    inline AbulEduMediaViewerInterface * abeMultiMediaGetViewerWidget(){return m_viewerWidget;}

    inline QTextEdit * abeMultiMediaGetTextEdit(){return abeMultiMediaGetViewerWidget()->abeMediaViewerGetTextEdit();}
    QString abeMultiMediaGetText();

    /** Retourne un pointeur sur le widget de télécommande
     *  @return Pointeur sur le widget de télécommande (AbuleduAudioControlV1)
     */
    AbulEduControlAudioV1 * abeMultiMediaGetAudioControlWidget();

    AbulEduMediaMedias abeMultiMediaGetCurrentMediaMedia();

    /** Met à jour le sous-titre dans la structure de données */
    void abeMultiMediaUpdateSubtitle(const QString& st);

    /** Retourne la langue utilisée pour la synthèse vocale
     * @return AbulEduMultiMediaSettingsV1::None si on n'est pas en synthèse vocale */
    inline AbulEduPicottsV1::AbulEduLang abeMultiMediaGetTTSlang() {return m_TTSLanguage;}

    /** Permet de démarrer le lecteur depuis "l'extérieur" */
    void abeMultiMediaPlay();

    /** Permet de lancer un media precis depuis l'extérieur */
    void abeMultiMediaPlay(int mediaToPlay );

    /** Permet d'arreter le lecteur depuis "l'extérieur" */
    void abeMultiMediaStop();

    /** Permet de mettre le lecteur en pause depuis "l'extérieur" */
    void abeMultiMediaPause();

    /** Permet de passer au média suivant depuis "l'extérieur" */
    void abeMultiMediaNext();

    /** Permet de passer au média précédent depuis "l'extérieur" */
    void abeMultiMediaPrevious();

    /** Afficher ou pas la barre de controle (télécommande)
     *  @param visible true, on la voit, false elle est cachée
     */
    void abeMultiMediaSetControlVisible(bool visible);

    /** Afficher ou pas les boutons passés en paramètre
     * @param buttons Combinaison de ControlButton
     * @param visible Visibilité des boutons
     */
    void abeMultiMediaSetButtonVisible(flagControlButtons buttons, bool visible);

    /** Active/désactive les boutons passés en paramètre
     * @param buttons Combinaison de ControlButton
     * @param enabled Activation des boutons
     */
    void abeMultiMediaSetButtonEnabled(flagControlButtons buttons, bool enabled);

    /** Fixe la position du texte par rapport à l'image
     *  @param align Positon du texte par rapport à l'image valeurs possibles Top Right, Bottom, Left
     */
    void abeMultiMediaSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align);

    /// Affiche ou cache le texte du viewer
    /// @param visible Affiche ou masque le texte
    void abeMultiMediaSetTextVisible(bool visible);

    void abeMultiMediaSetLevelMeterVisible(bool visible);

    /** Active ou désactive la possibilité de lire le texte (TTS)
     * @param activeTTS Activé  ou Désactivé
     */
    void abeMultimediaSetTTS(AbulEduPicottsV1::AbulEduLang language);

    /** Fixe le délai après lequel on passe à l'image suivante s'il n'y a pas de son enregistré */
    inline void abeMultiMediaSetDefaultDelay(int delay) {m_defaultDelay = delay;}

    /** Retourne le délai après lequel on passe à l'image suivante s'il n'y a pas de son enregistré */
    inline int abeMultiMediaGetDefaultDelay() {return m_defaultDelay;}

    /** Force (c'est à dire sans attendre de signal) l'exécution du slotAbeViewerPlayingMediaFinished(); */
    inline void abeMultiMediaForceStop(){slotAbeViewerPlayingMediaFinished();}

    enumPlayerState abeMultiMediaGetPlayerState() const {return m_playerState;}

    int abeMultiMediaGetPicoSpeed() const;

#if defined (Q_OS_WIN)
    /** Lance la construction d'une QMap dont les keys sont les noms (relatifs) des fichiers ogg et les values la durée de ces fichiers */
    void abeMultiMediaMapDurations();

    /** Getteur de la QMap qui permet de stocker les durées des fichiers ogg */
    inline QMap<QString,qint64> abeMultiMediaGetSoundDurations(){return m_soundDurations;}
#endif

public slots:
    void abeMultiMediaSetPicoSpeed(int picoSpeed);

private:
    Ui::AbulEduMultiMediaV1 *ui;
    void initialize();
    void initStateMachine();

    /** Envoie les données au viewer et comble les paramètres manquants avec "" */
    void abeMultiMediaSetMedia(int numMedia);

    /** Crée un lecteur PicoTTS
     * @param lang la langue dans laquelle le texte sera lu
     */
    void initPicoTTS(AbulEduPicottsV1::AbulEduLang lang);

    QStateMachine   *m_stateMachine;
    QState          *m_initialState;
    QState          *m_stoppedState;
    QState          *m_playingState;
    QState          *m_pausedState;
    QState          *m_waitingState; // La lecture est terminée, on attend la suite
    QState          *m_recordingState;
    QFinalState     *m_finalState;

    /** @note La liste est composée d'instances de AbulEduMediaMedias */
    QList<AbulEduMediaMedias>                      m_listMedias;
    enumPlayerState                                m_playerState;
    enumMediaViewerType                            m_playerType;
    AbulEduMultiMediaSettingsV1::enumPlayMode      m_playMode;
    AbulEduMediaViewerInterface                   *m_viewerWidget;
    AbulEduPicottsV1                              *m_picoTTS;
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    QAudioDeviceInfo                               m_outputDevice;
#endif
    AbulEduPicottsV1::AbulEduLang   m_TTSLanguage; //On garde en variable locale pour pouvoir avoir un tts allemand en plus d'un tts par defaut par exemple
    QSharedPointer<AbulEduMultiMediaSettingsV1>    m_abuleduMultiMediaSettings;

    int m_currentMedia;
    bool m_isTextVisible;
    int m_defaultDelay;
    int m_picoSpeed;

#if defined (Q_OS_WIN)
    QMap<QString,qint64>                m_soundDurations;
#endif

public slots:
    inline void slotAbeMultimediaSetTTSfromIso6391(const QString& codeIso){abeMultimediaSetTTS(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetTTSLanguageFromIso6391(codeIso));}

private slots:
    /** Joue le media suivant */
    void playNextMedia();

    /* Les slots de la StateMachine */
    void slotAbeInitialStateEntered();
    void slotAbePlayingStateEntered();
    void slotAbePausedStateEntered();
    void slotAbeStoppedStateEntered();
    void slotAbeWaitingStateEntered();
    void slotAbeRecordingStateEntered();
    void slotAbeRecordingStateExited();

    /* Slot concernant le media joué */
    void slotAbeViewerPlayingMediaFinished();
    void slotAbeViewerPlayingMediaStoppedCurrentTime   (qint64 currentTime);
    void slotAbeViewerPlayingMediaPausedCurrentTime    (qint64  currentTime);
    void slotAbeViewerLevel(qreal level);

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    void slotAbePlayerToggleOutput(bool checked);
#endif
    /* Slot permettant de modifier le déroulement normal */
    void slotAbeNextMediaRequested();
    void slotAbePreviousMediaRequested();
    void slotAbeMediaRequested(int mediaToPlay);
    void slotAbeStopAndHideMultiMedia();
    void slotAbeRecordingRequested();
    void slotAbeZoomRequested();

    void paintEvent(QPaintEvent *);

#if defined (Q_OS_WIN)
    /** Ajoute chaque fois qu'un signal durationChanged(qint64) est émis par le QMediaPlayer un élément dans la QMap m_soundDurations */
    void slotAddDuration(qint64 duration);
#endif
signals:
    /* Signaux concernant la gestion des médias */
    void signalAbeMultiMediaStarted            (int currentMedia);
    void signalAbeMultiMediaCurrentMediaChanged(int currentMedia);
    void signalAbeMultiMediaCurrentMediaPaused (int currentMedia);
    void signalAbeMultiMediaStateChanged       (AbulEduMultiMediaV1::enumPlayerState newState);
    void signalAbeMultiMediaStopped            (int currentMedia);
    void signalAbeMultiMediaFinished(); // Tous les médias ont été joués

    /* Signaux concernant le controle du Player */
    void signalAbePlayRequested();
    void signalAbePauseRequested();
    void signalAbeResumeRequested();
    void signalAbeStopRequested();
    void signalAbeWaitingRequested();
    /** Le MultiMedia est arrêté et caché, en général après un appui sur le bouton de fermeture */
    void signalAbeMultiMediaStoppedAndHidden();
    void signalAbeMultimediaZoomRequested();
    /* Signaux apportant des renseignements sur le média en cours */
    void signalAbeMediaDuration(qint64 duration);
    void signalAbeMediaPosition(qint64 position);
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    void signalAbeMultiMediaMediaStatusChanged(QMediaPlayer::MediaStatus newStatus);
#endif

#if defined (Q_OS_WIN)
    void signalAbeMultiMediaDurationsMapped();
#endif
};
    Q_DECLARE_OPERATORS_FOR_FLAGS(AbulEduMultiMediaV1::flagControlButtons)
#endif // ABULEDUMULTIMEDIAV1_H
