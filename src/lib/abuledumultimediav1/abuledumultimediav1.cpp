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

#include "abuledumultimediav1.h"
#include "ui_abuledumultimediav1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMultiMediaV1::AbulEduMultiMediaV1(QList<AbulEduMediaMedias> medias, AbulEduMultiMediaV1::enumMediaViewerType type, AbulEduPicottsV1::AbulEduLang language, bool isTextVisible, AbulEduMultiMediaSettingsV1::enumPlayMode mode, QWidget *parent):
    QWidget(parent),
    ui(new Ui::AbulEduMultiMediaV1),
    m_listMedias(medias),
    m_playerState(Stopped),
    m_playerType(type),
    m_viewerWidget(0),
    m_picoTTS(0),
    m_currentMedia(-1),
    m_defaultDelay(-1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->setupUi(this);
    setObjectName("MultiMediaWidget");
    m_isTextVisible = isTextVisible;
    abeApp->getAbeApplicationMultiMediaSettings()->abeMultiMediaSettingsSetIsTextVisible(m_isTextVisible);
    initialize();

    /* Specificites */
    m_TTSLanguage = language;
    m_playMode = mode;
}

AbulEduMultiMediaV1::AbulEduMultiMediaV1(enumMediaViewerType type, QWidget *parent):
    QWidget(parent),
    ui(new Ui::AbulEduMultiMediaV1),
    m_listMedias(QList<AbulEduMediaMedias>()),
    m_playerState(Stopped),
    m_playerType(type),
    m_viewerWidget(0),
    m_picoTTS(0),
    m_currentMedia(-1),
    m_defaultDelay(-1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    setObjectName("MultiMediaWidget");
    initialize();
}

AbulEduMultiMediaV1::AbulEduMultiMediaV1(QWidget *parent):
    QWidget(parent),
    ui(new Ui::AbulEduMultiMediaV1),
    m_listMedias(QList<AbulEduMediaMedias>()),
    m_playerState(Stopped),
    m_playerType(Full),
    m_viewerWidget(0),
    m_picoTTS(0),
    m_currentMedia(-1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    setObjectName("MultiMediaWidget");
    initialize();
}

void AbulEduMultiMediaV1::initialize()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //On recupere les parametres "partagés"
    m_abuleduMultiMediaSettings = abeApp->getAbeApplicationMultiMediaSettings();

    m_playMode            = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetPlayMode();
    m_TTSLanguage         = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetTTSLanguage();
    m_isTextVisible       = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetIsTextVisible();
    abeMultiMediaSetPicoSpeed(80);
    connect(abeMultiMediaGetAudioControlWidget(), SIGNAL(signalAbeControlAudioSpeedChanged(int)),this, SLOT(abeMultiMediaSetPicoSpeed(int)),Qt::UniqueConnection);

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    m_outputDevice = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioOutputDeviceInfo();
    if(m_outputDevice != m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetHeadPhone()) {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonToggleOutputDeviceChecked(false);
    }
    else {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonToggleOutputDeviceChecked(true);
    }
