/** Classe WaveHeaderReader : Gestion Entête Wave
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

#ifndef ABULEDUWAVEHEADERREADER_H
#define ABULEDUWAVEHEADERREADER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <vector>
#include <inttypes.h>
#include <stdint.h>
#include <math.h>

#include "abuleduloggerv1.h"
#ifndef DEBUG_ABULEDUAUDIOV1
    #include "abuledudisableloggerv1.h"
#endif

using namespace std;
// if your compiler does not have pshpack1.h and poppack.h, then
// use #pragma pack instead. It is important that these structures
// be byte-alignd!
//#include <pshpack1.h>

/* Cette directive permet d'éviter le rembourrage du compilateur
 * Evite le padding entre les membres d'une structure afin que sa taille soit exactement egale à la somme des tailles de ces membres
 * cf : http://melem.developpez.com/tutoriels/divers/raw-sockets/#LII-B Les contraintes d'alignement
 */
#ifdef _WIN32
/* We need the following two to set fileIn/fileOut to binary */
    #include <io.h>
    #include <fcntl.h>
/* pragma pack(1) sur windows ne fonctionne pas, donc inclusion de ça =) */
    #include <pshpack1.h>
#endif

#ifdef __unix
    #pragma pack(1)
#endif

namespace AbulEduAudioV1 {

#ifdef _WIN32
    typedef __int32 int32_t;
    typedef unsigned __int32 uint32_t;
    typedef __int16 int16_t;
    typedef unsigned __int8 uint8_t;
#endif
struct s_riff_hdr
{
    char id[4];
    uint32_t size;
    char type[4];
};

struct s_chunk_hdr
{
    char id[4];
    uint32_t size;
};

struct s_wavefmt
{
    unsigned short format_tag;
    unsigned short channels;
    uint32_t sample_rate;
    uint32_t avg_bytes_sec;
    unsigned short block_align;
};

struct s_wavefmtex
{
    s_wavefmt fmt;
    unsigned short bits_per_sample;
    unsigned short extra_size;
};

struct s_pcmwavefmt
{
    s_wavefmt fmt;
    unsigned short bits_per_sample;
};

const int CHANNELS_EXPECTED     = 2;
const int SAMPLE_RATE_EXPECTED  = 44100;
const int BYTES_SAMPLE_EXPECTED = 16;
//AVG BYTES SEC = 176400
//BLOCK ALIGN   = 4
//1 2 44100 176400 4
//bits_per_sample  s_pcmwavefmt= 16

#ifdef _WIN32
/* inclusion de poppack après les structs, sinon l'alignement de bits est faussé */
/* il faut que dans les debug : sizeof(s_pcmwavefmt) soit égale à 16 et non 20*/
    #include <poppack.h>
#endif

class WaveHeaderReader : public QObject
{
    Q_OBJECT

public:

    enum Status{
        FileOK                      = 0x0,
        FileError                   = 0x2,
        RiffHeader_ID               = 0x4,
        RiffHeader_Type             = 0x6,
        Header_Size                 = 0x8,
        WaveFMT_Size                = 0x10,
        PCMWaveFMT_Size             = 0x12,
        WaveFMTExtr_Size            = 0x14,
        ChannelsNotSupported        = 0x16,
        SampleRateNotSupported      = 0x18,
        BytesPerSampleNotSupported  = 0x20
    };
    Q_ENUMS(Status)

    struct WaveInformations{
        int formatTag ;
        int sizeRiffHeader;
        int channels;
        int sampleRate;
        int avgBytesSecond;
        int blockAlign;
        int bitsPerSample;
        int size;
        int paddedSize;
        int dataloop;
    };

    explicit WaveHeaderReader(QObject *parent = 0);

    WaveInformations getInformations() const { return informations;}

signals:
    void signalWaveHeaderReaderStatus(WaveHeaderReader::Status);

public slots:
    void slotWaveHeaderReaderReadWave(ifstream &file);

private:
    WaveInformations informations;
};

}

#endif // ABULEDUWAVEHEADERREADER_H
