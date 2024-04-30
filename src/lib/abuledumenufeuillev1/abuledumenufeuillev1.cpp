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

#include "abuledumenufeuillev1.h"
#include "ui_abuledumenufeuillev1.h"

#ifndef DEBUG_ABULEDUMENUFEUILLEV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMenuFeuilleV1::AbulEduMenuFeuilleV1(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AbulEduMenuFeuilleV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_dragPosition = QPoint();
    m_isWindowMoving = false;
    m_isWindowMovingAllowed = true;

    ui->setupUi(this);
    ui->abeMenuFeuilleGapFrame->setFixedWidth(0);
    m_parentMainWindow = qobject_cast<QMainWindow *>(parent->parentWidget());
    if(m_parentMainWindow != 0) {
        ABULEDU_LOG_DEBUG() << "  mon parent est une QMainWindow ...";
        //        setParent(m_parentMainWindow->centralWidget());
        //        if(QGridLayout *g = qobject_cast<QGridLayout *>(m_parentMainWindow->centralWidget()->layout())) {
        //            g->addWidget(this,0,0,1,g->columnCount());
        //        }
    }
    else {
        ABULEDU_LOG_DEBUG() << "  mon parent n'est PAS une QMainWindow ...";
        m_parentMainWindow = qobject_cast<QMainWindow *>(parent->parentWidget()->parentWidget());
        if(m_parentMainWindow != 0) {
            ABULEDU_LOG_DEBUG() << "  mais mon grand-parent est une QMainWindow ...";
        }
    }

    slotAbeMenuFeuilleUpdateAvatar(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());
    connect(abeApp->getAbeNetworkAccessManager(), SIGNAL(ssoAuthStatus(int)),this, SLOT(slotAbeMenuFeuilleUpdateAvatar(int)), Qt::UniqueConnection);

    //    QShortcut *q = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), parent, SLOT(on_abeMenuFeuilleBtnQuit_clicked()));
    //#ifdef Q_OS_ANDROID
    //    /* Android a la "bonne" idée de prendre le menu et de l'intégrer dans sa barre d'outils ...
    //     * il faut donc éviter d'afficher le menufeuille comme sur les autres OS */
    //Note 20140221 : finalement c'est pas si bien que ça: certaines versions du droid genrent bien et d'autres
    //c'est minable ... le menu feuille est peut être mieux finalement
    //    QMenuBar *menuBar= new QMenuBar(m_parentMainWindow);
    //    QMenu *menuFichier = new QMenu(menuBar);
    //    menuFichier->setObjectName(QStringLiteral("menuFichier"));
    //    m_parentMainWindow->setMenuBar(menuBar);
    //    menuBar->addAction(menuFichier->menuAction());

    //    /* En version android on cree une action pour chaque bouton rencontre */
    //    foreach(AbulEduFlatBoutonV1* btn, findChildren<AbulEduFlatBoutonV1*>()) {
    //        if(!btn->text().isEmpty()) {
    //            //Et je tente la creation d'une QAction de ce nom la (intégration Android)
    //            //Temporaire on supprime le choix de la langue et le user pour se simplifier le menu
    //            if(btn->parentWidget()->objectName() == "abeMenuFeuilleV1frmChoixLangues") {

    //            }
    //            else if(btn->parentWidget()->objectName() == "abeMenuFeuilleV1frmUserMenu") {

    //            }
    //            else {
    //                QAction *Act = new QAction(this);
    //                Act->setObjectName("action" + btn->objectName());
    //                Act->setText(btn->text());
    //                Act->setIcon(btn->icon());
    //                menuFichier->addAction(Act);
    //                connect(Act, SIGNAL(triggered()), btn, SLOT(click()));
    //            }
    //        }
    //    }

    //#endif

    /* Adaptation du menu des langues à lang.pri */
    QDir langDirectory("lang");
    QStringList foundedLangs;
    QFileInfoList listLangue(langDirectory.entryInfoList(QStringList() << "*.qm", QDir::Files | QDir::NoDotAndDotDot));

    for(int i=0; i < listLangue.count(); i++){
        QString fichier = listLangue[i].absoluteFilePath();
        const int sizeFile = listLangue[i].size();
        fichier.truncate(fichier.indexOf(".qm"));
        QString aEnlever = abeApp->applicationName() + "_";
        QString laLangue = fichier.remove(aEnlever).split("/").last();

        if(!laLangue.isEmpty() && sizeFile > 1000) {
            foundedLangs << laLangue;
        }
    }

    foreach(AbulEduFlatBoutonV1* btn,ui->abeMenuFeuilleV1frmChoixLangues->findChildren<AbulEduFlatBoutonV1*>())
    {
        btn->setVisible(foundedLangs.contains(btn->whatsThis()));
    }
    ui->abeMenuFeuilleBtnLanguage->setVisible(foundedLangs.size() > 1);
    ui->abeMenuFeuilleBtnLangueAnnuler->setVisible(foundedLangs.size() > 1);
    /* Je mets en français à l'origine, mais on pourrait faire une détection de la locale QLocale::system().name().section('_', 0, 0)
     * Il faudrait cependant vérifier si la transmission se fait bien à toute l'application. Le signal de changement de langue semble être émis trop tôt */
    slotAbeMenuFeuilleChangeLanguage(QLocale::system().name().section('_', 0, 0));

    ui->abeMenuFeuillePrincipale->move(0,0);
    ui->abeMenuFeuilleV1frmButtons->move(0,40);

    /* Liste de tous les objets "enfants" avant de changer le parent
     * le probleme était que le findChildren utilisé après le changement de parent ne nous permet
     * plus d'avoir accès à ces objets */
    QList<QWidget *> widgets = findChildren<QWidget *>();
    QList<QAction *> actions = findChildren<QAction *>();

    /* On sort de l'ui pour que les objets puissent se positionner au dessus des autres */
    ui->abeMenuFeuilleV1frmButtons->setParent(parent);

    ui->abeMenuFeuilleV1frmChoixLangues->setParent(parent);
    ui->abeMenuFeuilleV1frmChoixLangues->adjustSize();
    ui->abeMenuFeuilleV1frmUserMenu->setParent(parent);
    ui->abeMenuFeuilleV1frmUserMenu->adjustSize();

    //================================================================================
    /* Lecture du fichier conf pour attribuer les btn */
    QSettings config(":/abuledumenufeuillev1/abuledumenufeuillev1.conf",QSettings::IniFormat, this);
    foreach(const QString &gr, config.childGroups()) {
        config.beginGroup(gr);
        {
            /* Sous android c'est un menu intégré à base de QActions */
            //#ifdef Q_OS_ANDROID
            //        for(int i = 0; i < actions.count(); i++) {
            //            if(actions.at(i)->objectName() == ("action" + gr)) {
            //                QAction *btn = actions.takeAt(i);
            //                if(m_localDebug) qDebug() << " on a trouvé " << btn->objectName();

            //                foreach(QString propertie, grProperties) {
            //                    if(m_localDebug) qDebug() << " propriété -> " << propertie.toLatin1()<<" contient "<<config.value(propertie);
            //                    /* Dans le cas de la propriété text seulement, on gère l'utf8 */
            //                    if(propertie.toLatin1() == "text")
            //                    {
            //                        QByteArray str = config.value(propertie).toByteArray();
            //                        btn->setProperty("text", QString::fromUtf8(str));
            //                    }
            //                    else
            //                    {
            //                        btn->setProperty(propertie.toLatin1(), config.value(propertie));
            //                    }
            //                }
            //            }
            //        }
            //#else

        }
        /* Sur les autres OS, on cherche le widget qui porte ce nom dans notre liste de pointeurs de qwidgets */
        /* 1. Foreach sur tous les widgets */
        foreach (QWidget *var, widgets) {
            /* 2. Si le widget == à gr */
            if(var->objectName() == gr){
                /* 3. On définit les propriétés */
                foreach (const QString &propertie, config.childKeys()) {
                    if(propertie.toLatin1() == "text"){
                        QByteArray str = config.value(propertie).toByteArray();
                        var->setProperty("text", QString::fromUtf8(str));
                    }
                    else{
                        var->setProperty(propertie.toUtf8(), config.value(propertie));
                    }
                }
                var->style()->unpolish(var);
                var->style()->polish(var);
                var->update();
            }
        }
        config.endGroup();
    }

    //#ifdef Q_OS_ANDROID
    //    setVisible(false);
    //#else
    setVisible(true);

    ui->abeMenuFeuilleV1frmButtons->setMaximumWidth(m_parentMainWindow->width());
    ui->abeMenuFeuilleV1frmButtons->adjustSize();

    /* Affectation du "titre" de l'application */
    ui->abeMenuFeuilleLblTitre->setText(abeApp->getAbeApplicationLongName());
    //#endif