#endif
    /* Ici, on spécifie le type de media à visionner. Cette liste ne demande qu'à grandir :-) */
    switch (m_playerType)
    {
    case Full: // Type Raconte-moi ou image séquentielles + enregistreur
        m_viewerWidget = new AbulEduSlidesRecorderV1();
        ui->wgAbulEduControlAudio->abeControlAudioSetType(AbulEduControlAudioV1::RecorderType);
        ui->wgAbulEduControlAudio->abeControlAudioGetAudioLevelMeter()->setVisible(true);

        abeMultiMediaSetButtonVisible(AbulEduMultiMediaV1::BtnRecord, true);
        break;
    case Diapos: /* Type Raconte-moi ou image séquentielles */
        m_viewerWidget = new AbulEduSlidesViewerV1();
        break;
    case AnimatedGif: // Comme son nom l'indique
        m_viewerWidget = new AbulEduAnimatedGifViewerV1();
        break;
    case Video: // Comme son nom l'indique
        m_viewerWidget = new AbulEduVideoViewerV1();
        break;
    case Sound: // Comme son nom l'indique
        m_viewerWidget = new AbulEduSoundPlayerV1();
        break;
    case Recorder: // Comme ...
        m_viewerWidget = new AbulEduSoundRecorderV1();
        ui->wgAbulEduControlAudio->abeControlAudioSetType(AbulEduControlAudioV1::RecorderType);
        ui->wgAbulEduControlAudio->abeControlAudioGetAudioLevelMeter()->setVisible(true);
        break;
    default:
        break;
    }
    /* On ne peut pas promouvoir dans le designer car on ne sait pas de quel type il va s'agir */
    ui->vlGlobal->insertWidget(0, m_viewerWidget);
    //    m_viewerWidget->setStyleSheet("background-color:white");
    connect(m_viewerWidget, SIGNAL(signalAbeMediaStopped(qint64 )),
            this, SLOT(slotAbeViewerPlayingMediaStoppedCurrentTime(qint64 )));
    connect(m_viewerWidget, SIGNAL(signalAbeMediaPaused(qint64)),
            this, SLOT(slotAbeViewerPlayingMediaPausedCurrentTime(qint64 )));
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    connect(m_viewerWidget, SIGNAL(signalAbeMediaViewerMediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SIGNAL(signalAbeMultiMediaMediaStatusChanged(QMediaPlayer::MediaStatus)));
#endif
    /* Le signal est émis à la fin de chaque image, on s'en sert pour passer à la lecture de la suivante s'il y en a une */
    connect(m_viewerWidget, SIGNAL(signalAbeMediaFinished()),
            this, SLOT(slotAbeViewerPlayingMediaFinished()));
    connect(m_viewerWidget, SIGNAL(signalAbeMediaDuration(qint64)),
            this, SIGNAL(signalAbeMediaDuration(qint64)));
    connect(m_viewerWidget, SIGNAL(signalAbeMediaPositionChanged(qint64)),
            this, SIGNAL(signalAbeMediaPosition(qint64)));
    /* Les signaux émis par les bouton Play Stop Pause Record sont traités par la statemachine */
    connect(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioNextClicked()),     this, SLOT(slotAbeNextMediaRequested()), Qt::UniqueConnection);
    connect(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioPreviousClicked()), this, SLOT(slotAbePreviousMediaRequested()), Qt::UniqueConnection);
    connect(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioBtnHideClicked()),  this, SLOT(slotAbeStopAndHideMultiMedia()), Qt::UniqueConnection);
    connect(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioMagnifyingGlassClicked()),  this, SLOT(slotAbeZoomRequested()), Qt::UniqueConnection);
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    connect(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioToggleOutput(bool)), this, SLOT(slotAbePlayerToggleOutput(bool)), Qt::UniqueConnection);
#endif
    if(m_playerType == Recorder || m_playerType == Full)
    {
        connect(m_viewerWidget, SIGNAL(signalAbeMediaViewerLevel(qreal)), this, SLOT(slotAbeViewerLevel(qreal)), Qt::UniqueConnection);
    }
    /* Créer un lecteur TTS  si nécessaire */
    initPicoTTS(m_TTSLanguage);

    abeMultiMediaSetTextVisible(m_isTextVisible);
    /* Une petite statemachine pour gérer tout ça */
    initStateMachine();
    m_stateMachine->start(); // Et hop !
}

AbulEduMultiMediaV1::~AbulEduMultiMediaV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* erics : tentative de non suppression de l'objet si c'est pas qapplication qui supprime (je schématise) */
    if(parentWidget() != 0) {
//        QList<AbulEduMediaMedias> li;
//        AbulEduMediaMedias med(QString(),QDir::homePath()+"/Bureau/vie_se_chante.ogg",QString());
//        li.append(med);
//        AbulEduMultiMediaV1 *toto = new AbulEduMultiMediaV1(li,AbulEduMultiMediaV1::Sound,AbulEduMultiMediaV1::fre,false,AbulEduMultiMediaV1::AutoStart);
        if(m_viewerWidget)
        {
            delete m_viewerWidget;
        }
        if(m_stateMachine)
        {
            m_stateMachine->stop();
            delete m_stateMachine;
        }
        if(m_picoTTS)
        {
            delete m_picoTTS;
        }
        delete ui;
    }
}

void AbulEduMultiMediaV1::abeMultiMediaSetNewMedia(QList<AbulEduMediaMedias> medias)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_listMedias = medias;
    /* On arrête tout */
//    m_viewerWidget->slotAbeMediaViewerStop();
    /* Et on redémarre à zéro */
    m_currentMedia = 0;
    abeMultiMediaSetMedia(m_currentMedia);
}

void AbulEduMultiMediaV1::abeMultiMediaSetNewMedia(AbulEduMediaMedias media)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(media.isEmpty()){
        abeMultiMediaSetNewMedia(QList<AbulEduMediaMedias>());
    }
    QList<AbulEduMediaMedias> list;
    list.append(media);
    abeMultiMediaSetNewMedia(list);
}

void AbulEduMultiMediaV1::abeMultiMediaSetPlayMode(AbulEduMultiMediaSettingsV1::enumPlayMode mode)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_playMode = mode;
}

