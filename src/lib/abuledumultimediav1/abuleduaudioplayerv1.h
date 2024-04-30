/** Classe AbulEduAudioPlayerV1
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 3 of the License, or
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

#ifndef ABULEDUAUDIOPLAYERV1_H
#define ABULEDUAUDIOPLAYERV1_H

#include <QObject>
#include <QPointer>
#include <QAudioOutput>
#include <QAudio>
#include <QUrl>
#include "abuleduaudiodecoderv1.h"
#include "abuledumultimediasettingsv1.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    #include <QMediaPlayer>
#endif

class AbulEduAudioPlayerV1 : public QObject
{
    Q_OBJECT
public:

    /** On spécifie la sortie audio matérielle à utiliser */
    explicit AbulEduAudioPlayerV1(QAudioDeviceInfo deviceInfo, QObject *parent = 0);
    /** On utilise la sortie audio matérielle de AbulEduMultimediaSettings */
    explicit AbulEduAudioPlayerV1(QObject *parent = 0);

    ~AbulEduAudioPlayerV1();
    /** Par soucis de cohérence avec QMediaPlayer, les fonctions gardent le même nom.
        il suffira donc de remplacer new QMediaPlayer par new AbulEduAudioPlayerV1 */
    void setMedia(const QString &localFilenamePath);

    /** Assigne le volume de sortie
     * @param volume la valeur est comprise entre 0.0 et 1.0 inclus
     */
    void setVolume(qreal volume);

    void abeAudioPlayerSetOutputDevice(QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultOutputDevice());

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    QMediaPlayer *abeAudioPlayerGetMediaPlayer() const;
    void abeAudioPlayerSetMediaPlayer(QMediaPlayer *mediaPlayer);
#endif

private:
    QString                         m_fileToPlay;
    QAudioFormat                    m_format;
    QAudioDeviceInfo                m_outputDeviceInfo;
    QPointer<QAudioOutput>          m_player;
    QPointer<AbulEduAudioDecoderV1> m_decoder;
    qreal                           m_volume;
    QSharedPointer<AbulEduMultiMediaSettingsV1>    m_abuleduMultiMediaSettings;

#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    QMediaPlayer *m_abeAudioPlayerMediaPlayer;
#endif

    void initialize();
signals:
#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
    void stateChanged(QAudio::State);
#endif
    void error(QAudio::Error);
    void durationChanged(qint64);
    void positionChanged(qint64);
    void signalAbeAudioPlayerMediaFinished();

public slots:
    void play();
    void play(QPointer<QIODevice> ioDevice, qint64 duration);
    void stop();
    void pause();
    void resume();

private slots:
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    void handleMediaStateChanged(QMediaPlayer::State);
#else
    void handleStateChanged(QAudio::State newState);
#endif
    void slotNotify();
};

#endif // ABULEDUAUDIOPLAYERV1_H
