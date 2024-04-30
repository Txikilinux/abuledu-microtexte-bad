/** Classe WaveToOgg : Encoder Wave -> Ogg
  *
  * @author 2014 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
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

#ifndef DEBUG_ABULEDUAUDIOV1
    #include "abuledudisableloggerv1.h"
#endif

#include "wavetoogg.h"
#include "wavetooggthread.h" //Ici pour ne pas avoir de pb d'inclusion récursive

/** @todo METHODE POUR AJOUTER DES COMMENTAIRES ET AUTRES */
AbulEduAudioV1::WaveToOgg::WaveToOgg(QObject *parent) :
    QObject(parent),
    m_tagTitle(""),
    m_tagEncoder(""),
    m_tagVersion(""),
    m_tagAlbum(""),
    m_tagTrackNumber(""),
    m_tagArtist(""),
    m_tagPerformer(""),
    m_tagCopyright(""),
    m_tagLicence(""),
    m_tagOrganization(""),
    m_tagDescription(""),
    m_tagGenre(""),
    m_tagDate(""),
    m_tagLocation(""),
    m_tagContact(""),
    m_tagISRCNumber("")
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    qRegisterMetaType< AbulEduAudioV1::WaveToOgg::Status >("AbulEduAudioV1::WaveToOgg::Status");


    m_waveHeaderReader = new  AbulEduAudioV1::WaveHeaderReader(this);
    connect(m_waveHeaderReader, SIGNAL(signalWaveHeaderReaderStatus(WaveHeaderReader::Status)),this, SLOT(slotWaveToOggStatus(WaveHeaderReader::Status)), Qt::UniqueConnection);

    /* Quelques variables par defaut */
    m_tagEncoder = "AbulEduAudioV1::Vorbis";
    m_tagGenre = "Raconte-moi";
}

AbulEduAudioV1::WaveToOgg::~WaveToOgg()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduAudioV1::WaveToOgg::encodeWavFile()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalWaveToOggStatus(WaveToOgg::ProcessStarted, WaveHeaderReader::FileOK, 0);
    emit signalWaveToOggStatus(AbulEduAudioV1::WaveToOgg::ProcessStarted, AbulEduAudioV1::WaveHeaderReader::FileOK, 0);

    WaveToOggThread *workerThread = new WaveToOggThread();
    workerThread->setFiles(m_fileInPath,m_fileOutPath);
    workerThread->setMetaData(m_tagTitle, m_tagEncoder, m_tagVersion, m_tagAlbum, m_tagTrackNumber, m_tagArtist, m_tagPerformer, m_tagCopyright, m_tagLicence, m_tagOrganization, m_tagDescription, m_tagGenre, m_tagDate, m_tagLocation, m_tagContact, m_tagISRCNumber);