void AbulEduMultiMediaV1::abeMultiMediaSetMediaViewerType(AbulEduMultiMediaV1::enumMediaViewerType mediaType)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_playerType = mediaType;
    /* Il faut détruire le viewerWidget qui existe avant d'en crée un autre */
    if(m_viewerWidget)
    {
        delete m_viewerWidget;
    }
    initialize();
}

QString AbulEduMultiMediaV1::abeMultiMediaGetText()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString retour = QString();
    if(abeMultiMediaGetTextEdit())
    {
        retour = abeMultiMediaGetTextEdit()->toPlainText();
    }
    return retour;
}

AbulEduControlAudioV1 *AbulEduMultiMediaV1::abeMultiMediaGetAudioControlWidget()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return ui->wgAbulEduControlAudio;
}

AbulEduMediaMedias AbulEduMultiMediaV1::abeMultiMediaGetCurrentMediaMedia()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ABULEDU_LOG_DEBUG() << m_listMedias.size()<<"::"<<m_currentMedia;
    if(m_listMedias.isEmpty()){
        return AbulEduMediaMedias();
    }
    if(m_currentMedia < m_listMedias.size()){
        if(m_currentMedia == -1)
        {
            return m_listMedias.at(0);
        }
        return m_listMedias[m_currentMedia];
    }
    else {
        ABULEDU_LOG_DEBUG() << trUtf8("Pas de media à retourner");
        return AbulEduMediaMedias();
    }
}

void AbulEduMultiMediaV1::abeMultiMediaUpdateSubtitle(const QString &st)
{
    if(m_listMedias[m_currentMedia].abeMediaMediaGetText().simplified() != st.simplified()){
        m_listMedias[m_currentMedia].abeMediaMediaSetText(st.simplified());
        abeMultiMediaGetTextEdit()->clearFocus();
    }
}

void AbulEduMultiMediaV1::abeMultiMediaPlay()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbePlayRequested();
}

void AbulEduMultiMediaV1::abeMultiMediaStop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeStopRequested();
}

void AbulEduMultiMediaV1::abeMultiMediaPause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbePauseRequested();
}

void AbulEduMultiMediaV1::abeMultiMediaNext()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeNextMediaRequested();
}

void AbulEduMultiMediaV1::abeMultiMediaPrevious()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbePreviousMediaRequested();
}

void AbulEduMultiMediaV1::abeMultiMediaPlay(int mediaToPlay)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaRequested(mediaToPlay);
}

void AbulEduMultiMediaV1::abeMultiMediaSetControlVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->wgAbulEduControlAudio->abeControlAudioSetVisible(visible);
}

//void AbulEduMultiMediaV1::abeMultiMediaSetNextAndPreviousVisible(bool visible)
//{
//    if(m_localDebug) qDebug()  <<"++++++++"<<__FUNCTION__;
//    ui->wgControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnNext | AbulEduControlAudioV1::BtnPrevious, visible);
//}

void AbulEduMultiMediaV1::abeMultiMediaSetButtonVisible(flagControlButtons buttons, bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(buttons.testFlag(BtnPlay))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnPlay, visible);
    }
    if(buttons.testFlag(BtnPause))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnPause, visible);
    }
    if(buttons.testFlag(BtnStop))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnStop, visible);
    }
    if(buttons.testFlag(BtnMagnifyingGlass))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnMagnifyingGlass, visible);
    }
    if(buttons.testFlag(BtnNext))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnNext, visible);
    }
    if(buttons.testFlag(BtnPrevious))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnPrevious, visible);
    }
    if(buttons.testFlag(BtnHide))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnHide, visible);
    }
    if(buttons.testFlag(BtnRecord))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnRecord, visible);
    }
    if(buttons.testFlag(BtnToggleOutput))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonVisible(AbulEduControlAudioV1::BtnToggleOutput, visible);
    }
}

void AbulEduMultiMediaV1::abeMultiMediaSetButtonEnabled(flagControlButtons buttons, bool enabled)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(buttons.testFlag(BtnPlay))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPlay, enabled);
    }
    if(buttons.testFlag(BtnPause))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPause, enabled);
    }
    if(buttons.testFlag(BtnStop))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnStop, enabled);
    }
    if(buttons.testFlag(BtnMagnifyingGlass))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnMagnifyingGlass, enabled);
    }
    if(buttons.testFlag(BtnNext))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnNext, enabled);
    }
    if(buttons.testFlag(BtnPrevious))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPrevious, enabled);
    }
    if(buttons.testFlag(BtnHide))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnHide, enabled);
    }
    if(buttons.testFlag(BtnRecord))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnRecord, enabled);
    }
    if(buttons.testFlag(BtnToggleOutput))
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnToggleOutput, enabled);
    }
}

