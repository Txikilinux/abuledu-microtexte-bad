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

#include "waveheaderreader.h"

AbulEduAudioV1::WaveHeaderReader::WaveHeaderReader(/*const QString &waveFile,*/ QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    qRegisterMetaType< AbulEduAudioV1::WaveHeaderReader::Status >("AbulEduAudioV1::WaveHeaderReader::Status");

    /* Que faire return + prevention ;) */
    informations.formatTag = 0;
    informations.sizeRiffHeader = 0;
    informations.channels = 0;
    informations.sampleRate = 0;
    informations.avgBytesSecond = 0;
    informations.blockAlign= 0;
    informations.bitsPerSample = 0;
}

void AbulEduAudioV1::WaveHeaderReader::slotWaveHeaderReaderReadWave(ifstream &file)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    s_riff_hdr riff_hdr;
    s_chunk_hdr chunk_hdr;
    uint32_t padded_size;
    vector<uint8_t> fmt_data;
    s_wavefmt *fmt = NULL;

    file.read(reinterpret_cast<char*>(&riff_hdr), sizeof(riff_hdr));

    /* Condition de sortie qui empêchait "des fois" le transcodage du wav en ogg de se faire */
    if (!file) {
        emit signalWaveHeaderReaderStatus(FileError); /* Problème Fichier */
        return ;
    }

    if (memcmp(riff_hdr.id, "RIFF", 4) != 0){
        emit signalWaveHeaderReaderStatus(RiffHeader_ID); /* Probleme entete non RIFF */
        return ;
    }

    informations.sizeRiffHeader = riff_hdr.size;
    ABULEDU_LOG_DEBUG() << "SIZE : " << riff_hdr.size;
    //    ABULEDU_LOG_DEBUG() << "TYPE : " << type.toLocal8Bit();

    if (memcmp(riff_hdr.type, "WAVE", 4) != 0){
        emit signalWaveHeaderReaderStatus(RiffHeader_Type); /* Probleme entete non WAVE */
        return;
    }

    // chunks can be in any order!
    // there is no guarantee that "fmt" is the first chunk.
    // there is no guarantee that "fmt" is immediately followed by "data".
    // There can be other chunks present!

    /* Au premier tour de boucle, le informations.dataloop est bon, sinon après c'est égal à la taille du fichier */
    /* informations.dataloop correspond au nombre de tour de boucles nécessaires à l'encodage (pour affichage progression ;p) */
    int compteur = 0;
    do
    {
        file.read(reinterpret_cast<char*>(&chunk_hdr), sizeof(chunk_hdr));

        /* Condition de sortie qui empêchait "des fois" le transcodage du wav en ogg de se faire */
        if(!file){
            emit signalWaveHeaderReaderStatus(FileError); /* Probleme fichier */
            return;
        }

        padded_size = ((chunk_hdr.size + 1) & ~1);

        informations.size       = chunk_hdr.size;
        informations.paddedSize = padded_size;
        if(compteur == 0)
            informations.dataloop = informations.size;


        if (memcmp(chunk_hdr.id, "fmt ", 4) == 0)
        {
            if (chunk_hdr.size < sizeof(s_wavefmt)){
                emit signalWaveHeaderReaderStatus(WaveFMT_Size); /* Probleme chunk wavefmt */
                return;
            }

            fmt_data.resize(padded_size);

            file.read(reinterpret_cast<char*>(&fmt_data[0]), padded_size);
            if (!file){
                emit signalWaveHeaderReaderStatus(FileError); /* Probleme fichier */
                return ;
            }

            fmt = reinterpret_cast<s_wavefmt*>(&fmt_data[0]);

            informations.formatTag  = fmt->format_tag;
            informations.channels   = fmt->channels;
            informations.sampleRate = fmt->sample_rate ;
            informations.avgBytesSecond = fmt->avg_bytes_sec;
            informations.blockAlign     = fmt->block_align;

            if(informations.channels != CHANNELS_EXPECTED){
                emit signalWaveHeaderReaderStatus(ChannelsNotSupported);
                return;
            }

            if(informations.sampleRate != SAMPLE_RATE_EXPECTED){
                emit signalWaveHeaderReaderStatus(SampleRateNotSupported);
                return;
            }

            if (informations.formatTag == 1) // PCM
            {
                if (chunk_hdr.size < sizeof(s_pcmwavefmt)){
                    emit signalWaveHeaderReaderStatus(PCMWaveFMT_Size); /* Probleme chunk pcmwavefmt */
                    return;
                }

                s_pcmwavefmt *pcm_fmt = reinterpret_cast<s_pcmwavefmt*>(fmt);
                informations.bitsPerSample = pcm_fmt->bits_per_sample;
                ABULEDU_LOG_DEBUG() << "  bytes sample (1) : " << informations.bitsPerSample;
                if(informations.bitsPerSample != BYTES_SAMPLE_EXPECTED){
                    emit signalWaveHeaderReaderStatus(BytesPerSampleNotSupported);
                    return;
                }
            }
            else
            {
                if (chunk_hdr.size < sizeof(s_wavefmtex)){
                    emit signalWaveHeaderReaderStatus(WaveFMTExtr_Size); /* Probleme chunk pcmwavefmt */
                    return;
                }

                s_wavefmtex *fmt_ex = reinterpret_cast<s_wavefmtex*>(fmt);

                informations.bitsPerSample = fmt_ex->bits_per_sample;
                ABULEDU_LOG_DEBUG() << "  bytes sample (2) : " << informations.bitsPerSample;
                if(informations.bitsPerSample != BYTES_SAMPLE_EXPECTED){
                    emit signalWaveHeaderReaderStatus(BytesPerSampleNotSupported);
                    return;
                }

                if (fmt_ex->extra_size != 0)
                {
                    if (chunk_hdr.size < (sizeof(s_wavefmtex) + fmt_ex->extra_size)){
                        emit signalWaveHeaderReaderStatus(WaveFMTExtr_Size); /* Probleme chunk wavefmtex */
                        return;
                    }

                    uint8_t *extra_data = reinterpret_cast<uint8_t*>(fmt_ex + 1);
                    Q_UNUSED(extra_data)
                    // use extra_data, up to extra_size bytes, as needed...
                }
            }
        }
        else if (memcmp(chunk_hdr.id, "data", 4) == 0)
        {
            // process chunk data, according to fmt, as needed...
            file.ignore(padded_size);
            if (!file){
                emit signalWaveHeaderReaderStatus(FileError); /* Probleme fichier*/
                return;
            }
        }
        else
        {
            // process other chunks as needed...
            file.ignore(padded_size);

            /* Condition de sortie qui empêchait "des fois" le transcodage du wav en ogg de se faire */
            if (!file){
                emit signalWaveHeaderReaderStatus(FileError); /* Probleme fichier */
                return;
            }
        }
        compteur += 1;
    }
    while (!file.eof());

    /* Si on est arrivé jusqu'ici, c'est que le fichier est conforme */
   if(file.is_open()){
       ABULEDU_LOG_DEBUG() << "  Fermeture du stream";
       file.sync();
       file.close();
       if(file.is_open()) {
           ABULEDU_LOG_DEBUG() << "    Erreur de fermeture du stream";
           ABULEDU_LOG_DEBUG() << "  suite";
       }
       else {
           ABULEDU_LOG_DEBUG() << "    fermeture stream ok";
       }
   }
    emit signalWaveHeaderReaderStatus(FileOK);
}

