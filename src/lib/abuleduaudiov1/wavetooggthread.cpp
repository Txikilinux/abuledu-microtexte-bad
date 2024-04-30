#include "wavetooggthread.h"
#include "wavetoogg.h"

WaveToOggThread::WaveToOggThread() :
    QThread()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //qRegisterMetaType< AbulEduAudioV1::WaveToOgg::Status >("AbulEduAudioV1::WaveToOgg::Status"); // Permet de passer des AbulEduAudioV1::WaveToOgg::Status dans les signaux
    //qRegisterMetaType< AbulEduAudioV1::WaveHeaderReader::Status >("AbulEduAudioV1::WaveHeaderReader::Status"); // Permet de passer des AbulEduAudioV1::WaveHeaderReader::Status dans les signaux
}

WaveToOggThread::~WaveToOggThread()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void WaveToOggThread::run()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ogg_stream_state os; /* take physical pages, weld into a logical stream of packets */
    ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
    ogg_packet       op; /* one raw packet of data for decode */
    vorbis_info      vi; /* struct that stores all the static vorbis bitstream settings */
    vorbis_comment   vc; /* struct that stores all the user comments */
    vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
    vorbis_block     vb; /* local working space for packet->PCM decode */

    int eos=0;
    int ret =0;
    int i = 0;
    int founddata =0;

    /* we cheat on the WAV header; we just bypass 44 bytes (simplest WAV header is 44 bytes)
     *and assume that the data is 44.1khz, stereo, 16 bit little endian pcm samples. This is just an example, after all. */
    FILE *file;
    file=fopen(m_fileInPath.toLocal8Bit(),"r");
    if(file==NULL){
        qDebug() << "NULL";
        emit signalWaveToOggThreadStatus(AbulEduAudioV1::WaveToOgg::FileToConvertNotOpenable, AbulEduAudioV1::WaveHeaderReader::FileError, m_fileInPath);
        return;
    }

    QFile fout(m_fileOutPath);
    if(fout.exists()) {
        if(fout.remove()) {
            ABULEDU_LOG_DEBUG() << "  fichier supprime il n'existe plus : " << m_fileOutPath.toLocal8Bit();
        }
        else {
            ABULEDU_LOG_DEBUG() << "  impossible de supprimer le fichier : " << m_fileOutPath.toLocal8Bit();
        }
    }
    FILE *fileOut;
    fileOut=fopen(m_fileOutPath.toLocal8Bit(),"wb");
    if(fileOut==NULL){
        ABULEDU_LOG_DEBUG() << "  " << "impossible d'ouvir le fichier en ecriture : " << m_fileOutPath.toLocal8Bit();
        emit signalWaveToOggThreadStatus(AbulEduAudioV1::WaveToOgg::FileToWriteNotOpenable, AbulEduAudioV1::WaveHeaderReader::FileError, m_fileOutPath);
        return;
    }

#ifdef _WIN32 /* We need to set fileIn/fileOut to binary mode. Damn windows. */
    /* if we were reading/writing a file, it would also need to in
       binary mode, eg, fopen("file.wav","wb");
     Beware the evil ifdef. We avoid these where we can, but this one we
       cannot. Don't add any more, you'll probably go to hell if you do. */
    _setmode( _fileno( file ), _O_BINARY );
    _setmode( _fileno( fileOut ), _O_BINARY );