void AbulEduMultiMediaV1::abeMultiMediaSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_viewerWidget->abeMediaViewerSetTextPosition(align);
}

void AbulEduMultiMediaV1::abeMultiMediaSetTextVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_viewerWidget->abeMediaViewerSetTextVisible(visible);
}

void AbulEduMultiMediaV1::abeMultiMediaSetLevelMeterVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->wgAbulEduControlAudio->abeControlAudioGetAudioLevelMeter()->setVisible(visible);
}

void AbulEduMultiMediaV1::abeMultimediaSetTTS(AbulEduPicottsV1::AbulEduLang language)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Pour changer de langue, il faut re créer un lecteur */
//    if(m_picoTTS)
//    {
        /** @todo vérifier que les index correspondent */
//        if(m_picoTTS->AbePicottsGetlangIndex() == (int)language){
//            return;
//        }
//        delete m_picoTTS;
//        m_picoTTS = 0;
//    }
    initPicoTTS(language);
}

void AbulEduMultiMediaV1::initStateMachine()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_stateMachine      = new QStateMachine();
    m_initialState      = new QState();
    m_initialState      ->setObjectName("initialState");
    m_stoppedState      = new QState(m_initialState);
    m_stoppedState      ->setObjectName("stoppedState");
    m_playingState      = new QState(m_initialState);
    m_playingState      ->setObjectName("playingState");
    m_pausedState       = new QState(m_initialState);
    m_pausedState       ->setObjectName("pausedState");
    m_waitingState      = new QState(m_initialState);
    m_waitingState      ->setObjectName("waitingState");
    m_recordingState    = new QState(m_initialState);
    m_recordingState    ->setObjectName("recordingState");
    m_finalState        = new QFinalState();
    m_finalState        ->setObjectName("finalState");
    m_stateMachine->addState(m_initialState);
    m_stateMachine->setInitialState(m_initialState);
    m_initialState->setInitialState((m_stoppedState));

    /* Les Transitions */
    m_stoppedState->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioPlayClicked()),   m_playingState);
    m_stoppedState->addTransition(this,               SIGNAL(signalAbePlayRequested()),             m_playingState);
    m_stoppedState->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioRecordClicked()), m_recordingState);
    m_playingState->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioPauseClicked()),  m_pausedState);
    m_playingState->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioStopClicked()),   m_stoppedState);
    m_pausedState ->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioPlayClicked()),   m_playingState);
    m_pausedState ->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioStopClicked()),   m_stoppedState);
    m_pausedState ->addTransition(this,               SIGNAL(signalAbePlayRequested()),             m_playingState);
    m_playingState->addTransition(this,               SIGNAL(signalAbeWaitingRequested()),          m_waitingState);
    m_playingState->addTransition(this,               SIGNAL(signalAbePauseRequested()),            m_pausedState);
    m_playingState->addTransition(this,               SIGNAL(signalAbeStopRequested()),             m_stoppedState);
    m_playingState->addTransition(this,               SIGNAL(signalAbeMultiMediaFinished()),       m_stoppedState);
    m_waitingState->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioPlayClicked()),   m_playingState);
    m_recordingState->addTransition(ui->wgAbulEduControlAudio, SIGNAL(signalAbeControlAudioStopClicked()), m_stoppedState);

    /* Les connexions */
    connect(m_initialState,     SIGNAL(entered()),  this, SLOT(slotAbeInitialStateEntered()));
    connect(m_playingState,     SIGNAL(entered()),  this, SLOT(slotAbePlayingStateEntered()));
    connect(m_pausedState,      SIGNAL(entered()),  this, SLOT(slotAbePausedStateEntered()));
    connect(m_stoppedState,     SIGNAL(entered()),  this, SLOT(slotAbeStoppedStateEntered()));
    connect(m_waitingState,     SIGNAL(entered()),  this, SLOT(slotAbeWaitingStateEntered()));
    connect(m_recordingState,   SIGNAL(entered()),  this, SLOT(slotAbeRecordingStateEntered()));
    connect(m_recordingState,   SIGNAL(exited()),   this, SLOT(slotAbeRecordingStateExited()));
}

