/** Implémentation Automatique du menu Aide
 *
 * @see https://redmine.ryxeo.com/projects/leterrier-developpeu/
 * @author 2010 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
 * @author 2010-2011 Éric Seigne <eric.seigne@ryxeo.com>
 * @see The GNU Public License (GPL)
 */

/**
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

#include "abuleduaproposv1.h"
#include "ui_abuleduaproposv1.h"
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>
#include <QDesktopServices>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QtXml>
#include <QHostInfo>
#include <QNetworkProxy>

#ifndef DEBUG_ABULEDUAPROPOSV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduAproposV1::AbulEduAproposV1(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AbulEduAproposV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    this->setWindowTitle(trUtf8("A propos de")+" "+abeApp->getAbeApplicationLongName()+" "+abeApp->applicationVersion());
    //    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    //Initialisation de quelques variables ...
    QString adresseForum = "http://forum.abuledu.org/rss/tags/" + abeApp->applicationName() + "/redirect/on/lang/" + QLocale::system().name().section('_', 0, 0);
    ui->lblPosezVotreQuestion->setText("<a href='" + adresseForum + "'>" + trUtf8("Posez votre question sur le forum des utilisateurs ...") + "</a>");

    //Pour eviter de re-telecharger 10 fois les ressources en ligne, j'utilise un flag tout con pour chaque ressource.
    p_pedaDownloaded  = false;
    p_forumDownloaded = false;
    p_newsDownloaded  = false;

    //Un seul networkmanager pour tout lemonde me semble suffisant :)
    m_nam = abeApp->getAbeNetworkAccessManager();

    /* Normalement
    QSettings config(":/abuleduaproposv1/abuleduaproposv1.conf",QSettings::IniFormat); */
    if(QFile(":/abuleduaproposv1/abuleduaproposv1.conf").exists())
    {
        ABULEDU_LOG_DEBUG() <<" Conf trouvé ";
    }
    else
    {
        ABULEDU_LOG_DEBUG() <<" Conf PAS trouvé ";
    }
    QString locale = QLocale::system().name().section('_', 0, 0);
    ui->textPresentation->clear();
    QFile presentationFile(":/abuleduaproposv1/presentation_"+locale+".html");
    QString presentationHtml;
    if(presentationFile.open(QFile::ReadOnly| QIODevice::Text))
    {
        QTextStream in(&presentationFile);
#ifdef Q_OS_WIN
        in.setCodec(QTextCodec::codecForName("UTF-8"));
#endif
        while (!in.atEnd())
        {
            presentationHtml.append(in.readLine()+"\n");
        }
        ui->textPresentation->setHtml(presentationHtml);
    }
    ui->textHelp->clear();
    QFile helpFile(":/abuleduaproposv1/help_"+locale+".html");
    QString helpHtml;
    if(helpFile.open(QFile::ReadOnly| QIODevice::Text))
    {
        QTextStream in(&helpFile);
        /* 20140521 Philippe : la lecture sous windows du fichier renvoie des caractères accentués tout pourris. Ce n'est pas normal car le fichier est bien au départ
         * encodé en UTF-8... la solution ci-dessous permet de résoudre le problème, même si elle risque d'en poser d'autres ailleurs */
        /** @todo regarder si ça ne vient pas du fait qu'on utilise un fichier ressource */
#ifdef Q_OS_WIN
        in.setCodec(QTextCodec::codecForName("UTF-8"));
#endif
        while (!in.atEnd())
        {
            helpHtml.append(QString(in.readLine()));
            helpHtml.append("\n");
        }
        ui->textHelp->setHtml(helpHtml);
    }
    ui->textAbout->clear();
    QFile aboutFile(":/abuleduaproposv1/about_"+locale+".html");
#ifdef __ABULEDU_CODE_REVNO__
    QString aboutHtml=QString("<b><center>%1 %2 %3</center></b>").arg(abeApp->getAbeApplicationLongName()).arg(abeApp->applicationVersion()).arg(__ABULEDU_CODE_REVNO__);
#else
    QString aboutHtml=QString("<b><center>%1 %2</center></b>").arg(abeApp->getAbeApplicationLongName()).arg(abeApp->applicationVersion());
