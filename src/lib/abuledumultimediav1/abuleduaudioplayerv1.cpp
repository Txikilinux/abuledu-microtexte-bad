/** Classe AbulEduAudioPlayerV1
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

#include "abuleduaudioplayerv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduAudioPlayerV1::AbulEduAudioPlayerV1(QObject *parent) :
    QObject(parent),
    m_fileToPlay(""),
    m_player (0),
    m_decoder(0),
    m_volume(1.0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    m_abeAudioPlayerMediaPlayer = 0;
#endif
    initialize();

    m_outputDeviceInfo = m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioOutputDeviceInfo();
}

AbulEduAudioPlayerV1::AbulEduAudioPlayerV1(QAudioDeviceInfo deviceInfo, QObject *parent) :
    QObject(parent),
    m_fileToPlay(""),
    m_player (0),
    m_decoder(0),
    m_volume(1.0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    initialize();
    m_outputDeviceInfo = deviceInfo;
}

void AbulEduAudioPlayerV1::initialize()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setObjectName("AbulEduAudioPlayerV1");

    m_format = QAudioFormat();
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setSampleRate(44100);
    m_format.setCodec("audio/pcm");
    m_format.setSampleType(QAudioFormat::SignedInt);

    /* On recupere les parametres "partagés" */
    m_abuleduMultiMediaSettings = abeApp->getAbeApplicationMultiMediaSettings();

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    m_abeAudioPlayerMediaPlayer = new QMediaPlayer(this);
    m_abeAudioPlayerMediaPlayer->setObjectName("QMediaPlayer::AbulEduAudioPlayerV1");
    connect(m_abeAudioPlayerMediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handleMediaStateChanged(QMediaPlayer::State)), Qt::UniqueConnection);
#endif
}

AbulEduAudioPlayerV1::~AbulEduAudioPlayerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_decoder)
    {
        delete m_decoder;
    }
    if(m_player)
    {
        delete m_player;
    }
}

void AbulEduAudioPlayerV1::setMedia(const QString &localFilenamePath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << localFilenamePath;
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
        if(QFile::exists(localFilenamePath)){
            ABULEDU_LOG_DEBUG() << "  OK: Le fichier existe bien";
            m_abeAudioPlayerMediaPlayer->setMedia(QUrl::fromLocalFile(localFilenamePath));
        }
        else {
            ABULEDU_LOG_DEBUG() << "  ERROR: le fichier " << localFilenamePath << " n'existe pas !";
        }
#endif

    if(m_fileToPlay != localFilenamePath)
    {
        m_fileToPlay = localFilenamePath;
        if(m_decoder)
        {
            m_decoder->stop();
            m_decoder->setSourceFilename(m_fileToPlay);
        }
    }
}

void AbulEduAudioPlayerV1::play()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    ABULEDU_LOG_DEBUG() << m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetTTSLanguage();
//    m_abuleduMultiMediaSettings->abeMultiMediaSettingsSetTTSLanguage(AbulEduMultiMediaSettingsV1::None);
//    if(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetTTSLanguage() == AbulEduMultiMediaSettingsV1::None){
//        qDebug()<<"Je fais le setMedia";
//        m_mediaPlayer->setMedia(QUrl::fromLocalFile(m_fileToPlay));
//    }
    m_abeAudioPlayerMediaPlayer->setVolume(100);

    ABULEDU_LOG_DEBUG() << "Je play Maintenant" << m_abeAudioPlayerMediaPlayer->media().canonicalUrl().toLocalFile();
    m_abeAudioPlayerMediaPlayer->play();

#else
    if(m_decoder)
    {
        m_decoder->disconnect(this);
        delete m_decoder;
    }
    if(m_player)
    {
        delete m_player;
    }
    m_decoder = new AbulEduAudioDecoderV1();
    m_decoder->setOutputLocation(AbulEduAudioDecoderV1::BUFFER);
    m_decoder->setSourceFilename(m_fileToPlay);
    connect(m_decoder, SIGNAL(done(QPointer<QIODevice>, qint64)), this, SLOT(play(QPointer<QIODevice>, qint64)), Qt::UniqueConnection);
    m_decoder->start();
#endif
}

