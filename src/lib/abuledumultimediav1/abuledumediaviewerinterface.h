#ifndef ABULEDUMEDIAVIEWERINTERFACE_H
#define ABULEDUMEDIAVIEWERINTERFACE_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioProbe>
#include <QAudioDeviceInfo>
#include <QBoxLayout>
#include <QTextEdit>
#include "abuledumultimediasettingsv1.h"

class QMediaPlayer;

///
/// \brief La classe AbulEduMediaViewerInterface classe d'interface permettant d'implémenter des "viewer"
///
class AbulEduMediaViewerInterface : public QWidget
{
    Q_OBJECT
    Q_ENUMS(enumAlignment)

public:
    virtual ~AbulEduMediaViewerInterface() {qDebug() << __PRETTY_FUNCTION__;}

    /// Définit la position du texte par rapport à l'image
    /// \li Top    : le texte est au dessus de l'image
    /// \li Right  : le texte est à droite de l'image
    /// \li Bottom : le texte est sous l'image
    /// \li Left   : le texte est à gauche de l'image
    enum enumAlignment{Top, Right, Bottom, Left};

    /// Fixe le chemin du media (image, video...)
    ///  @param mediaPath le chemin absolu vers le média à afficher
    virtual void abeMediaViewerSetMediaPath(const QString &mediaPath)=0;

    /// Fixe le texte à afficher
    ///  @param text Le texte à afficher
    virtual void abeMediaViewerSetSubTitle(const QString &text) =0;

    /// Fixe le chemin du fichier son à jouer
    ///  @param soundPath le chemin absolu vers le son à jouer
    virtual void abeMediaViewerSetSoundPath(const QString &soundPath)=0;

    /// Fixe en une fois tous les paramètres du media
    ///  @param mediaPath le chemin absolu vers le média à afficher
    ///  @param soundPath le chemin absolu vers le son à jouer
    ///  @param text Le text à afficher
    virtual void abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text) =0;

    /// Fixe la position du texte par rapport à l'image
    ///  @param align Positon du texte par rapport à l'image valeurs possibles Top Right, Bottom, Left
    virtual void abeMediaViewerSetTextPosition(enumAlignment align) =0;

    /// Montre/cache le texte associé au média
    /// @param visible
    virtual void abeMediaViewerSetTextVisible(bool ){;}

    /// Assigne la sortie du son (ie. casque/hp)
    /// @param deviceInfo
    virtual void abeMediaViewerSetOutputDevice(QAudioDeviceInfo ){;}

    /// Assigne le device d'entrée pour l'enregistrement (ie. micro USB/micro interne)
    /// @param inputDeviceInfo
    virtual void abeMediaViewerSetInputDevice(QAudioDeviceInfo ){;}

    virtual QTextEdit* abeMediaViewerGetTextEdit(){return 0;}

    /// Retourne un pointeur vers le layout du viewer
    /// return Pointeur vers le layout principal du viewer.
    virtual QBoxLayout * abeMediaViewerGetLayout() =0;

    /// Retourne un pointeur vers la sonde attachée au buffer de son
    /// @return le pointeur vers la sonde.
    virtual QAudioProbe *abeMediaRecorderGetAudioProbe(){return 0;}

    /// Fixe dans le recorder le nom de sortie du fichier enregistré
    virtual void abeMediaViewerSetOutputFilePath(const QString &) {;}

    virtual inline void abeMultiMediaResizeLabel(){return;}

    ///
    /// \brief abeMultiMediaGetMediaPlayer
    /// \return QMediaPlayer qui ne sera instancié dans les classes que sous Windows
    /// \note Ces méthodes ne sont pas à spécialiser car elles sont appelées sous toutes les plateformes
    ///
    virtual QMediaPlayer *abeMultiMediaGetMediaPlayer() {return 0;}
    virtual void abeMultiMediaSetMediaPlayer(QMediaPlayer *){;}

    virtual Qt::Alignment abeMediaViewerGetSubTitleAlignment() const {return Qt::AlignLeft;}
    virtual void abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &) {;}

    virtual int abeMediaViewerGetSubTitlePointSize() const {return -1;}
    virtual void abeMediaViewerSetSubTitlePointSize(int) {;}

public slots:
    virtual void slotAbeMediaViewerPlay()   =0;
    virtual void slotAbeMediaViewerPause()  =0;
    virtual void slotAbeMediaViewerStop()   =0;
    virtual void slotAbeMediaViewerResume() =0;
    // Les slots pour l'enregistrement, ils sont facultatifs dans les classes dérivées
    /// @note L'implémentation de ce slot dans les classes dérivées est facultative.
    virtual void slotAbeMediaViewerStartRecording(){}
    /// @note L'implémentation de ce slot dans les classes dérivées est facultative.
    virtual void slotAbeMediaViewerStopRecording(){}
    /// @note L'implémentation de ce slot dans les classes dérivées est facultative.
    virtual void slotAbeMediaViewerPauseRecording(){}
    /// @note L'implémentation de ce slot dans les classes dérivées est facultative.
    virtual void slotAbeMediaViewerResumeRecording(){}
    /// @note L'implémentation de ce slot dans les classes dérivées est facultative.
    virtual void slotAbeMediaViewerAvailabilityChanged(){}

signals:
    /// Signal émis lorsque la présentation du média est terminée
    void signalAbeMediaFinished();

    /// Signal émis lorsque le média est mis en pause
    ///  @param time le moment où le média a été mis en pause
    void signalAbeMediaPaused(qint64  time);

    /// Signal émis lorsque le média est stoppé
    ///  @param time le moment où le média a été arreté
    void signalAbeMediaStopped(qint64  time);

    /// Signal émis lorsque lorsque la "durée" du média change
    ///  @param duration la longueur en ms du média
    void signalAbeMediaDuration(qint64  duration);

    /// Signal émis régulièrement indiquant la position du lecteur en ms
    ///  @param position le moment entrain d'être lu
    void signalAbeMediaPositionChanged(qint64 position);

    /// Signal émis à chaque changement de status.
    /// @param newStatus Le nouveau status du QMediaPlayer.
    void signalAbeMediaViewerMediaStatusChanged(QMediaPlayer::MediaStatus newStatus);

    /// Signal émis au début de l'enregistrement.
    void signalAbeMediaViewerRecordingStarted();

    /// Signal émis après l'appui sur Stop (fin d'enregistrement).
    void signalAbeMediaViewerRecordingStopped();

    /// Signal émis après l'appui sur Stop (fin d'enregistrement).
    void signalAbeMediaViewerRecordingPaused();

    /// Signal émis après l'appui sur Stop (fin d'enregistrement).
    void signalAbeMediaViewerRecordingResumed();

    /// Signal émis lorsque le recorder est disponible
    void signalAbeMediaViewerIsAvailable(bool available);

    /// Signal émis régulièrement pour informer sur le niveau d'enregistrement.
    void signalAbeMediaViewerLevel(qreal level);

protected:
    QSharedPointer<AbulEduMultiMediaSettingsV1>     m_abuleduMultiMediaSettings;
    QPair<QString,QString>                          m_abuleduMultiMediaSubTitleAlignment;
};

#endif // ABULEDUMEDIAVIEWERINTERFACE_H
