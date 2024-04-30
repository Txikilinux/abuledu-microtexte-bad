/** Classe AbulEduSplashScreenV1
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author http://developer.qt.nokia.com/wiki/QSplashScreen_replacement_for_semitransparent_images
  * @see The GNU Public License (GNU/GPL) v3
  * @see cc-by-sa for code from qt.nokia.com
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduSplashScreenV1
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
  *
  * @note Utilisation possible depuis le main.cpp (exemple1, splashscreen classique)
  * @code
  * #include "version.h"
  * #include "mainwindow.h"
  * #include "abuleduapplicationv1.h"
  * #include "abuledusplashscreenv1.h"
  *
  * int main(int argc, char *argv[])
  * {
  *     AbulEduApplicationV1 a(argc, argv,VER_INTERNALNAME_STR, VER_PRODUCTVERSION_STR, VER_COMPANYDOMAIN_STR, "leterrier");
  *     a.setAbeApplicationLongName(QObject::trUtf8(VER_FILEDESCRIPTION_STR));
  *     MainWindow *w;
  *
  *     AbulEduSplashScreenV1 *splash = new AbulEduSplashScreenV1(0, false);
  *     splash->show();
  *     for (int i = 1; i <= 5; i++)
  *     {
  *         splash->showMessage(QObject::trUtf8("Chargement en cours, merci de patienter ... %1%").arg(QString::number(i*20)), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
  *         //On lance le constructeur de la mainwindows en "arrière plan" lors du 1er passage
  *         //ca permet d'avoir les requetes reseau en arriere plan (mise a jour etc.) c'est "cool" :)
  *         if(i == 1) {
  *             w = new MainWindow(0);
  *             splash->setMainWindow(w);
  *         }
  *         qApp->processEvents();
  *         AbulEduSleeperThread::msleep(1000);
  *     }
  *     w->show();
  *     return a.exec();
  * }
  * @endcode
  * @note Utilisation possible depuis le main.cpp (exemple2: avec les boutons authentifié/anonyme)
  * @code
  * #include "version.h"
  * #include "mainwindow.h"
  * #include "abuleduapplicationv1.h"
  * #include "abuledusplashscreenv1.h"
  *
  * int main(int argc, char *argv[])
  * {
  *     AbulEduApplicationV1 a(argc, argv,VER_INTERNALNAME_STR, VER_PRODUCTVERSION_STR, VER_COMPANYDOMAIN_STR, "leterrier");
  *     a.setAbeApplicationLongName(QObject::trUtf8(VER_FILEDESCRIPTION_STR));
  *     MainWindow *w;
  *
  *     AbulEduSplashScreenV1 *splash = new AbulEduSplashScreenV1(0, true);
  *     splash->show();
  *     for (int i = 1; i <= 5; i++)
  *     {
  *         splash->showMessage(QObject::trUtf8("Chargement en cours, merci de patienter ... %1%").arg(QString::number(i*20)), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
  *         //On lance le constructeur de la mainwindows en "arrière plan" lors du 1er passage
  *         //ca permet d'avoir les requetes reseau en arriere plan (mise a jour etc.) c'est "cool" :)
  *         if(i == 1) {
  *             w = new MainWindow(0);
  *             splash->setMainWindow(w);
  *         }
  *         qApp->processEvents();
  *         AbulEduSleeperThread::msleep(1000);
  *     }
  *     w->hide();
  *     return a.exec();
  * }
  *
  * //Et dans l'objet w
  *
  * void MainWindow::slotSessionAuthenticated(bool enable)
  * {
  *      //if (m_localDebug)  //qDebug() << "MainWindow::slotSessionAuthenticated" << enable;
  *     if(enable)
  *         abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
  *     showMaximized();
  * }
  *
  * @endcode
  */

#include "abuledusplashscreenv1.h"

#ifndef DEBUG_ABULEDUSPLASHSCREENV1
#include "abuledudisableloggerv1.h"
#endif

//note: android est un linux donc pour l'exclure il faut le mettre à côté
#if defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
#if QT_VERSION < 0x050000
extern void qt_x11_wait_for_window_manager(QWidget *m_mainWindow);
#endif
#endif