void AbulEduMultiMediaV1::playNextMedia()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_currentMedia++; // Media suivant
    ABULEDU_LOG_DEBUG() << m_currentMedia<<"Images No"<<m_currentMedia;
    if(m_currentMedia >= m_listMedias.size()){
        return;
    }
    /* On passe les paramètres du media à lire */

    abeMultiMediaSetMedia(m_currentMedia);
    /* Tests pour savoir si on doit utiliser le lecteur picotts */
    if(m_listMedias[m_currentMedia].AbulEduMediaMediasSound.isEmpty()
            && m_TTSLanguage != AbulEduPicottsV1::None
            && !m_listMedias[m_currentMedia].AbulEduMediaMediasText.isEmpty())
    {
        if(!m_picoTTS)
        {
            ABULEDU_LOG_DEBUG() << "J'initialise mon pico";
            initPicoTTS(m_TTSLanguage);
        }
        ABULEDU_LOG_DEBUG() << " Synthèse vocale pour  "<<m_listMedias[m_currentMedia].AbulEduMediaMediasText
                            << m_listMedias.count() << m_currentMedia;
        m_picoTTS->abePicoStop();
        if(m_currentMedia < m_listMedias.count()){
            ABULEDU_LOG_DEBUG() << "Je play mon PicoMedia :" << m_listMedias[m_currentMedia].AbulEduMediaMediasText;
            m_picoTTS->abePicottsSetLangIndex(m_TTSLanguage);
            m_picoTTS->abePicoPlay(m_listMedias[m_currentMedia].AbulEduMediaMediasText);
            ABULEDU_LOG_DEBUG() << m_listMedias[m_currentMedia].AbulEduMediaMediasText;
        }
    }
    /** warning eric: j'aimerais avoir des explications sur ce bout de code */
    /* On passe habituellement au média suivant en fin de lecture du son associé
     * Mais s'il n'y a pas de son associé(condition 1), si on n'est pas en synthèse vocale (condition 2) et si on a bien une valeur m_defaultDelay
     * Alors on passe au média suivant au bout de "m_defaultDelay" secondes */
    if(m_listMedias[m_currentMedia].AbulEduMediaMediasSound.isEmpty()
            && m_TTSLanguage == AbulEduPicottsV1::None
            && m_defaultDelay > 0)
    {
        if(m_currentMedia != m_listMedias.size()-1) {
            QTimer::singleShot(m_defaultDelay*1000,this,SLOT(playNextMedia()));
        }
    }
    else {
        m_viewerWidget->slotAbeMediaViewerPlay();
        m_playerState = Playing;
    }
    emit signalAbeMultiMediaCurrentMediaChanged(m_currentMedia + 1);

    /* Gestion des boutons Next et Previous */
    if(m_listMedias.size() > 1)
    {
        if(m_currentMedia == 0) // Premier média
        {
            ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPrevious,false);
            ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnNext,true);
        }
        else if(m_currentMedia == m_listMedias.size()-1) // dernier media
        {
            ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnNext,false);
            ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPrevious,true);}
        else // entre les deux
        {
            ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPrevious,true);
            ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnNext,true);}
    }
    else // Il n'y a qu'un seul média donc pas de next ni de previous
    {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPrevious,false);
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnNext,false);}
}

void AbulEduMultiMediaV1::slotAbeInitialStateEntered()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeMultiMediaSetMedia(0);
}

void AbulEduMultiMediaV1::slotAbePlayingStateEntered()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<"m_playerState : "<<m_playerState;
    if(m_playerState == Playing && m_playMode != AbulEduMultiMediaSettingsV1::AutoStart)
    {
        m_viewerWidget->slotAbeMediaViewerStop();
        slotAbePlayingStateEntered();
    }

    switch(m_playerState)
    {
    case Stopped:
    {
        /* Après un stop, on redémarre au début */
        m_currentMedia = -1;
        // Lancer la lecture
        playNextMedia();
        break;
    }
    case Paused:
        /* Après une pause, on reprend où on était */
        emit signalAbeResumeRequested();
        m_viewerWidget->slotAbeMediaViewerResume();
        m_playerState = Playing;
        break;
    case Waiting:
        playNextMedia();
        break;
    case Playing:
        if(m_playMode == AbulEduMultiMediaSettingsV1::AutoStart)
        {
            playNextMedia();
        }
        break;
    default:
        ABULEDU_LOG_DEBUG() << "**********************************Bizarre, PlayerState : "<<m_playerState;
        break;
    }
    emit signalAbeMultiMediaStarted(m_currentMedia);
    emit signalAbeMultiMediaStateChanged(m_playerState);
    /* La télécommande passe en mode playing : bouton Stop et Pause actif bouton lecture désactivé */
    ui->wgAbulEduControlAudio->abeControlAudioSetState(AbulEduControlAudioV1::Playing);

    ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnPrevious,m_currentMedia > 0);
    ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnNext,m_currentMedia < m_listMedias.size()-1);
}

