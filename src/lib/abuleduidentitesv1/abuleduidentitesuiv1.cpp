/** Classe AbulEduIdentitesUIV1
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "abuleduidentitesuiv1.h"

#ifndef DEBUG_ABULEDUIDENTITESV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduIdentitesUIV1::AbulEduIdentitesUIV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduIdentitesUIV1),
    m_identites(QList<QPointer<AbulEduIdentitesV1> >()),
    m_abuleduIdentite(new AbulEduIdentitesV1(this)),
    m_selectedAvatar(0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->setupUi(this);
    m_abuleduIdentite = abeApp->getAbeIdentite();
    connect(m_abuleduIdentite,SIGNAL(signalLoaded()),this,SLOT(slotIdentitieLoaded()),Qt::UniqueConnection);
    connect(m_abuleduIdentite,SIGNAL(signalMessageAvailable(QString)),this,SLOT(slotMessageAvailable(QString)),Qt::UniqueConnection);

    /* Photo anonyme par defaut */
    ui->btnPhoto->setIconeNormale(":/abuleduidentitesv1/avatars/anonyme");

    /* pageDetail par defaut*/
    ui->stackedWidget->setCurrentWidget(ui->pageIdentiteDetails);
}

AbulEduIdentitesUIV1::~AbulEduIdentitesUIV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduIdentitesUIV1::abeLoad()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->cbIdentite->setCurrentIndex(-1);
    ui->cbIdentite->clear();
    m_identites.clear();
}

void AbulEduIdentitesUIV1::abeRefresh()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << m_abuleduIdentite->abeGetLogin()
                        << abeApp->getAbeNetworkAccessManager()->abeGetSSOLogin();

    /* Si c'est l'identifiant sous lequel je me suis authentifié je peux changer mes informations, sinon c'est en lecture seule */
    if(m_abuleduIdentite->abeGetLogin() == abeApp->getAbeNetworkAccessManager()->abeGetSSOLogin()) {
        QList<QLineEdit *> allLE = findChildren<QLineEdit *>();
        QListIterator<QLineEdit*> i(allLE);
        while (i.hasNext()) {
            //qDebug() << "objet (a): " << i.next()->objectName();
            i.next()->setEnabled(true);
        }
    }
    else {
        QList<QLineEdit *> allLE = findChildren<QLineEdit *>();
        QListIterator<QLineEdit*> i(allLE);
        while (i.hasNext()) {
            //qDebug() << "objet (b): " << i.next()->objectName();
            i.next()->setEnabled(false);
        }
    }
    ui->leDomaine->setText(m_abuleduIdentite->abeGetDomaine());
    ui->leNom->setText(m_abuleduIdentite->abeGetNom());
    ui->lePrenom->setText(m_abuleduIdentite->abeGetPreNom());
    ui->leTelephone->setText(m_abuleduIdentite->abeGetTelephone());
    ui->leMobile->setText(m_abuleduIdentite->abeGetMobile());
    if(m_abuleduIdentite->abeGetEmail().size() > 0) {
        ui->leMail->setText(m_abuleduIdentite->abeGetEmail().at(0));
    }
    else {
        ui->leMail->setText("");
    }
    ui->leAdresse1->setText(m_abuleduIdentite->abeGetAdresse());
    ui->leAdresse2->setText(m_abuleduIdentite->abeGetCodePostal());
    ui->leAdresse3->setText(m_abuleduIdentite->abeGetVille());
    if(m_abuleduIdentite->abeGetResponsables().size() > 0)
        ui->leResponsable1->setText(m_abuleduIdentite->abeGetResponsables().at(0));
    ui->leLogin->setText(m_abuleduIdentite->abeGetLogin());
    if(!m_abuleduIdentite->abeGetPhoto()->isNull()) {
        if(m_localDebug) {
            ABULEDU_LOG_DEBUG() << "  on a une photo ...";
            ABULEDU_LOG_DEBUG() << "  et on la sauvegarde ici " << abeApp->getAbeHomepath() + "/leterrier/avatars/" + m_abuleduIdentite->abeGetLogin() + ".jpg";
        }
        QPixmap p(*m_abuleduIdentite->abeGetPhoto());
        QDir dir(abeApp->getAbeHomepath() + "/leterrier/avatars/");
        if(!dir.exists()) {
            dir.mkpath(abeApp->getAbeHomepath() + "/leterrier/avatars/");
        }
        p.save(abeApp->getAbeHomepath() + "/leterrier/avatars/" + m_abuleduIdentite->abeGetLogin() + ".jpg");
        ui->btnPhoto->setIconeNormale(abeApp->getAbeHomepath().replace("\\","/") + "/leterrier/avatars/" + m_abuleduIdentite->abeGetLogin() + ".jpg");
    }
    else {
        ABULEDU_LOG_DEBUG() << "On n'a pas de photo ...";
        ui->btnPhoto->setIconeNormale(":/abuleduidentitesv1/avatars/defaut");
        m_abuleduIdentite->abeSetPhoto(new QPixmap(":/abuleduidentitesv1/avatars/defaut"));
    }

    if(m_abuleduIdentite->abeGetDomaine().contains("unifiedaccounts.zz")) {
        ui->cbIdentite->insertItem(0,trUtf8("%1 (identité principale)").arg(m_abuleduIdentite->abeGetLogin()));
    }
    else {
        ui->cbIdentite->insertItem(0,m_abuleduIdentite->abeGetLogin());
    }
}

