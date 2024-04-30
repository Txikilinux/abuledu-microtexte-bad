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

#ifndef ABULEDUMEDIAMEDIAS_H
#define ABULEDUMEDIAMEDIAS_H

#include "abuleduloggerv1.h"

class AbulEduMediaMedias {

public:
    /** Constructeur d'un AbulEduMediaMedias, association d'une image avec un son et un commentaire
     * @param fileNameMedia le nom d'un fichier media (image, gif, video)
     * @param fileNameMedia le nom d'un fichier son au format .ogg
     * @param text le sous-titre de l'image */
    AbulEduMediaMedias(const QString &fileNameMedia="", const QString &fileNameSound="", const QString &text="");
    ~AbulEduMediaMedias();
    inline QString abeMediaMediaGetImage() const {return AbulEduMediaMediasImage;}
    inline QString abeMediaMediaGetSound() const {return AbulEduMediaMediasSound;}
    inline QString abeMediaMediaGetText()  const {return AbulEduMediaMediasText;}
    void abeMediaMediaSetText(const QString& text);
    bool operator ==(AbulEduMediaMedias referentMedia);
    /** @return true si les trois QString sont vides */
    bool isEmpty();

    friend class AbulEduMultiMediaV1;

protected:
    QString AbulEduMediaMediasImage;
    QString AbulEduMediaMediasText;
    QString AbulEduMediaMediasSound;
    /* Il aurait été plus élégant de garder la duration dans un attribut de la classe AbulEduMediaMedias,
     * plutôt que dans une structure de données d'AbulEduMultiMediaV1 (classe qui les utilise).
     * Hélas impossible de résoudre le problème suivant : un "getDuration" retourne 0, puis un "setDuration" de 1040, puis un "getDuration" : il retourne encore 0 ?!
     * Je garde le "matériel" si on décide de revenir à cette solution
     *
    int m_soundDuration;
    AbulEduMediaMedias(const QString &fileNameMedia="", const QString &fileNameSound="", const QString &text="") :
        AbulEduMediaMediasImage(fileNameMedia), AbulEduMediaMediasText(text), AbulEduMediaMediasSound(fileNameSound), m_soundDuration(0) { }
    qint64 abeMediaMediaGetSoundDuration() {qDebug() << m_soundDuration ; return m_soundDuration;}
    void abeMediaMediaSetSoundDuration(qint64 duration); */
};

QDebug operator<<(QDebug dbg, const AbulEduMediaMedias &param);

#endif // ABULEDUMEDIAMEDIAS_H