/** Autre constructeur où on indique qu'on souhaite avoir les boutons anonyme/authentifié */
AbulEduSplashScreenV1::AbulEduSplashScreenV1(QWidget *parent, bool askAuth, Qt::WindowStates showMode)
    : QWidget(parent, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint),
      m_btnAuthenticated(0),
      m_btnAnonymous(0),
      #ifndef __ABULEDUPRODUCTIONRELEASE__MODE__
      m_pixmapBeta(QPixmap(":/abuledusplashscreenv1-lib/tampon-beta")),
      #endif
      m_pixmap(QPixmap(":/abuledusplashscreenv1/splashscreen")),
      m_message(""),
      m_color(),
      m_buttonFont(),
      m_mainWindow(0),
      m_labelPhoto(new QLabel(this)),
      m_showMode(showMode),
      m_alignment(0),
      m_askAuth(askAuth),
      m_flagEnd(false),
      m_isUtilitaire(false),
      m_msec(),
      m_timer(0),
      m_percent()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ///@todo: décider (erics 20140216 : je force le passage en mode plein écran pour les android)
#ifdef Q_OS_ANDROID
    m_showMode = Qt::WindowFullScreen;
#endif

    /* En Qt 5.1.1 sous windows ça donne un splash invisible ... il faut donc faire une fenetre "normale" */
#if (QT_VERSION >= 0x050000) && (defined(Q_OS_WIN))
#else
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
#endif
    /* @warning pas de |Qt::SplashScreen : un clic sur le splash provoque sa disparition ! */
    setAttribute(Qt::WA_QuitOnClose, false);

    /* connecte signal de l'application: si elle a un message a afficher il faut qu'il arrive au dessus/alaplace du splashscreen */
    //    connect(abeApp, SIGNAL(signalApplicationMessageStart()), this, SLOT(hide()), Qt::UniqueConnection);
    //    connect(abeApp, SIGNAL(signalApplicationMessageEnd()), this, SLOT(show())  , Qt::UniqueConnection);

    /* 10 secondes pas de réponses => fin du chargement */
    QTimer::singleShot(10000,this,SLOT(slotFinChargement()));
    /* Et si jamais alacarte nous a passé une identité on essaye de la récupérer */
    connect(abeApp->getAbeIdentite(), SIGNAL(signalLoaded()), this, SLOT(slotFinChargement()), Qt::UniqueConnection);

    QFont fonteBoutons(abeApp->font().family(), 22,QFont::DemiBold);
    fonteBoutons.setLetterSpacing(QFont::AbsoluteSpacing,-1);
    m_buttonFont = fonteBoutons;
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedSize(m_pixmap.size());
    /* Evite d'avoir le fond noir quand on est sous linux sans compiz */
    this->setMask(m_pixmap.mask());

    /* On centre le splashscreen */
    abeApp->abeCenterWindow(this);

    setStyleSheet("QPushButton{border-radius:10px;}");
    setWindowTitle(abeApp->applicationName());
    /* Pour fermer le splashscreen à l'emission du signal */
    connect(this,SIGNAL(signalSessionAuthenticated(bool)), this, SLOT(finish()), Qt::UniqueConnection);
}

AbulEduSplashScreenV1::~AbulEduSplashScreenV1()
{
    //ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(m_btnAnonymous)      m_btnAnonymous->deleteLater();
    if(m_btnAuthenticated)  m_btnAuthenticated->deleteLater();
    if(m_labelPhoto)        m_labelPhoto->deleteLater();
    if(m_timer)             m_timer->deleteLater();
}

void AbulEduSplashScreenV1::clearMessage()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_message.clear();
    repaint();
}

void AbulEduSplashScreenV1::showMessage(const QString& message, int alignment/* = Qt::AlignLeft*/, const QColor& color/* = Qt::black*/)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_message  = message;
    m_alignment = alignment;
    m_color  = color;
    repaint();
}

void AbulEduSplashScreenV1::paintEvent(QPaintEvent* pe)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QWidget::paintEvent(pe);
    QRect aTextRect(rect());
    aTextRect.setRect(aTextRect.x(), aTextRect.y()+5, aTextRect.width()-10, aTextRect.height()-10);

    QPainter aPainter(this);
    aPainter.drawPixmap(rect(), m_pixmap);
    if(!m_isUtilitaire){
        QSettings* conf = new QSettings(":/abuledusplashscreenv1/conf",QSettings::IniFormat);
        QFont normalFont = aPainter.font();
        QFont f = abeApp->font();
        f.setPointSize(conf->value("pointsize",20).toInt());
        f.setWeight(QFont::Bold);
        f.setFamily(conf->value("family","Liberation Sans").toString());
        f.setLetterSpacing(QFont::PercentageSpacing,conf->value("percentagespacing",100).toReal());
        aPainter.setPen(conf->value("color", "#000000").toString());
        aPainter.setFont(f);
        aPainter.drawText(QRect(conf->value("abscissa",100).toInt(),conf->value("ordinate",50).toInt(),220,80),abeApp->applicationVersion().left(abeApp->applicationVersion().lastIndexOf(".")));
        aPainter.setFont(normalFont);
    }