void AbulEduMultiMediaV1::slotAbePausedStateEntered()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<sender();
    emit signalAbePauseRequested();
    if(m_playerState==Paused)
    {
        // Normalement on ne passe pas là car dans cet état, le bouton pause est désactivé */
        ABULEDU_LOG_DEBUG() << "********************************************* Oups";
        m_viewerWidget->slotAbeMediaViewerResume();
        m_playerState = Playing;
        emit signalAbeMultiMediaStateChanged(m_playerState);
        emit signalAbePlayRequested(); // Fait passer dans l'état PlayingState
    }
    else
    {
        m_viewerWidget->slotAbeMediaViewerPause();
        m_playerState = Paused;
        emit signalAbeMultiMediaStateChanged(m_playerState);
        emit signalAbeMultiMediaCurrentMediaPaused(m_currentMedia);
        ui->wgAbulEduControlAudio->abeControlAudioSetState(AbulEduControlAudioV1::Paused);
    }
}

void AbulEduMultiMediaV1::slotAbeStoppedStateEntered()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeStopRequested();
    // Stopper la lecture
    m_viewerWidget->slotAbeMediaViewerStop();
    /* La télécommande passe en mode stopped : bouton Stop et Pause inactif bouton lecture activé */
    ui->wgAbulEduControlAudio->abeControlAudioSetState(AbulEduControlAudioV1::Stopped);
    m_playerState = Stopped;

    emit signalAbeMultiMediaStateChanged(m_playerState);

    /** @todo vérifier si cette émission est utile : rien n'y est connecté */
    emit signalAbeMultiMediaStopped(m_currentMedia);

    if(m_playMode == AbulEduMultiMediaSettingsV1::AutoStart)
    {
        m_playerState = Playing;
        /* On passe en mode playing */
        emit signalAbeMultiMediaStateChanged(m_playerState);
        /* Pour modifier l'affichage des boutons */
        emit signalAbePlayRequested();// Fait passer dans l'état PlayingState
        /* On lance la lecture une fois seulement lors du lancement du fichier son */
        m_viewerWidget->slotAbeMediaViewerPlay();
        m_currentMedia++; // Evite de jouer deux fois le premier média
        /* On revient au mode automatique */
        m_playMode = AbulEduMultiMediaSettingsV1::Automatic;
    }
#if defined (Q_OS_WIN)
    /* Philippe 20141125
         * Je suis obligé de mettre d'abord mon média à la valeur dernier. Sous Windows, en effet, le stop entraine le passage en QMediaPlayer::EndOfMedia
         * du coup le signal signalAbeMediaFinished() est émis. Et il va lancer le slotAbeViewerPlayingMediaFinished qui fait un playNextMedia */
        m_currentMedia = m_listMedias.size()-1;
        abeMultiMediaSetMedia(m_currentMedia);
        /* Par contre après comme sous Linux je mets mon média à la valeur premier pour qu'il redémarre de là */
#endif
        m_currentMedia = 0;
        abeMultiMediaSetMedia(m_currentMedia);
    if(m_TTSLanguage == AbulEduPicottsV1::None) {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnRecord,true);
    }
    else {
        ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnRecord,false);
    }
}

void AbulEduMultiMediaV1::slotAbeWaitingStateEntered()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->wgAbulEduControlAudio->abeControlAudioSetState(AbulEduControlAudioV1::Paused);
    m_playerState = Waiting;
    emit signalAbeMultiMediaStateChanged(m_playerState);
}

void AbulEduMultiMediaV1::slotAbeRecordingStateEntered()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeMultiMediaStop();

    m_playerState = Recording;
    ui->wgAbulEduControlAudio->abeControlAudioSetState(AbulEduControlAudioV1::Recording);
    ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnRecord,false);
    ui->wgAbulEduControlAudio->abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::BtnStop,true);
    emit signalAbeMultiMediaStateChanged(m_playerState);
    m_viewerWidget->slotAbeMediaViewerStartRecording();
}

void AbulEduMultiMediaV1::slotAbeRecordingStateExited()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_viewerWidget->slotAbeMediaViewerStopRecording();
}

void AbulEduMultiMediaV1::slotAbeViewerPlayingMediaFinished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_currentMedia < m_listMedias.size()-1)
    {
        /* Il reste encore des trucs à lire */
        if(m_playMode == AbulEduMultiMediaSettingsV1::Manual)
        {
            /* On va attendre une action sur les boutons */
            emit signalAbeWaitingRequested();
        }
        else
        {
            /* On joue la suite immédiatement */
            if(m_TTSLanguage == AbulEduPicottsV1::None){
                playNextMedia();
            }
        }
    }
    else
    {
        /* Il n'y a plus rien à lire, on passe en mode Stopped */
        ui->wgAbulEduControlAudio->abeControlAudioSetState(AbulEduControlAudioV1::Stopped);
        m_currentMedia = -1;
        m_playerState = Stopped;
        emit signalAbeMultiMediaFinished(); // Permet de rentrer dans l'état stopped
    }
}

