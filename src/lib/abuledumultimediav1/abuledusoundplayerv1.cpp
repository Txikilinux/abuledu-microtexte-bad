/** Classe AbulEduSoundPlayerV1
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

#include "abuledusoundplayerv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduSoundPlayerV1::AbulEduSoundPlayerV1(const QString &mediaPath, const QString &soundPath, const QString &text) :
    AbulEduMediaViewerInterface(),
    m_layout(0),
    m_textString(text),
    m_textViewer(0),
    m_soundPath(soundPath),
    m_duration(0)
{
    Q_UNUSED(mediaPath)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_layout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_textViewer = new QTextEdit();
    m_textViewer->setReadOnly(true);
    m_layout->addWidget(m_textViewer,1);

    initAudioPlayer();
}

void AbulEduSoundPlayerV1::initAudioPlayer()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abuleduMultiMediaSubTitleAlignment = QPair<QString,QString> ("<div align=\"right\">","</div>");

    //On recupere les parametres "partagés"
    m_abuleduMultiMediaSettings = abeApp->getAbeApplicationMultiMediaSettings();
    m_audioPlayer = new AbulEduAudioPlayerV1();
    m_audioPlayer->setVolume(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioOutputLevel());
    m_audioPlayer->setMedia(m_soundPath);

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    connect(m_audioPlayer->abeAudioPlayerGetMediaPlayer(), SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus)),  Qt::UniqueConnection);
#else
    connect(m_audioPlayer, SIGNAL(stateChanged(QAudio::State)), this, SLOT  (slotAbeStateChanged(QAudio::State)),  Qt::UniqueConnection);
#endif
    connect(m_audioPlayer, SIGNAL(durationChanged(qint64)),     this, SIGNAL(signalAbeMediaDuration(qint64)),    Qt::UniqueConnection);
    connect(m_audioPlayer, SIGNAL(positionChanged(qint64)),     this, SIGNAL(signalAbeMediaPositionChanged(qint64)), Qt::UniqueConnection);
    connect(m_audioPlayer, SIGNAL(signalAbeAudioPlayerMediaFinished()),    this, SIGNAL(signalAbeMediaFinished()),          Qt::UniqueConnection);
}

AbulEduSoundPlayerV1::~AbulEduSoundPlayerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_layout)
    {
        delete m_layout;
    }
    if(m_textViewer)
    {
        delete m_textViewer;
    }
}

void AbulEduSoundPlayerV1::abeMediaViewerSetOutputDevice(QAudioDeviceInfo deviceInfo)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->abeAudioPlayerSetOutputDevice(deviceInfo);
}

Qt::Alignment AbulEduSoundPlayerV1::abeMediaViewerGetSubTitleAlignment() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_abuleduMultiMediaSubTitleAlignment.first.contains("center")){
        return Qt::AlignHCenter;
    }
    else if(m_abuleduMultiMediaSubTitleAlignment.first.contains("right")){
        return Qt::AlignRight;
    }
    return Qt::AlignLeft;
}

void AbulEduSoundPlayerV1::abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &subTitleAlignment)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(subTitleAlignment == Qt::AlignHCenter){
        m_abuleduMultiMediaSubTitleAlignment = QPair<QString,QString> ("<div align=\"center\">","</div>");
    }
    else if(subTitleAlignment == Qt::AlignRight){
        m_abuleduMultiMediaSubTitleAlignment = QPair<QString,QString> ("<div align=\"right\">","</div>");
    }
    else {
        m_abuleduMultiMediaSubTitleAlignment = QPair<QString,QString> ("<div align=\"left\">","</div>");
    }
}

int AbulEduSoundPlayerV1::abeMediaViewerGetSubTitlePointSize() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_textViewer->font().pointSize();
}

void AbulEduSoundPlayerV1::abeMediaViewerSetSubTitlePointSize(int pointSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFont f = m_textViewer->font();
    f.setPointSize(pointSize);
    m_textViewer->setFont(f);
}

#ifdef Q_OS_WIN
QMediaPlayer *AbulEduSoundPlayerV1::abeMultiMediaGetMediaPlayer()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "JE RETOURNE LE MEDIAPLAYER DE SOUNDPLAYER";
    return m_audioPlayer->abeAudioPlayerGetMediaPlayer();
}

void AbulEduSoundPlayerV1::abeMultiMediaSetMediaPlayer(QMediaPlayer *mediaPlayer)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->abeAudioPlayerSetMediaPlayer(mediaPlayer);
}
#endif

void AbulEduSoundPlayerV1::abeMediaViewerSetMediaPath(const QString &mediaPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(mediaPath)
    /* Pas d'image ou de video dans ce module */
}