#endif

    /* Pour avoir la taille d'un FILE * en C */
    struct stat buf;
    fstat(fileno(file), &buf);
    int totalSize = buf.st_size;
    //    qDebug() << "SIZE : " << totalSize;

    //    char **ptr=ov_comment(&vf,-1)->user_comments;
    //    vorbis_info *vi=ov_info(&vf,-1);
    //    while(*ptr){
    //        fprintf(stderr,"%s\n",*ptr);
    //        ++ptr;
    //    }
    //    qDebug() << QString("Encoded by: %1").arg(ov_comment(&vf,-1)->vendor);

    readbuffer[0] = '\0';
    float percent = 0.0;
    for (i=0, founddata=0; i<30 && ! feof(file) && ! ferror(file); i++)
    {
        if ( ! strncmp((char*)readbuffer, "da", 2) ){
            founddata = 1;
            break;
        }
        percent = (float(i)/ float(80)) * 100;
        emit signalWaveToOggThreadProcessPercent(percent);
    }

    /* On est a 96%, il manque 4% qui se font ici */

    /********** Encode setup ************/
    vorbis_info_init(&vi);

    /* choose an encoding mode.  A few possibilities commented out, one actually used: */
    /*********************************************************************
     Encoding using a VBR quality mode.  The usable range is -.1
     (lowest quality, smallest file) to 1. (highest quality, largest file).
     Example quality mode .4: 44kHz stereo coupled, roughly 128kbps VBR
     ret = vorbis_encode_init_vbr(&vi,2,44100,.4);

     ---------------------------------------------------------------------
     Encoding using an average bitrate mode (ABR).
     example: 44kHz stereo coupled, average 128kbps VBR
     ret = vorbis_encode_init(&vi,2,44100,-1,128000,-1);

     ---------------------------------------------------------------------
     Encode using a quality mode, but select that quality mode by asking for
     an approximate bitrate.  This is not ABR, it is true VBR, but selected
     using the bitrate interface, and then turning bitrate management off:
     ret = ( vorbis_encode_setup_managed(&vi,2,44100,-1,128000,-1) ||
             vorbis_encode_ctl(&vi,OV_ECTL_RATEMANAGE2_SET,NULL) ||
             vorbis_encode_setup_init(&vi));

     *********************************************************************/

    ret=vorbis_encode_init_vbr(&vi, 2,44100,0.1);
    ABULEDU_LOG_DEBUG() << QString("Bitstream is %1 : %2 channel, %3Hz").arg(vi.bitrate_nominal).arg(vi.channels).arg(vi.rate);

    percent = 40.00;
    emit signalWaveToOggThreadProcessPercent(percent);

    /* do not continue if setup failed; this can happen if we ask for a
       mode that libVorbis does not support (eg, too low a bitrate, etc,
       will return 'OV_EIMPL') */
    if(ret){
        emit signalWaveToOggThreadStatus(AbulEduAudioV1::WaveToOgg::ChoosenFormatNotSupported, AbulEduAudioV1::WaveHeaderReader::FileError, 0);
        return;
    }

    /* Gestion TAGS*/
    vorbis_comment_init(&vc);
    vorbis_comment_add_tag(&vc,"ENCODER",m_tagEncoder.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "TITLE", m_tagTitle.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "VERSION", m_tagVersion.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "ALBUM", m_tagAlbum.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "TRACKNUMBER", m_tagTrackNumber.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "ARTIST", m_tagArtist.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "PERFORMER", m_tagPerformer.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "COPYRIGHT", m_tagCopyright.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "LICENCE", m_tagLicence.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "ORGANIZATION", m_tagOrganization.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "DESCRIPTION", m_tagDescription.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "GENRE", m_tagGenre.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "DATE", m_tagDate.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "LOCATION", m_tagLocation.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "CONTACT", m_tagContact.toLocal8Bit().constData());
    vorbis_comment_add_tag(&vc, "ISRC", m_tagISRCNumber.toLocal8Bit().constData());

    /* set up the analysis state and auxiliary encoding storage */
    vorbis_analysis_init(&vd,&vi);
    vorbis_block_init(&vd,&vb);

    /* set up our packet->stream encoder */
    /* pick a random serial number; that way we can more likely build chained streams just by concatenation */
    srand(time(NULL));
    ogg_stream_init(&os,rand());

    percent = 50.00;
    emit signalWaveToOggThreadProcessPercent(percent);

    /* Vorbis streams begin with three headers; the initial header (with
       most of the codec setup parameters) which is mandated by the Ogg
       bitstream spec.  The second header holds any comment fields.  The
       third header holds the bitstream codebook.  We merely need to
       make the headers, then pass them to libvorbis one at a time;
       libvorbis handles the additional Ogg bitstream constraints */
    {
        ogg_packet header;
        ogg_packet header_comm;
        ogg_packet header_code;

        vorbis_analysis_headerout(&vd,&vc,&header,&header_comm,&header_code);
        ogg_stream_packetin(&os,&header); /* automatically placed in its own page */
        ogg_stream_packetin(&os,&header_comm);
        ogg_stream_packetin(&os,&header_code);

        /* This ensures the actual audio data will start on a new page, as per spec */
        while(!eos){
            int result=ogg_stream_flush(&os,&og);
            if(result==0)break;
            fwrite(og.header,1,og.header_len,fileOut);
            fwrite(og.body,1,og.body_len,fileOut);
        }
    }

    percent = 60.00;
    emit signalWaveToOggThreadProcessPercent(percent);

    long samplesdone = 0;
    while(!eos){
        // long index;
        long bytes=fread(readbuffer,1,READ_MAX*4,file); /* stereo hardwired here */


        if(bytes==0){
            /* end of file.  this can be done implicitly in the mainline,
           but it's easier to see here in non-clever fashion.
           Tell the library we're at end of stream so that it can handle
           the last frame and mark end of stream in the output properly */
            vorbis_analysis_wrote(&vd,0);

        }else{
            /* data to encode */
            /* expose the buffer to submit data */
            float **buffer=vorbis_analysis_buffer(&vd,READ_MAX);

            //C'est ici qu'il faudrait calculer la progression ...
            samplesdone += bytes;
            percent = (samplesdone*100)/totalSize;
            emit signalWaveToOggThreadProcessPercent(percent);
            //qDebug() << "on en est à " << samplesdone << " sur " << totalSize << " soit " << percent;

            /* uninterleave samples */
            for(i=0;i<bytes/4;i++){
                buffer[0][i]=((readbuffer[i*4+1]<<8)|
                        (0x00ff&(int)readbuffer[i*4]))/32768.f;
                buffer[1][i]=((readbuffer[i*4+3]<<8)|
                        (0x00ff&(int)readbuffer[i*4+2]))/32768.f;
            }

            /* tell the library how much we actually submitted */
            vorbis_analysis_wrote(&vd,i);

        }

        /* vorbis does some data preanalysis, then divvies up blocks for
         more involved (potentially parallel) processing.  Get a single
         block for encoding now */
        while(vorbis_analysis_blockout(&vd,&vb)==1){
            /* analysis, assume we want to use bitrate management */
            vorbis_analysis(&vb,NULL);
            vorbis_bitrate_addblock(&vb);

            while(vorbis_bitrate_flushpacket(&vd,&op)){

                /* weld the packet into the bitstream */
                ogg_stream_packetin(&os,&op);

                /* write out pages (if any) */
                while(!eos){
                    int result=ogg_stream_pageout(&os,&og);
                    if(result==0) break;
                    fwrite(og.header,1,og.header_len,fileOut);
                    fwrite(og.body,1,og.body_len,fileOut);
                    /* this could be set above, but for illustrative purposes, I do it here (to show that vorbis does know where the stream ends) */
                    if(ogg_page_eos(&og)) eos=1;
                }
            }
        }
    }

    /* clean up and exit.  vorbis_info_clear() must be called last */
    ogg_stream_clear(&os);
    vorbis_block_clear(&vb);
    vorbis_dsp_clear(&vd);
    vorbis_comment_clear(&vc);
    vorbis_info_clear(&vi);

    /* ogg_page and ogg_packet structs always point to storage in libvorbis. They're never freed or manipulated directly */
    percent = 100;
    emit signalWaveToOggThreadProcessPercent(percent);

    fflush(fileOut);
    /* Fermeture du fichier */
    if(fclose(fileOut) == 0){
        ABULEDU_LOG_DEBUG() << "  Fermeture fichier Sortie ok";
    }
    else {
        ABULEDU_LOG_DEBUG() << "  Fermeture fichier Sortie err" ;
    }
    if(fclose(file) == 0){
        ABULEDU_LOG_DEBUG() << "  Fermeture fichier Entree ok" ;
        /* Suppression fichier en entree */
        if(!QFile(m_fileInPath.toLocal8Bit()).remove()){
            ABULEDU_LOG_DEBUG() << "  Suppression Fichier en Entree impossible";
        }
        else{
            ABULEDU_LOG_DEBUG() << "  Suppression fichier en entree ok" ;
        }
        emit signalWaveToOggThreadStatus(AbulEduAudioV1::WaveToOgg::ProcessOK, AbulEduAudioV1::WaveHeaderReader::FileOK, m_fileOutPath);
    }
    else {
        /** @todo Emit ERROR */
        ABULEDU_LOG_DEBUG() << "  Impossible de fermer le fichier";
    }
    qDebug() << "Finish";
}