#endif
    aboutHtml.append("\n");

    if(aboutFile.open(QFile::ReadOnly| QIODevice::Text))
    {
        QTextStream in(&aboutFile);
#ifdef Q_OS_WIN
        in.setCodec(QTextCodec::codecForName("UTF-8"));
#endif
        while (!in.atEnd())
        {
            aboutHtml.append(in.readLine()+"\n");
        }
        ui->textAbout->setHtml(aboutHtml);
    }
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tabForum));
    ui->tabWidget->setCurrentWidget(ui->tabHelp);
    connect(this, SIGNAL(signalAbeAproposSetindex(int)),this,SLOT(slotAbeAproposChangeTab(int)),Qt::UniqueConnection);

    /* Personnalisation du QTabWidget en fonction du fichier abuleduapropos.conf */
    QSettings config(":/abuleduaproposv1/abuleduaproposv1.conf",QSettings::IniFormat);
    ABULEDU_LOG_DEBUG()<<config.childGroups();
    config.beginGroup("AbulEduAproposV1");
    ABULEDU_LOG_DEBUG()<<config.childKeys();
    QString lect = config.value("background-color",QString()).toString();
    if(!lect.isEmpty()){
        setStyleSheet(QString("background-color:%1;").arg(lect));
    }
    config.endGroup();

    config.beginGroup("tabWidget");
    ABULEDU_LOG_DEBUG() << "tabWidget";
    QString tabWidgetCSS;
    foreach(QString level2,config.childGroups()){
        ABULEDU_LOG_DEBUG() << "    "<<level2;
        config.beginGroup(level2);
        foreach(QString level3,config.childGroups()){
            ABULEDU_LOG_DEBUG() << "        "<<level3;
            bool empty = true;
            QString element = level2+"::"+level3+"{\n";
            config.beginGroup(level3);
            foreach(QString prop,config.childKeys()){
                QString val = config.value(prop,QString()).toString();
                ABULEDU_LOG_DEBUG() << "            "<<prop<<":"<<val;
                if(!val.isEmpty()){
                    empty = false;
                    element.append(prop+":"+val+";\n");
                }
            }
            element.append("}\n");
            config.endGroup();
            if(!empty){
                tabWidgetCSS.append(element);
            }
        }
        config.endGroup();
    }
    config.endGroup();
    ABULEDU_LOG_DEBUG() << "CSS ";
    ABULEDU_LOG_DEBUG() << tabWidgetCSS;
    if(!tabWidgetCSS.isEmpty()){
        ui->tabWidget->setStyleSheet(tabWidgetCSS);
    }

    config.beginGroup("btnAproposClose");
        foreach(QString propertie, config.childKeys()) {
            ABULEDU_LOG_DEBUG() << " propriété -> " << propertie.toLatin1()<<" contient "<<config.value(propertie);
            /* Dans le cas de la propriété text seulement, on gère l'utf8 */
            if(propertie.toLatin1() == "text")
            {
                QByteArray str = config.value(propertie).toByteArray();
                ui->btnAproposClose->setProperty("text", QString::fromUtf8(str));
            }
            else
            {
                ui->btnAproposClose->setProperty(propertie.toLatin1(), config.value(propertie));
            }
        }
    config.endGroup();

//    /* Liste de tous les objets "enfants" avant de changer le parent
//     * le probleme était que le findChildren utilisé après le changement de parent ne nous permet
//     * plus d'avoir accès à ces objets */
//    QList<QWidget *> widgets = findChildren<QWidget *>();
//    QList<QAction *> actions = findChildren<QAction *>();

//    /* On sort de l'ui pour que les objets puissent se positionner au dessus des autres */
//    ui->abeMenuFeuilleV1frmButtons->setParent(parent);

//    ui->abeMenuFeuilleV1frmChoixLangues->setParent(parent);
//    ui->abeMenuFeuilleV1frmChoixLangues->adjustSize();
//    ui->abeMenuFeuilleV1frmUserMenu->setParent(parent);
//    ui->abeMenuFeuilleV1frmUserMenu->adjustSize();

//    /* Lecture du fichier conf pour attribuer les btn */
//    QSettings config(":/abuleduaproposv1/abuleduaproposv1.conf",QSettings::IniFormat);
//    QStringList groups = config.childGroups();
//    foreach(QString gr, groups) {
//        if(m_localDebug) qDebug() << " on cherche " << gr;
//        config.beginGroup(gr);
//        QStringList grProperties = config.childKeys();

