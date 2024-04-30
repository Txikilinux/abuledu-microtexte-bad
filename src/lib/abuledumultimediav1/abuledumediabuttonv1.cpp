/** Classe AbulEduMediaButtonV1
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

#include "abuledumediabuttonv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMediaButtonV1::AbulEduMediaButtonV1(QWidget *parentForButton, QWidget *parentForMultiMedia, QList<AbulEduMediaMedias> listMedias, AbulEduPicottsV1::AbulEduLang language, bool isTextVisible) :
    AbulEduFlatBoutonV1(parentForButton),
    m_medias(listMedias),
    m_buttonSize(QSize(20,20)),
    m_language(language),
    m_textIsVisible(isTextVisible),
    m_multiMedia(0),
    m_parentPlayer(parentForMultiMedia)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    initAbulEduMediaButton(parentForButton);
}

void AbulEduMediaButtonV1::initAbulEduMediaButton(QWidget* parentForButton)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeMediaButtonSetFixedSize(m_buttonSize);
    setIcones(":/controlaudio/more", ":/controlaudio/more_hover", ":/controlaudio/more_hover");
    if(parentForButton)
    {
        move(parentForButton->width() - m_buttonSize.width(), parentForButton->height() - m_buttonSize.height());
    }
    connect(this, SIGNAL(clicked()), this, SLOT(slotAbeShowMultiMedia()));
}

AbulEduMediaButtonV1::~AbulEduMediaButtonV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

}

void AbulEduMediaButtonV1::abeMediaButtonSetFixedSize(QSize size)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_buttonSize = size;
    setFixedSize(m_buttonSize);
}

void AbulEduMediaButtonV1::abeMediaButtonSetFixedSize(int width, int height)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeMediaButtonSetFixedSize(QSize(width, height));
}

void AbulEduMediaButtonV1::slotAbeShowMultiMedia()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!m_multiMedia)
    {
        m_multiMedia = new AbulEduMultiMediaV1(m_medias, AbulEduMultiMediaV1::Diapos, m_language, m_textIsVisible, AbulEduMultiMediaSettingsV1::Automatic, m_parentPlayer);
        m_multiMedia->abeMultiMediaSetButtonVisible(AbulEduMultiMediaV1::BtnNext
                                                    | AbulEduMultiMediaV1::BtnPrevious
                                                    | AbulEduMultiMediaV1::BtnMagnifyingGlass,
                                                    false);
        m_multiMedia->move(0,0);
    }
    m_multiMedia->show();
    emit signalAbeButtonMultiMediaWidgetCreated(m_multiMedia);
}
