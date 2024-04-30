#ifndef WAVETOOGGTHREAD_H
#define WAVETOOGGTHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#ifdef Q_OS_WIN /* We need the following two to set fileIn/fileOut to binary */
#include <io.h>
#include <fcntl.h>
#endif

/* Les librairies qu'il faut pour encoder */
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include <QThread>
#include "waveheaderreader.h"
#include "wavetoogg.h"

#include "abuleduloggerv1.h"
#ifndef DEBUG_ABULEDUAUDIOV1
    #include "abuledudisableloggerv1.h"
#endif

class WaveToOggThread : public QThread
{
    Q_OBJECT


public:
    explicit WaveToOggThread();
    ~WaveToOggThread();
    void setFiles(const QString &fileInPath, const QString &fileOutPath);
    void setMetaData(const QString &tagTitle, const QString &tagEncoder, const QString &tagVersion, const QString &tagAlbum, const QString &tagTrackNumber, const QString &tagArtist, const QString &tagPerformer, const QString &tagCopyright, const QString &tagLicence, const QString &tagOrganization, const QString &tagDescription, const QString &tagGenre, const QString &tagDate, const QString &tagLocation, const QString &tagContact, const QString &tagISRCNumber);
    void run() Q_DECL_OVERRIDE;

signals:
    void signalWaveToOggThreadStatus(AbulEduAudioV1::WaveToOgg::Status, AbulEduAudioV1::WaveHeaderReader::Status, const QString&);
    void signalWaveToOggThreadInformationsAvailable(AbulEduAudioV1::WaveHeaderReader::WaveInformations);
    void signalWaveToOggThreadProcessPercent(float);

public slots:

private:
#define READ_MAX 1024
    signed char readbuffer[READ_MAX*4+44]; /* out of the data segment, not the stack */
    char pcmout[4096];
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

#endif // WAVETOOGGTHREAD_H