#ifndef __ABULEDUPRODUCTIONRELEASE__MODE__ // Mode Beta
    if(m_isUtilitaire) {
        aPainter.drawPixmap(0,0,381,290, m_pixmapBeta);
    }
    else {
        aPainter.drawPixmap(x()/2,50,381,290, m_pixmapBeta);
    }
    aPainter.setPen(QColor(255,25,25,255));
#else
    aPainter.setPen(m_color);
#endif
    aPainter.drawText(aTextRect, m_alignment, m_message);
}

/** Gestion des slots des deux boutons */
void AbulEduSplashScreenV1::on_btnAuthenticated_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setEnabled(false);
    emit signalSessionAuthenticated(true);
    abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
    if(m_mainWindow)
        m_mainWindow->setWindowState(m_showMode);
}

void AbulEduSplashScreenV1::on_btnAnonymous_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setEnabled(false);
    emit signalSessionAuthenticated(false);
    if(m_mainWindow)
        m_mainWindow->setWindowState(m_showMode);
}

void AbulEduSplashScreenV1::slotChangeLogin()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeApp->getAbeNetworkAccessManager()->abeSSOLogout();
    abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
    finish();
}

void AbulEduSplashScreenV1::slotAfficheIdentite()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString id = abeApp->getAbeIdentite()->abeGetNom();
    ABULEDU_LOG_DEBUG() << id;
}

void AbulEduSplashScreenV1::slotGrandAvatar()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() == 1)
    {
        if(m_isUtilitaire)
        {
            QPixmap photoRetaillee(abeApp->getAbeIdentite()->abeGetPhoto()->scaledToWidth(70,Qt::SmoothTransformation));
            m_labelPhoto->setPixmap(photoRetaillee);
            m_labelPhoto->resize(70,70);
            m_labelPhoto->move(219, m_topButtonsUtilitaire+1);
        }
        else
        {
            QPixmap photoRet;
            photoRet = abeApp->getAbeIdentite()->abeGetPhoto()->scaledToWidth(60,Qt::SmoothTransformation);
            m_labelPhoto->setStyleSheet("QLabel { background-color : transparent; }");
            m_labelPhoto->setPixmap(photoRet);
            m_labelPhoto->resize(60,60);
            m_labelPhoto->move(m_cornerButton-2+40, m_topButtons-1);
        }
    }
}

void AbulEduSplashScreenV1::slotPetitAvatar()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() == 1)
    {
        if(m_isUtilitaire)
        {
            QPixmap photoRetaillee(abeApp->getAbeIdentite()->abeGetPhoto()->scaledToWidth(64,Qt::SmoothTransformation));
            m_labelPhoto->setPixmap(photoRetaillee);
            m_labelPhoto->resize(64,64);
            m_labelPhoto->move(222, m_topButtonsUtilitaire+4);
        }
        else
        {
            QPixmap photoRetaillee(abeApp->getAbeIdentite()->abeGetPhoto()->scaledToWidth(54,Qt::SmoothTransformation));
            m_labelPhoto->setPixmap(photoRetaillee);
            m_labelPhoto->resize(54,54);
            m_labelPhoto->move(m_cornerButton+41, m_topButtons+3);
        }
    }
}

void AbulEduSplashScreenV1::slotFinChargement()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Evite le double appel entre propagation d'id d'alacarte et "timeout"
    if(m_flagEnd == false) {
        m_flagEnd = true;
        finish();
    }
}