void AbulEduAudioPlayerV1::play(QPointer<QIODevice> ioDevice, qint64 duration)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(ioDevice == 0 || duration == -1)
    {
        return;
    }
    if(m_player)
    {
        m_player->disconnect(this);
        delete m_player;
    }
    ioDevice->open(QIODevice::ReadOnly);
    ioDevice->seek(350) ;

    m_player = new QAudioOutput(m_outputDeviceInfo, m_format, this );
    m_player->setVolume(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioOutputLevel());

#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
    connect(m_player, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)), Qt::UniqueConnection);
#endif
    connect(m_player, SIGNAL(notify()), this, SLOT(slotNotify()), Qt::UniqueConnection);
    emit durationChanged(duration);
    m_player->start(ioDevice);
}

void AbulEduAudioPlayerV1::stop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_player)
    {
        ABULEDU_LOG_DEBUG() << "Je stoppe le m_player";
        m_player->stop();
    }

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    if(m_abeAudioPlayerMediaPlayer){
        ABULEDU_LOG_DEBUG() << "Je stoppe le m_mediaplayer";
        Q_ASSERT_X(m_abeAudioPlayerMediaPlayer, "AbulEduAudioPlayerV1", "MediaPlayer est pas bon");
        if(m_abeAudioPlayerMediaPlayer->state() != QMediaPlayer::StoppedState){
            m_abeAudioPlayerMediaPlayer->stop();
//            m_abeAudioPlayerMediaPlayer->setMedia(QMediaContent());
        }
    }
#endif

    QFile file(m_fileToPlay);
    if(!file.isOpen()){
        file.close();
    }
}

void AbulEduAudioPlayerV1::pause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_player)
    {
        m_player->suspend();
    }

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    if(m_abeAudioPlayerMediaPlayer)
        m_abeAudioPlayerMediaPlayer->pause();
#endif
}

void AbulEduAudioPlayerV1::resume()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_player)
    {
        m_player->resume();
    }

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    if(m_abeAudioPlayerMediaPlayer)
        m_abeAudioPlayerMediaPlayer->play();
#endif
}

void AbulEduAudioPlayerV1::setVolume(qreal volume)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_volume != volume)
    {
        m_volume = volume;
        if(m_player)
        {
            m_player->setVolume(volume);
        }
    }
}

void AbulEduAudioPlayerV1::abeAudioPlayerSetOutputDevice(QAudioDeviceInfo deviceInfo)
{
    //todo: Question, faudrait-il remonter comme périphérique par défaut ?
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< deviceInfo.deviceName();
    m_outputDeviceInfo = deviceInfo;
}

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
QMediaPlayer *AbulEduAudioPlayerV1::abeAudioPlayerGetMediaPlayer() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<"je retourne le mediaPlayer :" << m_abeAudioPlayerMediaPlayer;
    return m_abeAudioPlayerMediaPlayer;
}
#endif

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
void AbulEduAudioPlayerV1::abeAudioPlayerSetMediaPlayer(QMediaPlayer *mediaPlayer)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abeAudioPlayerMediaPlayer = mediaPlayer;
}
#endif

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
void AbulEduAudioPlayerV1::handleMediaStateChanged(QMediaPlayer::State newState)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< newState << sender()->objectName();

    switch (newState) {
    /** Comment peut on passer là ?! LoadedMedia est un status et pas un state ... */
    case QMediaPlayer::LoadedMedia:
        m_abeAudioPlayerMediaPlayer->play();
        break;

    case QMediaPlayer::StoppedState:
        emit signalAbeAudioPlayerMediaFinished();
        break;

    default:
        break;
    }
}
#else
void AbulEduAudioPlayerV1::handleStateChanged(QAudio::State newState)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    emit stateChanged(newState);
    switch (newState) {

    case QAudio::ActiveState:
        break;
    case QAudio::SuspendedState:
        emit error(m_player->error());
        break;
    case QAudio::StoppedState:

        break;
    case QAudio::IdleState:
        /* Plus rien à jouer */
        emit signalAbeAudioPlayerMediaFinished();
        break;
    default:
        break;
    }
}
#endif

void AbulEduAudioPlayerV1::slotNotify()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit durationChanged(m_player->processedUSecs() / 1000.0); // en millisecondes
}
