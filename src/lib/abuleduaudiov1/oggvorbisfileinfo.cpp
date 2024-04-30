/***************************************************************************
                          soundsourceoggvorbis.cpp  -  ogg vorbis decoder
                             -------------------
    copyright            : (C) 2003 by Svein Magne Bang
    email                :
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "oggvorbisfileinfo.h"
#ifndef DEBUG_ABULEDUAUDIOV1
    #include "abuledudisableloggerv1.h"
#endif

#ifdef __WINDOWS__
#include <io.h>
#include <fcntl.h>
#endif

#ifdef __APPLE__
#ifdef __i386
#define OV_ENDIAN_ARG 0
#else
#define OV_ENDIAN_ARG 1
#endif
#endif

#ifdef __LINUX__
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define OV_ENDIAN_ARG 0
#else
#define OV_ENDIAN_ARG 1
#endif
#else
#define OV_ENDIAN_ARG 0
#endif
/*
   Class for reading Ogg Vorbis
 */

AbulEduAudioV1::OggVorbisFileInfo::OggVorbisFileInfo(const QString &qFilename):
    m_vf(OggVorbis_File()),
    m_qFilename(qFilename),
    m_channels(0),
    m_filelength(0),
    m_current_section(0),
    m_iSampleRate(0),
    m_duration(0),
    m_title(QString()),
    m_artist(QString()),
    m_album(QString()),
    m_year(QString()),
    m_genre(QString()),
    m_track(QString()),
    m_comment(QString()),
    m_bpm(0.0)

{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

AbulEduAudioV1::OggVorbisFileInfo::~OggVorbisFileInfo()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_filelength > 0){
        ov_clear(&m_vf);
    }
}

int AbulEduAudioV1::OggVorbisFileInfo::open()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QByteArray qBAFilename = m_qFilename.toUtf8();

#ifdef __WINDOWS__
    if(ov_fopen(qBAFilename.data(), &m_vf) < 0) {
        qDebug() << "oggvorbis: Input does not appear to be an Ogg bitstream.";
        m_filelength = 0;
        return -1;
    }
#else
    FILE *vorbisfile =  fopen(qBAFilename.data(), "r");

    if (!vorbisfile) {
        ABULEDU_LOG_ERROR() << "oggvorbis: cannot open" << m_qFilename;
        return -1;
    }

    if(ov_open(vorbisfile, &m_vf, NULL, 0) < 0) {
        ABULEDU_LOG_ERROR() << "oggvorbis: Input does not appear to be an Ogg bitstream.";
        m_filelength = 0;
        return -2;
    }
#endif

    // ov_pcm_total returns the total number of frames in the ogg file. The
    // frame is the channel-independent measure of samples. The total samples in
    // the file is channels * ov_pcm_total. rryan 7/2009 I verified this by
    // hand. a 30 second long 48khz mono ogg and a 48khz stereo ogg both report
    // 1440000 for ov_pcm_total.
    ogg_int64_t ret = ov_pcm_total(&m_vf, -1);

    if (ret >= 0) {
        // We pretend that the file is stereo to the rest of the world.
        m_filelength = ret * 2;
    }
    else //error
    {
        if (ret == OV_EINVAL) {
            //The file is not seekable. Not sure if any action is needed.
        }
    }

    return 1;
}

/*
   seek to <filepos>
 */

long AbulEduAudioV1::OggVorbisFileInfo::seek(long filepos)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    // In our speak, filepos is a sample in the file abstraction (i.e. it's
    // stereo no matter what). filepos/2 is the frame we want to seek to.
    Q_ASSERT(filepos%2==0);

    if (ov_seekable(&m_vf)){
        if(ov_pcm_seek(&m_vf, filepos/2) != 0) {
            // This is totally common (i.e. you're at EOF). Let's not leave this
            // qDebug on.

            // qDebug() << "ogg vorbis: Seek ERR on seekable.";
        }

        // Even if an error occured, return them the current position because
        // that's what we promised. (Double it because ov_pcm_tell returns
        // frames and we pretend to the world that everything is stereo)
        return ov_pcm_tell(&m_vf) * 2;
    } else{
        ABULEDU_LOG_ERROR() << "ogg vorbis: Seek ERR.";
        return 0;
    }
}

