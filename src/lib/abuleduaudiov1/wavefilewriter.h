/** @class WaveFileWriter
  * @file  wavefilewriter.h
  *
  * @mainpage   classe WaveFileWriter qui permet de gérer le format wav (fichier musical non compressé).\n
  *             Cette classe est une adaptation à la sauce "AbulEdu" d'un exemple QtMultimédia\n
  *
  * @author Copyright(C) 2012 Digia Plc and/or its subsidiary(-ies).
  * @author Icham Sirat <icham.sirat@ryxeo.com> 2013
  * @author 2015 Eric Seigne <eric.seigne@ryxeo.com>
  * @date 2013 12 18
  *
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

#ifndef WAVEFILEWRITERV1_H
#define WAVEFILEWRITERV1_H

#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QAudioOutput>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QBuffer>
#include <QPointer>
#include <QtCore/qendian.h>

#include "abuleduloggerv1.h"
#ifndef DEBUG_ABULEDUAUDIOV1
    #include "abuledudisableloggerv1.h"
#endif

struct Q_PACKED chunk
{
    char        id[4];
    quint32     size;
};

struct Q_PACKED RIFFHeader
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
};

struct Q_PACKED WAVEHeader
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
};

struct Q_PACKED DATAHeader
{
    chunk       descriptor;
};

struct Q_PACKED CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
    DATAHeader  data;
};
static const int HeaderLength = sizeof(CombinedHeader);

/******************************************************************************
 *
 *                        CLASS WaveFileWriter
 *
 ******************************************************************************/
class WaveFileWriter : public QObject
{
    Q_OBJECT

public:
    explicit WaveFileWriter(QObject *parent = 0);
    ~WaveFileWriter();

    bool open(const QString &fileName, const QAudioFormat &format);
    bool write(const QAudioBuffer &buffer);
    bool close();

    void makeOutputFile(bool yesNo){ m_makeFile = yesNo;}

    bool isOpen() const;

    const QPointer<QIODevice> getDecodedFile();

private:
    bool writeHeader(const QAudioFormat &format);
    bool writeDataLength();

    qint64 m_dataLength;
    bool m_makeFile;

    QFile m_file;
    QBuffer m_buffer;
    QAudioFormat m_format;
};

#endif // WAVEFILEWRITERV1_H