AbulEduIdentitesV1 *AbulEduIdentitesUIV1::abeGetIdentite(const QString &login)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On cherche dans la liste des identites s'il y en a une qui a email comme adresse email, si c'est le cas on la retourne */
    for(int i = 0; i < m_identites.count(); i++) {
            ABULEDU_LOG_DEBUG() << login << " pour " << i;
        if(m_identites.at(i).data()->abeGetLogin() == login) {
            ABULEDU_LOG_DEBUG() << i << " : " << m_identites.at(i).data()->abeGetLogin();
            m_abuleduIdentite = m_identites.at(i);
        }
    }
    return m_abuleduIdentite;
}

void AbulEduIdentitesUIV1::on_btnUpdate_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Si pas authentifie, le bouton update est pas bon */
    m_abuleduIdentite->abeSetMobile(ui->leMobile->text().trimmed());
    m_abuleduIdentite->abeSetTelephone(ui->leTelephone->text().trimmed());
    m_abuleduIdentite->abeSetNom(ui->leNom->text().trimmed());
    m_abuleduIdentite->abeSetPreNom(ui->lePrenom->text().trimmed());
    m_abuleduIdentite->abeSetEmail(ui->leMail->text().trimmed());
    m_abuleduIdentite->abeSetAdresse(ui->leAdresse1->text().trimmed());
    m_abuleduIdentite->abeSetCodePostal(ui->leAdresse2->text().trimmed());
    m_abuleduIdentite->abeSetVille(ui->leAdresse3->text().trimmed());

    m_abuleduIdentite->abeSetPhoto(new QPixmap(ui->btnPhoto->getIconeNormale()));

    QStringList resps;
    resps.append(ui->leResponsable1->text().trimmed());
    resps.append(ui->leResponsable2->text().trimmed());
    m_abuleduIdentite->abeSetResponsables(resps);

    /* Si le mot de passe n'est pas vide *et* confirmé */
    if(!ui->lePassword1->text().trimmed().isEmpty() &&
            (ui->lePassword1->text().trimmed() == ui->lePassword2->text().trimmed())
            ) {
        m_abuleduIdentite->abeSetPassword(ui->lePassword1->text().trimmed());
    }
    m_abuleduIdentite->abeUpdateIdentitie();
}


void AbulEduIdentitesUIV1::on_btnUnifier_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* On affiche une popup qui demande le mail principal */
    bool ok;
    QString text = QInputDialog::getText(this, trUtf8("Unification de compte"),
                                         trUtf8("Adresse email du compte principal:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        ABULEDU_LOG_DEBUG() << "Demande d'unification avec le compte mail " << text;
        ui->lblMessage->document()->setPlainText(trUtf8("Demande d'unification du compte %1 avec %2 en cours.").arg(ui->lblLogin->text()).arg(text));

        /* @todo 20121105 */
        // m_abuleduIdentite->abeUnifiedWith(text);
    }
}

void AbulEduIdentitesUIV1::on_cbIdentite_currentIndexChanged(const int index)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << index;

    //    QString responsable;
    //    if(m_identites.count() >= index && index >= 0) {
    //        if(!m_identites.at(index).data()->abeGetResponsables().isEmpty())
    //            responsable = m_identites.at(index).data()->abeGetResponsables().at(0);
    //        this->refresh();
    //    }
}

void AbulEduIdentitesUIV1::on_btnMesLogs_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abuleduIdentite->abeGetMyLogsAsPDF();
}


void AbulEduIdentitesUIV1::slotChoixAvatar(const QString &avatar)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->btnPhoto->setIconeNormale(avatar);
}

