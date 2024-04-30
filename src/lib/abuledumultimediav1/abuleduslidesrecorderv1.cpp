/** Classe AbulEduSlidesRecorderV1
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

#include "abuleduslidesrecorderv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

static qreal getPeakValue(const QAudioFormat &format);
//static qreal getBufferLevel(const QAudioBuffer &buffer);

//template <class T>
//static qreal getBufferLevel(const T *buffer, int samples);

static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);


AbulEduSlidesRecorderV1::AbulEduSlidesRecorderV1(QWidget *parent)
    :AbulEduSlidesViewerV1(parent),
      m_outputFileName(QString()),
      m_audioProbe(0),
      m_availabilityStatus(QMultimedia::Available)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeSlidesRecorderInitialize();
}

AbulEduSlidesRecorderV1::AbulEduSlidesRecorderV1(const QString &pixmapPath,
                                                 const QString &soundPath,
                                                 const QString &text)
    :AbulEduSlidesViewerV1(pixmapPath, soundPath, text),
      m_outputFileName(soundPath),
      m_audioProbe(0),
      m_availabilityStatus(QMultimedia::Available)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeSlidesRecorderInitialize();
}

void AbulEduSlidesRecorderV1::abeSlidesRecorderInitialize()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_recorder = new QAudioRecorder();
    connect(m_recorder, SIGNAL(availabilityChanged(QMultimedia::AvailabilityStatus)), this, SLOT(slotAbeMediaViewerAvailabilityChanged(QMultimedia::AvailabilityStatus)),Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(error(QMediaRecorder::Error)), this, SLOT(slotErrorRecorder(QMediaRecorder::Error)), Qt::UniqueConnection);
    m_recorder->setAudioInput(m_abuleduMultiMediaSettings->abeMultiMediaSettingsGetAudioInputDeviceInfo().deviceName());

    ABULEDU_LOG_DEBUG() << "  Input device : " << m_recorder->audioInput();

    //Un nom par defaut si le fichier de sortie est vide
    if(m_outputFileName.isEmpty()) {
        ABULEDU_LOG_DEBUG() << "  Outfilename est vide, on affecte une valeur par defaut";
        m_outputFileName = QDir::tempPath() + "/essai.ogg";
    }

    QAudioEncoderSettings audioSettings;
#ifdef Q_OS_WIN
    audioSettings.setCodec("audio/pcm");
    audioSettings.setBitRate(128000);
    audioSettings.setChannelCount(2);
    audioSettings.setSampleRate(44100);
    audioSettings.setEncodingMode(QMultimedia::ConstantBitRateEncoding);
    m_recorder->setEncodingSettings(audioSettings);
    //Et donc le nom du fichier n'est pas .ogg mais .wav
    m_outputFileName.replace(".ogg",".wav");
#else /** @todo ifdef linux et/ou android */
    audioSettings.setCodec("audio/vorbis");
//    audioSettings.setSampleRate(44100);
//    audioSettings.setBitRate(128000);
//    audioSettings.setEncodingMode(QMultimedia::ConstantBitRateEncoding);
//    audioSettings.setChannelCount(2);
    m_recorder->setAudioSettings(audioSettings);
    qDebug() <<m_recorder->isAvailable() << m_recorder->supportedAudioCodecs() << m_recorder->supportedContainers();
    m_recorder->setContainerFormat("ogg");
#endif

    ABULEDU_LOG_DEBUG() << "  Outfilename : " << m_outputFileName;
    abeMediaViewerSetOutputFilePath(m_outputFileName);
    AbulEduSlidesViewerV1::abeMediaViewerSetSoundPath(m_outputFileName);

    m_audioProbe = new QAudioProbe;
    connect(m_audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(slotAbeMediaViewerProcessBuffer(QAudioBuffer)),Qt::UniqueConnection);
    m_audioProbe->setSource(m_recorder);

}

void AbulEduSlidesRecorderV1::abeMediaViewerSetOutputFilePath(const QString &filePath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< " " << filePath;

    QFileInfo fileinfo(filePath);
    if(!QDir(fileinfo.absolutePath()).exists()){
        QDir().mkpath(fileinfo.absolutePath());
    }
    m_outputFileName = filePath;
    m_recorder->setOutputLocation(QUrl(filePath));
}

QString AbulEduSlidesRecorderV1::abeMediaViewerGetOutputFilePath() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_outputFileName;
}

void AbulEduSlidesRecorderV1::abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeMediaViewerSetOutputFilePath(soundPath);
    AbulEduSlidesViewerV1::abeMediaViewerSetMediaDatas(mediaPath,soundPath,text);
}

void AbulEduSlidesRecorderV1::slotAbeMediaViewerStartRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
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

void AbulEduSlidesRecorderV1::slotAbeMediaViewerStopRecording()
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

void AbulEduSlidesRecorderV1::slotAbeMediaViewerDisplayProgression(float percent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< "  Progression encodage ::" << percent;
}