void WaveToOggThread::setFiles(const QString &fileInPath, const QString &fileOutPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_fileInPath = fileInPath;
    m_fileOutPath = fileOutPath;
}

void WaveToOggThread::setMetaData(const QString &tagTitle, const QString &tagEncoder, const QString &tagVersion, const QString &tagAlbum, const QString &tagTrackNumber, const QString &tagArtist, const QString &tagPerformer, const QString &tagCopyright, const QString &tagLicence, const QString &tagOrganization, const QString &tagDescription, const QString &tagGenre, const QString &tagDate, const QString &tagLocation, const QString &tagContact, const QString &tagISRCNumber)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_tagTitle        = tagTitle;
    m_tagEncoder      = tagEncoder;
    m_tagVersion      = tagVersion;
    m_tagAlbum        = tagAlbum;
    m_tagTrackNumber  = tagTrackNumber;
    m_tagArtist       = tagArtist;
    m_tagPerformer    = tagPerformer;
    m_tagCopyright    = tagCopyright;
    m_tagLicence      = tagLicence;
    m_tagOrganization = tagOrganization;
    m_tagDescription  = tagDescription;
    m_tagGenre        = tagGenre;
    m_tagDate         = tagDate;
    m_tagLocation     = tagLocation;
    m_tagContact      = tagContact;
    m_tagISRCNumber   = tagISRCNumber;
}