#if defined(__ABULEDUTABLETTEV1__MODE__) || defined(Q_OS_ANDROID)
    ui->abeMenuFeuilleBtnFullScreen->setVisible(false);
    ui->abeMenuFeuilleBtnMinimized->setVisible(false);
#endif

    /* On cache les objets */
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    ui->abeMenuFeuilleV1frmUserMenu->setVisible(false);
    /* Je cache le bouton Journal en attendant de voir comment le gérer */
    ui->abeMenuFeuilleBtnLogs->setVisible(false);
    /* Installation d'un event filter pour cacher la frameButtons si la position de l'evenement souris est en dehors de l'objet */
    abeApp->installEventFilter(this);

#if defined(__ABULEDUTABLETTEV1__MODE__) || defined(Q_OS_ANDROID)
    /* Pas de connect car le slot n'existe pas en mode tab & android */
#else
    //erics / 20140219
    //creation de abeMenuFeuilleBtnFeuille_mouseOver pour eviter le comportement suivant:
    //je survole le 1er coup ça ouvre le menu, je reviens sur l'objet, ça ferme le menu ...
    connect(ui->abeMenuFeuilleBtnFeuille, SIGNAL(signalSurvolIn()), this, SLOT(abeMenuFeuilleBtnFeuille_mouseOver()),Qt::UniqueConnection);
