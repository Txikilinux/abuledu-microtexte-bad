/** Classe AbulEduVideoViewerV1
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

#include "abuleduvideoviewerv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduVideoViewerV1::AbulEduVideoViewerV1(const QString &mediaPath, const QString &soundPath, const QString &text):
    AbulEduMediaViewerInterface(),
    m_videoPath(mediaPath),
    m_textString(text),
    m_soundPath(soundPath),
    m_duration(0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Création du mediaplayer pour les video
     * Il faut un QMediaPlayer ainsi qu'un QVideoWidget pour le visualiser
     */
    m_videoPlayer = new QMediaPlayer;
    connect(m_videoPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),  this, SLOT(slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus)), Qt::UniqueConnection);
    connect(m_videoPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),              this, SLOT(slotAbeStateChanged(QMediaPlayer::State)), Qt::UniqueConnection);
    connect(m_videoPlayer, SIGNAL(durationChanged(qint64)),                        this, SIGNAL(signalAbeMediaDuration( qint64)));
    connect(m_videoPlayer, SIGNAL(positionChanged(qint64)),                         this, SIGNAL(signalAbeMediaPositionChanged(qint64)));
    m_videoWidget = new QVideoWidget;
    /* On affiche la video dans le VideoWidget */
    m_videoPlayer->setVideoOutput(m_videoWidget);

    m_textViewer = new QTextEdit();
    m_textViewer->setReadOnly(true);

    m_layout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addWidget(m_textViewer,1);
    m_layout->addWidget(m_videoWidget,3);
    initAudioFileV1();
}

void AbulEduVideoViewerV1::initAudioFileV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_abuleduMultiMediaSubTitleAlignment = QPair<QString,QString> ("<div align=\"left\">","</div>");

    //On recupere les parametres "partagés"
    m_abuleduMultiMediaSettings = abeApp->getAbeApplicationMultiMediaSettings();

    m_audioPlayer = new AbulEduAudioPlayerV1();
    m_audioPlayer->setVolume(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioOutputLevel());
}

AbulEduVideoViewerV1::~AbulEduVideoViewerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_layout)
    {
        delete m_layout;
    }
    if(m_videoPlayer)
    {
        delete m_videoPlayer;
    }
    if(m_textViewer)
    {
        delete m_textViewer;
    }
    if(m_videoWidget)
    {
        delete m_videoWidget;
    }
    if(m_audioPlayer)
    {
        delete m_audioPlayer;
    }
}

void AbulEduVideoViewerV1::abeMediaViewerSetMediaPath(const QString &mediaPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_videoPath = mediaPath;
    m_videoPlayer->setMedia(QUrl::fromLocalFile(m_videoPath));
}

void AbulEduVideoViewerV1::abeMediaViewerSetSubTitle(const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_textString = text;
    m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+m_textString+m_abuleduMultiMediaSubTitleAlignment.second);
}

void AbulEduVideoViewerV1::abeMediaViewerSetSoundPath(const QString &soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
}

void AbulEduVideoViewerV1::abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_videoPath = mediaPath;
    m_videoPlayer->setMedia(QUrl::fromLocalFile(m_videoPath));
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
    m_textString = text;
    updateMediaDatas(m_videoPath, m_soundPath, m_textString);
}

void AbulEduVideoViewerV1::abeMediaViewerSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch (align)
    {
    case Top:
        m_layout->setDirection(QBoxLayout::TopToBottom);
        break;
    case Right:
        m_layout->setDirection(QBoxLayout::RightToLeft);
        break;
    case Bottom:
        m_layout->setDirection(QBoxLayout::BottomToTop);
        break;
    case Left:
        m_layout->setDirection(QBoxLayout::LeftToRight);
        break;
    default:
        break;
    }
}

Qt::Alignment AbulEduVideoViewerV1::abeMediaViewerGetSubTitleAlignment() const
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

void AbulEduVideoViewerV1::abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &subTitleAlignment)
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

int AbulEduVideoViewerV1::abeMediaViewerGetSubTitlePointSize() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_textViewer->font().pointSize();
}

void AbulEduVideoViewerV1::abeMediaViewerSetSubTitlePointSize(int pointSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFont f = m_textViewer->font();
    f.setPointSize(pointSize);
    m_textViewer->setFont(f);
}

void AbulEduVideoViewerV1::updateMediaDatas(const QString &videoPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_videoPlayer->play();

    if(text.isEmpty())
    {
        m_textViewer->setVisible(false);
        m_textViewer->setAlignment(abeMediaViewerGetSubTitleAlignment());
    }
    else
    {
        m_textViewer->setVisible(true);
        m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+text+m_abuleduMultiMediaSubTitleAlignment.second);
    }
    if(m_soundPath != "" || m_soundPath != soundPath)
    {
        m_soundPath = soundPath;
        m_audioPlayer->setMedia(m_soundPath);
    }

    if(m_videoPath != videoPath)
    {
        m_videoPlayer->setMedia(QUrl::fromLocalFile(m_videoPath));
    }
}

void AbulEduVideoViewerV1::slotAbeMediaViewerPlay()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!m_soundPath.isEmpty())
    {
        m_audioPlayer->play();
    }
    m_videoPlayer->play();
}

void AbulEduVideoViewerV1::slotAbeMediaViewerPause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!m_soundPath.isEmpty())
    {
        m_audioPlayer->pause();
    }
    m_videoPlayer->pause();
    emit signalAbeMediaPaused(m_videoPlayer->position());
}

void AbulEduVideoViewerV1::slotAbeMediaViewerStop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!m_soundPath.isEmpty())
    {
        m_audioPlayer->stop();
    }
    m_videoPlayer->stop();
    emit signalAbeMediaStopped(m_videoPlayer->position());
}

void AbulEduVideoViewerV1::slotAbeMediaViewerResume()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!m_soundPath.isEmpty())
    {
            m_audioPlayer->resume();
    }
    m_videoPlayer->play();
}

void AbulEduVideoViewerV1::showEvent(QShowEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    updateMediaDatas(m_videoPath, m_soundPath, m_textString);
}

void AbulEduVideoViewerV1::slotAbePlayingFinished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
}

void AbulEduVideoViewerV1::slotAbeStateChanged(QAudio::State newState)
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

void AbulEduVideoViewerV1::slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus newStatus)
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
}

void AbulEduVideoViewerV1::slotAbeDurationChanged(qint64 duration)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_duration = duration;
    emit signalAbeMediaDuration(m_duration);
}
