/** Classe AbulEduSoundRecorderV1
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2013-2015 Eric Seigne <eric.seigne@ryxeo.com>
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
#include "abuledusoundrecorderv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

static qreal getPeakValue(const QAudioFormat &format);
static qreal getBufferLevel(const QAudioBuffer &buffer);

template <class T>
static qreal getBufferLevel(const T *buffer, int samples);

AbulEduSoundRecorderV1::AbulEduSoundRecorderV1(const QString &outputFileName) :
    AbulEduSoundPlayerV1("",outputFileName,""),
    m_availabiliyStatus(QMultimedia::Available)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_recorder = new QAudioRecorder;
    connect(m_recorder, SIGNAL(availabilityChanged(QMultimedia::AvailabilityStatus)), this, SLOT(slotAvailabilityChanged(QMultimedia::AvailabilityStatus)),Qt::UniqueConnection);

    ABULEDU_LOG_DEBUG()<<"Carte utilisée : "<<m_recorder->audioInputDescription(m_recorder->audioInput());
    m_recorder->setAudioInput(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioInputDeviceInfo().deviceName());

#ifdef Q_OS_WIN
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setBitRate(128000);
    audioSettings.setChannelCount(2);
    audioSettings.setSampleRate(44100);
    audioSettings.setEncodingMode(QMultimedia::ConstantBitRateEncoding);
    m_recorder->setEncodingSettings(audioSettings);
    //Et donc le nom du fichier n'est pas .ogg mais .wav
    m_outputFileName.replace(".ogg",".wav");
#else /** @todo ifdef linux et/ou android */
    m_recorder->setContainerFormat("ogg");
#endif

    if(!m_outputFileName.isEmpty())
    {
        ABULEDU_LOG_DEBUG()<<"Outfilename est pas vide";;
        /** @deprecated abeSoundRecorderSetOutputFileName(outputFileName); */
        abeMediaViewerSetOutputFilePath(outputFileName);
        AbulEduSoundPlayerV1::abeMediaViewerSetSoundPath(outputFileName);
    }
    else
    {
        ABULEDU_LOG_DEBUG()<<"Outfilename est vide";;

        /** @deprecated abeSoundRecorderSetOutputFileName(QDir::tempPath() + "/essai.ogg"); */
        abeMediaViewerSetOutputFilePath(QDir::tempPath() + "/essai.ogg");
    }

    m_audioProbe = new QAudioProbe;
    connect(m_audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(slotAbeSoundRecorderProcessBuffer(QAudioBuffer)));
    m_audioProbe->setSource(m_recorder);
}

AbulEduSoundRecorderV1::~AbulEduSoundRecorderV1()
{
    if(m_recorder)
        m_recorder->deleteLater();;
    if(m_audioProbe)
        m_audioProbe->deleteLater();
}

void AbulEduSoundRecorderV1::abeSoundRecorderSetOutputFileName(const QString &locationPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ABULEDU_LOG_WARN()  <<  " IS DEPRECATED please use AbulEduSoundRecorderV1::abeMediaViewerSetOutputFilePath(QString filePath)";
    m_recorder->setOutputLocation(QUrl(locationPath));
}

void AbulEduSoundRecorderV1::abeMediaViewerSetOutputFilePath(const QString &filePath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<filePath;
    QFileInfo fileinfo(filePath);
    if(!QDir(fileinfo.absolutePath()).exists()){
        QDir().mkpath(fileinfo.absolutePath());
    }
    m_outputFileName = filePath;
    m_recorder->setOutputLocation(QUrl(m_outputFileName));
}

QString AbulEduSoundRecorderV1::abeMediaViewerGetOutputFilePath() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_outputFileName;
}

void AbulEduSoundRecorderV1::abeMediaViewerSetSoundPath(const QString &soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<soundPath;
    /** @deprecated abeSoundRecorderSetOutputFileName(soundPath); */
    abeMediaViewerSetOutputFilePath(soundPath);
    AbulEduSoundPlayerV1::abeMediaViewerSetSoundPath(soundPath);
}

void AbulEduSoundRecorderV1::abeMediaViewerSetInputDevice(QAudioDeviceInfo deviceInfo)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_inputDeviceInfo = deviceInfo;
    m_recorder->setAudioInput(m_inputDeviceInfo.deviceName());
}

void AbulEduSoundRecorderV1::abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<mediaPath<<soundPath<<text;
    abeMediaViewerSetOutputFilePath(soundPath);
    AbulEduSoundPlayerV1::abeMediaViewerSetMediaDatas(mediaPath,soundPath,text);
}