#endif
}

void AbulEduMenuFeuilleV1::abeMenuFeuilleSetTitle(const QString &title)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleLblTitre->setText(title);
}

QString AbulEduMenuFeuilleV1::abeMenuFeuilleGetCurrentLang()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return ui->abeMenuFeuilleBtnLanguage->getIconeNormale().remove(":/abuledumenufeuillev1/");
}

void AbulEduMenuFeuilleV1::abeMenuFeuilleSetButtonsFrameWidth(int w)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->resize(w,ui->abeMenuFeuilleV1frmButtons->height());
}

void AbulEduMenuFeuilleV1::abeMenuFeuilleSetGap(int leftGap)
{
    ui->abeMenuFeuilleGapFrame->setFixedWidth(leftGap);
}

void AbulEduMenuFeuilleV1::abeMenuFeuilleMoveButtonsFrame(int x, int y)
{
    ui->abeMenuFeuilleV1frmButtons->move(x,y);
}

void AbulEduMenuFeuilleV1::abeMenuFeuilleSetIsWindowMovingAllowed(bool isWindowMovingAllowed)
{
    m_isWindowMovingAllowed = isWindowMovingAllowed;
    ui->abeMenuFeuilleBtnFullScreen->setVisible(isWindowMovingAllowed);
    ui->abeMenuFeuilleBtnMinimized->setVisible(isWindowMovingAllowed);
}

void AbulEduMenuFeuilleV1::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    QWidget *w = this;
    while (w->parentWidget() != Q_NULLPTR)
        w = w->parentWidget();
    Qt::WindowFlags flags = w->windowFlags();
    if(!flags.testFlag(Qt::FramelessWindowHint)){
        qDebug() << "~~~~~ ON EST PAS framless";
        /* On dégomme les boutons que l'on ne veut pas */
        ui->abeMenuFeuilleBtnFullScreen->hide();
        ui->abeMenuFeuilleBtnMinimized->hide();
        ui->abeMenuFeuilleBtnQuit->hide();
        ui->abeMenuFeuilleLblTitre->setText("");
    }
//    if(ui->abeMenuFeuilleV1frmUserMenu->isVisible())
//        calculatePosFrmUserMenu();
}


AbulEduMenuFeuilleV1::~AbulEduMenuFeuilleV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

#ifndef __ABULEDUTABLETTEV1__MODE__
void AbulEduMenuFeuilleV1::resizeEvent(QResizeEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->abeMenuFeuilleV1frmUserMenu->isVisible())
        calculatePosFrmUserMenu();
}