void AbulEduSplashScreenV1::updateLoadingMessage()
{
    /* Trouver le pourcentage d'avancement */
    m_percent += static_cast<double>(m_timer->interval()) / m_msec  * 100;

    if(m_percent <= 100){
        showMessage(QObject::trUtf8("Chargement en cours, merci de patienter ... %1%").arg(QString::number(qCeil(m_percent))), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    }
    else
    {
        m_timer->stop();
        m_message = QObject::trUtf8("Chargement terminé.");
#ifdef VER_PRODUCTVERSION_STR
        m_message += QString(" Version : %1").arg(VER_PRODUCTVERSION_STR);
#endif
#ifdef __ABULEDU_CODE_REVNO__
        m_message.append(__ABULEDU_CODE_REVNO__);
#endif
        showMessage(m_message, Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    }
}

void AbulEduSplashScreenV1::closeEvent(QCloseEvent *ev)
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;
    QWidget::closeEvent(ev);
    deleteLater();
}

void AbulEduSplashScreenV1::finish()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //qDebug() << "Emission SIGNAL signalAbeSplashScreenLaunched";
    emit signalAbeSplashScreenLaunched();
    if(m_askAuth) {

        ABULEDU_LOG_DEBUG() << "on ajoute les boutons d'authentification";
        m_btnAuthenticated = new AbulEduFlatBoutonV1(this);
        m_btnAuthenticated->stackUnder(m_labelPhoto);
        connect(m_btnAuthenticated, SIGNAL(signalSurvolIn()),this,SLOT(slotGrandAvatar()), Qt::UniqueConnection);
        connect(m_btnAuthenticated, SIGNAL(signalSurvolOut()),this,SLOT(slotPetitAvatar()), Qt::UniqueConnection);
        m_btnAuthenticated->setObjectName("btnAuthenticated");
        m_btnAuthenticated->setCouleursTexte(QColor("#ecf2f4"),Qt::white,Qt::white,Qt::lightGray);
        m_btnAuthenticated->setTextePaddingRight(15);
        if(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() == 1)
        {
            QString intitule = abeApp->getAbeIdentite()->abeGetPreNom();
            if (!intitule.isEmpty() && !abeApp->getAbeIdentite()->abeGetNom().isEmpty())
            {
                intitule.append(" "+abeApp->getAbeIdentite()->abeGetNom());
            }
            // Est-il possible qu'une connection soit établie avec un login (et donc un mdp !!) valide mais que les champs nom et prénom n'aient pas été renseignés ?
            if (intitule.isEmpty())
            {
                intitule = abeApp->getAbeNetworkAccessManager()->abeGetSSOLogin().split("@").at(0);
            }
            m_btnAuthenticated->setText(intitule);
            m_btnAuthenticated->setIconeNormale(QString(":/button/id"));
            m_btnAuthenticated->setIconeSurvol(QString(":/button/id-over"));
            m_btnAuthenticated->setIconePressed(QString(":/button/id-pressed"));

            QPixmap photoRetaillee = abeApp->getAbeIdentite()->abeGetPhoto()->scaledToWidth(56,Qt::SmoothTransformation);

            m_labelPhoto->setPixmap(photoRetaillee);
            m_labelPhoto->resize(56,56);
            m_labelPhoto->move(m_cornerButton, m_topButtons);
            m_labelPhoto->show();
            slotPetitAvatar();
        }
        else
        {
            //2013.01.16 : decision pour la tablette pour eviter le bug qui fait que la sso popup se cache dessous
            //donc, si on est authentifie dans alacarte on propose deux boutons: continuer avec mon compte actuellement
            //authentifié ou passer en anonyme
#ifdef __ABULEDUTABLETTEV1__MODE__
            if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() == 1) {
                m_btnAuthenticated->setEnabled(true);
            }
            else {
                m_btnAuthenticated->setEnabled(false);
            }
#else
            m_btnAuthenticated->setEnabled(true);
            m_btnAuthenticated->setText(trUtf8("Authentification"));
            m_btnAuthenticated->setIconeNormale(QString(":/button/authenticate"));
            m_btnAuthenticated->setIconeSurvol(QString(":/button/authenticate-over"));
            m_btnAuthenticated->setIconePressed(QString(":/button/authenticate-pressed"));
#endif
        }

        bool tailleOK = false;
        do
        {
            m_buttonFont.setPointSize(m_buttonFont.pointSize()-1);
            QFontMetrics fm(m_buttonFont);
            if (fm.boundingRect(m_btnAuthenticated->text()).width() < 300) tailleOK = true;
        }
        while(tailleOK == false);
        m_btnAuthenticated->setFont(m_buttonFont);
        m_btnAuthenticated->setFixedSize(QPixmap(":/button/authenticate").size());
        if(m_isUtilitaire)
        {
            m_btnAuthenticated->move((width()-m_btnAuthenticated->width())/2, m_topButtonsUtilitaire);
            m_btnAuthenticated->setCouleursFond(QColor("#99d2ff"),QColor("#99d2ff"),QColor("#328aec"),QColor(Qt::lightGray));
            m_btnAuthenticated->setTexteAlignement(Qt::AlignLeft);
            m_btnAuthenticated->setTextePaddingLeft(120);
        }
        else
        {
            m_btnAuthenticated->move(m_cornerButton, m_topButtons);
            m_btnAuthenticated->setTexteAlignement(Qt::AlignRight);
        }
        m_btnAuthenticated->show();

        m_btnAnonymous = new AbulEduFlatBoutonV1(this);
        m_btnAnonymous->setObjectName("btnAnonymous");
        m_btnAnonymous->setFont(m_buttonFont);
        m_btnAnonymous->setCouleursTexte(QColor("#ecf2f4"),Qt::white,Qt::white,Qt::lightGray);
        m_btnAnonymous->setTexteAlignement(Qt::AlignRight);
        m_btnAnonymous->setText(trUtf8("Anonyme"));
        m_btnAnonymous->setIconeNormale(QString(":/button/anonymous"));
        m_btnAnonymous->setIconeSurvol(QString(":/button/anonymous-over"));
        m_btnAnonymous->setIconePressed(QString(":/button/anonymous-pressed"));
        m_btnAnonymous->setFixedSize(QPixmap(":/button/anonymous").size());
        if(m_isUtilitaire)
        {
            m_btnAnonymous->move((width()-m_btnAnonymous->width())/2, m_topButtonsUtilitaire + m_btnAuthenticated->height() + m_gapButtonsUtilitaire);
            m_btnAnonymous->setCouleursFond(QColor("#99d2ff"),QColor("#99d2ff"),QColor("#328aec"),QColor(Qt::lightGray));
            m_btnAnonymous->setStyleSheet(m_btnAnonymous->styleSheet().replace("border: none","border: 6px solid red;border-radius:5px"));
            m_btnAnonymous->setTexteAlignement(Qt::AlignLeft);
            m_btnAnonymous->setTextePaddingLeft(120);
        }
        else
        {
            m_btnAnonymous->move(m_cornerButton, m_topButtons + m_gapButtons + m_btnAuthenticated->height());
            m_btnAnonymous->setTexteAlignement(Qt::AlignRight);
            m_btnAnonymous->setTextePaddingRight(15);
            //            m_btnAnonymous->setCouleurFondNormale(QColor(255,0,0));
        }
        m_btnAnonymous->show();

        /* Ctrl+Shift+P pour anonyme cf session privé en firefox */
        m_btnAnonymous->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
        /* Ctrl+Shift+A pour authentifié */
        m_btnAuthenticated->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));

        QMetaObject::connectSlotsByName(this);

        //Et pour le prochain passage
        m_askAuth = false;
        repaint();
