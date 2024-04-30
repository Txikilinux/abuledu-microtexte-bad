/** Classe AbulEduMessageBoxV1
  *
  * @author 2012-2016 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2013 Éric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
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

#ifndef ABULEDUMESSAGEBOX_H
#define ABULEDUMESSAGEBOX_H

#include <QMessageBox>
#include <QPainter>
#include <QBitmap>
#include "abuleduapplicationv1.h"
#include "abuleduflatboutonv1.h"
#include "ui_abuledumessageboxv1.h"
#include "abuleduloggerv1.h"
#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
    #include "abuledumultimediav1.h"
#endif

namespace Ui {
class AbulEduMessageBoxV1;
}

class AbulEduMessageBoxV1 : public QWidget
{
    Q_OBJECT
    Q_ENUMS(enumAbulEduMessageBoxMode
            enumAbulEduMessageBoxButtonClicked)

    Q_PROPERTY(enumAbulEduMessageBoxMode mode READ abeGetModeEnum WRITE abeSetModeEnum USER true)

public:
    enum enumAbulEduMessageBoxMode {abeNoButton, abeCloseButton, abeAcceptCancelButton, abeAcceptCancelCloseButton, abeYesNoButton, abeYesNoCancelButton, abeOKButton};

    enum enumAbulEduMessageBoxButtonClicked {Accept,      // Ok
                                             Reject,      // Cancel
                                             CloseOrHide, // CloseOrHide
                                             Yes,         // Yes
                                             No           // No
                                            };
    /** Construit une fenêtre de message avec un titre, un icône, un texte, un comportement attendu au masquage, et un parent
      * L'icône est une tête étonnée par défaut, mais peut être remplacée par une tête qui fait un clin d'oeil pour la méthode setWink()
      * Par défaut l'objet est fermé (et donc détruit) quand on clique sur Fermer, Annuler ou OK mais si on souhaite en faire une variable membre on peut préférer le cacher
      * L'objet a par défaut un bouton Fermer, mais on peut l'afficher sans bouton ou avec des boutons Annuler et OK par la méthode abeSetModeEnum
      */
    explicit AbulEduMessageBoxV1(const QString &titre = trUtf8("Problème"),
                                 const QString &texte = trUtf8("Impossible de continuer"),
                                 bool doYouWantToClose = true,
                                 QWidget *parent = 0);

    /** Destructeur */
    ~AbulEduMessageBoxV1();

#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
    ///
    /// \brief Ajoute un AbulEduMultiMediaV1 à la l'AbulEduMessageBoxV1, ce qui permet (en particulier pour les tout-petits) de lire ce qui est écrit dedans...
    /// \param oggPath est le chemin d'un fichier enregistré et embarqué dans l'application qui contient la lecture du texte écrit (vu que le texte est écrit par le développeur, on peut
    /// avoir un fichier audio sans crainte qu'il ne corresponde pas au texte). Si le chemin est vide, on a une lecture du texte écrit en synthèse vocale.
    ///
    void abeMessageBoxSetMultimedia(QString oggPath = QString());