void AbulEduMultiMediaV1::slotAbeViewerPlayingMediaStoppedCurrentTime(qint64 currentTime)
{
    Q_UNUSED(currentTime)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduMultiMediaV1::slotAbeViewerPlayingMediaPausedCurrentTime(qint64 currentTime)
{
    Q_UNUSED(currentTime)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduMultiMediaV1::slotAbeViewerLevel(qreal level)
{
    // Trop d'appels je désactive le debug
    // ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->wgAbulEduControlAudio->abeControlAudioGetAudioLevelMeter()->slotAbulEduAudioLevelWidgetSetLevel(level);
}

void AbulEduMultiMediaV1::slotAbeNextMediaRequested()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    m_viewerWidget->slotAbeMediaViewerStop();
    m_viewerWidget->slotAbeMediaViewerPlay();
#else
    playNextMedia();
#endif
}

void AbulEduMultiMediaV1::slotAbePreviousMediaRequested()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_currentMedia -= 2; // On recule de 2 car playNextMedia avance de 1
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    m_viewerWidget->slotAbeMediaViewerStop();
    m_viewerWidget->slotAbeMediaViewerPlay();
#else
    playNextMedia();
#endif
}

void AbulEduMultiMediaV1::slotAbeMediaRequested(int mediaToPlay)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(mediaToPlay > -1 && mediaToPlay < m_listMedias.size())
    {
        m_currentMedia = mediaToPlay-1; // On recule de 1 car playnextMedia avance de 1
        playNextMedia();
    }
}

void AbulEduMultiMediaV1::slotAbeStopAndHideMultiMedia()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeStopRequested();
    hide();
    emit signalAbeMultiMediaStoppedAndHidden();
}

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
void AbulEduMultiMediaV1::slotAbePlayerToggleOutput(bool checked)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<checked;
    if(checked)
    {
        /* Sortie Casque USB */
        m_abuleduMultiMediaSettings->abeMultiMediaSettingsSetAudioOutputDeviceInfo(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetHeadPhone());
        m_outputDevice = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetHeadPhone();
    }
    else
    {
        /* Sortie HP par defaut */
        m_abuleduMultiMediaSettings->abeMultiMediaSettingsSetAudioOutputDeviceInfo(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetLoudSpeaker());
        m_outputDevice = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetLoudSpeaker();

        ABULEDU_LOG_DEBUG() << "\n     "<<m_outputDevice.deviceName();
    }
    m_viewerWidget->abeMediaViewerSetOutputDevice(m_outputDevice);
    ABULEDU_LOG_DEBUG() << " -----------------------> output device : "<<m_outputDevice.deviceName();
}
#endif

void AbulEduMultiMediaV1::slotAbeRecordingRequested()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<m_viewerWidget;
    m_viewerWidget->slotAbeMediaViewerStartRecording();
}

void AbulEduMultiMediaV1::slotAbeZoomRequested()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMultimediaZoomRequested();
}

void AbulEduMultiMediaV1::abeMultiMediaSetMedia(int numMedia)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<numMedia<<"::"<<m_listMedias.size();
    if(!m_listMedias.isEmpty() && (numMedia < m_listMedias.size()))
    {
        m_viewerWidget->abeMediaViewerSetMediaDatas(m_listMedias[numMedia].AbulEduMediaMediasImage,
                                                    m_listMedias[numMedia].AbulEduMediaMediasSound,
                                                    m_listMedias[numMedia].AbulEduMediaMediasText);
    }
    ui->wgAbulEduControlAudio->abeControlAudioSetState(ui->wgAbulEduControlAudio->abeControlAudioGetState());
}