#if defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
#if QT_VERSION < 0x050000
        qt_x11_wait_for_window_manager(m_mainWindow);
#endif
#endif
    }
    else {
        if (m_mainWindow) {
#if defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
#if QT_VERSION < 0x050000
            qt_x11_wait_for_window_manager(m_mainWindow);
#endif
#endif
            //qDebug() << "ON LAnce la MAINWINDOW";
            m_mainWindow->show();
            m_mainWindow->setWindowState(m_showMode);

#ifdef __ABULEDUTABLETTEV1__MODE__
            //2013.03.26 (eric):
            //Astuce pour images sequentielles qui restait sur un fond bleu foncé ... aucune idée de pourquoi seul images seq est impacté
            //par ce bug mais cette solution corrige le bug donc pour l'instant on fait comme ca.
            //Autre piste: des qu'on a un window manager le bug n'existe plus ! c'est donc super spécifique tablette sans windowmanager
            m_mainWindow->repaint();
#endif
        }
        //qDebug() << "ON CLOSE";
        close();
    }
    //qDebug() << "FIN FONCTION";
}

/** @todo: remettre en service le msleep / multithread  /autre */
void AbulEduSplashScreenV1::launch(int msec)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_msec = msec;
    QTimer::singleShot(msec,this,SLOT(slotFinChargement()));

    m_timer =  new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateLoadingMessage()));
    m_timer->start(1000);
}

void AbulEduSplashScreenV1::abeSplashScreenLauchUtilitaire(bool trueFalse)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_isUtilitaire = trueFalse;
}

void AbulEduSplashScreenV1::setMainWindow(QWidget *mainWindow)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_mainWindow = mainWindow;
    connect(this,SIGNAL(signalSessionAuthenticated(bool)),mainWindow,SLOT(slotSessionAuthenticated(bool)), Qt::UniqueConnection);
}

void AbulEduSplashScreenV1::show()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QWidget::show();
#if defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
#if QT_VERSION < 0x050000
    qt_x11_wait_for_window_manager(this);
#endif
#endif
}