//        /* Sous android c'est un menu intégré à base de QActions */
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
//        /* Sur les autres OS, on cherche le widget qui porte ce nom dans notre liste de pointeurs de qwidgets */
//        for(int i = 0; i < widgets.count(); i++) {
//            if(widgets.at(i)->objectName() == gr) {
//                QWidget *btn = widgets.takeAt(i);
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
//#endif

    ABULEDU_LOG_DEBUG() << " fin de constructeur"
                        <<ui->btnAproposClose->styleSheet();

    ui->btnAproposClose->setTexteAlignement(Qt::AlignHCenter | Qt::AlignBottom);
    QFontMetrics fm(ui->btnAproposClose->font());
    ui->btnAproposClose->setMinimumWidth(fm.boundingRect(ui->btnAproposClose->text()).width());
    ui->btnAproposClose->setMinimumHeight(ui->btnAproposClose->minimumWidth());
}

AbulEduAproposV1::~AbulEduAproposV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduAproposV1::installeMenu(QMainWindow *mw)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QMenuBar *b = mw->findChild<QMenuBar *>(); // Cherche la barre de menu
    QMenu *m = mw->findChild<QMenu *>("menuAide"); //On cherche si le menu aide existe

    if(!m)  //sinon, on crée le menu Aide
    {
        QMenu *menuAide = new QMenu(trUtf8("&Aide"),b);
        menuAide->setObjectName("menuAide");
        b->addMenu(menuAide); //Ajoute l'entrée Aide au menu

        //Ajout des entrées de menu

        /* Obsolète : plus d'Aide par redirection vers une page web
        QString nomItem=trUtf8("Aide");
        actionAide=new QAction(nomItem,mw);
        menuAide->addAction(actionAide);
        if(abeApp->organizationDomain()=="")
        {
            actionAide->setDisabled(true);
        }
        connect(actionAide, SIGNAL(triggered()), this, SLOT(aide())); */

        //
        QString nomItem=trUtf8("Aide intégrée...");
        actionAideIntegree=new QAction(nomItem,mw);
        actionAideIntegree->setShortcut(Qt::Key_F1);
        menuAide->addAction(actionAideIntegree);
        connect(actionAideIntegree, SIGNAL(triggered()),this,SLOT(slotAbeAproposShowHelp()));

        // Ressources péda
        nomItem=trUtf8("Ressources pédagogiques...");
        actionPeda=new QAction(nomItem,mw);
        menuAide->addAction(actionPeda);
        connect(actionPeda, SIGNAL(triggered()),this,SLOT(slotAbeAproposShowResource()));

        // News
        nomItem=trUtf8("Nouveautés...");
        actionNews=new QAction(nomItem,mw);
        menuAide->addAction(actionNews);
        connect(actionNews, SIGNAL(triggered()),this,SLOT(slotAbeAproposShowNews()));

        /* Contact Forum : le forum est dans les chous pour l'instant, je n'ajoute pas l'entrée au menu.
        nomItem=trUtf8("Forum des utilisateurs...");
        actionForum=new QAction(nomItem,mw);
        menuAide->addAction(actionForum);
        connect(actionForum, SIGNAL(triggered()),this,SLOT(montreForum())); */

        // Traductions
        nomItem=trUtf8("Contribuez...");
        actionTraductions=new QAction(nomItem,mw);
        menuAide->addAction(actionTraductions);
        connect(actionTraductions, SIGNAL(triggered()),this,SLOT(slotAbeAproposShowContribute()));

        // A propos
        nomItem=trUtf8("À Propos de ce logiciel...");
        actionTraductions=new QAction(nomItem,mw);
        menuAide->addAction(actionTraductions);
        connect(actionTraductions, SIGNAL(triggered()),this,SLOT(slotAbeAproposShowAbout()));

    }
    connect(ui->btnAproposClose, SIGNAL(clicked()),this, SLOT(close()),Qt::UniqueConnection);
}

//--------------------------------------------------------------------------------------
//
//------------------------------Gestion fenêtre A Propos--------------------------------
//
//--------------------------------------------------------------------------------------


/**
  *les fonctions montreAide(), montreNews(), montreForum(), montreTraduire(), montreAPropos()
  *créent la fenêtre d'APropos en mode modal et positionnent le bon onglet
  *en émettant le signal setIndex(int)
  */
void AbulEduAproposV1::slotAbeAproposShowHelp()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposSetindex(ui->tabWidget->indexOf(ui->tabHelp));
    this->show();
}

