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

#ifndef ABULEDUMEDIABUTTONV1_H
#define ABULEDUMEDIABUTTONV1_H

#include <QSize>
#include "abuledumultimediav1.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

///
/// \brief La classe AbulEduMediaButtonV1 ajoute un bouton sur un widget permettant de lire un objet multimedia
///
class AbulEduMediaButtonV1 : public AbulEduFlatBoutonV1
{
    Q_OBJECT
public:
    /** Constructeur du médiaBouton
     * @param parentForButton le widget sur lequel sera greffé le bouton
     * @param parentForMultiMedia le widget dans lequel sera inséré l'AbulEduMultiMediaV1
     * @param listMedias une liste de AbulEduMediaMedias (chemin vers les composants du média)
     */
    explicit AbulEduMediaButtonV1(QWidget *parentForButton, QWidget *parentForMultiMedia,
                                  QList<AbulEduMediaMedias> listMedias,
                                  AbulEduPicottsV1::AbulEduLang language = AbulEduPicottsV1::None,
                                   bool isTextVisible = true);

    /** Mutualisation de code pour les deux constructeurs */
    void initAbulEduMediaButton(QWidget *parentForButton);

    ~AbulEduMediaButtonV1();
    /** Fixe la taille du bouton
     * @param size QSize des dimensions du bouton
     */
    void abeMediaButtonSetFixedSize(QSize size);

    /** Fixe la taille du bouton
     * @param width  largeur du bouton en pixel
     * @param height hauteur du bouton en pixel
     */
    void abeMediaButtonSetFixedSize(int width, int height);

private:
    QList<AbulEduMediaMedias>  m_medias;
    QSize                      m_buttonSize;
    AbulEduPicottsV1::AbulEduLang     m_language;
    bool                       m_textIsVisible;
    AbulEduMultiMediaV1       *m_multiMedia;
    QWidget                   *m_parentPlayer;

signals:
    /// Signal émis lors de la création du multimediawidget
    void signalAbeButtonMultiMediaWidgetCreated(AbulEduMultiMediaV1* multimediawidget);
public slots:

private slots:
    void slotAbeShowMultiMedia();

};

#endif // ABULEDUMEDIABUTTONV1_H
