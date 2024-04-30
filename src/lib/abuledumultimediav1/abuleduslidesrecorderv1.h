/** Classe AbulEduSlidesRecorderV1
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

#ifndef ABULEDUSLIDESRECORDERV1_H
#define ABULEDUSLIDESRECORDERV1_H

#include <QAudioRecorder>
#include <QAudioProbe>
#include "abuleduslidesviewerv1.h"
#include "abuleduloggerv1.h"

#ifdef Q_OS_WIN
#include "wavetoogg.h"
using namespace AbulEduAudioV1;
#endif

class AbulEduSlidesRecorderV1 : public AbulEduSlidesViewerV1
{
    Q_OBJECT
public:
    /// Constructeur destiné à la promotion depuis le designer
    AbulEduSlidesRecorderV1(QWidget* parent);

    /// Constructeur destiné à être appelé depuis l'AbulEduMultiMediaV1
    AbulEduSlidesRecorderV1(const QString &pixmapPath = "",
                            const QString &soundPath = "",
                            const QString &text = "");
    /// Permet d'écrire le son enregistré à un endroit différent du soundPath prévu par défaut
    /// @param filePath Le chemin du son à enregistrer
    virtual void abeMediaViewerSetOutputFilePath(const QString &filePath);
    virtual QString abeMediaViewerGetOutputFilePath() const;

    /** Charge en une fois tous les paramètres du media
     *  @param mediaPath le chemin absolu vers le média à afficher
     *  @param soundPath le chemin absolu vers le son à jouer
     *  @param text Le texte à afficher
     */
    virtual void abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text);

    /** Pointeur vers le QAudioProbe, utilisé par le vu-mètre */
    virtual QAudioProbe *abeMediaRecorderGetAudioProbe();

    virtual ~AbulEduSlidesRecorderV1();


private:
    void abeSlidesRecorderInitialize();
    QString         m_outputFileName;
    QAudioProbe    *m_audioProbe;
    QMultimedia::AvailabilityStatus m_availabilityStatus;
    QAudioRecorder *m_recorder;

#ifdef Q_OS_WIN
    WaveToOgg *m_waveEncoder;
#endif
public slots:
    void slotAbeMediaViewerStartRecording();
    void slotAbeMediaViewerStopRecording();
    void slotAbeMediaViewerPauseRecording();
    void slotAbeMediaViewerResumeRecording();

private slots:
    void slotAbeMediaViewerProcessBuffer(const QAudioBuffer &buffer);
    void slotAbeMediaViewerAvailabilityChanged(QMultimedia::AvailabilityStatus status);

    void slotErrorRecorder(QMediaRecorder::Error error);
    void slotAbeMediaViewerDisplayProgression(float);
#ifdef Q_OS_WIN
    void slotAbeMediaViewerWaveEncoderStatus(WaveToOgg::Status, WaveHeaderReader::Status, QString);
#endif
signals:
};

#endif // ABULEDUSLIDESRECORDERV1_H
