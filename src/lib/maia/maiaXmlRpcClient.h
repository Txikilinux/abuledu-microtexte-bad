/*
 * libMaia - maiaXmlRpcClient.h
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

#ifndef MAIAXMLRPCCLIENT_H
#define MAIAXMLRPCCLIENT_H

#include <QtCore>
#include <QtXml>
#include <QtNetwork>
#include <QPointer>

#ifndef QT_NO_SSL
#include <QSslConfiguration>
#endif

//#include "abuleduapplicationv1.h"
#include "maiaFault.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "maiaObject.h"
#include "abuledudownloadprogressbar.h"

class MaiaXmlRpcClient : public QObject {
    Q_OBJECT

public:
    MaiaXmlRpcClient(QObject* parent = 0);
    MaiaXmlRpcClient(QUrl url, QObject* parent = 0);
    MaiaXmlRpcClient(QUrl url, QString userAgent, QObject *parent = 0);
    void setUrl(QUrl url);
    void setUserAgent(QString userAgent);
    QNetworkReply* call(QString method, QList<QVariant> args,
                        QObject* responseObject, const char* responseSlot,
                        QObject* faultObject, const char* faultSlot);

#ifndef QT_NO_SSL
    void setSslConfiguration(const QSslConfiguration &config);
    QSslConfiguration sslConfiguration () const;
#endif

    /**
      * Ajout d'une progress bar pour afficher la progression de download / upload
      * Si on passe un qwidget en parent alors elle s'incrustera dans ce parent
      */
    void activateProgressBar(bool valeur, bool upload = false, QWidget *parent = 0);

signals:
#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

private slots:
    void replyFinished(QNetworkReply*);

private:
    void init();
    QPointer<AbulEduNetworkAccessManagerV1> m_manager;
    QNetworkRequest                         m_request;
    QMap<QNetworkReply*, MaiaObject*>       m_callmap;
    QString                                 m_userAgentSignature;
    int                                     m_localDebug;
    QList<QNetworkReply*>                   m_replyAttacheAProgressBar; //Pour supprimer la progress barr du bon reply
    QPointer<AbulEduDownloadProgressBar>    m_downloadProgressBar;
    QPointer<QWidget>                       m_downloadProgressBarParent;
    bool                                    m_isDownloadProgressBarActive;
    bool                                    m_isDownloadProgressBarAnUploadBar;
};

#endif
