#include "abuleduslidesviewerv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduSlidesViewerV1::AbulEduSlidesViewerV1(const QString &pixmapPath, const QString &soundPath, const QString &text):
    AbulEduMediaViewerInterface(),
    m_pixPath(pixmapPath),
    m_textString(text),
    m_soundPath(soundPath),
    m_audioPlayer(0),
    m_duration(0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    initGui();
    initAudioPlayer();
}

AbulEduSlidesViewerV1::AbulEduSlidesViewerV1(QWidget *parent):
    AbulEduMediaViewerInterface(),
    m_pixPath(),
    m_textString(),
    m_soundPath(),
    m_audioPlayer(0),
    m_duration(0)
{
    Q_UNUSED(parent)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    initGui();
    initAudioPlayer();
}

void AbulEduSlidesViewerV1::initGui()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_layout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    m_layout->setAlignment(Qt::AlignCenter);
    m_layout->setContentsMargins(0,0,0,0);
    m_pixLabel = new QLabel(m_pixPath);
    m_pixLabel->setAlignment(Qt::AlignCenter);
    m_pixLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_textViewer = new QTextEdit();
    m_textViewer->setReadOnly(true);
    m_textViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_layout->addWidget(m_textViewer,0);
    m_layout->addWidget(m_pixLabel,100);
}

void AbulEduSlidesViewerV1::initAudioPlayer()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<m_soundPath;
    setObjectName("AbulEduSlidesViewerV1");
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

AbulEduSlidesViewerV1::~AbulEduSlidesViewerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_pixLabel)
    {
        m_layout->removeWidget(m_pixLabel);
        m_pixLabel->deleteLater();
    }
    if(m_textViewer)
    {
        m_layout->removeWidget(m_textViewer);
        m_textViewer->deleteLater();
    }
    if(m_audioPlayer)
    {
        m_audioPlayer->stop();
        m_audioPlayer->deleteLater();
    }
}

void AbulEduSlidesViewerV1::abeMediaViewerSetMediaPath(const QString &mediaPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_pixPath = mediaPath;
    setLabelSize();
}

void AbulEduSlidesViewerV1::abeMediaViewerSetSubTitle(const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_textString = text;
    m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+m_textString+m_abuleduMultiMediaSubTitleAlignment.second);
}

void AbulEduSlidesViewerV1::abeMediaViewerSetSoundPath(const QString &soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
}

void AbulEduSlidesViewerV1::abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediaViewerStop();
    m_pixPath = mediaPath;
    m_soundPath = soundPath;
    m_audioPlayer->setMedia(m_soundPath);
    m_textString = text;
    updateMediaDatas(m_pixPath, m_soundPath, m_textString);
}

void AbulEduSlidesViewerV1::abeMediaViewerSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align)
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

void AbulEduSlidesViewerV1::abeMediaViewerSetOutputDevice(QAudioDeviceInfo deviceInfo)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->abeAudioPlayerSetOutputDevice(deviceInfo);
}

void AbulEduSlidesViewerV1::abeMediaViewerSetTextVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_textViewerIsVisible = visible;
    updateMediaDatas(m_pixPath, m_soundPath, m_textString);
}

Qt::Alignment AbulEduSlidesViewerV1::abeMediaViewerGetSubTitleAlignment() const
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

void AbulEduSlidesViewerV1::abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &subTitleAlignment)
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
    m_textViewer->setAlignment(subTitleAlignment);
}

int AbulEduSlidesViewerV1::abeMediaViewerGetSubTitlePointSize() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_textViewer->font().pointSize();
}

void AbulEduSlidesViewerV1::abeMediaViewerSetSubTitlePointSize(int pointSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFont f = m_textViewer->font();
    f.setPointSize(pointSize);
    m_textViewer->setFont(f);
}

void AbulEduSlidesViewerV1::updateMediaDatas(const QString &pixmapPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(text == "" && m_textViewer->isReadOnly())
    {
        m_textViewer->setVisible(false);
    }
    else
    {
        m_textViewer->setText(m_abuleduMultiMediaSubTitleAlignment.first+text+m_abuleduMultiMediaSubTitleAlignment.second);
        if(text.isEmpty()){
            m_textViewer->setAlignment(abeMediaViewerGetSubTitleAlignment());
        }

        m_textViewer->setVisible(m_textViewerIsVisible);

    }
    if(m_soundPath != soundPath)
    {
        m_soundPath = soundPath;
        m_audioPlayer->setMedia(m_soundPath);
    }
    m_pixPath = pixmapPath;
    setLabelSize();
}

void AbulEduSlidesViewerV1::slotAbeMediaViewerPlay()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ <<" Fichier de son : "<<m_soundPath;
    m_audioPlayer->play();
}

void AbulEduSlidesViewerV1::slotAbeMediaViewerPause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->pause();
}

void AbulEduSlidesViewerV1::slotAbeMediaViewerStop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->stop();
}

void AbulEduSlidesViewerV1::slotAbeMediaViewerResume()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_audioPlayer->resume();
}

void AbulEduSlidesViewerV1::slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus newStatus)
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
    /** @todo vérifier si ce signal est utile : aucun slot n'est connecté dessus */
    emit signalAbeMediaViewerMediaStatusChanged(newStatus);
}

#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
void AbulEduSlidesViewerV1::slotAbeStateChanged(QAudio::State newState)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch(newState)
    {
    case QAudio::ActiveState:
        ABULEDU_LOG_DEBUG() <<"              Entrée dans l'état ActiveState";
        break;
    case QAudio::SuspendedState:
        ABULEDU_LOG_DEBUG() <<"              Entrée dans l'état SuspendedState";
        break;
    case QAudio::StoppedState:
        ABULEDU_LOG_DEBUG() <<"              Entrée dans l'état StoppedState";
        break;
    case QAudio::IdleState:
        ABULEDU_LOG_DEBUG() <<"              Entrée dans l'état IdleState";
        break;
    default:
        break;
    }
}
#endif

void AbulEduSlidesViewerV1::slotAbeDurationChanged(qint64 duration)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_duration = duration;
    emit signalAbeMediaDuration(m_duration);
}

void AbulEduSlidesViewerV1::slotAbePlayingFinished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduSlidesViewerV1::resizeEvent(QResizeEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setLabelSize();
}

void AbulEduSlidesViewerV1::showEvent(QShowEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    // pour mes tests IS
//     m_pixLabel->setStyleSheet("background-color : red; border-color : yellow; border-width : 5px; border-style:inset;");
//    updateMediaDatas(m_pixPath, m_soundPath, m_textString);
//    m_pixLabel->setPixmap(QPixmap(m_pixPath).scaled(m_pixLabel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void AbulEduSlidesViewerV1::setLabelSize()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //Les conditions de retour ...
    if(!m_pixLabel) return;
    if(m_pixLabel->size().width() <= 0 || m_pixLabel->size().height() <= 0) return;

    if(!m_pixPath.isEmpty()){
        m_pixLabel->setPixmap(QPixmap(m_pixPath).scaled(m_pixLabel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {

        m_pixLabel->clear();
        ABULEDU_LOG_DEBUG() <<" Le chemin de l'image est vide ";
    }
}
