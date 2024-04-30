/** Classe AbulEduMediaGraphicsButtonV1
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

#ifndef ABULEDUMEDIAGRAPHICSBUTTONV1_H
#define ABULEDUMEDIAGRAPHICSBUTTONV1_H

#include <QPainter>
#include <QGraphicsObject>
#include "abuledumultimediawidgetcontainer.h"
#include "abuleduloggerv1.h"

class AbulEduMediaGraphicsButtonV1 : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit AbulEduMediaGraphicsButtonV1(QGraphicsItem *parentForButton, AbulEduMultiMediaWidgetContainer *parentForMultiMedia,
                                          QList<AbulEduMediaMedias> listMedias,
                                          AbulEduPicottsV1::AbulEduLang language = AbulEduPicottsV1::None,
                                          bool isTextVisible = true);

    /** Mutualisation de code pour les deux constructeurs */
    void initAbulEduMediaGraphicsButton(QGraphicsItem *parentForButton);

    ~AbulEduMediaGraphicsButtonV1();

    /** Fixe la taille du bouton
     * @param size QSize des dimensions du bouton
     */
    void abeMediaGraphicsButtonSetFixedSize(QSize size);

    /** Fixe la taille du bouton
     * @param width  largeur du bouton en pixel
     * @param height hauteur du bouton en pixel
     */
    void abeMediaGraphicsButtonSetFixedSize(int width, int height);

    inline QSize abeMediaGraphicsButtonGetSize() {return m_buttonSize;}

    /** Retourne un pointeur vers le AbulEduMultiMediaV1
     * return Pointeur vers AbulEduMultiMediaV1
     **/
private:
    QList<AbulEduMediaMedias>  m_medias;
    AbulEduPicottsV1::AbulEduLang m_language;
    AbulEduMultiMediaV1       *m_multiMedia;
    bool                       m_textIsVisible;
    QWidget                   *m_parentPlayer;
    bool                       m_isNormalState;
    QSize                      m_buttonSize;
    QString                    m_pathPixmapNormal;
    QPixmap                    m_pixMapNormal;
    QString                    m_pathPixmapHover;
    QPixmap                    m_pixMapHover;

signals:
    /** Ce signal est émis lorsque le bouton droit de la souris est relâché */
    void signalAbeGraphicsButtonClicDroitReleased(QPoint ButtonCenterPosition);

    /** Ce signal est émis lorsque le bouton gauche de la souris est relâché */
    void signalAbeGraphicsButtonClicGaucheReleased(QPoint ButtonCenterPosition);

    /// Signal émis lors de la création du multimediawidget
    void signalAbeGraphicsButtonMultiMediaWidgetCreated(AbulEduMultiMediaV1* multimediawidget);

public slots:
protected slots:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private slots:
    void slotAbeCreateMultiMedia();
};

#endif // ABULEDUMEDIAGRAPHICSBUTTONV1_H