//    connect(workerThread, SIGNAL(signalWaveToOggThreadStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)), this, SIGNAL(signalWaveToOggStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)));
    connect(workerThread, SIGNAL(signalWaveToOggThreadStatus(AbulEduAudioV1::WaveToOgg::Status,AbulEduAudioV1::WaveHeaderReader::Status,QString)), this, SLOT(slotWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::Status,AbulEduAudioV1::WaveHeaderReader::Status,QString)));
    connect(workerThread, SIGNAL(signalWaveToOggThreadProcessPercent(float)), this, SIGNAL(signalWaveToOggProcessPercent(float)));
    connect(workerThread, &WaveToOggThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

void AbulEduAudioV1::WaveToOgg::processWavFile(const QString &fileInPath, const QString &fileOutPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Attention il faut faire un appel a waveEncoder->encodeWavFile();
    //pour générer réellement le fichier ogg
    m_fileInPath = fileInPath;
    m_fileOutPath = fileOutPath;

    /** @todo Verifier FileOut (droit d'écriture) */
    QFileInfo fi(fileOutPath);
    if(!fi.exists()) {
        QFile fileOut(fileOutPath);
        if(fileOut.open(QIODevice::ReadWrite)) {
            fileOut.close();
        }
        else {
            ABULEDU_LOG_DEBUG() << "  " << "impossible d'ouvir le fichier en ecriture : " << fileOutPath;
            emit signalWaveToOggStatus(FileToWriteNotOpenable, WaveHeaderReader::FileError, fileOutPath);
            emit signalWaveToOggStatus(AbulEduAudioV1::WaveToOgg::FileToWriteNotOpenable, AbulEduAudioV1::WaveHeaderReader::FileError, fileOutPath);
            return;//error
        }
    }
    /* Si le fichier n'existe pas, bye bye */

    ABULEDU_LOG_DEBUG() << "  " << m_fileInPath << " => " << m_fileOutPath;
    QFile fileToControl(m_fileInPath);
    if(!fileToControl.exists()){
        ABULEDU_LOG_DEBUG() << "  Le fichier In n'existe pas ";
        emit signalWaveToOggStatus(FileToEncodeNotFound, WaveHeaderReader::FileError, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::FileToEncodeNotFound, AbulEduAudioV1::WaveHeaderReader::FileError, 0);
        return ;
    }

    /* Si le fichier n'est pas ouvrable en lecture */
    if(!fileToControl.open(QIODevice::ReadOnly)){
        ABULEDU_LOG_DEBUG() << "  Le fichier In n'a pas pu être ouvert ";
        emit signalWaveToOggStatus(FileNotReadable, WaveHeaderReader::FileError, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::FileNotReadable, AbulEduAudioV1::WaveHeaderReader::FileError, 0);
        return ;
    }

    fileToControl.close();

    ifstream wave(m_fileInPath.toLocal8Bit());
    m_waveHeaderReader->slotWaveHeaderReaderReadWave(wave);
}

void AbulEduAudioV1::WaveToOgg::setOggTagTitle(const QString &s)
{
    m_tagTitle = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagEncoder(const QString &s)
{
    m_tagEncoder = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagVersion(const QString &s)
{
    m_tagVersion = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagAlbum(const QString &s)
{
    m_tagAlbum = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagTrackNumber(const QString &s)
{
    m_tagTrackNumber = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagArtist(const QString &s)
{
    m_tagArtist = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagPerformer(const QString &s)
{
    m_tagPerformer = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagCopyright(const QString &s)
{
    m_tagCopyright = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagLicence(const QString &s)
{
    m_tagLicence = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagOrganization(const QString &s)
{
    m_tagOrganization = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagDescription(const QString &s)
{
    m_tagDescription = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagGenre(const QString &s)
{
    m_tagGenre = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagDate(const QString &s)
{
    m_tagDate = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagLocation(const QString &s)
{
    m_tagLocation = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagContact(const QString &s)
{
   m_tagContact = s;
}

void AbulEduAudioV1::WaveToOgg::setOggTagISRCNumber(const QString &s)
{
    m_tagISRCNumber = s;
}

void AbulEduAudioV1::WaveToOgg::slotWaveToOggStatus(WaveHeaderReader::Status status)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch (status) {
    case WaveHeaderReader::FileOK:
        ABULEDU_LOG_DEBUG() << "  FileOK, on peut y aller";
        emit signalWaveToOggStatus(EncoderAvailable, WaveHeaderReader::FileOK, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderAvailable, AbulEduAudioV1::WaveHeaderReader::FileOK, 0);
        emit signalWaveToOggInformationsAvailable(m_waveHeaderReader->getInformations());
        /** @warning : est commenté parceque sous windows on a un probleme de latence ou d'asynchrone ou ... qui fait que le fichier n'est pas relaché assez tôt
         *  il faut donc faire le nécessaire avec un connect comme par exemple
         *  connect(m_waveEncoder, SIGNAL(signalWaveToOggStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)), this, SLOT(slotWaveEncoderStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)), Qt::UniqueConnection);
         *  m_waveEncoder->processWavFile("/tmp/fichier.wav", "/tmp/fichier.ogg");
         */
//        encodeWavFile();
        break;
    case WaveHeaderReader::FileError:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::FileError, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::FileError, 0);
        ABULEDU_LOG_DEBUG() << "  FileError, STOP";
        break;
    case WaveHeaderReader::RiffHeader_ID:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::RiffHeader_ID, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::RiffHeader_ID, 0);
        ABULEDU_LOG_DEBUG() << "  RiffHeader_ID, STOP";
        break;
    case WaveHeaderReader::RiffHeader_Type:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::RiffHeader_Type, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::RiffHeader_Type, 0);
        ABULEDU_LOG_DEBUG() << "  RiffHeader_Type, STOP";
        break;
    case WaveHeaderReader::Header_Size:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::Header_Size, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::Header_Size, 0);
        ABULEDU_LOG_DEBUG() << "  Header_Size, STOP";
        break;
    case WaveHeaderReader::WaveFMT_Size:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::WaveFMT_Size, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::WaveFMT_Size, 0);
        ABULEDU_LOG_DEBUG() << "  WaveFMT_Size, STOP";
        break;
    case WaveHeaderReader::PCMWaveFMT_Size:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::PCMWaveFMT_Size, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::PCMWaveFMT_Size, 0);
        ABULEDU_LOG_DEBUG() << "  PCMWaveFMT_Size, STOP";
        break;
    case WaveHeaderReader::WaveFMTExtr_Size:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::WaveFMTExtr_Size, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::WaveFMTExtr_Size, 0);
        ABULEDU_LOG_DEBUG() << "  WaveFMTExtr_Size, STOP";
        break;
    case WaveHeaderReader::ChannelsNotSupported:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::ChannelsNotSupported, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::ChannelsNotSupported, 0);
        ABULEDU_LOG_DEBUG() << "  ChannelsNotSupported, STOP";
        break;
    case WaveHeaderReader::SampleRateNotSupported:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::SampleRateNotSupported, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::SampleRateNotSupported, 0);
        ABULEDU_LOG_DEBUG() << "  SampleRateNotSupported, STOP";
        break;
    case WaveHeaderReader::BytesPerSampleNotSupported:
        emit signalWaveToOggStatus(EncoderNotAvailable, WaveHeaderReader::BytesPerSampleNotSupported, 0);
        emit signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::EncoderNotAvailable, AbulEduAudioV1::WaveHeaderReader::BytesPerSampleNotSupported, 0);
        ABULEDU_LOG_DEBUG() << "  BytesPerSampleNotSupported, STOP";
        break;
    default:
        ABULEDU_LOG_DEBUG() << "  Default Case ! : Status : " << status;
        break;
    }
}

void AbulEduAudioV1::WaveToOgg::slotWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::Status a, AbulEduAudioV1::WaveHeaderReader::Status b, const QString &c)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //signaux en double pour qu'on puisse répondre au pb des namespaces, cf https://redmine.ryxeo.com/issues/4609
    emit signalWaveToOggStatus(a, b, c );
    emit signalWaveToOggStatusPub(a, b, c );
}
