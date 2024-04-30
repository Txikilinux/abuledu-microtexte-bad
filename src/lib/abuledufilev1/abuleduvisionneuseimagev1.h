/** Classe AbulEduVisionneuseImageV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Icham Sirat <icham.sirat@ryxeo.com>
  * @author 2012 Éric Seigne <eric.seigne@ryxeo.com>
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
  * with this program. If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef ABULEDUVISIONNEUSEIMAGE_H
#define ABULEDUVISIONNEUSEIMAGE_H

#include <QMainWindow>
#include <QUrl>
#include <QPixmap>
#include <QLabel>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif
#include <QPrintDialog>
#include <QScrollArea>
#include <QScrollBar>
#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QtGui>
#include "abuledunetworkaccessmanagerv1.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class AbulEduVisionneuseImageV1 : public QMainWindow
{
    Q_OBJECT

public:
    AbulEduVisionneuseImageV1(QWidget *parent = 0, bool closeOnPress=true);
    ~AbulEduVisionneuseImageV1();

    /** Ouvre une image locale */
    void ouvrirFicher(const QString &chemin);

    /** Téléchargement d'une image */
    void ouvrirFicher(const QUrl &uri);

    /** Réglage en millisecondes du temps maximum d'affichage de la visionneuse
      * @note Valeur par défaut : 5000
      * @note Valeur 0 ou < pour stopper le temps d'affichage
      */
    void abeVisionneuseImageSetTimeout(int ms);

    /**
      * @brief fixeTempsAffichageMax
      * @param ms
      * @deprecated
      */
    void fixeTempsAffichageMax(int ms);
    inline void abeVisionneuseImageSetDeleteOnClose(Qt::WidgetAttribute attribute) {setAttribute(attribute);}

public slots:
    void ajusterFenetreAuMieux();
    void ouvrir();

private slots:
    void imprimer();
    void zoomIn();
    void zoomOut();
    void tailleNormale();
    void ajusterFenetre();
    void aPropos();
    void slotUrlDownloaded(QNetworkReply* reply);

private:
    void creerActions();
    void creerMenus();
    void majActions();
    void scaleImage(double facteur);
    void adjustScrollBar(QScrollBar *scrollBar, double facteur);
    void mouseReleaseEvent(QMouseEvent *event);

    QLabel      *m_imageLabel;
    QScrollArea *m_scrollArea;
    QWidget     *m_parent;
    double      m_scaleFactor;

    QAction *m_action_ouvrir;
    QAction *m_action_imprimer;
    QAction *m_action_quitter;
    QAction *m_action_zoomIn;
    QAction *m_action_zoomOut;
    QAction *m_action_tailleNormal;
    QAction *m_action_ajusterFenetre;
    QAction *m_action_aPropos;
    QAction *m_action_aProposQt;

    QMenu *m_menu_fichier;
    QMenu *m_menu_vue;
    QMenu *m_menu_aide;

    /** La liste des network reply que je suis sensé gérer ... */
    QList<QNetworkReply *> m_listNetworkReply;

    /** QTimer utilisé par défaut pour éviter le double-clic. On peut grâce à lui définir une durée d'affichage maximum. */
    QTimer* m_timer;
    bool    m_isTimerUsed; /** permet de desactiver l'utilisation du timer */
    bool    m_closeOnPress;

#ifndef QT_NO_PRINTER
    QPrinter* m_printer;
#endif
};

#endif // ABULEDUVISIONNEUSEIMAGE_H