void AbulEduMenuFeuilleV1::mouseMoveEvent(QMouseEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_isWindowMoving && m_parentMainWindow != 0 && m_isWindowMovingAllowed) {
        m_parentMainWindow->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void AbulEduMenuFeuilleV1::mousePressEvent(QMouseEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (event->button() == Qt::LeftButton && ui->abeMenuFeuillePrincipale->rect().contains(event->pos()) &&  m_parentMainWindow != 0) {
        m_dragPosition = event->globalPos() - m_parentMainWindow->frameGeometry().topLeft();
        event->accept();
        m_isWindowMoving = true;
    }
}

void AbulEduMenuFeuilleV1::mouseReleaseEvent(QMouseEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(event);
    m_isWindowMoving = false;
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnFullScreen_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_parentMainWindow->isMaximized())
    {
        ABULEDU_LOG_DEBUG() << "  est plein ecran, on essaye de revenir fenetre";
        m_parentMainWindow->showNormal();
        ui->abeMenuFeuilleBtnFullScreen->setIconeNormale(":/abuledumenufeuillev1/showMaximized");
    }
    else
    {
        ABULEDU_LOG_DEBUG() << "  est fenetre, on essaye de passer plein ecran";
        m_parentMainWindow->showMaximized();
        ui->abeMenuFeuilleBtnFullScreen->setIconeNormale(":/abuledumenufeuillev1/showNormal");
    }
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnMinimized_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_parentMainWindow->showMinimized();
}

void AbulEduMenuFeuilleV1::abeMenuFeuilleBtnFeuille_mouseOver()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Si c'est la 1ere fois qu'on survole on fait la même chose que le clic: on ouvre le menu
    //mais si le menu est déjà ouvert on ne le ferme pas ...
    if(! ui->abeMenuFeuilleV1frmButtons->isVisible()) {
        on_abeMenuFeuilleBtnFeuille_clicked();
    }
}
#endif

bool AbulEduMenuFeuilleV1::eventFilter(QObject *, QEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Evite de planter si on est en train de fermer ...
    if(event->type() == QEvent::Destroy) {
        return true;
    }
    /* Si les boutons sont visibles et que l'evenement est un clic souris -> on cache la frame */
    if((ui->abeMenuFeuilleV1frmButtons->isVisible()) && (event->type() == QEvent::MouseButtonRelease)){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!((ui->abeMenuFeuilleV1frmButtons->geometry().contains(mouseEvent->pos())) || (ui->abeMenuFeuilleBtnFeuille->geometry().contains(mouseEvent->pos()))) && this->isVisible())
            ui->abeMenuFeuilleV1frmButtons->setVisible(false);
    }
    if((ui->abeMenuFeuilleV1frmUserMenu->isVisible()) && (event->type() == QEvent::MouseButtonRelease)){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!ui->abeMenuFeuilleV1frmUserMenu->geometry().contains(mouseEvent->pos()) && this->isVisible())
            ui->abeMenuFeuilleV1frmUserMenu->setVisible(false);
    }
    if((ui->abeMenuFeuilleV1frmChoixLangues->isVisible()) && (event->type() == QEvent::MouseButtonRelease)){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(!ui->abeMenuFeuilleV1frmChoixLangues->geometry().contains(mouseEvent->pos()) && this->isVisible())
            ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    }
    return false;
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnFeuille_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (ui->abeMenuFeuilleV1frmButtons->isVisible())
    {
        //erics 20140313 : si on clique sur le menu feuille alors qu'il est deja visible, on ne veut plus le cacher
        //        ui->abeMenuFeuilleV1frmButtons->setVisible(false);
        //A verifier si c'est toujours indispensable
        //        ui->abeMenuFeuilleBtnFeuille->setStyleSheet("QPushButton > *{color:red;}QPushButton{border: none; color:rgba(0,0,0,255);background-repeat: no-repeat;background-color:rgba(6,109,255,255);border-image:url(':/abuledumenufeuillev1/leaf');image-position: center;}");
    }
    else
    {
        ui->abeMenuFeuilleV1frmButtons->setVisible(true);
        ui->abeMenuFeuilleV1frmButtons->raise();
        //A verifier si c'est toujours indispensable
        //        ui->abeMenuFeuilleBtnFeuille->setStyleSheet("QPushButton > *{color:red;}QPushButton{border: none; color:rgba(0,0,0,255);background-repeat: no-repeat;background-color:rgba(53,166,247,255);border-image:url(':/abuledumenufeuillev1/leaf');image-position: center;}");
    }
}