void AbulEduAproposV1::slotAbeAproposShowResource()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposSetindex(ui->tabWidget->indexOf(ui->tabResources));
    this->show();

    if(!p_pedaDownloaded)
    {
        p_pedaDownloaded = true;
        //Essaye de telecharger les news du projet redmine
        //Message d'erreur
        QString messageErreur = trUtf8("Le site n'est pas accessible");
        //Récupère l'adresse qui est passée dans le textBrowser
        QUrl adresseFlux = abeBuildUrl("peda","");

        //Message d'attente ...
        QString message = trUtf8("Téléchargement en cours ... veuillez patienter.");
        ui->textResources->setHtml(message);

        if(!adresseFlux.isValid())
        {
            ui->textResources->setHtml(messageErreur);
        }
        else
        {
            //On y va
            m_nam->disconnect(SIGNAL(finished(QNetworkReply*)));
            connect(m_nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotAbeAproposFinishedResources(QNetworkReply*)));
            m_nam->get(QNetworkRequest(adresseFlux));
        }
    }
}

void AbulEduAproposV1::slotAbeAproposShowNews()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposSetindex(ui->tabWidget->indexOf(ui->tabNews));
    this->show();

    if(!p_newsDownloaded)
    {
        p_newsDownloaded = true;
        //Essaye de telecharger les news du projet redmine
        //Message d'erreur
        QString messageErreur = trUtf8("Le site n'est pas accessible");
        //Récupère l'adresse qui est passée dans le textBrowser
        QUrl adresseFlux = abeBuildUrl("news","");

        //Message d'attente ...
        QString message = trUtf8("Téléchargement en cours ... veuillez patienter.");
        ui->textNews->setHtml(message);

        if(!adresseFlux.isValid())
        {
            ui->textNews->setHtml(messageErreur);
        }
        else
        {
            //On y va
            m_nam->disconnect(SIGNAL(finished(QNetworkReply*)));
            connect(m_nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotAbeAproposFinishedNews(QNetworkReply*)));
            m_nam->get(QNetworkRequest(adresseFlux));
        }
    }
}

void AbulEduAproposV1::slotAbeAproposShowForum()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposSetindex(ui->tabWidget->indexOf(ui->tabForum));
    this->show();

    if(!p_forumDownloaded)
    {
        p_forumDownloaded = true;
        //Essaye de telecharger le contenu du forum ...
        //Message d'erreur
        QString messageErreur = trUtf8("Le forum n'est pas accessible");
        //Récupère l'adresse qui est passée dans le textBrowser
        QUrl adresseFlux = abeBuildUrl("forum","");

        //Message d'attente ...
        QString message = trUtf8("Téléchargement en cours ... veuillez patienter.");
        ui->textForum->setHtml(message);

        if(!adresseFlux.isValid())
        {
            ui->textForum->setHtml(messageErreur);
        }
        else
        {
            //On y va
            m_nam->disconnect(SIGNAL(finished(QNetworkReply*)));
            connect(m_nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotAbeAproposFinishedForum(QNetworkReply*)));
            m_nam->get(QNetworkRequest(adresseFlux));
        }
    }
}

void AbulEduAproposV1::slotAbeAproposShowContribute()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposSetindex(ui->tabWidget->indexOf(ui->tabContribute));
//    this->exec();
}

void AbulEduAproposV1::slotAbeAproposShowAbout()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposSetindex(ui->tabWidget->indexOf(ui->tabAbout));
//    this->exec();
}

void AbulEduAproposV1::slotAbeAproposChangeTab(int index)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->tabWidget->setCurrentIndex(index);
}

/** Slot lançant un navigateur vers l'Url passée dans abeApp->setOrganizationDomain(); */
void AbulEduAproposV1::slotAbeAproposShowWebAide()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QUrl urlSite="http://"+abeApp->organizationDomain()+"/"+abeApp->organizationName()+"/"+abeApp->applicationName();
    QDesktopServices::openUrl(urlSite);
}

