/** Classe AbulEduSplashScreenV1
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author http://developer.qt.nokia.com/wiki/QSplashScreen_replacement_for_semitransparent_images
  * @see The GNU Public License (GNU/GPL) v3
  * @see cc-by-sa for code from qt.nokia.com
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

#ifndef ABULEDUSPLASHSCREENV1_H
#define ABULEDUSPLASHSCREENV1_H

#include <QDesktopWidget>
#include <QProgressBar>
#include <QPainter>
#include <QBitmap>
#include <QWidget>
#include <QPushButton>
#include <QCloseEvent>
#include <QPointer>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QLabel>
#include "abuleduflatboutonv1.h"
#include "abuleduapplicationv1.h"
#include "abuleduidentitesv1.h"
#include "abuleduloggerv1.h"
#include "version.h"

/**
* La classe AbulEduSplashScreen permet d'avoir des splashscreens avec un canal alpha
* ... suite à la demande graphique d'Arnaud, forcément ! :)
*/

class AbulEduSplashScreenV1 : public QWidget
{
    Q_OBJECT

public:

    /** Par defaut le constructeur du mode tablette est plein ecran, pour les autres cas on est en maximized,
     * pour les utilitaires on passera Qt::WindowNoState */
#ifdef __ABULEDUTABLETTEV1__MODE__
    AbulEduSplashScreenV1(QWidget *parent=0, bool askAuth = false, Qt::WindowStates showMode = Qt::WindowFullScreen);
#else
    AbulEduSplashScreenV1(QWidget *parent=0, bool askAuth = false, Qt::WindowStates showMode = Qt::WindowMaximized);
#endif
    ~AbulEduSplashScreenV1();
    void clearMessage();
    void showMessage(const QString& message, int alignment = Qt::AlignLeft, const QColor& color = Qt::black);
    void setMainWindow(QWidget *mainWindow);
    void launch(int msec);
    void abeSplashScreenLauchUtilitaire(bool trueFalse);

signals:
    void signalSessionAuthenticated(bool enable);
    void signalAbeSplashScreenLaunched();

public slots:
    void show();

private slots:
    void finish();
    void on_btnAuthenticated_clicked();
    void on_btnAnonymous_clicked();
    /* changement de login */
    void slotChangeLogin();
    void slotAfficheIdentite();
    void slotGrandAvatar();
    void slotPetitAvatar();
    void slotFinChargement();
    void updateLoadingMessage();

protected:
    void closeEvent(QCloseEvent* ev);

private:
    virtual void paintEvent(QPaintEvent* pe);

    /* Les AbulEduFlatBoutonV1 ne sont pas prévus pour avoir un border-radius autre qu'identique pour les 4 coins. Je l'avance donc de 8 "en trop" pour ne pas être embêté avec ça */
    static int const m_cornerButton           = 482;
    static int const m_topButtons             = 210;
    static int const m_topButtonsUtilitaire   = 155;
    static int const m_gapButtons             = 5;
    static int const m_gapButtonsUtilitaire   = 40;

    AbulEduFlatBoutonV1 *m_btnAuthenticated;
    AbulEduFlatBoutonV1 *m_btnAnonymous;
#ifndef __ABULEDUPRODUCTIONRELEASE__MODE__
    QPixmap             m_pixmapBeta;//l'image a utiliser
#endif
    QPixmap             m_pixmap;    //l'image a utiliser
    QString             m_message;   //le texte a afficher
    QColor              m_color;     //la couleur du texte
    QFont               m_buttonFont;
    QPointer<QWidget>   m_mainWindow; //la fenetre principale pour qu'on puisse mourrir en paix
    QLabel*             m_labelPhoto;
    Qt::WindowStates    m_showMode;
    int                 m_alignment; //l'alignement
    bool                m_askAuth;
    bool                m_flagEnd; //un flag pour la gestion de la fin du splash (concurrence entre alacarte propagation d'id et timeout)
    /** Permet de tester si le logiciel qu'on va lancer fait partie de la famille des utilitaires d'AbulÉdu */
    bool                m_isUtilitaire;
    int                 m_msec;
    QTimer              *m_timer;
    double              m_percent;
};

#endif // ABULEDUSPLASHSCREENV1_H
