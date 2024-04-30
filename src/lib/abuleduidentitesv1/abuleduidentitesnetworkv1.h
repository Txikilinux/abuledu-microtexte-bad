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

#ifndef ABULEDUIDENTITESNETWORKV1_H
#define ABULEDUIDENTITESNETWORKV1_H

#include <QObject>
#include <QVariantMap>
#include <QUrl>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QUrlQuery>
#endif

#include "abuleduapplicationv1.h"
#include "abuledudownloadprogressbar.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "abuleduloggerv1.h"

class MaiaXmlRpcClient;

class AbulEduIdentitesNetworkV1 : public QObject
{
    Q_OBJECT
public:
    explicit AbulEduIdentitesNetworkV1(QObject *parent = 0);
    virtual ~AbulEduIdentitesNetworkV1();

signals:
    void signalServerReturnIdentite(QVariant &v, QNetworkReply *reply);
    void signalServerMessageAvailable(const QString &message);
    void signalErrorPDFDownload(const QString &errorMessage);
    void signalPDFDownloaded(const QString &filename);

private slots:
    /** Permet de detecter un changement de domaine et donc de changer l'uri de webadminxml */
    void slotSSOStatusChanged(const int code);

    void rpcLoaded(QVariant &v, QNetworkReply *reply);
    void rpcFault(int code, const QString &errorMessage);
    void rpcUpdated(QVariant &v, QNetworkReply *reply);

public slots:

    /** Permet de mettre à jour l'identité sur le serveur central */
    void abeUpdateIdentitie(const QVariantMap& tableau);

    /** Exporte mes logs en un fichier PDF, lance la requete au serveur qui va
      * exporter les logs et generer un fichier pdf puis le retourner via le reseau
      */
    void slotGetMyLogsAsPDF();

    /** Réception des logs PDF depuis le serveur */
    void slotLogsPDFDownloaded();

private:
    MaiaXmlRpcClient              *m_rpc;
    AbulEduNetworkAccessManagerV1 *m_nam;
    AbulEduDownloadProgressBar    *m_downloadProgress;
    QNetworkReply                 *m_networkReplyDownload;
};

#endif // ABULEDUIDENTITESNETWORKV1_H
