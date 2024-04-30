#ifndef ABULEDUSOUNDPLAYERV1_H
#define ABULEDUSOUNDPLAYERV1_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QBoxLayout>
#include <QMediaPlayer>
#include <QMediaService>
#include <QAudio>
#include "abuleduaudioplayerv1.h"

#include "abuleduflatboutonv1.h"
#include "abuledumediaviewerinterface.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

class AbulEduSoundPlayerV1 : public AbulEduMediaViewerInterface
{
    Q_OBJECT
public:
    ///
    /// \brief AbulEduSoundPlayerV1 Affiche un texte si différent de "" et joue le fichier de son passé en paramètre
    /// \param mediaPath Ce paramètre ne sert à rien : on n'affiche pas d'image ou de video
    /// \param soundPath Le chemin vers le fichier à jouer
    /// \param text Le texte à afficher
    ///
    AbulEduSoundPlayerV1(const QString &mediaPath = "",
                         const QString &soundPath = "",
                         const QString &text = "");

    /** Fixe le chemin du media (image, video...) Inutilisé dans ce module
     *  @param mediaPath le chemin absolu vers le média à afficher
     */
    virtual void abeMediaViewerSetMediaPath(const QString &mediaPath);

    /** Fixe le texte à afficher
     *  @param text Le text à afficher
     */
    virtual void abeMediaViewerSetSubTitle(const QString &text);

    /** Fixe le chemin du fichier son à jouer
     *  @param soundPath le chemin absolu vers le son à jouer
     */
    virtual void abeMediaViewerSetSoundPath(const QString &soundPath);

    /** Fixe en une fois tous les paramètres du media
     *  @param mediaPath le chemin absolu vers le média à afficher inutile ici
     *  @param soundPath le chemin absolu vers le son à jouer
     *  @param text Le text à afficher
     */
    virtual void abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text);

    virtual void abeMediaViewerSetTextVisible(bool visible);

    /** Fonction inutile ici */
    virtual void abeMediaViewerSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align);

    /** Retourne le layout du widget pour par exemple y insérer un autre widget
     * @return Le QBoxLayout du widget
     */
    inline QBoxLayout * abeMediaViewerGetLayout(){ return m_layout;}

    ~AbulEduSoundPlayerV1();

    /** Assigne le périphérique de sortie du son
     * @param deviceInfo le périphérique de sortie souhaité
     */
    virtual void abeMediaViewerSetOutputDevice(QAudioDeviceInfo deviceInfo);

#ifdef Q_OS_WIN
    virtual QMediaPlayer *abeMultiMediaGetMediaPlayer();
    virtual void abeMultiMediaSetMediaPlayer(QMediaPlayer *mediaPlayer);
#endif

    Qt::Alignment abeMediaViewerGetSubTitleAlignment() const;
    void abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &subTitleAlignment);

    /** Retourne la taille (pointSize) des caractères utilisés pour les sous-titres */
    int abeMediaViewerGetSubTitlePointSize() const;
    /** Modifie la taille (pointSize) des caractères utilisés pour les sous-titres
     * @param pointSize la nouvelle taille */
    void abeMediaViewerSetSubTitlePointSize(int pointSize);

private:

    void updateMediaDatas(const QString &soundPath = "", const QString &text = "");
    void initAudioPlayer();

    QSizeF m_size;
    QBoxLayout           *m_layout;
    QString               m_textString;
    QTextEdit            *m_textViewer;
    QString               m_soundPath;
    AbulEduAudioPlayerV1 *m_audioPlayer;
    qint64                m_duration;
    bool                  m_textViewerIsVisible;

signals:

public slots:
    virtual void slotAbeMediaViewerPlay();
    virtual void slotAbeMediaViewerPause();
    virtual void slotAbeMediaViewerStop();
    virtual void slotAbeMediaViewerResume();
    virtual void slotAbeMediaViewerStartRecording(){}
    virtual void slotAbeMediaViewerStopRecording(){}
private slots:
    void slotAbePlayingFinished();
    void slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus newStatus);
#if !defined (Q_OS_WIN) && !defined(Q_OS_ANDROID)
    void slotAbeStateChanged      (QAudio::State newState);
#endif
    void slotAbeDurationChanged   (qint64 duration);
};

#endif // ABULEDUSOUNDPLAYERV1_H
