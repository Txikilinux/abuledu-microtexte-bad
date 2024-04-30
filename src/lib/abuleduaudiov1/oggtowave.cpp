#include "oggtowave.h"
#ifndef DEBUG_ABULEDUAUDIOV1
    #include "abuledudisableloggerv1.h"
#endif
/* Icham Sirat 20151026
 *
 * How it's work ?
 *
 *    AbulEduAudioV1::OggToWave oggEncoder;
 *    oggEncoder.processOggFile(QApplication::applicationDirPath() + "/test.ogg", "/tmp/test.wav");
 *
 *   No error -> the Wave file is "/tmp/test.wav"
 *   If Error -> you have a debug with the error string.
 *
 *
 *  @Warning NO DELETE THE COMMENTS !!
 *
 */

AbulEduAudioV1::OggToWave::OggToWave(QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;
    resetVariables();
    connect(this, SIGNAL(signalOggtoWaveError(OggToWave_Error)),this, SLOT(slotHandleOggToWaveError(OggToWave_Error)), Qt::UniqueConnection);
}

AbulEduAudioV1::OggToWave::~OggToWave()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;
    if(m_inFile)     free(m_inFile);
//    if(m_outFile)    free(m_outFile);
    if(m_vf)         free(m_vf);
    if(m_info)       free(m_info);
    if(m_pcm_buffer) free(m_pcm_buffer);
}

void AbulEduAudioV1::OggToWave::resetVariables()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;
    m_inFile        = NULL;
//    m_outFile       = NULL;
    m_vf            = NULL;
    m_info          = NULL;
    m_total_size    = 0;
    m_pcm_buffer   = NULL;
    m_buffer_size   = 0;
}

bool AbulEduAudioV1::OggToWave::processOggFile(const QString &fileInPath, const QString &fileOutPath)
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    /* Si appel en boucle */
    resetVariables();

    m_inFile = fopen(fileInPath.toLocal8Bit(), "rb");
    if (!m_inFile) {
        emit signalOggtoWaveError(Error_Open_Infile);
        return false;
    }

    //On ecris bien un fichier en sortie
    m_fileWriter.makeOutputFile(true);

#ifdef _WIN32
    _setmode( _fileno(m_inFile), _O_BINARY );
#endif

    /* Initialize an Ogg Vorbis reader instance, and obtain the source audio parameters. */
    m_vf = new OggVorbis_File();
    if (ov_open_callbacks(m_inFile, m_vf, NULL, 0, OV_CALLBACKS_NOCLOSE) != 0) {
        emit signalOggtoWaveError(Error_Open_Vorbis_CallBacks);
        return false;
    }

    m_info = ov_info(m_vf, -1);
    if (!m_info) {
        emit signalOggtoWaveError(Error_Vorbis_Info);
        return false;
    }

    /* Si c'est pas du mono ou du stéréo ... */
    if (m_info->channels != 1 && m_info->channels != 2) {
        emit signalOggtoWaveError(Error_Info_Channel);
        return false;
    }

    /* Read the PCM data into memory. */
    for (;;) {
        if (m_total_size >= m_buffer_size) {
            m_buffer_size += 1000000;
            char *new_buffer = static_cast<char *>(realloc(m_pcm_buffer, m_buffer_size));
            if (!new_buffer) {
                emit signalOggtoWaveError(Error_Out_Pcm_Buffer);
                return false;
            }
            m_pcm_buffer = new_buffer;
        }

        int bitstream_unused;
        const int32_t nread = ov_read(
            m_vf, (char *)(m_pcm_buffer + m_total_size), m_buffer_size - m_total_size,
            /*bigendianp*/ 0, /*word*/ 2, /*sgned*/ 1, &bitstream_unused
        );

        if (nread == 0 || nread == OV_EOF) {
            break;
        } else if (nread == OV_HOLE) {
            ABULEDU_LOG_WARN() << "Warning: Possible corrupt data at sample "
                               << m_total_size / (m_info->channels*2) << " continuing anyway";
        } else if (nread < 0) {
            emit signalOggtoWaveError(Error_Vorbis_Data);
            return false;
        } else {
            m_total_size += nread;
        }
    }

    QAudioFormat format;
    format.setChannelCount(m_info->channels);
    format.setSampleSize(16);
    format.setSampleRate(44100);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);

    QByteArray ba = QByteArray::fromRawData(reinterpret_cast<char*>(m_pcm_buffer),m_total_size*sizeof(char));
    QAudioBuffer buffer(ba,format,0);

    if (!m_fileWriter.isOpen() && !m_fileWriter.open(fileOutPath, format)) {
        ABULEDU_LOG_DEBUG() << "Decoder STOP : can't open file ..." << fileOutPath;
        return false;
    }
    m_fileWriter.write(buffer);

    if (!m_fileWriter.close()) {
        ABULEDU_LOG_DEBUG() << "Failed to finilize output file" << endl;
    }

    /* All done! */
    free(m_pcm_buffer);
    ov_clear(m_vf);
    fclose(m_inFile);    resetVariables();

    ABULEDU_LOG_DEBUG()<< "OGG => Wave OK" << fileOutPath.toLocal8Bit();
    return true;
}

void AbulEduAudioV1::OggToWave::slotHandleOggToWaveError(AbulEduAudioV1::OggToWave::OggToWave_Error e)
{
    switch (e) {
    case Error_Out_Pcm_Buffer:
        ABULEDU_LOG_ERROR() << "Erreur Out of memory PCM data ("<< m_buffer_size << "bytes)";
        free(m_pcm_buffer);
        break;
    case Error_Vorbis_Data:
        ABULEDU_LOG_ERROR() << "Erreur decompression Ogg Vorbis data";
        free(m_pcm_buffer);
        break;
    case Error_Wave_Header:
        ABULEDU_LOG_ERROR()<< "Erreur ecriture RIFF WAVE header";
        free(m_pcm_buffer);
        break;
    case Error_PCM_Data:
        ABULEDU_LOG_ERROR()<< "Erreur ecriture PCM data";
        free(m_pcm_buffer);
        break;
    case Error_Open_Infile:
        ABULEDU_LOG_ERROR() << "Erreur ouverture inFile";
        break;
    case Error_Open_Outfile:
        ABULEDU_LOG_ERROR() << "Erreur ouverture outFile";
        fclose(m_inFile);
        break;
    case Error_Open_Vorbis_CallBacks:
        ABULEDU_LOG_ERROR() << "Erreur ov_open_callbacks()";
//        fclose(m_outFile);
        break;
    case Error_Vorbis_Info:
        qDebug() << "ov_info() failed";
        ov_clear(m_vf);
        if(m_info) free(m_info);
        break;
    case Error_Info_Channel:
        qDebug() << "Erreur channel count";
        ov_clear(m_vf);
        if(m_info) free(m_info);
        break;
    }
}
