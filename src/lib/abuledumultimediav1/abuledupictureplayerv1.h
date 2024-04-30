/** Classe AbulEduPicturePlayerV1
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

#ifndef ABULEDUPICTUREPLAYERV1_H
#define ABULEDUPICTUREPLAYERV1_H

#include <QWidget>

#include "abuledumultimediav1.h"
#include "abuleduloggerv1.h"

class AbulEduPicturePlayerV1 : public AbulEduMultiMediaV1
{
    Q_OBJECT

    Q_PROPERTY(QString picturePath  READ abePicturePlayerGetPicturePath  WRITE abePicturePlayerSetPicturePath  NOTIFY signalAbulEduPicturePlayerPicturePathChanged USER true)
    Q_PROPERTY(QString soundPath    READ abePicturePlayerGetSoundPath    WRITE abePicturePlayerSetSoundPath    NOTIFY signalAbulEduPicturePlayerSoundPathChanged   USER true)
    Q_PROPERTY(QString text         READ abePicturePlayerGetText         WRITE abePicturePlayerSetText         NOTIFY signalAbulEduPicturePlayerTextChanged        USER true)
public:
    explicit AbulEduPicturePlayerV1(const QString& picturePath, const QString& soundPath = "",
                                    const QString& text = "", QWidget *parent = 0);
    explicit AbulEduPicturePlayerV1(QWidget *parent = 0);

    inline const QString& abePicturePlayerGetPicturePath(){ return m_picturePath;}
    inline const QString& abePicturePlayerGetSoundPath()  { return m_soundPath;}
    inline const QString& abePicturePlayerGetText()       { return m_text;}

    void abePicturePlayerSetPicturePath (const QString &picturePath);
    void abePicturePlayerSetSoundPath   (const QString &soundPath);
    void abePicturePlayerSetText        (const QString &text);
    void abePicturePlayerSetNewMedia(const QString &picturePath, const QString &soundPath, const QString &text);
private:
    QString m_picturePath;
    QString m_soundPath;
    QString m_text;
signals:
    void signalAbulEduPicturePlayerPicturePathChanged(const QString& newPicturePath);
    void signalAbulEduPicturePlayerSoundPathChanged(const QString& newSoundPath);
    void signalAbulEduPicturePlayerTextChanged(const QString& newText);
    void signalAbulEduPicturePlayerMediaChanged();
public slots:

private slots:
    void slotAbeSomethingChanged();

};

#endif // ABULEDUPICTUREPLAYER_H
