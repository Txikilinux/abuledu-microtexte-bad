/** Bouton plat et transparent, developpé initialement pour le projet
  * Imageo (le-terrier d'AbulEdu) et repris dans les classes de base
  * d'AbulÉdu (merci Thomas !)
  *
  * Utilisation: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFlatBoutonV1
  *
  * #AbulEduFlatBoutonV1
  * include(lib/abuleduflatboutonv1/abuleduflatboutonv1.pri)
  *
  * @see https://redmine.ryxeo.com/projects/leterrier-imageo
  * @author 2011 Thomas Grosjean-Pasut <thomas.grosjeanpasut@ryxeo.com>
  * @author 2011-2013 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  *
  * @see The GNU Public License (GPL)
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  *  for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef ABULEDUFLATBOUTONV1_H
#define ABULEDUFLATBOUTONV1_H

#include <QPushButton>
#include <QGraphicsSceneMouseEvent>
#include <QBitmap>
#include <QFile>
#include <QTimer>
#include <QApplication>
#include <QtCore/qmath.h>
#include <QMouseEvent>
#include "abuleduloggerv1.h"

class AbulEduFlatBoutonV1 : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(QColor CouleurTexteNormale  READ getCouleurTexteNormale  WRITE setCouleurTexteNormale  USER true)
    Q_PROPERTY(QColor CouleurTexteSurvol   READ getCouleurTexteSurvol   WRITE setCouleurTexteSurvol   USER true)
    Q_PROPERTY(QColor CouleurTextePressed  READ getCouleurTextePressed  WRITE setCouleurTextePressed  USER true)
    Q_PROPERTY(QColor CouleurTexteDisabled READ getCouleurTexteDisabled WRITE setCouleurTexteDisabled USER true)

    Q_PROPERTY(Qt::Alignment TexteAlignement READ getTexteAlignement WRITE setTexteAlignement USER true)
    Q_PROPERTY(QString BackgroundAlignement READ getBackgroundAlignement WRITE setBackgroundAlignement USER true)
    Q_PROPERTY(Qt::Alignment AbeFlatBoutonBackgroundAlignement READ abeFlatBoutonGetBackgroundAlignement WRITE abeFlatBoutonSetBackgroundAlignement USER true)

    Q_PROPERTY(qreal TextePaddingLeft READ getTextePaddingLeft WRITE setTextePaddingLeft USER true)
    Q_PROPERTY(int AbeFlatBoutonFontSize READ abeFlatBoutonGetFontSize WRITE abeFlatBoutonSetFontSize USER true)

    Q_PROPERTY(QColor CouleurFondNormale  READ getCouleurFondNormale  WRITE setCouleurFondNormale  USER true)
    Q_PROPERTY(QColor CouleurFondSurvol   READ getCouleurFondSurvol   WRITE setCouleurFondSurvol   USER true)
    Q_PROPERTY(QColor CouleurFondPressed  READ getCouleurFondPressed  WRITE setCouleurFondPressed  USER true)
    Q_PROPERTY(QColor CouleurFondDisabled READ getCouleurFondDisabled WRITE setCouleurFondDisabled USER true)

    Q_PROPERTY(QString IconeNormale  READ getIconeNormale  WRITE setIconeNormale  USER true)
    Q_PROPERTY(QString IconeSurvol   READ getIconeSurvol   WRITE setIconeSurvol   USER true)
    Q_PROPERTY(QString IconePressed  READ getIconePressed  WRITE setIconePressed  USER true)
    Q_PROPERTY(QString IconeDisabled READ getIconeDisabled WRITE setIconeDisabled USER true)

    Q_PROPERTY(int MarginTop    READ getMarginTop     WRITE setMarginTop  USER true)
    Q_PROPERTY(int MarginRight  READ getMarginRight   WRITE setMarginRight  USER true)
    Q_PROPERTY(int MarginBottom READ getMarginBottom  WRITE setMarginBottom   USER true)
    Q_PROPERTY(int MarginLeft   READ getMarginLeft    WRITE setMarginLeft USER true)

    Q_PROPERTY(int BorderRadius   READ getBorderRadius    WRITE setBorderRadius USER true)

    /* @deprecated : toutes les proprietes sont avec une majuscule au debut sauf celle-ci ... */
    Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius USER true)

