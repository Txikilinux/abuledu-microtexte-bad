#ifndef OGGTOWAVE_H
#define OGGTOWAVE_H

#include <QObject>
#include <QDataStream>
#include <QAudioFormat>
#include <QFile>
/* Les librairies qu'il faut pour encoder */
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include "wavefilewriter.h"
#include <qaudiobuffer.h>

#ifdef Q_OS_WIN /* We need the following two to set fileIn/fileOut to binary */
    #include <io.h>
    #include <fcntl.h>
    #include <stdint.h>
#endif

#include "abuleduloggerv1.h"

namespace AbulEduAudioV1 {

class OggToWave : public QObject
{
    Q_OBJECT

    typedef unsigned char      uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned int       uint32_t;

    char pcmout[4096];
    FILE *m_inFile;
    WaveFileWriter m_fileWriter;
    OggVorbis_File *m_vf;
    vorbis_info *m_info;
    uint32_t m_total_size;
    char *m_pcm_buffer;
    uint32_t m_buffer_size;
    void resetVariables();

public:
    enum OggToWave_Error {
        Error_Open_Infile           = 0x0,
        Error_Open_Outfile          = 0x2,
        Error_Open_Vorbis_CallBacks = 0x4,
        Error_Info_Channel          = 0x6,
        // Error_Close_Infile          = 0x8,
        // Error_Close_Outfile         = 0x10,
        Error_Vorbis_Info           = 0x8,
        Error_Out_Pcm_Buffer        = 0x10,
        Error_Vorbis_Data           = 0x12,
        Error_Wave_Header           = 0x14,
        Error_PCM_Data              = 0x16
    };
    Q_ENUMS(OggToWave_Error)

    explicit OggToWave(QObject *parent = 0);
    ~OggToWave();
    bool processOggFile(const QString &fileInPath, const QString &fileOutPath);

signals:
    void signalOggtoWaveError(OggToWave_Error);

private slots:
    void slotHandleOggToWaveError(OggToWave_Error);
};

}
#endif // OGGTOWAVE_H