void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnLanguage_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (ui->abeMenuFeuilleV1frmChoixLangues->isVisible()){
        ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    }
    else{
        ui->abeMenuFeuilleV1frmChoixLangues->setVisible(true);
        ui->abeMenuFeuilleV1frmChoixLangues->move(ui->abeMenuFeuilleBtnLanguage->pos().x()-ui->abeMenuFeuilleBtnFeuille->width(),
                                                  ui->abeMenuFeuilleBtnLanguage->pos().y()+ui->abeMenuFeuilleBtnLanguage->height());
        ui->abeMenuFeuilleV1frmChoixLangues->raise();
    }
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnNew_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnOpen_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnSave_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnPublish_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnImport_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnExport_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnPrint_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnSettings_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnHelp_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnCustom1_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnCustom2_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnQuit_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmButtons->setVisible(false);
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnFr_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("fr");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnEn_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("en");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnEs_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("es");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnIt_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("it");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnDe_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("de");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnNl_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("nl");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnOc_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    slotAbeMenuFeuilleChangeLanguage("oc_leng");
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnLangueAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
}

void AbulEduMenuFeuilleV1::slotAbeMenuFeuilleChangeLanguage(const QString& lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString titleWindow = ui->abeMenuFeuilleLblTitre->text();
    ui->abeMenuFeuilleV1frmChoixLangues->setVisible(false);
    ui->abeMenuFeuilleBtnLanguage->setIconeNormale(":/abuledumenufeuillev1/"+lang);
    ui->abeMenuFeuilleBtnLanguage->setIconeSurvol(":/abuledumenufeuillev1/"+lang+"Hover");
    ui->abeMenuFeuilleBtnLanguage->setIconePressed(":/abuledumenufeuillev1/"+lang+"Hover");
    emit signalAbeMenuFeuilleChangeLanguage(lang);
    ui->retranslateUi(this);
    ui->abeMenuFeuilleLblTitre->setText(titleWindow);
}

void AbulEduMenuFeuilleV1::slotAbeMenuFeuilleUpdateAvatar(int authenticated)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(authenticated)
    QIcon iconAvatar(*abeApp->getAbeIdentite()->abeGetPhoto());
    ui->abeMenuFeuilleBtnAvatar->setIcon(iconAvatar);
    ui->abeMenuFeuilleBtnAvatar->setIconSize(QSize(36,36));
    ui->abeMenuFeuilleBtnAvatar->setEnabled(true);
}

void AbulEduMenuFeuilleV1::calculatePosFrmUserMenu()
{
    /* On recupere le widget parent (raconte.ui) */
    QWidget *w = this;
    while (w->parentWidget() != Q_NULLPTR) w = w->parentWidget();
    /* Le widget reference pour le placement */
    QWidget *wRef = ui->abeMenuFeuilleBtnAvatar;
    /* Le widget "to move" -> frameUSer  */
    QWidget *wUser = ui->abeMenuFeuilleV1frmUserMenu;

    /* On le place en dessous du bouton, au milieu */
    wUser->move(QPoint((wRef->mapToParent(QPoint(0,0)).x()-(wUser->width()/2)-(wRef->width()/2)),
                       wRef->frameGeometry().bottom()));

    /* Tant que la frmUser n'est pas contenue dans la mainwindow */
    while(!(wUser->geometry().topRight().x() <= w->geometry().topRight().x())){
        wUser->move((wUser->mapToParent(QPoint(0,0)).x()-1),
                    wRef->frameGeometry().bottom());
    }
    wUser->raise();
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnAvatar_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#ifndef __ABULEDUTABLETTEV1__MODE__
    ui->abeMenuFeuilleV1frmUserMenu->setVisible(!ui->abeMenuFeuilleV1frmUserMenu->isVisible());
    if(ui->abeMenuFeuilleV1frmUserMenu->isVisible())
        calculatePosFrmUserMenu();
#endif
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnLogout_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmUserMenu->setVisible(false);
    abeApp->getAbeNetworkAccessManager()->abeSSOLogout();
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnLogin_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmUserMenu->setVisible(false);
    abeApp->getAbeNetworkAccessManager()->abeSSOLogout();
    abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
}

void AbulEduMenuFeuilleV1::on_abeMenuFeuilleBtnUserAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->abeMenuFeuilleV1frmUserMenu->setVisible(false);
}
