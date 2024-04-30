/** Classe AbulEduMenuFeuilleV1
  * @author 2014 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#ifndef ABULEDUMENUFEUILLEV1_H
#define ABULEDUMENUFEUILLEV1_H

#include <QMainWindow>
#include <QGridLayout>
#include <QShortcut>
#include <QSettings>
#include <QDebug>
#include <QLabel>
#include <QFrame>

#include <abuleduapplicationv1.h>
#include "abuledunetworkaccessmanagerv1.h"
#include <abuleduflatboutonv1.h>
#include "abuleduloggerv1.h"

#ifdef Q_OS_ANDROID
    #include <QMenuBar>
    #include <QMenu>
#endif

namespace Ui {
class AbulEduMenuFeuilleV1;
}

class AbulEduMenuFeuilleV1 : public QFrame
{
    Q_OBJECT

public:
    explicit AbulEduMenuFeuilleV1(QWidget *parent = 0);
    inline void slotAbeMenuFeuilleClickOnAvatar() {on_abeMenuFeuilleBtnAvatar_clicked();}
    void abeMenuFeuilleSetTitle(const QString &title);
    QString abeMenuFeuilleGetCurrentLang();
    ///
    /// \brief Permet de préciser la largeur de la frame buttons. La hauteur normalement n'est pas à modifier puisqu'elle est celle des boutons (à une marge près)
    /// \param w la largeur souhaitée en pixels
    ///
    void abeMenuFeuilleSetButtonsFrameWidth(int w);
    void abeMenuFeuilleSetGap(int leftGap);
    ~AbulEduMenuFeuilleV1();

    void abeMenuFeuilleMoveButtonsFrame(int x, int y);
    bool isWindowMovingAllowed() const;
    void abeMenuFeuilleSetIsWindowMovingAllowed(bool isWindowMovingAllowed);

    
private:
    Ui::AbulEduMenuFeuilleV1 *ui;
    QMainWindow *m_parentMainWindow;
    QStringList m_listeComposants;

    /** Position de la souris pour gerer le deplacement de la fenetre */
    QPoint m_dragPosition;
    bool   m_isWindowMoving;
    bool   m_isWindowMovingAllowed;

    void showEvent(QShowEvent *);
#ifndef __ABULEDUTABLETTEV1__MODE__
    void resizeEvent( QResizeEvent *);
#endif
    bool eventFilter(QObject *, QEvent *event);

private slots:

#ifndef __ABULEDUTABLETTEV1__MODE__
    /** sans le on_ au debut pour eviter le connect implicite */
    void abeMenuFeuilleBtnFeuille_mouseOver();
    void on_abeMenuFeuilleBtnFullScreen_clicked();
    void on_abeMenuFeuilleBtnMinimized_clicked();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
#endif
    /** Slots correspondant aux boutons de la frmTop */
    void on_abeMenuFeuilleBtnFeuille_clicked();

    void on_abeMenuFeuilleBtnLanguage_clicked();
    void on_abeMenuFeuilleBtnNew_clicked();
    void on_abeMenuFeuilleBtnOpen_clicked();
    void on_abeMenuFeuilleBtnSave_clicked();
    void on_abeMenuFeuilleBtnPublish_clicked();
    void on_abeMenuFeuilleBtnImport_clicked();
    void on_abeMenuFeuilleBtnExport_clicked();
    void on_abeMenuFeuilleBtnPrint_clicked();
    void on_abeMenuFeuilleBtnSettings_clicked();
    void on_abeMenuFeuilleBtnCustom1_clicked();
    void on_abeMenuFeuilleBtnCustom2_clicked();
    void on_abeMenuFeuilleBtnHelp_clicked();
    void on_abeMenuFeuilleBtnQuit_clicked();
    void on_abeMenuFeuilleBtnFr_clicked();
    void on_abeMenuFeuilleBtnEn_clicked();
    void on_abeMenuFeuilleBtnEs_clicked();
    void on_abeMenuFeuilleBtnIt_clicked();
    void on_abeMenuFeuilleBtnDe_clicked();
    void on_abeMenuFeuilleBtnNl_clicked();
    void on_abeMenuFeuilleBtnOc_clicked();
    void on_abeMenuFeuilleBtnLangueAnnuler_clicked();
    void on_abeMenuFeuilleBtnAvatar_clicked();
    void on_abeMenuFeuilleBtnLogout_clicked();
    void on_abeMenuFeuilleBtnLogin_clicked();
    void on_abeMenuFeuilleBtnUserAnnuler_clicked();

    /** Provoque la traduction de l'interface du menu Feuille, le changement de drapeau l'émission par un signal du paramètre reçu
     * @param lang est le code ISO 639-1 de la langue */
    void slotAbeMenuFeuilleChangeLanguage(const QString &lang);
    /** Récupère la photo de l'AbulEduIdentitesV1 connectée
     * @param authenticated n'est pas utilisé */
    void slotAbeMenuFeuilleUpdateAvatar(int authenticated);

    void calculatePosFrmUserMenu();
signals:
    /** Signal émis au changement de langue
     * @param le code ISO 639-1 de la langue */
    void signalAbeMenuFeuilleChangeLanguage(const QString&);
};

#endif // ABULEDUMENUFEUILLEV1_H
