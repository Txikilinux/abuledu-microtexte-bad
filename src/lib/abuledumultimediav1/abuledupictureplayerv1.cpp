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

#include "abuledupictureplayerv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduPicturePlayerV1::AbulEduPicturePlayerV1(const QString &picturePath, const QString &soundPath, const QString &text, QWidget *parent)
    : AbulEduMultiMediaV1(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    this->abeMultiMediaSetButtonVisible(  AbulEduMultiMediaV1::BtnMagnifyingGlass
                                        | AbulEduMultiMediaV1::BtnNext
                                        | AbulEduMultiMediaV1::BtnPrevious
                                        | AbulEduMultiMediaV1::BtnRecord
                                        , false);
    QList<AbulEduMediaMedias>list;
    list.append(AbulEduMediaMedias(picturePath, soundPath, text));
    abeMultiMediaSetNewMedia(list);
    connect(this, SIGNAL(signalAbulEduPicturePlayerPicturePathChanged(QString)), this, SLOT(slotAbeSomethingChanged()), Qt::UniqueConnection);
    connect(this, SIGNAL(signalAbulEduPicturePlayerSoundPathChanged(QString)), this, SLOT(slotAbeSomethingChanged()), Qt::UniqueConnection);
    connect(this, SIGNAL(signalAbulEduPicturePlayerTextChanged(QString)), this, SLOT(slotAbeSomethingChanged()), Qt::UniqueConnection);
    connect(this, SIGNAL(signalAbulEduPicturePlayerMediaChanged()), this, SLOT(slotAbeSomethingChanged()), Qt::UniqueConnection);

}


AbulEduPicturePlayerV1::AbulEduPicturePlayerV1(QWidget *parent)
    : AbulEduMultiMediaV1(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    this->abeMultiMediaSetButtonVisible(  AbulEduMultiMediaV1::BtnMagnifyingGlass
                                        | AbulEduMultiMediaV1::BtnNext
                                        | AbulEduMultiMediaV1::BtnPrevious
                                        | AbulEduMultiMediaV1::BtnRecord
                                        , false);
    connect(this, SIGNAL(signalAbulEduPicturePlayerPicturePathChanged(QString)), this, SLOT(slotAbeSomethingChanged(QString)), Qt::UniqueConnection);
    connect(this, SIGNAL(signalAbulEduPicturePlayerSoundPathChanged(QString)), this, SLOT(slotAbeSomethingChanged(QString)), Qt::UniqueConnection);
    connect(this, SIGNAL(signalAbulEduPicturePlayerTextChanged(QString)), this, SLOT(slotAbeSomethingChanged(QString)), Qt::UniqueConnection);
    connect(this, SIGNAL(signalAbulEduPicturePlayerMediaChanged()), this, SLOT(slotAbeSomethingChanged()), Qt::UniqueConnection);

}

void AbulEduPicturePlayerV1::abePicturePlayerSetPicturePath(const QString &picturePath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_picturePath = picturePath;
    emit signalAbulEduPicturePlayerPicturePathChanged(picturePath);
}

void AbulEduPicturePlayerV1::abePicturePlayerSetSoundPath(const QString &soundPath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_soundPath = soundPath;
    emit signalAbulEduPicturePlayerSoundPathChanged(soundPath);
}

void AbulEduPicturePlayerV1::abePicturePlayerSetText(const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_text = text;
    emit signalAbulEduPicturePlayerTextChanged(text);
}

void AbulEduPicturePlayerV1::abePicturePlayerSetNewMedia(const QString& picturePath, const QString& soundPath,
                                                         const QString& text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_picturePath   = picturePath;
    m_soundPath     = soundPath;
    m_text          = text;
    emit signalAbulEduPicturePlayerMediaChanged();
}

void AbulEduPicturePlayerV1::slotAbeSomethingChanged()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QList<AbulEduMediaMedias>list;
    list.append(AbulEduMediaMedias(m_picturePath, m_soundPath, m_text));
    abeMultiMediaSetNewMedia(list);
}