QAudioProbe *AbulEduSoundRecorderV1::abeMediaRecorderGetAudioProbe()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_audioProbe;
}

#ifdef Q_OS_WIN
void AbulEduSoundRecorderV1::slotAbeMediaViewerWaveEncoderStatus(WaveToOgg::Status statusWave,WaveHeaderReader::Status statusHeader,QString cheminFichier)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Recuperation name enum  WaveToOgg::Status */
    const QMetaObject &moWaveToOgg = WaveToOgg::staticMetaObject;
    QMetaEnum meWaveToOgg = moWaveToOgg.enumerator(moWaveToOgg.indexOfEnumerator("Status"));

    /* Recuperation name enum  WaveHeaderReader::Status */
    const QMetaObject &moWaveHeaderReader = WaveHeaderReader::staticMetaObject;
    QMetaEnum meWaveHeaderReader = moWaveHeaderReader.enumerator(moWaveHeaderReader.indexOfEnumerator("Status"));
    ABULEDU_LOG_DEBUG() << (QString("Status Encoder : %1").arg(QString(meWaveToOgg.valueToKey(statusWave))));
    ABULEDU_LOG_DEBUG() << (QString("Status WaveReader : %1").arg(QString(meWaveHeaderReader.valueToKey(statusHeader))));

    if(statusWave == WaveToOgg::EncoderAvailable) {
        m_waveEncoder->encodeWavFile();
    }

    /* si 3 conditions ci-dessous, le fichier est converti & ok */
    if(statusWave == WaveToOgg::ProcessOK && statusHeader == WaveHeaderReader::FileOK && !cheminFichier.isEmpty()){
            ABULEDU_LOG_DEBUG() << "Conversion Fichier OK";
            ABULEDU_LOG_DEBUG() << "Le chemin du fichier :: " << m_outputFileName;
        abeMediaViewerSetSoundPath(m_outputFileName.replace(".wav", ".ogg"));
    }
}
#endif

void AbulEduSoundRecorderV1::slotAbeMediaViewerStartRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<abeMediaViewerGetOutputFilePath();
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    /* Sous Windows, il faut faire "lâcher" le fichier au QMediaPlayer pour pouvoir le supprimer */
    abeMultiMediaGetMediaPlayer()->setMedia(QMediaContent());
    QFileInfo fi(abeMediaViewerGetOutputFilePath());
    if(QFile::exists(abeMediaViewerGetOutputFilePath())){
        /* Par contre certains fichiers - notamment les consignes par défaut apportées par les applications - ne peuvent pas être détruites
         * On se contente pour le moment de les renommer pour pouvoir utiliser leur nom et les recréer */
        ABULEDU_LOG_DEBUG() << "  Un fichier existe deja, on le renomme : " << abeMediaViewerGetOutputFilePath();
        if(QFile(abeMediaViewerGetOutputFilePath()).rename(fi.absolutePath()+QString("/aeffacer%1.ogg").arg(QTime::currentTime().toString().remove(":")))) {
            ABULEDU_LOG_DEBUG() << "    renommage ok";
        }
        else {
            ABULEDU_LOG_DEBUG() << "    renommage err";
        }
    }
    /* Sur windows, impossible de mettre le fichier au bon endroit... */
    if(!m_recorder->setOutputLocation(QUrl::fromLocalFile(m_outputFileName.replace(".ogg", ".wav")))){
        ABULEDU_LOG_DEBUG()<<"Impossible de mettre à jour le outPutLocation";
    }
#else
    if(QFile::exists(abeMediaViewerGetOutputFilePath())){
            ABULEDU_LOG_DEBUG() << "  Un fichier existe deja, on le supprime : " << abeMediaViewerGetOutputFilePath();
            if(QFile(abeMediaViewerGetOutputFilePath()).remove()) {
                ABULEDU_LOG_DEBUG() << "    suppression ok";
            }
            else {
                ABULEDU_LOG_DEBUG() << "    suppression err";
            }
        }
#endif

    // qDebug() << m_recorder->audioSettings().bitRate()<< m_recorder->audioSettings().channelCount()<< m_recorder->audioSettings().sampleRate();

    emit signalAbeMediaViewerRecordingStarted();
    m_recorder->record();
    ABULEDU_LOG_DEBUG() << "  On commence l'enregistrement : " << abeMediaViewerGetOutputFilePath() <<" / "<< m_outputFileName;
}

void AbulEduSoundRecorderV1::slotAbeMediaViewerStopRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_recorder->stop();
    QFile fileToConvert(m_recorder->outputLocation().toLocalFile());
    ABULEDU_LOG_DEBUG() << "  " << fileToConvert.exists() << fileToConvert.fileName();

