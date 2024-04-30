/*
 * libMaia - maiaXmlRpcClient.cpp
 * Copyright (c) 2007 Sebastian Wiedenroth <wiedi@frubar.net>
 *                and Karl Glatz
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "maiaXmlRpcClient.h"

MaiaXmlRpcClient::MaiaXmlRpcClient(QObject* parent) : QObject(parent)
{
    init();
}

MaiaXmlRpcClient::MaiaXmlRpcClient(QUrl url, QObject* parent) : QObject(parent)
{
    init();
    setUrl(url);
}

MaiaXmlRpcClient::MaiaXmlRpcClient(QUrl url, QString userAgent, QObject *parent) : QObject(parent) {
    // userAgent should adhere to RFC 1945 http://tools.ietf.org/html/rfc1945
    init();
    setUserAgent(userAgent);
    setUrl(url);
}

void MaiaXmlRpcClient::init() {
    m_localDebug = 0;

    //Par défaut on ne veut pas de barre de progression
    m_isDownloadProgressBarActive = false;
    //Par défaut c'est une barre de download
    m_isDownloadProgressBarAnUploadBar = false;

    if(m_localDebug) {
        qDebug() << "MaiaXmlRpcClient::init (1)";
    }
    m_manager  = abeApp->getAbeNetworkAccessManager();
    m_userAgentSignature = abeApp->applicationName() + "/" + abeApp->applicationVersion() + " libmaia/0.2ryxeo1";
    setUserAgent("");
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    if(m_localDebug) {
        qDebug() << "MaiaXmlRpcClient::init (2)";
    }
}

void MaiaXmlRpcClient::setUrl(QUrl url) {
    if(!url.isValid())
        return;
    m_request.setUrl(url);
}

void MaiaXmlRpcClient::setUserAgent(QString userAgent) {
    m_request.setRawHeader("User-Agent", userAgent.toLatin1() + " " + m_userAgentSignature.toLatin1());
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
}

QNetworkReply* MaiaXmlRpcClient::call(QString method, QList<QVariant> args,
                                      QObject* responseObject, const char* responseSlot,
                                      QObject* faultObject, const char* faultSlot) {
    MaiaObject* call = new MaiaObject(this);
    connect(call, SIGNAL(aresponse(QVariant &, QNetworkReply *)), responseObject, responseSlot);
    connect(call, SIGNAL(fault(int, const QString &, QNetworkReply *)), faultObject, faultSlot);

    //On force l'authentification dans l'url
    if(m_manager->abeSSOAuthenticationStatus() > 0) {
        QUrl urlTemp  = m_request.url();
        urlTemp.setUserName(m_manager->abeGetSSOLogin());
        urlTemp.setPassword(m_manager->abeGetSSOPassword());
        m_request.setUrl(urlTemp);
    }

    QNetworkReply* reply = m_manager->post( m_request,
                                            call->prepareCall(method, args).toUtf8() );

    if(m_isDownloadProgressBarActive) {
        if(m_localDebug) {
            qDebug() << "Maia : on créé une progress bar pour l'objet ";
        }
        /** @warning : erreur on ouvre une popup mais pb de focus & de dessus / dessous ... il faut trouver une autre solution */
        /** @todo : lire le message de warning ci-dessus */
        m_downloadProgressBar = new AbulEduDownloadProgressBar(reply, m_downloadProgressBarParent);
        m_downloadProgressBar->abeSetUploadBar(m_isDownloadProgressBarAnUploadBar);
        m_downloadProgressBar->show();
        m_replyAttacheAProgressBar << reply;
    }

    m_callmap[reply] = call;
    return reply;
}

#ifndef QT_NO_SSL
void MaiaXmlRpcClient::setSslConfiguration(const QSslConfiguration &config) {
    m_request.setSslConfiguration(config);
}

QSslConfiguration MaiaXmlRpcClient::sslConfiguration () const {
    return m_request.sslConfiguration();
}
#endif

void MaiaXmlRpcClient::activateProgressBar(bool valeur, bool upload, QWidget *parent)
{
    m_isDownloadProgressBarActive = valeur;
    m_isDownloadProgressBarAnUploadBar = upload;
    if(parent != 0) {
        m_downloadProgressBarParent = parent;
    }
}

void MaiaXmlRpcClient::replyFinished(QNetworkReply* reply) {
    if(m_localDebug) {
        qDebug() << "MaiaXmlRpcClient::replyFinished (1) " << objectName() << " :: " << reply->objectName();
    }

    QString response;
    //La voila l'astuce maia pour eviter de traiter les networkreply des copains
    //c'est super futé !
    if(!m_callmap.contains(reply))
        return;

    if(reply->error() != QNetworkReply::NoError) {
        if(reply->error() == QNetworkReply::UnknownContentError) {
            //QNetworkReply::UnknownContentError = 299
            //La qregexp c'est pour n'avoir que le message d'erreur du serveur
            MaiaFault fault(-32300, reply->errorString());
            response = fault.toString();
        }
        else {
            MaiaFault fault(-32300, reply->errorString() + " :: " + QString::number(reply->error()));
            response = fault.toString();
        }
    } else {
        response = reply->readAll();
    }

    if(m_downloadProgressBar && m_replyAttacheAProgressBar.contains(reply)) {
        if(m_localDebug) {
            qDebug() << "Maia: on supprime la progress bar";
        }
        m_downloadProgressBar->close();
        m_downloadProgressBar->deleteLater();
        m_replyAttacheAProgressBar.removeAll(reply);
    }

    // parseResponse deletes the MaiaObject
    m_callmap[reply]->parseResponse(response, reply);
    reply->deleteLater();
    m_callmap.remove(reply);
}