/*
   Parse the the file to get metadata
 */
int AbulEduAudioV1::OggVorbisFileInfo::parseHeader()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    vorbis_comment *comment = NULL;

    this->open();

    comment = ov_comment(&m_vf, -1);
    if (comment == NULL) {
        ABULEDU_LOG_ERROR() << "oggvorbis: fatal error reading file.";
        ov_clear(&m_vf);
        return -3;
    }

    //precache
    const char* title_p   = vorbis_comment_query(comment, (char*)"title", 0); //the char* cast is to shut up the compiler; libvorbis should take `const char*` here but I don't expect us to get them to change that -kousu 2009/02
    const char* artist_p  = vorbis_comment_query(comment, (char*)"artist", 0);
    const char* bpm_p     = vorbis_comment_query(comment, (char*)"TBPM", 0);
    const char* album_p   = vorbis_comment_query(comment, (char*)"album", 0);
    const char* year_p    = vorbis_comment_query(comment, (char*)"date", 0);
    const char* genre_p   = vorbis_comment_query(comment, (char*)"genre", 0);
    const char* track_p   = vorbis_comment_query(comment, (char*)"tracknumber", 0);
    const char* comment_p = vorbis_comment_query(comment, (char*)"description", 0);

    if (title_p)   m_title = title_p;
    if (artist_p)  m_artist = artist_p;
    if (album_p)   m_album = album_p;
    if (year_p)    m_year = year_p;
    if (genre_p)   m_genre = genre_p;
    if (track_p)   m_track = track_p;
    if (comment_p) m_comment = comment_p;
    if (bpm_p)     m_bpm = atof(bpm_p);
    ABULEDU_LOG_DEBUG() << m_title << m_artist << m_album << m_year << m_genre << m_track << m_comment << m_bpm;

    m_duration = ov_time_total(&m_vf, -1);
    if (m_duration == OV_EINVAL) {
        ov_clear(&m_vf);    //close on return !
        return -4;
    }
    //    this->setBitrate(ov_bitrate(&vf, -1)/1000);

    vorbis_info *vi=ov_info(&m_vf,-1);
    if (!vi)
    {
        ABULEDU_LOG_ERROR() << "oggvorbis::parseHeader : fatal error reading file.";
        ov_clear(&m_vf);
        return -5;
    }

    m_channels = vi->channels;
    m_iSampleRate = vi->rate;

    if(m_channels > 2){
        ABULEDU_LOG_DEBUG() << "oggvorbis: No support for more than 2 channels!";
        ov_clear(&m_vf);
        m_filelength = 0;
        return -3;
    }

    ov_clear(&m_vf);
    return 1;
}

/*
   Return the length of the file in samples.
 */
inline long unsigned AbulEduAudioV1::OggVorbisFileInfo::length()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_filelength == 0 && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_filelength;
}

QList<QString> AbulEduAudioV1::OggVorbisFileInfo::supportedFileExtensions()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QList<QString> list;
    list.push_back("ogg");
    return list;
}

double AbulEduAudioV1::OggVorbisFileInfo::abeGetDuration()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //re-Read headers, m_duration = 0 dans le constructeur
    if(m_duration == 0 && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_duration;
}

int AbulEduAudioV1::OggVorbisFileInfo::abeGetChannels()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_channels == 0 && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_channels;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetTitle()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_title.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_title;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetArtist()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_artist.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_artist;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetAlbum()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_album.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_album;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetYear()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_year.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_year;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetGenre()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_genre.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_genre;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetTrack()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_track.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_track;
}

QString AbulEduAudioV1::OggVorbisFileInfo::abeGetComment()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_comment.isEmpty() && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_comment;
}

float AbulEduAudioV1::OggVorbisFileInfo::abeGetBPM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_bpm == 0 && !m_qFilename.isEmpty()) {
        parseHeader();
    }
    return m_bpm;
}
