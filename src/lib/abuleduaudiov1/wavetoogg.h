/** Classe WaveToOgg : Encoder Wave -> Ogg
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


#ifndef WAVETOOGG_H
#define WAVETOOGG_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

/* Gestion du format wave supporté */
#include "waveheaderreader.h"

#include "abuleduloggerv1.h"

namespace AbulEduAudioV1 {

class WaveToOgg : public QObject
{
    Q_OBJECT

public:

    enum Status{
        ProcessOK                      = 0x0,
        ProcessStarted                 = 0x2,
        ProcessError                   = 0x4,
        FileToEncodeFound              = 0x6,
        FileToEncodeNotFound           = 0x8,
        FileNotReadable                = 0x10,
        EncoderAvailable               = 0x12,
        EncoderNotAvailable            = 0x14,
        FileToConvertNotOpenable       = 0x16,
        FileToWriteNotOpenable         = 0x18,
        ChoosenFormatNotSupported      = 0x20
    };

    Q_ENUMS(Status)

    explicit WaveToOgg(QObject *parent = 0);
    ~WaveToOgg();
    void processWavFile(const QString &fileInPath, const QString &fileOutPath);

    void setOggTagTitle(const QString &);
    void setOggTagEncoder(const QString &);
    void setOggTagVersion(const QString &);
    void setOggTagAlbum(const QString &);
    void setOggTagTrackNumber(const QString &);
    void setOggTagArtist(const QString &);
    void setOggTagPerformer(const QString &);
    void setOggTagCopyright(const QString &);
    void setOggTagLicence(const QString &);
    void setOggTagOrganization(const QString &);
    void setOggTagDescription(const QString &);
    void setOggTagGenre(const QString &);
    void setOggTagDate(const QString &);
    void setOggTagLocation(const QString &);
    void setOggTagContact(const QString &);
    void setOggTagISRCNumber(const QString &);

signals:
    void signalWaveToOggStatus(WaveToOgg::Status, WaveHeaderReader::Status, const QString& );
    void signalWaveToOggInformationsAvailable(WaveHeaderReader::WaveInformations);
    void signalWaveToOggProcessPercent(float);

    //signaux en double pour qu'on puisse répondre au pb des namespaces, cf https://redmine.ryxeo.com/issues/4609
    void signalWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::Status, AbulEduAudioV1::WaveHeaderReader::Status, const QString& );
    void signalWaveToOggInformationsAvailablePub(AbulEduAudioV1::WaveHeaderReader::WaveInformations);

public slots:
    void encodeWavFile();

private slots:
    void slotWaveToOggStatus(WaveHeaderReader::Status);
    void slotWaveToOggStatusPub(AbulEduAudioV1::WaveToOgg::Status, AbulEduAudioV1::WaveHeaderReader::Status, const QString&);

private:
    AbulEduAudioV1::WaveHeaderReader *m_waveHeaderReader;
    QString m_fileInPath;
    QString m_fileOutPath;

    /// Tags OGG
    QString m_tagTitle;
    QString m_tagEncoder;
    QString m_tagVersion;
    QString m_tagAlbum;
    QString m_tagTrackNumber;
    QString m_tagArtist;
    QString m_tagPerformer;
    QString m_tagCopyright;
    QString m_tagLicence;
    QString m_tagOrganization;
    QString m_tagDescription;
    QString m_tagGenre;
    QString m_tagDate;
    QString m_tagLocation;
    QString m_tagContact;
    QString m_tagISRCNumber;
};

} /* End NAMESPACE ABULEDUAUDIO */
#endif // WAVETOOGG_H