public:
    AbulEduFlatBoutonV1(QWidget* parent = 0);
    ~AbulEduFlatBoutonV1();

    /** couleur normale du texte */
    void setCouleurTexteNormale(const QColor& couleur);
    inline void setCouleurTexteNormale(const QString& couleur) {setCouleurTexteNormale(QColor(couleur));}
    QColor getCouleurTexteNormale();

    /** couleur du texte lors du survol */
    void setCouleurTexteSurvol(const QColor& couleur);
    inline void setCouleurTexteSurvol(const QString& couleur) {setCouleurTexteSurvol(QColor(couleur));}
    QColor getCouleurTexteSurvol();

    /** couleur du texte lorsqu'on appuie sur le bouton */
    void setCouleurTextePressed(const QColor& couleur);
    inline void setCouleurTextePressed(const QString& couleur) {setCouleurTextePressed(QColor(couleur));}
    QColor getCouleurTextePressed();

    /** couleur du texte lorsque le bouton est disabled */
    void setCouleurTexteDisabled(const QColor& couleur);
    inline void setCouleurTexteDisabled(const QString& couleur) {setCouleurTexteDisabled(QColor(couleur));}
    QColor getCouleurTexteDisabled();

    /** permet d'affecter les couleurs en un seul appel */
    void setCouleursTexte(const QColor& normal, const QColor& survol = QColor(QColor::Invalid),
                          const QColor& pressed = QColor(QColor::Invalid), const QColor& disabled = QColor(QColor::Invalid));

    /** alignement du texte */
    void setTexteAlignement(const Qt::Alignment& alignement = Qt::AlignHCenter);
    const Qt::Alignment& getTexteAlignement();

    /** Taille des caractères du texte */
    void abeFlatBoutonSetFontSize(int pointSize);
    inline int abeFlatBoutonGetFontSize(){return m_fontSize;}

    /** alignement du background */
    void setBackgroundAlignement(const QString& alignement = "");
    const QString& getBackgroundAlignement();
    /** alignement du background mais avec des Q_Alignment */
    void abeFlatBoutonSetBackgroundAlignement(Qt::Alignment alignement = Qt::AlignLeft | Qt::AlignTop);
    Qt::Alignment abeFlatBoutonGetBackgroundAlignement();

    /** écartement à la bordure gauche du texte */
    void setTextePaddingLeft(const qreal& padding);
    qreal getTextePaddingLeft();

    /** écartement à la bordure droite du texte */
    void setTextePaddingRight(const qreal& padding);
    qreal getTextePaddingRight();

    void setTextePadding(const qreal& left, const qreal& top, const qreal& right, const qreal& bottom);

    /** la couleur de fond du bouton lorsque son état est normal */
    void setCouleurFondNormale(const QColor& couleur);
    inline void setCouleurFondNormale(const QString& couleur) {setCouleurFondNormale(QColor(couleur));}
    QColor getCouleurFondNormale();

    /** la couleur de fond du bouton lors du survol */
    void setCouleurFondSurvol(const QColor& couleur);
    inline void setCouleurFondSurvol(const QString& couleur) {setCouleurFondSurvol(QColor(couleur));}
    QColor getCouleurFondSurvol();

    /** la couleur de fond du bouton lors du clic */
    void setCouleurFondPressed(const QColor& couleur);
    inline void setCouleurFondPressed(const QString& couleur) {setCouleurFondPressed(QColor(couleur));}
    QColor getCouleurFondPressed();

    /** la couleur de fond du bouton quand il est disabled */
    void setCouleurFondDisabled(const QColor& couleur);
    inline void setCouleurFondDisabled(const QString& couleur) {setCouleurFondDisabled(QColor(couleur));}
    QColor getCouleurFondDisabled();

    /** permet d'affecter les couleurs de fond en un seul appel */
    void setCouleursFond(const QColor& normal, const QColor& survol = QColor(QColor::Invalid),
                         const QColor& pressed = QColor(QColor::Invalid),const QColor& disabled = QColor(QColor::Invalid));

    /** l'icone du bouton normal */
    void setIconeNormale(const QString& normale);
    QString getIconeNormale();

    /** l'icone du bouton lors du survol */
    void setIconeSurvol(const QString& survol);
    QString getIconeSurvol();

    /** l'icone du bouton cliqué */
    void setIconePressed(const QString& pressed);
    QString getIconePressed();

    /** l'icone du bouton disabled */
    void setIconeDisabled(const QString& disabled);
    QString getIconeDisabled();

    /** affectation des icones en un seul appel */
    void setIcones(const QString& normale, const QString& survol = "",
                   const QString& pressed = "", const QString& disabled = "" );

    /** Permet de passer une feuille de style au AbulEduFlatBouton
      * @warning Attention, tout appel à une autre fonction détruira cette feuille de style !
      * @warning La feuille de style passée doit être valide : il n'y a pas de parsing préalable !
      */
    void setCSSFlatBouton(const QString& css);
    void setDisableTimeout(int msec);

    /** Fixe des écarts entre le bouton et son contenant (propriété CSS margin) */
    void setAllMargins(int top,int right, int bottom,int left);

    inline void setMarginTop(int value) {m_marginTop = value; m_newCSSAvailable = true; updateCSS();}
    inline int getMarginTop() {return m_marginTop;}

    inline void setMarginRight(int value ) {m_marginRight = value; m_newCSSAvailable = true; updateCSS();}
    inline int getMarginRight() {return m_marginRight;}

    inline void setMarginBottom(int value) {m_marginBottom = value; m_newCSSAvailable = true; updateCSS();}
    inline int getMarginBottom() {return m_marginBottom;}

    inline void setMarginLeft(int value) {m_marginLeft = value; m_newCSSAvailable = true; updateCSS();}
    inline int getMarginLeft() {return m_marginLeft;}

    /** Fixe un écart entre le bouton et son contenant (propriété CSS margin) */
    void setSameMargins(int margin);

    /** Fixe un arrondi en pixels pour la bordure */
    void setBorderRadius(int radius);
    int getBorderRadius();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    /** Map contenant les couleurs sous forme de QString rgba
      * prêtes à inclure dans une feuille de style : "(int r, int g, int b, int a)"
      */
    QMap<QString , QString> *m_couleursCSS;

    /** QMap contenant les chemins vers les fichiers d'image-icone */
    QMap<QString, QString> *m_icones;

    /** Contient la feuille de style des AbulEduFlatBoutonV1 */
    QString m_cssFlatBouton;

    /** Contient l'espacement entre la bordure et le texte (padding) à gauche */
    int m_paddingLeft;

    /** Contient l'espacement entre la bordure et le texte (padding) à droite */
    int m_paddingRight;

    /** Contient l'espacement entre la bordure et le texte (padding) en haut */
    int m_paddingTop;

    /** Contient l'espacement entre la bordure et le texte (padding) en bas */
    int m_paddingBottom;

    /** Contient l'espacement entre le bouton et son conteneur (margin) à gauche */
    int m_marginLeft;
    /** Contient l'espacement entre le bouton et son conteneur (margin) à droite */
    int m_marginRight;
    /** Contient l'espacement entre le bouton et son conteneur (margin) en haut */
    int m_marginTop;
    /** Contient l'espacement entre le bouton et son conteneur (margin) en bas */
    int m_marginBottom;

    /** Contient l'arrondi de la bordure
      * @note valeur unique : Qt semble ne pas interpréter des valeurs différentes pour les 4 coins */
    int m_borderRadius;

    /** L'alignement du texte (css et Qt::Alignment) */
    QString m_texteAlignement;
    Qt::Alignment m_alignement;

    /** La taille des caractères si on souhaite la fixer autre que celle de l'application */
    int m_fontSize;

    /** L'alignement du background */
    QString m_backgroundAlignement;
    Qt::Alignment m_abeFlatBoutonBackgroundAlignment;

    bool m_changeIcon;
    /** Convertit une QColor en QString "(int r, int g, int b, int a)" */
    QString toRGBA(const QColor& couleur);

    /** Convertit une QString "(int r, int g, int b, int a)" en QColor */
    QColor fromRGBA(QString couleur);

    /** Reconstruit la CSS du FlatBoutonV1 */
    void updateCSS();

    QTimer *m_timer;
    void mouseReleaseEvent(QMouseEvent *e);

    /** Flag pour eviter de recalculer la CSS 3 fois quand on fait trois appels a la suite dans le constructeur par exemple
      * on l'utilise dans le showEvent pour savoir s'il faut ou pas mettre à jour la CSS */
    bool m_newCSSAvailable;

    void showEvent(QShowEvent *);

public slots:
    void slotDisableTimeout();
    void slotClicked();

signals:
    void signalSurvolIn();
    void signalSurvolOut();

};

#endif // ABULEDUFLATBOUTONV1_H
