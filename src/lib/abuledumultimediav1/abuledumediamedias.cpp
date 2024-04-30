/** Classe AbulEduMultiMediaV1 : stocke les paramètres nécessaires pour jouer un média
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2015 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#include "abuledumediamedias.h"

AbulEduMediaMedias::AbulEduMediaMedias(const QString &fileNameMedia, const QString &fileNameSound, const QString &text) :
    AbulEduMediaMediasImage(fileNameMedia), AbulEduMediaMediasText(text), AbulEduMediaMediasSound(fileNameSound)
{
}

AbulEduMediaMedias::~AbulEduMediaMedias()
{
}

void AbulEduMediaMedias::abeMediaMediaSetText(const QString &text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<text;
    AbulEduMediaMediasText = text;
}

bool AbulEduMediaMedias::operator ==(AbulEduMediaMedias referentMedia)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool trueFalse = true;
    if(referentMedia.AbulEduMediaMediasImage != AbulEduMediaMediasImage){
        trueFalse = false;
    }
    if(referentMedia.AbulEduMediaMediasSound != AbulEduMediaMediasSound){
        trueFalse = false;
    }
    if(referentMedia.AbulEduMediaMediasText != AbulEduMediaMediasText){
        trueFalse = false;
    }
    return trueFalse;
}

bool AbulEduMediaMedias::isEmpty()
{
//    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(AbulEduMediaMediasImage.isEmpty() && AbulEduMediaMediasSound.isEmpty() && AbulEduMediaMediasText.isEmpty()) {
        return true;
    }
    else {
        return false;
    }
}

QDebug operator<<(QDebug dbg, const AbulEduMediaMedias &param)
{
    dbg.nospace() << param.abeMediaMediaGetImage()<<param.abeMediaMediaGetSound()<<param.abeMediaMediaGetText();
    return dbg.maybeSpace();

}
