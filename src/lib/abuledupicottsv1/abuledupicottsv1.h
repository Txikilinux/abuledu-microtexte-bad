/**
  * Classe
  * @author 2011 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2014 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  */
/*
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

#ifndef ABULEDUPICOTTSV1_H
#define ABULEDUPICOTTSV1_H

// Get declaration for f(int i, char c, float x)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abuleduloggerv1.h"

/*
  quand on aura trouve le moyen de compiler avec les pico*.h officiels
  qui sont en "c pur" on supprimera les .h livres avec cette lib et on
  passera sur les include ci-dessous
  #include <picoapi.h>
  #include <picoapid.h>
  #include <picoos.h>
*/
#include "pico/picoapi.h"
#include "pico/picoapid.h"
//#include "pico/picoos.h"

#include <QTemporaryFile>
#include <QSharedPointer>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QApplication>
#if QT_VERSION < 0x050000
  #include <phonon>
#else
  #include <QMediaPlayer>
#endif
#include <QMetaEnum>

class AbulEduPicottsV1: public QObject
{
    Q_OBJECT

public:

    /* @link normes ISO : http://fr.wikipedia.org/wiki/Liste_des_codes_ISO_639-1 */
    Q_ENUMS(AbulEduLang)
    enum AbulEduLang { None = -1,us = 0, en, de, es, fr, it};

    AbulEduPicottsV1(AbulEduLang lang, QObject *parent = 0, QMediaPlayer *player = 0);

    virtual ~AbulEduPicottsV1();

    QMediaPlayer *abePicottsGetAudioPlayer() const{ return m_audioPlayer;}
    inline AbulEduLang abePicottsGetlangIndex() const{return m_langIndex;}
    inline void abePicottsSetLangIndex(AbulEduLang langIndex){m_langIndex = langIndex;}
    /** Fonction de correspondance entre les AbulEduLang et les codes ISO 639-1
      * Elle peut servir dans les projets où on utilise directement AbulEduPicottsV1 sans AbulEduMultiMediaV1 */
    static AbulEduLang abePicottsGetAbulEduLangFromIso6391(QString codeIso6391);

    ///
    /// \brief Getteur de la vitesse de lecture
    /// \return la vitesse entre 20 et 500 (la vitesse normale est 100)
    ///
    int abePicottsGetSpeedLevel() const;

    ///
    /// \brief Setteur de la vitesse de lecture
    /// \param speedLevel la vitesse entre 20 et 500 (la vitesse normale est 100)
    ///
    void abePicottsSetSpeedLevel(int speedLevel);

private:
#if QT_VERSION < 0x050000
    Phonon::MediaObject *m_mediaObject;             /** La sortie audio, on passe par phonon */
    Phonon::AudioOutput *m_audioOutput;
#else
    QMediaPlayer        *m_audioPlayer;
#endif
    AbulEduLang          m_langIndex;                     /** numero de la langue a utiliser cf constructeur */

#ifdef Q_OS_ANDROID
    QFile               *m_fileTemp;
#else
    QTemporaryFile      *m_fileTemp;
#endif
    int m_speedLevel;

public slots:
    void abePicoPlay(const QString &texte);
    void abePicoPause();
    void abePicoResume();
    void abePicoStop();
    void slotAbePicoSetLang(const QString& );

private slots:
    void play_end();
#if QT_VERSION >= 0x050000
    void slotMediaStatusChanged(const QMediaPlayer::MediaStatus &status);
#endif
signals:
    void beginOfPlay();
    void endOfPlay();
};

#endif // ABULEDUPICOTTSV1_H