void AbulEduMultiMediaV1::initPicoTTS(AbulEduPicottsV1::AbulEduLang lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< lang << objectName();

    m_TTSLanguage = lang;
    /* on met à jour la valeur de la langue dans l'objet de l'application qui garde les paramètres */
    abeApp->getAbeApplicationMultiMediaSettings()->abeMultiMediaSettingsSetTTSLanguage(m_TTSLanguage);
    /* si on a déjà un objet pico tts on le supprime */
    if(m_picoTTS)
    {
        m_picoTTS->deleteLater();
        m_picoTTS = 0;
    }

    if(m_TTSLanguage != AbulEduPicottsV1::None)
    {
        QMediaPlayer *mpFromAudioPlayer;
        if(m_viewerWidget->inherits("AbulEduSoundPlayerV1")){
            AbulEduSoundPlayerV1 *cast = dynamic_cast<AbulEduSoundPlayerV1 *>(m_viewerWidget);
            ABULEDU_LOG_DEBUG() << "  CAST AbulEduSoundPlayerV1" << cast;
            if(cast)
                mpFromAudioPlayer = cast->abeMultiMediaGetMediaPlayer();
        }
        else if(m_viewerWidget->inherits("AbulEduSlidesViewerV1")){
            AbulEduSlidesViewerV1 *cast = dynamic_cast<AbulEduSlidesViewerV1 *>(m_viewerWidget);
            ABULEDU_LOG_DEBUG() << "  CAST AbulEduSlidesViewerV1" << cast;
            if(cast)
                mpFromAudioPlayer = cast->abeMultiMediaGetMediaPlayer();
        }

        ABULEDU_LOG_DEBUG() << "  INIT PICO AVEC =>>>>" << m_TTSLanguage << (int)m_TTSLanguage ;
        m_picoTTS = new AbulEduPicottsV1(m_TTSLanguage , this, mpFromAudioPlayer);
        m_picoTTS->abePicottsSetSpeedLevel(m_picoSpeed);
        abeMultiMediaGetAudioControlWidget()->abeControlAudioGetFrameSpeed()->setVisible(abeMultiMediaGetAudioControlWidget()->abeControlAudioGetSpeedControlVisible());
        connect(m_picoTTS, SIGNAL(endOfPlay()),this, SLOT(slotAbeViewerPlayingMediaFinished()),Qt::UniqueConnection);
        connect(this, SIGNAL(signalAbeStopRequested()),  m_picoTTS, SLOT(abePicoStop()),  Qt::UniqueConnection);
        connect(this, SIGNAL(signalAbePauseRequested()), m_picoTTS, SLOT(abePicoPause()), Qt::UniqueConnection);
        connect(this, SIGNAL(signalAbeResumeRequested()), m_picoTTS, SLOT(abePicoResume()), Qt::UniqueConnection);
    }
    else{
        abeMultiMediaGetAudioControlWidget()->abeControlAudioGetFrameSpeed()->setVisible(false);
    }
}
int AbulEduMultiMediaV1::abeMultiMediaGetPicoSpeed() const
{
    return m_picoSpeed;
}

void AbulEduMultiMediaV1::abeMultiMediaSetPicoSpeed(int picoSpeed)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<picoSpeed;
    m_picoSpeed = picoSpeed;
    if(m_picoTTS){
        m_picoTTS->abePicottsSetSpeedLevel(picoSpeed);
    }
    abeMultiMediaGetAudioControlWidget()->abeControlAudioSetSpeedValue(picoSpeed);
}


void AbulEduMultiMediaV1::paintEvent(QPaintEvent *)
{
//    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#if defined (Q_OS_WIN)
void AbulEduMultiMediaV1::abeMultiMediaMapDurations()
{
    connect(m_viewerWidget->abeMultiMediaGetMediaPlayer(),SIGNAL(durationChanged(qint64)),
            this, SLOT(slotAddDuration(qint64)),Qt::UniqueConnection);
    m_soundDurations.clear();
    ABULEDU_LOG_DEBUG()<<"Je lance le calcul des durées en passant au QMediaPlayer le premier de la liste : "<<m_listMedias.first().abeMediaMediaGetSound();
    m_viewerWidget->abeMultiMediaGetMediaPlayer()->setMedia(QUrl::fromLocalFile(m_listMedias.first().abeMediaMediaGetSound()));
}

void AbulEduMultiMediaV1::slotAddDuration(qint64 duration)
{
    if(duration > 0){
        QFileInfo fileinfo(m_viewerWidget->abeMultiMediaGetMediaPlayer()->media().canonicalUrl().toString());

        m_soundDurations.insert(fileinfo.fileName(),duration);
    }
    if(m_soundDurations.size() < m_listMedias.size()){
        AbulEduMediaMedias med = m_listMedias.at(m_soundDurations.size());
        QString s = QString(med.abeMediaMediaGetSound());
        m_viewerWidget->abeMultiMediaGetMediaPlayer()->setMedia(QUrl::fromLocalFile(s));
    }
    else{
        ABULEDU_LOG_DEBUG()<<"Et voilà c'est tout. J'ai relevé la durée de "<<m_soundDurations.size()<<" fichiers ogg.";
        disconnect(m_viewerWidget->abeMultiMediaGetMediaPlayer(),SIGNAL(durationChanged(qint64)),
                this, SLOT(slotAddDuration(qint64)));
        emit signalAbeMultiMediaDurationsMapped();
    }
}

/*
void AbulEduMediaMedias::abeMediaMediaSetSoundDuration(qint64 duration)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<AbulEduMediaMediasSound<<" dure "<<duration;
    m_soundDuration = duration;
    qDebug()<<abeMediaMediaGetImage()<<abeMediaMediaGetSound()<<abeMediaMediaGetSoundDuration()<<abeMediaMediaGetText();
}*/
#endif