#ifdef Q_OS_WIN
void AbulEduSlidesRecorderV1::slotAbeMediaViewerWaveEncoderStatus(WaveToOgg::Status statusWave,WaveHeaderReader::Status statusHeader,QString cheminFichier)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Recuperation name enum  WaveToOgg::Status */
    const QMetaObject &moWaveToOgg = WaveToOgg::staticMetaObject;
    QMetaEnum meWaveToOgg = moWaveToOgg.enumerator(moWaveToOgg.indexOfEnumerator("Status"));

    /* Recuperation name enum  WaveHeaderReader::Status */
    const QMetaObject &moWaveHeaderReader = WaveHeaderReader::staticMetaObject;
    QMetaEnum meWaveHeaderReader = moWaveHeaderReader.enumerator(moWaveHeaderReader.indexOfEnumerator("Status"));

    ABULEDU_LOG_DEBUG()  << "  Status Encoder : " << QString(meWaveToOgg.valueToKey(statusWave));
    ABULEDU_LOG_DEBUG()  << "  Status WaveReader : " << QString(meWaveHeaderReader.valueToKey(statusHeader));

    if(statusWave == WaveToOgg::EncoderAvailable) {
        m_waveEncoder->encodeWavFile();
    }

    /* si 3 conditions ci-dessous, le fichier est converti & ok */
    if(statusWave == WaveToOgg::ProcessOK && statusHeader == WaveHeaderReader::FileOK && !cheminFichier.isEmpty()){
        ABULEDU_LOG_DEBUG() << "  Conversion Fichier OK";
        ABULEDU_LOG_DEBUG() << "  Le chemin du fichier :: " << m_outputFileName;
        abeMediaViewerSetSoundPath(m_outputFileName.replace(".wav", ".ogg"));
    }
}
#endif

void AbulEduSlidesRecorderV1::slotAbeMediaViewerPauseRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMediaViewerRecordingPaused();
    m_recorder->pause();
}

void AbulEduSlidesRecorderV1::slotAbeMediaViewerResumeRecording()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMediaViewerRecordingResumed();
    m_recorder->record();
}

void AbulEduSlidesRecorderV1::slotAbeMediaViewerProcessBuffer(const QAudioBuffer &buffer)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QVector<qreal> levels = getBufferLevels(buffer);
    emit signalAbeMediaViewerLevel(levels.at(0));
}

void AbulEduSlidesRecorderV1::slotAbeMediaViewerAvailabilityChanged(QMultimedia::AvailabilityStatus status)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_availabilityStatus = status;
    if(m_availabilityStatus == QMultimedia::Available)
    {
        emit signalAbeMediaViewerIsAvailable(true);
    }
    else
    {
        emit signalAbeMediaViewerIsAvailable(false);
    }
}

QAudioProbe *AbulEduSlidesRecorderV1::abeMediaRecorderGetAudioProbe()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_audioProbe;
}

AbulEduSlidesRecorderV1::~AbulEduSlidesRecorderV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_audioProbe)
        m_audioProbe->deleteLater();
    if(m_recorder)
        m_recorder->deleteLater();
}

void AbulEduSlidesRecorderV1::slotErrorRecorder(QMediaRecorder::Error error)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << error;
}

//////////////////////////////////////////////////////////////////////////////////////////////
///    Fonctions utilisées pour mesurer le niveau d'enregistrement code issu de Qt exemple AudioRecorder
//////////////////////////////////////////////////////////////////////////////////////////////
// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

//qreal getBufferLevel(const QAudioBuffer& buffer)
//{
//    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
//        return 0.0;

//    if (buffer.format().codec() != "audio/pcm")
//        return 0.0;

//    qreal peak_value = getPeakValue(buffer.format());

//    // Jean-Louis Prise en compte du nb de canaux
////    if(buffer.format().channelCount() == 1)
////    {
////        peak_value = peak_value/2;
////    }

//    if (qFuzzyCompare(peak_value, 0.0))
//        return 0.0;

//    switch (buffer.format().sampleType()) {
//    case QAudioFormat::Unknown:
//    case QAudioFormat::UnSignedInt:
//        break;
//    case QAudioFormat::Float:
//        if (buffer.format().sampleSize() == 32)
//            return getBufferLevel(buffer.constData<float>(), buffer.sampleCount()) / peak_value;
//        break;
//    case QAudioFormat::SignedInt:
//        if (buffer.format().sampleSize() == 32)
////            return getBufferLevel(buffer.constData<qint32>(), buffer.sampleCount()) / peak_value;
//            return 0.5;
//        if (buffer.format().sampleSize() == 16)
//            return getBufferLevel(buffer.constData<short int>(), buffer.sampleCount()) / peak_value;
//        if (buffer.format().sampleSize() == 8)
//            return getBufferLevel(buffer.constData<signed char>(), buffer.sampleCount()) / peak_value;
//        break;
//    }

//    return 0.0;
//}

//template <class T>
//qreal getBufferLevel(const T *buffer, int samples)
//{
//    qreal max_value = 0.0;

//    for (int i = 0; i < samples; ++i) {
//        qreal value = qAbs(qreal(buffer[i]));
//        if (value > max_value)
//            max_value = value;
//    }

//    return max_value;
//}
// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

//void AudioRecorder::processBuffer(const QAudioBuffer& buffer)
//{
//    QVector<qreal> levels = getBufferLevels(buffer);
//    for (int i = 0; i < levels.count(); ++i)
//    {
//        if(audioLevels.size()>0)
//        audioLevels.at(i)->setLevel(levels.at(i));
//    }
//}