#endif

    /** Change la couleur du titre
      * @param color : la couleur choisie en hexadécimal, rgb ou noms CSS */
    void setTitleColor(const QColor &color);

    ///
    /// \brief Permet de modifier la taille des caractères du titre
    /// \param pointSize la taille souhaitée en pt
    ///
    void abeMessageBoxSetTitleFontPointSize(int pointSize);

    /** Change la couleur du texte
      * @param color : la couleur choisie en hexadécimal, rgb ou noms CSS */
    void setTextColor(const QColor &color);

    /** Change la couleur du fond
      * @param color : la couleur choisie en hexadécimal, rgb ou noms CSS */
    void setBackgroundColor(const QColor &color);

    /** Affecte l'icône "clin d'oeil" à la message box pour les utilisations en vue d'aide */
    void setWink(bool trueFalse = true);

    /** Affecte l'icône iconPath à la message box pour des utilisations particulières */
    void abeMessageBoxSetSpecialIcon(const QString& iconPath);

    /** Contrôle l'affichage des boutons
      * @param mode : le mode choisi parmi abeNoButton (aucun bouton affiché), abeCloseButton (bouton Fermer, comportement par défaut),
      * abeAcceptCancelButton (un bouton Annuler et un bouton OK) et abeAcceptCancelCloseButton (les trois boutons présents)  */
    void abeSetModeEnum(enumAbulEduMessageBoxMode mode);

    /** Retourne le mode correspond à l'affichage des boutons en cours */
    inline enumAbulEduMessageBoxMode abeGetModeEnum() const {return m_mode;}

    /// \brief abeMessageBoxGetText
    /// \return le texte de l'AbulEduMessageBoxV1
    inline QString abeMessageBoxGetText() const {return ui->lblTexte->text();}

    /// \brief abeMessageBoxGetTextFont
    /// \return la police de caractère du texte de l'AbulEduMessageBoxV1
    inline QFont abeMessageBoxGetTextFont() const {return ui->lblTexte->font();}

    /// \brief abeMessageBoxSetTextFont
    /// \param font la police de caractère souhaitée pour le texte de l'AbulEduMessageBoxV1
    inline void abeMessageBoxSetTextFont(const QFont &font){ui->lblTexte->setFont(font);}

    /// \brief abeMessageBoxGetTextAlignment
    /// \return l'alignement du texte de l'AbulEduMessageBoxV1
    inline Qt::Alignment abeMessageBoxGetTextAlignment() const {return ui->lblTexte->alignment();}

    /// \brief abeMessageBoxSetTextAlignment
    /// \param align l'alignement souhaité pour le texte de l'AbulEduMessageBoxV1
    inline void abeMessageBoxSetTextAlignment(const Qt::Alignment &align){ui->lblTexte->setAlignment(align);}

    /// \brief abeMessageBoxGetTitleFont
    /// \return la police de caractère du titre de l'AbulEduMessageBoxV1
    inline QFont abeMessageBoxGetTitleFont() const {return ui->lblTitre->font();}

    /// \brief abeMessageBoxSetTitleFont
    /// \param font la police de caractère souhaitée pour le titre de l'AbulEduMessageBoxV1
    inline void abeMessageBoxSetTitleFont(const QFont &font){ui->lblTitre->setFont(font);}

    /// \brief abeMessageBoxGetTitle
    /// \return le titre de l'AbulEduMessageBoxV1
    inline QString abeMessageBoxGetTitle() const {return ui->lblTitre->text();}

    ///
    /// \brief Getteur de la QFrame (vide à la construction) qui permet d'afficher des éléments supplémentaires dans l'AbulEduMessageBoxV1
    /// \return le pointeur
    ///
    inline QFrame* abeMessageBoxGetCustomFrame(){return ui->abeMessageBoxCustomFrame;}

    ///
    /// \brief abeMessageBoxGetWink
    /// \return true si la tête à afficher est celle avec le clin d'oeil
    ///
    bool abeMessageBoxGetWink();

    ///
    /// \brief Redessine l'AbulEduMessageBoxV1
    /// \param newTitle le titre à afficher
    /// \param newText le texte à afficher
    /// \param isWink true si on veut le clin d'oeil
    ///
    void abeMessageBoxUpdate(const QString &newTitle, const QString &newText, bool isWink = false);

    ///
    /// \brief Gestion des boutons par les touches Entrée ou Esc
    ///
    bool eventFilter(QObject *obj, QEvent *event);

    /** @deprecated */
    static void showInformation(const QString& text, QWidget* parent = 0, const QString& title = trUtf8("Problème"), bool doYouWantToClose = true);

    static void abeMessageBoxShowInformation(const QString& text, QWidget* parent = 0, const QString& title = trUtf8("Problème"), bool isWink = false, bool doYouWantToClose = true);

protected:
    ///
    /// \brief Getteur de la QFrame principale de l'AbulEduMessageBoxV1
    /// \return le pointeur
    ///
    inline QFrame* abeMessageBoxGetMainFrame() {return ui->mainFrame;}

private:
    Ui::AbulEduMessageBoxV1 *ui;
    QWidget* m_parent;
    QString m_titleColor;
    bool premierAffichage;
    bool m_hasToBeClosed;
    /** Fichier de paramètres */
    QSettings *m_config;
    AbulEduFlatBoutonV1* m_lastClicked;
    enumAbulEduMessageBoxMode m_mode;
    bool m_isWink;

protected slots:
    virtual void on_btnOK_clicked();

private slots:
    void on_btnFermer_clicked();
    void on_btnAnnuler_clicked();
    void on_btnOui_clicked();
    void on_btnNon_clicked();
    AbulEduFlatBoutonV1* getLastClicked();
    void setLastClicked();

signals:
    void signalAbeMessageBoxCloseOrHide();
    void signalAbeMessageBoxCancel();
    void signalAbeMessageBoxOK();
    void signalAbeMessageBoxYES();
    void signalAbeMessageBoxNO();
    void signalAbeInputBoxOK(const QString &);
    void signalAbeMessageBoxButtonClicked(AbulEduMessageBoxV1::enumAbulEduMessageBoxButtonClicked );
};

#endif // ABULEDUMESSAGEBOX_H