#ifdef Q_OS_WIN
    /* cas particulier windows le fichier enregistre est en fait un wav qui est nommé en "dur" .ogg (merci Eric) */
    QString nouveauFilename = fileToConvert.fileName().replace(".ogg", ".wav");
    ABULEDU_LOG_DEBUG() << "  RENOMMAGE : " << nouveauFilename;
    if(QFile::rename(fileToConvert.fileName(), nouveauFilename)){
        ABULEDU_LOG_DEBUG() << fileToConvert.fileName();
        fileToConvert.setFileName(nouveauFilename);
    }
    /* Changement du nom du fichier avant de créer l'objet AbulEduAudioV1::WaveToOgg car dans son constructeur, ou plutot
       dans le constructeur d'un de ses objet "fils" auquel on passe this comme parent il y a une ouverture de l'entete du
       fichier wav ... qui est peut-être justement le fichier en cours ! */

    /** @todo gestion des chemins*/
    m_waveEncoder = new AbulEduAudioV1::WaveToOgg(this);
    /* Connection sur le status */
    connect(m_waveEncoder, SIGNAL(signalWaveToOggStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)), this, SLOT(slotAbeMediaViewerWaveEncoderStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)), Qt::UniqueConnection);
    /* Connection sur les pourcentages */
    connect(m_waveEncoder, SIGNAL(signalWaveToOggProcessPercent(float)), this, SLOT(slotAbeMediaViewerDisplayProgression(float)), Qt::UniqueConnection);

    if(fileToConvert.isOpen()) {
        fileToConvert.close();
    }
    m_waveEncoder->processWavFile(fileToConvert.fileName(), fileToConvert.fileName().replace(".wav", ".ogg"));
#endif
    emit signalAbeMediaViewerRecordingStopped();
}

void AbulEduSoundRecorderV1::slotAbeMediaViewerPauseRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMediaViewerRecordingPaused();
    m_recorder->pause();
}

void AbulEduSoundRecorderV1::slotAbeMediaViewerResumeRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMediaViewerRecordingResumed();
    m_recorder->record();
}

void AbulEduSoundRecorderV1::slotAbeSoundRecorderProcessBuffer(const QAudioBuffer& buffer)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< getBufferLevel(buffer);
    emit signalAbeMediaViewerLevel(getBufferLevel(buffer));
}

void AbulEduSoundRecorderV1::slotAvailabilityChanged(QMultimedia::AvailabilityStatus status)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_availabiliyStatus = status;
    if(m_availabiliyStatus == QMultimedia::Available)
    {
        emit signalAbeMediaViewerIsAvailable(true);
    }
    else
    {
        emit signalAbeMediaViewerIsAvailable(false);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
///    Fonctions utilisées pour mesurer le niveau d'enregistrement code issu de Qt exemple AudioRecorder
//////////////////////////////////////////////////////////////////////////////////////////////
// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return 0.0;

    if (format.codec() != "audio/pcm")
        return 0.0;

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return 0.0;
        return 1.00003;
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return 2147483648.0;
        if (format.sampleSize() == 16)
            return 32768.0;
        if (format.sampleSize() == 8)
            return 128.0;
        break;
    case QAudioFormat::UnSignedInt:
        // Unsigned formats are not supported in this example
        break;
    }

    return 0.0;
}

qreal getBufferLevel(const QAudioBuffer& buffer)
{
    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return 0.0;

    if (buffer.format().codec() != "audio/pcm")
        return 0.0;

    qreal peak_value = getPeakValue(buffer.format());

    // Jean-Louis Prise en compte du nb de canaux
    if(buffer.format().channelCount() == 1)
    {
        peak_value = peak_value/2;
    }

    if (qFuzzyCompare(0.0, peak_value)) {
        return 0.0;
    }

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32)
            return getBufferLevel(buffer.constData<float>(), buffer.sampleCount()) / peak_value;
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            return getBufferLevel(buffer.constData<long int>(), buffer.sampleCount()) / peak_value;
        if (buffer.format().sampleSize() == 16)
            return getBufferLevel(buffer.constData<short int>(), buffer.sampleCount()) / peak_value;
        if (buffer.format().sampleSize() == 8)
            return getBufferLevel(buffer.constData<signed char>(), buffer.sampleCount()) / peak_value;
        break;
    }

    return 0.0;
}

template <class T>
qreal getBufferLevel(const T *buffer, int samples)
{
    qreal max_value = 0.0;

    for (int i = 0; i < samples; ++i) {
        qreal value = qAbs(qreal(buffer[i]));
        if (value > max_value)
            max_value = value;
    }

    return max_value;
}