void AbulEduAproposV1::slotAbeAproposFinishedForum(QNetworkReply* reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (reply->error() == QNetworkReply::NoError)
    {
        // lectures des données issues du "reply"
        xml= new QDomDocument();
        xml->setContent(reply->readAll());
        QDomElement xmlElem = xml->documentElement();
        QDomNode node = xmlElem.firstChild();
        while(!node.isNull())
        {
            QDomElement element = node.toElement(); // On essaie de convertir le node en element
            if(!element.isNull())
            {
                if(element.tagName()=="channel")    // Le node est bien un élément.
                {
                    QDomNode node=element.firstChild(); //On parcourt le document à partir du tag "channel"
                    while(!node.isNull())
                    {
                        QDomElement elem=node.toElement(); //On cherche le titre du "Channel"
                        if(elem.tagName()=="title")
                        {
                            QDomNode node=elem.firstChild();
                            QString titre=node.toText().data();
                            ui->textForum->setHtml("<h4><font color=red>"+titre+"</font></h4>");
                        }
                        else if((elem.tagName()=="item"))
                        {
                            QDomNode node=elem.firstChild();
                            while(!node.isNull())
                            {
                                QDomElement elem=node.toElement(); //Titre du fils de discussion
                                if(elem.tagName()=="title")
                                {
                                    QDomNode node=elem.firstChild();
                                    itemTitre=node.toText().data();
                                    node=node.nextSiblingElement("link");
                                }
                                if(elem.tagName()=="link")      // Lien vers post
                                {
                                    QDomNode node=elem.firstChild();
                                    itemLink=node.toText().data();
                                    node=node.nextSiblingElement("description");
                                }
                                if(elem.tagName()=="description") // Texte du post
                                {
                                    QDomNode node=elem.firstChild();
                                    itemDescription =node.toText().data();
                                    node=node.nextSibling();
                                }
                                node=node.nextSibling();
                            }
                            QString itemView= "<a href="+itemLink+">"
                                              "<font color=green>"+itemTitre+"</font></a>"
                                              +itemDescription;
                            ui->textForum->append(itemView);
                        }
                        node=node.nextSibling();
                    }
                }
                node = node.nextSibling();
            }
        }
    }
    // Some http error received
    else
    {
        QString messageErreur = trUtf8("Le forum n'est pas accessible");
        ui->textForum->setHtml(messageErreur);
    }
    QScrollBar *sb = ui->textForum->verticalScrollBar();       //On scrolle vers le bas
    sb->setValue(sb->minimum());
    disconnect(m_nam,SIGNAL(finished(QNetworkReply*)),this, SLOT(slotAbeAproposFinishedForum(QNetworkReply*)));
}

void AbulEduAproposV1::slotAbeAproposFinishedNews(QNetworkReply* reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (reply->error() == QNetworkReply::NoError)
    {
        // lectures des données issues du "reply"
        xml= new QDomDocument();
        xml->setContent(reply->readAll());

        QDomElement root = xml->documentElement();
        QDomElement element = root.firstChildElement("title");
        if(!element.isNull())
        {
            ui->textNews->setHtml("<h4><font color=red>" + element.text().trimmed() + "</font></h4>");
        }

        QDomElement child = root.firstChildElement("entry").toElement();
        QDomElement childL;
        while(!child.isNull())
        {
            //qDebug() << "Lecture XML -> " << child.tagName();
            childL = child.childNodes().at(0).toElement();
            if (childL.tagName() == "title")
            {
                itemTitre = childL.text().trimmed();
                childL = childL.nextSiblingElement("id");
                //qDebug() << "title :: " << itemTitre;
            }
            if(childL.tagName()=="id")      // Lien vers post
            {
                itemLink = childL.text().trimmed();
                childL = childL.nextSiblingElement("updated");
                //qDebug() << "Lien :: " << itemLink;
            }
            if(childL.tagName()=="updated") // Date
            {
                itemDate = childL.text().trimmed();
                childL=childL.nextSiblingElement("content");
            }
            if(childL.tagName()=="content") // Texte du post
            {
                itemDescription = childL.text().trimmed();
            }
            QString itemView= "<a href="+itemLink+">"
                              "<font color=green>"+itemTitre+"</font></a>"
                              +itemDescription;
            ui->textNews->append(itemView);

            //child = root.nextSiblingElement("entry").toElement();
            child = child.nextSibling().toElement();
        }
    }
    // Some http error received
    else
    {
        QString messageErreur = trUtf8("Le site n'est pas accessible");
        ui->textNews->setHtml(messageErreur);
    }
    QScrollBar *sb = ui->textNews->verticalScrollBar(); //On scrolle vers le bas
    sb->setValue(sb->minimum());
    disconnect(m_nam,SIGNAL(finished(QNetworkReply*)),this, SLOT(slotAbeAproposFinishedNews(QNetworkReply*)));
}

