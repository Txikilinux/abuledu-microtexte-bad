/** @class AbulEduAudioDecoderV1
  * @file  abuledudecoderv1.h
  *
  * @mainpage   AbulEduAudioDecoderV1 \n
  *             Cette classe permet de décoder (décompresser) des fichiers musicaux (*.ogg, *.mp3).\n
  *             Elle utilise un QAudioDecoder.\n
  *             Ce QAudioDecoder est relié à la classe WaveFileWriter qui permet de gérer le format wav (fichier musical non compressé).\n
  *             Cette classe est une adaptation à la sauce "AbulEdu" d'un exemple QtMultimédia\n
  *
  * @author Copyright(C) 2012 Digia Plc and/or its subsidiary(-ies).
  * @author Icham Sirat <icham.sirat@ryxeo.com> 2013
  * @author 2014 Eric Seigne <eric.seigne@ryxeo.com>
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

#ifndef ABULEDUAUDIODECODERV1_H
#define ABULEDUAUDIODECODERV1_H

#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QAudioOutput>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QBuffer>
#include <QPointer>
#include "abuleduloggerv1.h"
#include "wavefilewriter.h"

/******************************************************************************
 *
 *                        CLASS AbulEduAudioDecoderV1
 *
 ******************************************************************************/
//using namespace AbulEduSingleton;

class AbulEduAudioDecoderV1 : public QObject/*: public AbulEduSingletonV1<AbulEduAudioDecoderV1>*/
{
    Q_OBJECT

//    friend class AbulEduSingletonV1<AbulEduAudioDecoderV1>;

public:

    enum SaveLocation {
        BUFFER = 0x0,
        FILE   = 0x2
    };

    AbulEduAudioDecoderV1();
    ~AbulEduAudioDecoderV1(){}

    void setSourceFilename(const QString &fileName);
    void setTargetFilename(const QString &fileName);
    void setOutputLocation(SaveLocation saveLocation);
    void start();
    void stop();
private:
    QString getTargetFileName(){ return m_targetFilename ;}

signals:
    void done(QPointer<QIODevice> = 0, qint64 duration = -1);

public slots:
    void bufferReady();
    void error(QAudioDecoder::Error error);
#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
    void stateChanged(QAudioDecoder::State newState);
#endif
    void finished();

private slots:
    void updateProgress();

private:
    QAudioDecoder m_decoder;

    QString m_sourceFilename;
    QString m_targetFilename;

    WaveFileWriter m_fileWriter;
    qreal m_progress;
};






#endif // ABULEDUAUDIODECODERV1_H
