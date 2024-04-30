/** Classe AbulEduAnimatedGifViewerV1
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
#include "abuleduanimatedgifviewerv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduAnimatedGifViewerV1::AbulEduAnimatedGifViewerV1(QString pixmapPath,QString soundPath,QString text):
    AbulEduMediaViewerInterface(),
    m_layout(0),
    m_gifPath(pixmapPath),
    m_movieLabel(0),
    m_movie(0),
    m_textString(text),
    m_textViewer(0),
    m_soundPath(soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_layout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    m_layout->setContentsMargins(0,0,0,0);

    m_movieLabel = new QLabel();
    m_movieLabel->setAlignment(Qt::AlignCenter);

    m_textViewer = new QTextEdit();
    m_textViewer->setReadOnly(true);

    m_layout->addWidget(m_textViewer,1);
    m_layout->addWidget(m_movieLabel,3);

    initAudioFileV1();

    m_movie = new QMovie();
}

void AbulEduAnimatedGifViewerV1::initAudioFileV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abuleduMultiMediaSubTitleAlignment = QPair<QString,QString> ("<div align=\"left\">","</div>");

    //On recupere les parametres "partagés"
    m_abuleduMultiMediaSettings = abeApp->getAbeApplicationMultiMediaSettings();
    m_audioPlayer = new AbulEduAudioPlayerV1();
    m_audioPlayer->setVolume(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioOutputLevel());

    m_audioPlayer->setMedia(m_soundPath);

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    connect(m_audioPlayer->abeAudioPlayerGetMediaPlayer(), SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slotAbeMediaStatusChanged(QMediaPlayer::State)),  Qt::UniqueConnection);
#else
    connect(m_audioPlayer, SIGNAL(stateChanged(QAudio::State)), this, SLOT(slotAbeStateChanged(QAudio::State)),  Qt::UniqueConnection);
#endif
    connect(m_audioPlayer, SIGNAL(durationChanged(qint64)),     this, SIGNAL(signalAbeMediaDuration(qint64)),    Qt::UniqueConnection);
    connect(m_audioPlayer, SIGNAL(positionChanged(qint64)),     this, SIGNAL(signalAbeMediaPositionChanged(qint64)), Qt::UniqueConnection);
    connect(m_audioPlayer, SIGNAL(signalAbeAudioPlayerMediaFinished()),    this, SIGNAL(signalAbeMediaFinished()),          Qt::UniqueConnection);
}


AbulEduAnimatedGifViewerV1::~AbulEduAnimatedGifViewerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_layout)
    {
        delete m_layout;
    }
    if(m_movieLabel)
    {
        delete m_movieLabel;
    }
    if(m_movie)
    {
        delete m_movie;
    }
    if(m_textViewer)
    {
        delete m_textViewer;
    }
}

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetMediaPath(const QString &mediaPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_gifPath = mediaPath;
}

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetSubTitle(const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_textString = text;
    m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+m_textString+m_abuleduMultiMediaSubTitleAlignment.second);
}

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetSoundPath(const QString &soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
}

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_gifPath = mediaPath;
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
    m_textString = text;
    updateMediaDatas(m_gifPath, m_soundPath, m_textString);
}

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align)
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

Qt::Alignment AbulEduAnimatedGifViewerV1::abeMediaViewerGetSubTitleAlignment() const
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

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &subTitleAlignment)
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

int AbulEduAnimatedGifViewerV1::abeMediaViewerGetSubTitlePointSize() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_textViewer->font().pointSize();
}

void AbulEduAnimatedGifViewerV1::abeMediaViewerSetSubTitlePointSize(int pointSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFont f = m_textViewer->font();
    f.setPointSize(pointSize);
    m_textViewer->setFont(f);
}

void AbulEduAnimatedGifViewerV1::updateMediaDatas(QString gifPath, QString soundPath, QString text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ABULEDU_LOG_DEBUG() << gifPath<< soundPath<< text<<"m_movieLabel.size"<<m_movieLabel->size();
    if(m_gifPath != gifPath)
    {
        m_movie->setFileName(m_gifPath);
        /* On dimensionne le media à la taille du label qui le contient */
        int height = qMin(m_movieLabel->height(), m_movieLabel->width());
        m_movie->setScaledSize(QSize(height, height));
        m_movieLabel->setMovie(m_movie);
        m_movie->jumpToNextFrame(); // Actualise l'affichage
    }
    if(text == "")
    {
        m_textViewer->setVisible(false);
        m_textViewer->setAlignment(abeMediaViewerGetSubTitleAlignment());
    }
    else
    {
        m_textViewer->setVisible(true);
        m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+text+m_abuleduMultiMediaSubTitleAlignment.second);
    }
    if(m_soundPath != soundPath)
    {
        m_soundPath = soundPath;
        m_audioPlayer->setMedia(m_soundPath);
    }
}

void AbulEduAnimatedGifViewerV1::slotAbeMediaViewerPlay()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_movie->start();
    m_audioPlayer->play();
}

void AbulEduAnimatedGifViewerV1::slotAbeMediaViewerPause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_movie->setPaused(true);
    m_audioPlayer->pause();
    /* TODO passer le currentTime à la place de -1 */
//    emit signalAbeMediaPaused(m_audioPlayer->position());
}

void AbulEduAnimatedGifViewerV1::slotAbeMediaViewerStop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_movie->stop();
    m_audioPlayer->stop();
    /* TODO passer le currentTime à la place de -1 */
//    emit signalAbeMediaStopped(m_audioPlayer->position());
}

void AbulEduAnimatedGifViewerV1::slotAbeMediaViewerResume()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_movie->setPaused(false);
    m_audioPlayer->play();
}

void AbulEduAnimatedGifViewerV1::resizeEvent(QResizeEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_movie->setFileName(m_gifPath);
    /* On dimensionne le media à la taille du label qui le contient */
    int height = qMin(m_movieLabel->height(), m_movieLabel->width());
    m_movie->setScaledSize(QSize(height, height));
    m_movieLabel->setMovie(m_movie);
    m_movie->jumpToNextFrame();
}

void AbulEduAnimatedGifViewerV1::showEvent(QShowEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduAnimatedGifViewerV1::slotAbePlayingFinished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduAnimatedGifViewerV1::slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus newStatus)
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

#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
void AbulEduAnimatedGifViewerV1::slotAbeStateChanged(QAudio::State newState)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch(newState)
    {
    case QAudio::ActiveState:
        ABULEDU_LOG_DEBUG() << "Entrée dans l'état ActiveState";
        break;
    case QAudio::SuspendedState:
        ABULEDU_LOG_DEBUG() << "Entrée dans l'état SuspendedState";
        break;
    case QAudio::StoppedState:
        ABULEDU_LOG_DEBUG() << "Entrée dans l'état StoppedState";
        break;
    case QAudio::IdleState:
        ABULEDU_LOG_DEBUG() << "Entrée dans l'état IdleState";
        break;
    default:
        break;
    }
}
#endif