void AbulEduIdentitesUIV1::slotIdentitieLoaded()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeLoad();
    abeRefresh();
}

void AbulEduIdentitesUIV1::slotMessageAvailable(const QString &message)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->lblMessage->document()->setPlainText(message);
    QTimer::singleShot(3000,ui->lblMessage,SLOT(clear()));
    /* si on arrive là, c'est que la requête est revenue et est ok -> emission du signal update */
    emit signalAbeIdentityUpdated();
}

void AbulEduIdentitesUIV1::on_lePassword1_textChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(arg1);

    if(ui->lePassword1->text() != ui->lePassword2->text()) {
        ui->btnUpdate->setEnabled(false);
    }
    else {
        ui->btnUpdate->setEnabled(true);
    }
}

void AbulEduIdentitesUIV1::on_lePassword2_textEdited(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    on_lePassword1_textChanged(arg1);
}

// //////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       GESTION AVATARS
//
// //////////////////////////////////////////////////////////////////////////////////////////////////////
void AbulEduIdentitesUIV1::on_btnPhoto_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->stackedWidget->setCurrentWidget(ui->pageIdentiteAvatars);
    /* Par défaut le  bouton "Mettre a jour Avatar" n'est pas actif (tant qu'une vignette n'est pas selectionnée */
    ui->btnUpdateAvatar->setEnabled(false);

    QDir dir(":/abuleduidentitesv1/avatars/");
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    int ligne = 0;
    int col   = 0;
    const int listSize = list.size();
    for (int i = 0; i < listSize; ++i) {
        QPushButton *btn = new QPushButton(this);
        ui->gridLayoutAvatars->addWidget(btn,ligne,col,1,1,0);
        btn->setFixedSize(64,64);
        btn->setIcon(QIcon(list.at(i).absoluteFilePath()));
        btn->setIconSize(QSize(54,54));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setStyleSheet("background-color: transparent;");
        btn->show();
        btn->setObjectName(list.at(i).absoluteFilePath());
        connect(btn, SIGNAL(clicked()),this,SLOT(slotSelectAvatar()),Qt::UniqueConnection);

        col++;
        if(col == 10) { /* 10 icones par ligne */
            col = 0;
            ligne++;
        }
    }
}

void AbulEduIdentitesUIV1::slotSelectAvatar()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Cast Sender en AbulEduFlatButtonV1 */
    QPushButton *cast = qobject_cast<QPushButton *>(sender());
    if(cast){
        if(m_selectedAvatar != cast){
            ABULEDU_LOG_DEBUG() << "Selection Avatar 1:" << cast->objectName();

            /* Gestion styleSheet */
            cast->setStyleSheet("background-color: orange;");
            if(m_selectedAvatar)
                m_selectedAvatar->setStyleSheet("background-color: transparent;");

            m_selectedAvatar = cast;
            ui->btnUpdateAvatar->setEnabled(true);
        }
        else{
            ABULEDU_LOG_DEBUG() << "Deselection avatar :" << cast->objectName();

            cast->setStyleSheet("background-color: transparent;");
            m_selectedAvatar = 0;
            ui->btnUpdateAvatar->setEnabled(false);
        }
    }
}

void AbulEduIdentitesUIV1::hideEvent(QHideEvent *hideEvent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    hideEvent->accept();

    /* click btn -> passage par slotSelectAvatar() qui s'occupe de remettre le stylesheet et ce pointeur à 0 */
    if(m_selectedAvatar)
        m_selectedAvatar->click();

    ui->stackedWidget->setCurrentWidget(ui->pageIdentiteDetails);
}

void AbulEduIdentitesUIV1::on_btnUpdateAvatar_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Si on a un avatar selectionné */
    if(m_selectedAvatar){
        ABULEDU_LOG_DEBUG() << "Nouvel avatar selectionné :" << m_selectedAvatar->objectName();
        slotChoixAvatar(m_selectedAvatar->objectName());
        /* click btn -> passage par slotSelectAvatar() qui s'occupe de remettre le stylesheet et ce pointeur à 0 */
        m_selectedAvatar->click();
        ui->stackedWidget->setCurrentWidget(ui->pageIdentiteDetails);

        /* MAJ identite complete */
        on_btnUpdate_clicked();
    }
}

void AbulEduIdentitesUIV1::on_btnAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* click btn -> passage par slotSelectAvatar() qui s'occupe de remettre le stylesheet et ce pointeur à 0 */
    if(m_selectedAvatar)
        m_selectedAvatar->click();

    ui->stackedWidget->setCurrentWidget(ui->pageIdentiteDetails);
}
