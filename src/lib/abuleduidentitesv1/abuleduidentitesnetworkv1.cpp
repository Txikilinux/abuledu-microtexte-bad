/** Classe AbulEduIdentitesNetworkV1 qui se charge des échanges réseaux
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

#include "abuleduidentitesnetworkv1.h"
#include "maiaXmlRpcClient.h"

#ifndef DEBUG_ABULEDUIDENTITESV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduIdentitesNetworkV1::AbulEduIdentitesNetworkV1(QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_nam = abeApp->getAbeNetworkAccessManager();
    m_rpc = new MaiaXmlRpcClient(QUrl(abe::ABE_WAN_SERVERWEBADMINURI + "login.php"), this);
    connect(m_nam,SIGNAL(ssoAuthStatus(int)),this,SLOT(slotSSOStatusChanged(int)), Qt::UniqueConnection);
}

AbulEduIdentitesNetworkV1::~AbulEduIdentitesNetworkV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    delete m_rpc;
    /* 20130715 : pas de delete de m_downloadProgress */
    //    delete m_downloadProgress;
}

void AbulEduIdentitesNetworkV1::rpcFault(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< code << errorMessage;
}

void AbulEduIdentitesNetworkV1::rpcLoaded(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    emit signalServerReturnIdentite(v,reply);
}

void AbulEduIdentitesNetworkV1::rpcUpdated(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "==================================="
                        << "Requette de mise à jour : " << v
                        << "===================================";

    emit signalServerMessageAvailable(QString::fromLatin1(v.toMap().value("Message").toByteArray()));
}

void AbulEduIdentitesNetworkV1::slotGetMyLogsAsPDF()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QUrl url;
    url.setUrl("https://leterrier.abuledu.net/export_logs.php");
    url.setUserName(m_nam->abeGetSSOLogin());
    url.setPassword(m_nam->abeGetSSOPassword());

    /* Si on est alacarte on recupere tous les logs, sinon c'est qu'on est lance depuis un logiciel du genre mulot et on ne veut pas les logs d'imageo */
    if(abeApp->applicationName() != "abuledu-alacarte") {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    url.addQueryItem("logiciel",abeApp->applicationName());
#else
    QUrlQuery query(url);
    query.addQueryItem("logiciel",abeApp->applicationName());
    url.setQuery(query);
#endif
    }

    QNetworkReply *networkReplyDownload = m_nam->get(QNetworkRequest(url));
    networkReplyDownload->setObjectName("downloadLogs");

    /** @warning : erreur on ouvre une popup mais pb de focus & de dessus / dessous ... il faut trouver une autre solution */
    /** @todo : lire le message de warning ci-dessus */
    m_downloadProgress = new AbulEduDownloadProgressBar(networkReplyDownload);
    m_downloadProgress->show();
    connect(networkReplyDownload,SIGNAL(finished()),this,SLOT(slotLogsPDFDownloaded()),Qt::UniqueConnection);
}

void AbulEduIdentitesNetworkV1::slotLogsPDFDownloaded()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(sender()->objectName() == "downloadLogs") {
        QNetworkReply *networkReplyDownload = (QNetworkReply*) sender();

        QString fic = m_downloadProgress->fileNameFromContentDisposition(networkReplyDownload->rawHeader("Content-Disposition"));
        if(fic.isEmpty())
            fic = "tempNameAlaCarteLogs.pdf";

        QString filename = QDir::tempPath() + "/" + fic;

        ABULEDU_LOG_DEBUG() << "Download save to" << filename;

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            emit signalErrorPDFDownload(trUtf8("Erreur d'écriture du fichier : %1\n").arg(filename));
            networkReplyDownload->deleteLater();
            m_downloadProgress->close();
            return;
        }

        file.write(networkReplyDownload->readAll());
        file.close();

        networkReplyDownload->deleteLater();

        m_downloadProgress->close();
        /* Maintenant on ouvre le fichier PDF */
        emit signalPDFDownloaded(filename);
    }
}


void AbulEduIdentitesNetworkV1::abeUpdateIdentitie(const QVariantMap &tableau)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << tableau;

    /* On fait une requête de mise à jour */
    QList<QVariant> params = QList<QVariant>() << tableau;
    m_rpc->call("abuledu.sso.updateLogin", params,
                this, SLOT(rpcUpdated(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));

}

void AbulEduIdentitesNetworkV1::slotSSOStatusChanged(const int code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << code;

    if(code == 1) {
        /* Si @servecole -> authentification locale */
        if(m_nam->abeGetSSOLogin().contains("@" + abe::ABE_LAN_SERVERNAME)) {
            m_rpc->setUrl(QUrl(abe::ABE_LAN_SERVERWEBADMINURI + "login.php"));
        }
    }
}
