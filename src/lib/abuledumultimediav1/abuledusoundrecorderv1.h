/** Classe AbulEduSoundRecorderV1
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2013 Eric Seigne <eric.seigne@ryxeo.com>
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

#ifndef ABULEDUSOUNDRECORDERV1_H
#define ABULEDUSOUNDRECORDERV1_H

#include <QObject>
#include <QUrl>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QAudioDeviceInfo>
#include "abuledusoundplayerv1.h"
#include "abuleduloggerv1.h"

#ifdef Q_OS_WIN
#include "wavetoogg.h"
using namespace AbulEduAudioV1;
#endif

///
/// \brief La classe AbulEduSoundRecorderV1 pour enregistrer des sons
///
class AbulEduSoundRecorderV1 : public AbulEduSoundPlayerV1
{
    Q_OBJECT

public:
    explicit AbulEduSoundRecorderV1(const QString &outputFileName ="");
    ~AbulEduSoundRecorderV1();

    /// Précise le fichier de sortie
    /// @param locationPath Le chemin du son à enregistrer
    void abeSoundRecorderSetOutputFileName(const QString &locationPath);
    virtual void abeMediaViewerSetOutputFilePath(const QString &filePath);
    virtual QString abeMediaViewerGetOutputFilePath() const;

    /** Fixe le chemin du fichier son à enregistrer
     *  @param soundPath le chemin absolu vers le son à enregistrer
     */
    virtual void abeMediaViewerSetSoundPath(const QString &soundPath);

    /// Assigne le device d'entrée pour l'enregistrement (ie. micro USB/micro interne)
    /// @param inputDeviceInfo
    virtual void abeMediaViewerSetInputDevice(QAudioDeviceInfo deviceInfo);

    /** Charge en une fois tous les paramètres du media
     *  @param mediaPath le chemin absolu vers le média à afficher
     *  @param soundPath le chemin absolu vers le son à jouer
     *  @param text Le texte à afficher
     */
    virtual void abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text);

    virtual QAudioProbe *abeMediaRecorderGetAudioProbe();

private:
    QAudioRecorder  *m_recorder;
    QString         m_outputFileName;
    QAudioProbe     *m_audioProbe;
    QAudioDeviceInfo m_inputDeviceInfo;
    QMultimedia::AvailabilityStatus m_availabiliyStatus;
#ifdef Q_OS_WIN
    WaveToOgg *m_waveEncoder;
#endif

signals:

public slots:
    void slotAbeMediaViewerStartRecording();
    void slotAbeMediaViewerStopRecording();
    void slotAbeMediaViewerPauseRecording();
    void slotAbeMediaViewerResumeRecording();
    void slotAbeSoundRecorderProcessBuffer(const QAudioBuffer &buffer);

private slots:
    void slotAvailabilityChanged(QMultimedia::AvailabilityStatus status);
#ifdef Q_OS_WIN
    void slotAbeMediaViewerWaveEncoderStatus(WaveToOgg::Status statusWave,WaveHeaderReader::Status statusHeader,QString cheminFichier);
#endif

};

#endif // ABULEDUSOUNDRECORDERV1_H
