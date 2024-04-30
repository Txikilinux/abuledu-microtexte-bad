/** Classe AbulEduVCardComboBox
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_LOM_LOMFR_ScoLOMFR
  * @see http://www.lom-fr.fr/lomfr-unt/unisciel/modeles-de-fiche/LOMFRTemplate-Unisciel-v1.0-complet.xml/view
  *
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
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

#include "abuleduvcardcombobox.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduVCardComboBox::AbulEduVCardComboBox(QWidget *parent) :
    QComboBox(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setEditable(true);
    connect(this, SIGNAL(editTextChanged(QString)),this, SLOT(abeEditText(QString)), Qt::UniqueConnection);
    m_completer = new QCompleter();
    m_initialCompleter = new QCompleter();
    m_validator = new QRegExpValidator(this);
    m_nam = abeApp->getAbeNetworkAccessManager();
    m_delayRequest = new QTimer();
    connect(m_delayRequest, SIGNAL(timeout()), this, SLOT(delayReached()), Qt::UniqueConnection);
    m_isSearchInProgress = false;
    m_isCompleterActivated = false;

    m_rpc = new MaiaXmlRpcClient(QUrl(abe::ABE_WAN_SERVERWEBADMINURI + "login.php"), this);
    connect(m_nam,SIGNAL(ssoAuthStatus(int)),this,SLOT(slotSSOStatusChanged(int)), Qt::UniqueConnection);
}

AbulEduVCardComboBox::~AbulEduVCardComboBox()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete m_delayRequest;
    delete m_completer;
    delete m_initialCompleter;
    delete m_validator;
    delete m_rpc;
}

void AbulEduVCardComboBox::slotSSOStatusChanged(const int code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << code;

    if(code == 1) {
        //Si @servecole -> authentification locale
        if(m_nam->abeGetSSOLogin().contains("@" + abe::ABE_LAN_SERVERNAME)) {
            m_rpc->setUrl(QUrl(abe::ABE_LAN_SERVERWEBADMINURI + "login.php"));
        }
    }
}

vCard AbulEduVCardComboBox::abeGetVCard(QString name)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << name;

    for (int i=0; i < m_vCardList.count(); i++) {
        vCard v = m_vCardList.at(i);
        qDebug()<<"vCard cherchee avec "<<v;
        if (v.getFullName() == name)
        {
            ABULEDU_LOG_DEBUG() <<"vCard trouvee avec "<<name
                                << m_vCardList.at(i);
            return m_vCardList.at(i);
        }
    }
    ABULEDU_LOG_DEBUG()<<"vCard PAS trouvee avec "<<name;
    return vCard();
}

vCard AbulEduVCardComboBox::abeGetCurrentVCard()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< m_vCardList.at(0);

    if(m_vCardList.isEmpty()) return vCard();

    return m_vCardList.last();
}

bool AbulEduVCardComboBox::abeAddVCard(vCard card)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_vCardList << card;
    return true;
}

void AbulEduVCardComboBox::abeEditText(const QString &entity)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << entity;

    m_entity = entity;

    //On ne lance pas de recherche s'il y en a déjà une en cours
    if ((m_entity.size() > 2) && (m_isSearchInProgress == false))
    {
        //On remet le timer à zéro -> permet de faire une recherche un peu moins rapide
        m_delayRequest->stop();
        m_delayRequest->start(1250);
    }
    else {
        m_completer = m_initialCompleter;
    }
}

void AbulEduVCardComboBox::rpcGetSearchVCard(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << v.toMap();
    Q_UNUSED(reply);
    QApplication::restoreOverrideCursor();
    //Parcours des vcards
    m_vCardList.clear();
    QStringList liste;
    for(int i = 0; i < v.toMap().count(); i++) {
        vCard vcard = vCard::fromByteArray(v.toMap().value(QString::number(i)).toMap().value("vcard").toByteArray()).first();

        ABULEDU_LOG_DEBUG() << vcard.toByteArray();
        m_vCardList.append(vcard);
        if(vcard.getFullName().trimmed() != "") {
            ABULEDU_LOG_DEBUG() << " --> ok on l'ajoute !";
            liste << vcard.getFullName();
        }
    }

    m_completer = new QCompleter(liste,this);
    m_completer.data()->setCompletionRole(Qt::EditRole);
    m_completer.data()->setCaseSensitivity(Qt::CaseInsensitive);
    setCompleter(m_completer.data());
    m_completer.data()->complete();
    connect(m_completer, SIGNAL(activated(QString)), this, SLOT(completerActivated(QString)), Qt::UniqueConnection);
    m_isSearchInProgress=false;
}

void AbulEduVCardComboBox::completerActivated(QString value)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << value;
    m_isCompleterActivated = true;
}

void AbulEduVCardComboBox::delayReached()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ ;
    m_delayRequest->stop();

    //Si on a cliqué sur le completer et on le remet a false pour le prochain coup
    //si l'envie nous venait de chercher une autre vcard
    if(m_isCompleterActivated == true) {
        m_isCompleterActivated = false;
        return;
    }
    //Et sinon , c'est qu'on a pas cliqué sur le completer -> on lance la recherche reseau

    //uniquement si on est authentifié
    if(m_nam->abeSSOAuthenticationStatus() == 1) {
        //Pour les recherches c'est un peu plus complique, il faut passer un tableau
        QList<QVariant> params = QList<QVariant>() << m_entity;
        m_isSearchInProgress = true;
        m_rpc->call("abuledu.sso.getVcards", params,
                    this, SLOT(rpcGetSearchVCard(QVariant&, QNetworkReply*)),
                    this, SLOT(rpcFault(int, const QString &)));
        //On affiche le sablier le temps que ça télécharge ...
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    }
    else {
    //        m_nam->abeSSOLogin();
    }
}

void AbulEduVCardComboBox::rpcFault(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << code << errorMessage;
    //Tentative de réouverture d'une session SSO en cas de timeout
    //    if(code == -32300) {
    //        if (m_localDebug) qDebug() << "AbulEduVCardComboBox::rpcFault : abessoauthstatus: " << m_nam->abeSSOAuthenticationStatus();
    //        m_nam->abeSSOLogin();
    //    }
    QApplication::restoreOverrideCursor();
    m_isSearchInProgress=false;
}