void AbulEduSoundPlayerV1::abeMediaViewerSetSubTitle(const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_textString = text;
    m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+m_textString+m_abuleduMultiMediaSubTitleAlignment.second);
}

void AbulEduSoundPlayerV1::abeMediaViewerSetSoundPath(const QString &soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_soundPath = soundPath;
//    m_audioPlayer->setMedia(QUrl::fromLocalFile(m_soundPath));
    updateMediaDatas(m_soundPath);
}

void AbulEduSoundPlayerV1::abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(mediaPath)
    slotAbeMediaViewerStop();
    m_soundPath = soundPath;
//    m_audioPlayer->setMedia(QUrl::fromLocalFile(m_soundPath));
    m_textString = text;
    updateMediaDatas(m_soundPath, m_textString);
}

void AbulEduSoundPlayerV1::abeMediaViewerSetTextVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_textViewerIsVisible = visible;
    updateMediaDatas(m_soundPath, m_textString);
}

void AbulEduSoundPlayerV1::abeMediaViewerSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(align)
    /* Pas de sens ici */
}

void AbulEduSoundPlayerV1::updateMediaDatas(const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(text.isEmpty())
    {
        m_textViewer->setVisible(false);
        m_textViewer->setAlignment(abeMediaViewerGetSubTitleAlignment());
    }
    else
    {
        m_textViewer->setVisible(m_textViewerIsVisible);
        m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+text+m_abuleduMultiMediaSubTitleAlignment.second);
    }
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
}

void AbulEduSoundPlayerV1::slotAbeMediaViewerPlay()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<"fichier à jouer :"<<m_soundPath;
    m_audioPlayer->play();
}

void AbulEduSoundPlayerV1::slotAbeMediaViewerPause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->pause();
    /* TODO passer le currentTime à la place de -1 */
//    emit signalAbeMediaPaused(m_audioPlayer->position());
}

void AbulEduSoundPlayerV1::slotAbeMediaViewerStop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->stop();
    /* TODO passer le currentTime à la place de -1 */
//    emit signalAbeMediaStopped(m_audioPlayer->position());
}

void AbulEduSoundPlayerV1::slotAbeMediaViewerResume()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->play();
}

void AbulEduSoundPlayerV1::slotAbePlayingFinished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
void AbulEduSoundPlayerV1::slotAbeStateChanged(QAudio::State newState)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch(newState)
    {
    case QAudio::ActiveState:
        ABULEDU_LOG_DEBUG() << "              Entrée dans l'état ActiveState";
        break;
    case QAudio::SuspendedState:
        ABULEDU_LOG_DEBUG() << "              Entrée dans l'état SuspendedState";
        break;
    case QAudio::StoppedState:
        ABULEDU_LOG_DEBUG() << "              Entrée dans l'état StoppedState";
        break;
    case QAudio::IdleState:
        ABULEDU_LOG_DEBUG() << "              Entrée dans l'état IdleState";
        break;
    default:
        break;
    }
}
#endif

void AbulEduSoundPlayerV1::slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus newStatus)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch(newStatus)
    {
    case QMediaPlayer::EndOfMedia:
        emit signalAbeMediaFinished();
        break;

    default:
        break;
    }
    emit signalAbeMediaViewerMediaStatusChanged(newStatus);
}
void AbulEduSoundPlayerV1::slotAbeDurationChanged(qint64 duration)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_duration = duration;
    emit signalAbeMediaDuration(m_duration);
}
