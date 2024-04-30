/***************************************************************************
                          soundsourceoggvorbis.h  -  ogg vorbis decoder
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

#ifndef SOUNDSOURCEOGGVORBIS_H
#define SOUNDSOURCEOGGVORBIS_H

#include <QString>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "abuleduloggerv1.h"

namespace AbulEduAudioV1 {

class OggVorbisFileInfo {

public:
    OggVorbisFileInfo(const QString &qFilename);
    ~OggVorbisFileInfo();
    double abeGetDuration();
    int abeGetChannels();
    QString abeGetTitle();
    QString abeGetArtist();
    QString abeGetAlbum();
    QString abeGetYear();
    QString abeGetGenre();
    QString abeGetTrack();
    QString abeGetComment();
    float abeGetBPM();

private:
    OggVorbis_File  m_vf;
    QString         m_qFilename;
    int             m_channels;
    unsigned long   m_filelength;
    int             m_current_section;
    long            m_iSampleRate;
    double          m_duration;

    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_year;
    QString m_genre;
    QString m_track;
    QString m_comment;
    float   m_bpm;

    long seek(long);
    inline long unsigned length();
    int parseHeader();
    static QList<QString> supportedFileExtensions();
    int open();
};
}
#endif