void AbulEduAproposV1::slotAbeAproposFinishedResources(QNetworkReply* reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (reply->error() == QNetworkReply::NoError)
    {
        // lectures des données issues du "reply"
        xml= new QDomDocument();
        xml->setContent(reply->readAll());

        QDomElement xmlElem = xml->documentElement();
        QDomNode node = xmlElem.firstChild();
        while(!node.isNull())
        {
            QDomElement element = node.toElement(); // On essaie de convertir le node en element
            if(!element.isNull())
            {
                if(element.tagName()=="channel")    // Le node est bien un élément.
                {
                    QDomNode node=element.firstChild(); //On parcourt le document à partir du tag "channel"
                    while(!node.isNull())
                    {
                        QDomElement elem=node.toElement(); //On cherche le titre du "Channel"
                        if(elem.tagName()=="title")
                        {
                            QDomNode node=elem.firstChild();
                            QString titre=node.toText().data();
                            ui->textResources->setHtml("<h4><font color=red>"+titre+"</font></h4>");
                        }
                        else if((elem.tagName()=="item"))
                        {
                            QDomNode node=elem.firstChild();
                            while(!node.isNull())
                            {
                                QDomElement elem=node.toElement(); //Titre du fils de discussion
                                if(elem.tagName()=="title")
                                {
                                    QDomNode node=elem.firstChild();
                                    itemTitre=node.toText().data();
                                    node=node.nextSiblingElement("link");
                                }
                                if(elem.tagName()=="link")      // Lien vers post
                                {
                                    QDomNode node=elem.firstChild();
                                    itemLink=node.toText().data();
                                    node=node.nextSiblingElement("description");
                                }
                                if(elem.tagName()=="description") // Texte du post
                                {
                                    QDomNode node=elem.firstChild();
                                    itemDescription =node.toText().data();
                                    node=node.nextSibling();
                                }
                                node=node.nextSibling();
                            }
                            QString itemView= "<p><a href='"+itemLink+"'>"
                                              "<font color=green>"+itemTitre+"</font></a></p>"
                                              +itemDescription;
                            ui->textResources->append(itemView);
                        }
                        node=node.nextSibling();
                    }
                }
                node = node.nextSibling();
            }
        }
    }
    // Some http error received
    else
    {
        QString messageErreur = trUtf8("Le site n'est pas accessible");
        ui->textResources->setHtml(messageErreur);
    }
    QScrollBar *sb = ui->textResources->verticalScrollBar();       //On scrolle vers le bas
    sb->setValue(sb->minimum());
    disconnect(m_nam,SIGNAL(finished(QNetworkReply*)),this, SLOT(slotAbeAproposFinishedResources(QNetworkReply*)));
}

void AbulEduAproposV1::abeAproposSetMainWindow(QMainWindow *mw)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    installeMenu(mw);
}

void AbulEduAproposV1::on_lblPosezVotreQuestion_linkActivated(QString link)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(link);
    QString adresseForum = "http://forum.abuledu.org/";
    QDesktopServices::openUrl(adresseForum);
}

void AbulEduAproposV1::on_tabWidget_currentChanged(int index)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Petite astuce pour lancer les download des ressources en ligne si
    //on change d'onglet au lieu de cliquer directment dans le menu principal */
    if(index == ui->tabWidget->indexOf(ui->tabResources)) {
        slotAbeAproposShowResource();
    }
    if(index == ui->tabWidget->indexOf(ui->tabNews)) {
        slotAbeAproposShowNews();
    }
    if(index == ui->tabWidget->indexOf(ui->tabForum)) {
        slotAbeAproposShowForum();
    }
}

QUrl AbulEduAproposV1::abeBuildUrl(QString reflector, QString action)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString os;
#if defined(Q_OS_MAC)
    os="osx";
#elif defined(Q_OS_WIN)
    os="windows";
#elif defined(Q_OS_ANDROID)
    os="android" ;
#elif defined(Q_OS_LINUX)
    os="linux";
#endif

    QUrl adresseFlux = "http://updates.ryxeo.com/application/" + abeApp->applicationName() + "/version/" + abeApp->applicationVersion() + "/os/" + os + "/action/" + action + "/lang/" + QLocale::system().name().section('_', 0, 0) + "/reflector/" + reflector;
    return adresseFlux;
}

void AbulEduAproposV1::on_btnAproposClose_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeAproposBtnCloseClicked();
}
