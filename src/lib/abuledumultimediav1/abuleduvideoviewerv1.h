/** Classe AbulEduVideoViewerV1
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

#ifndef ABULEDUVIDEOVIEWERV1_H
#define ABULEDUVIDEOVIEWERV1_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QBoxLayout>
#include <QMovie>
#include <QDebug>
#include <QMediaPlayer>
#include <QVideoWidget>
#include "abuleduflatboutonv1.h"
#include "abuledumediaviewerinterface.h"
#include "abuleduaudioplayerv1.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

class AbulEduVideoViewerV1 : public AbulEduMediaViewerInterface
{
    Q_OBJECT
public:
    ///
    /// \brief AbulEduVideoViewerV1 Affiche une Vidéo et texte si différent de "" et joue le fichier de son passé en paramètre
    /// \param mediaPath Le chemin vers la video
    /// \param soundPath Le chemin vers le fichier à jouer
    /// \param text Le texte à afficher
    ///
    AbulEduVideoViewerV1(const QString &mediaPath = "",
                         const QString &soundPath = "",
                         const QString &text = "");

    /** Fixe le chemin du media (image, video...)
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
     *  @param mediaPath le chemin absolu vers le média à afficher
     *  @param soundPath le chemin absolu vers le son à jouer
     *  @param text Le text à afficher
     */
    virtual void abeMediaViewerSetMediaDatas(const QString &mediaPath, const QString &soundPath, const QString &text);

    virtual void abeMediaViewerSetTextPosition(AbulEduMediaViewerInterface::enumAlignment align);

    /** Retourne le layout du widget pour par exemple y insérer un autre widget
     * @return Le QBoxLayout du widget
     */
    inline QBoxLayout * abeMediaViewerGetLayout(){ return m_layout;}

    Qt::Alignment abeMediaViewerGetSubTitleAlignment() const;
    void abeMediaViewerSetSubTitleAlignment(const Qt::Alignment &subTitleAlignment);

    /** Retourne la taille (pointSize) des caractères utilisés pour les sous-titres */
    int abeMediaViewerGetSubTitlePointSize() const;
    /** Modifie la taille (pointSize) des caractères utilisés pour les sous-titres
     * @param pointSize la nouvelle taille */
    void abeMediaViewerSetSubTitlePointSize(int pointSize);

    ~AbulEduVideoViewerV1();

private:
    void updateMediaDatas(const QString &videoPath, const QString &soundPath = "", const QString &text = "");
    void initAudioFileV1();

    QSizeF m_size;
    QBoxLayout          *m_layout;
    QString              m_videoPath;
    QMediaPlayer        *m_videoPlayer;
    QVideoWidget        *m_videoWidget;
    QString              m_textString;
    QTextEdit           *m_textViewer;
    QString              m_soundPath;

    AbulEduAudioPlayerV1 *m_audioPlayer;
    qint64               m_duration;
signals:

public slots:
    virtual void slotAbeMediaViewerPlay();
    virtual void slotAbeMediaViewerPause();
    virtual void slotAbeMediaViewerStop();
    virtual void slotAbeMediaViewerResume();
private slots:
    void slotAbeMediaStatusChanged(QMediaPlayer::MediaStatus newStatus);
    void slotAbePlayingFinished();
    void slotAbeStateChanged(QAudio::State newState);
    void slotAbeDurationChanged(qint64 duration);
    void showEvent(QShowEvent *);
public slots:
};

#endif // ABULEDUVIDEOVIEWERV1_H
