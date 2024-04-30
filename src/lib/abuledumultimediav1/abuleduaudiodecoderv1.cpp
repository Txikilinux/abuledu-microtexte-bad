/** @file abuledudecoderv1.cpp
  *
  * @author Copyright(C) 2012 Digia Plc and/or its subsidiary(-ies).
  * @author Icham Sirat <icham.sirat@ryxeo.com>
  * @author Eric Seigne <eric.seigne@ryxeo.com>
  * @date 2013 12 18
  * @see The GNU Public License (GPL)
  *
  *
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

#include "abuleduaudiodecoderv1.h"
#include <QDir>

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

/**
 * @brief AbulEduAudioDecoderV1::AbulEduAudioDecoderV1
 *
 *
 *
 * README : CLASSE DE TEST...
 * UTILISATION (non definitive) :
 *
 *      AbulEduAudioDecoderV1 *toto = new AbulEduAudioDecoderV1();
 *      connect(toto, SIGNAL(done(QPointer<QIODevice>)), this, SLOT(play(QPointer<QIODevice>)));
 *      toto->setSourceFilename("/tmp/music.mp3");
 *      toto->setOutputLocation(AbulEduAudioDecoderV1::BUFFER);
 *      toto->start();
 *
 *     // [... dans une autre classe]
 *     void MaClasse::play(QPointer<QIODevice> fileDecoded)
 *     {
 *          qDebug() << fileDecoded;
 *          if(toto)
 *              delete toto;
 *          toto = 0;
 *     }
 *
 *
 */

AbulEduAudioDecoderV1::AbulEduAudioDecoderV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    // On s'assure que les données recues sont dans un format correct (PCM).
    // WaveFileWriter supporte que des eechantillons de type SignedInt.
    QAudioFormat format;
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setSampleRate(44100);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    m_decoder.setAudioFormat(format);

    connect(&m_decoder, SIGNAL(bufferReady()), this, SLOT(bufferReady()), Qt::UniqueConnection);
    connect(&m_decoder, SIGNAL(error(QAudioDecoder::Error)), this, SLOT(error(QAudioDecoder::Error)), Qt::UniqueConnection);
#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
    connect(&m_decoder, SIGNAL(stateChanged(QAudioDecoder::State)), this, SLOT(stateChanged(QAudioDecoder::State)), Qt::UniqueConnection);
#endif
    connect(&m_decoder, SIGNAL(finished()), this, SLOT(finished()), Qt::UniqueConnection);
    connect(&m_decoder, SIGNAL(positionChanged(qint64)), this, SLOT(updateProgress()), Qt::UniqueConnection);
    connect(&m_decoder, SIGNAL(durationChanged(qint64)), this, SLOT(updateProgress()), Qt::UniqueConnection);
    m_progress = -1.0;

    setTargetFilename(QDir::tempPath() + QDir::separator() + "out.wav");
}

void AbulEduAudioDecoderV1::setSourceFilename(const QString &fileName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_sourceFilename = fileName;
    m_decoder.setSourceFilename(fileName);
}

void AbulEduAudioDecoderV1::setTargetFilename(const QString &fileName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_targetFilename = fileName;
}

void AbulEduAudioDecoderV1::setOutputLocation(AbulEduAudioDecoderV1::SaveLocation saveLocation)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch (saveLocation) {
    case BUFFER:
        m_fileWriter.makeOutputFile(false);
        break;
    case FILE:
        m_fileWriter.makeOutputFile(true);
        break;
    }
}

void AbulEduAudioDecoderV1::start()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< m_sourceFilename;
    /* Le fichier existe, sinon basta */
    if(!QFile::exists(m_sourceFilename)) {
        ABULEDU_LOG_DEBUG() << trUtf8("Le fichier transmis %1 pour décodage n'existe pas").arg(m_sourceFilename);
        return;
    }

    if(m_decoder.isAvailable()){
        ABULEDU_LOG_DEBUG() << "Start ! ";
        m_decoder.start();
    }
}

void AbulEduAudioDecoderV1::stop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_decoder.stop();
}

void AbulEduAudioDecoderV1::bufferReady()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    // read a buffer from audio decoder
    QAudioBuffer buffer = m_decoder.read();
    if (!buffer.isValid()) {
        return;
    }
    if (!m_fileWriter.isOpen() && !m_fileWriter.open(m_targetFilename, buffer.format())) {
        ABULEDU_LOG_DEBUG() << "Decoder STOP : can't open file ..." << m_targetFilename;
        m_decoder.stop();
        return;
    }
    m_fileWriter.write(buffer);
}

void AbulEduAudioDecoderV1::error(QAudioDecoder::Error error)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch (error) {
    case QAudioDecoder::NoError:
        return;
    case QAudioDecoder::ResourceError:
        ABULEDU_LOG_DEBUG() << "Resource error" << endl;
        break;
    case QAudioDecoder::FormatError:
        ABULEDU_LOG_DEBUG() << "Format error" << endl;
        break;
    case QAudioDecoder::AccessDeniedError:
        ABULEDU_LOG_DEBUG() << "Access denied error" << endl;
        break;
    case QAudioDecoder::ServiceMissingError:
        ABULEDU_LOG_DEBUG() << "Service missing error" << endl;
        break;
    }
    ABULEDU_LOG_DEBUG() << "Emission SIGNAL DONE";
    emit done();
}

#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
void AbulEduAudioDecoderV1::stateChanged(QAudioDecoder::State newState)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch (newState) {
    case QAudioDecoder::DecodingState:
        ABULEDU_LOG_DEBUG() << "Decoding..." << endl;
        break;
    case QAudioDecoder::StoppedState:
        ABULEDU_LOG_DEBUG() << "Decoding stopped" << endl;
        break;
    }
}
#endif

void AbulEduAudioDecoderV1::finished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (!m_fileWriter.close()) {
        ABULEDU_LOG_DEBUG() << "Failed to finilize output file" << endl;
    }

    ABULEDU_LOG_DEBUG() << "Decoding finished: 100 %" << endl
                        << "Emission SIGNAL DONE" << m_fileWriter.getDecodedFile()<<m_decoder.duration()<<"ms";
    emit done(m_fileWriter.getDecodedFile(), m_decoder.duration());
}

void AbulEduAudioDecoderV1::updateProgress()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qint64 position = m_decoder.position();
    qint64 duration = m_decoder.duration();
    qreal progress = m_progress;
    if (position >= 0 && duration > 0)
        progress = position / (qreal)duration;
    if (progress > m_progress + 0.1) {
        ABULEDU_LOG_DEBUG() << "Decoding progress: " << (int)(progress * 100.0) << "%";
        m_progress = progress;
    }
}

