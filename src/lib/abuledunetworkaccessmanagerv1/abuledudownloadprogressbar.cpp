/** Classe AbulEduDownloadProgressBar : popup de téléchargement
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2014 Icham Sirat <icham.sirat@ryxeo.com>
  *
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

#include "abuledudownloadprogressbar.h"
#include "abuleduapplicationv1.h" /* @see #4006 */

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduDownloadProgressBar::AbulEduDownloadProgressBar(QNetworkReply *reply, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduDownloadProgressBar),
    m_reply(reply),
    m_time(QTime()),
    m_total(0)
{
    ui->setupUi(this);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* qu'on uploade ou downloade peu importe ... on affiche la progression :) */
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(slotLogicielDownloadInProgress(qint64,qint64)), Qt::UniqueConnection);
    m_time.start();

    /* On efface le message par defaut pour éviter le sentiment étrange lors de l'ouverture de la popup */
    abeUpdateStatus("");

    abeApp->abeCenterWindow(this);

    ui->AbulEduDownloadProgressBarProgressBar->setMinimum(0);
    ui->AbulEduDownloadProgressBarProgressBar->setMaximum(0);
}

AbulEduDownloadProgressBar::~AbulEduDownloadProgressBar()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduDownloadProgressBar::abeUpdateStatus(const QString &message)
{
    ABULEDU_LOG_TRACE() << message;

    ui->AbulEduDownloadProgressBarLblTexte->setText(message);
}

void AbulEduDownloadProgressBar::slotLogicielDownloadInProgress(qint64 a, qint64 b)
{
    ABULEDU_LOG_TRACE() << "a=" << a << "b=" << b;

    QString message;
    int tempsrestant = 0;
    int total        = 0;
    /* Liste des Headers existants  m_reply->rawHeaderList() :
     * ("Date", "Server", "X-Powered-By", "Content-Transfer-Encoding", "Content-MD5SUM", "Content-Length", "Content-Disposition", "Keep-Alive", "Connection", "Content-Type")
     * @see https://redmine.ryxeo.com/projects/abuledu-bibliotheque/wiki/Stockage_de_donn%C3%A9es
     * Cas Particulier serveur data "Content-Length" = "Content-Length2"
     *
     * Cas particulier logs
     * ("Date", "Server", "X-Powered-By", "Cache-Control", "Pragma", "Expires", "Last-Modified", "Content-Disposition", "Keep-Alive", "Connection", "Transfer-Encoding", "Content-Type")
     */

//    foreach (QByteArray var, m_reply->rawHeaderList()) {
//        qDebug() << var << m_reply->rawHeader(var);
//    }

    if(m_reply->hasRawHeader("Content-length2")){
        total = m_reply->rawHeader("Content-length2").toInt();
    }
    else
        total = m_reply->rawHeader("Content-length").toInt();

    m_total = total;
    b = total;

    /* Merci de laisser ce code ici, la valeur de b n'est pas forcement fiable d'ou les tests precedents */
    ui->AbulEduDownloadProgressBarProgressBar->setValue(a);
    ui->AbulEduDownloadProgressBarProgressBar->setMaximum(b);
    ui->AbulEduDownloadProgressBarProgressBar->update();

    QString unite       = trUtf8("o");
    QString uniteTemps  = trUtf8("secondes");
    int actuel          = a/1024;

    int vitesse = (a/m_time.elapsed()) * 1000; /* En o/sec */
    if(a > 0 && b > 0 && vitesse > 0) {
        int reste = b - a;
        tempsrestant = reste / vitesse;
        if(tempsrestant > 60) {
            tempsrestant = reste / vitesse / 60;
            uniteTemps = trUtf8("minutes");
        }
        else {
            tempsrestant++ ;
        }
        if ( vitesse > 1024){
            vitesse /= 1024;
            unite = trUtf8("Ko");
        }

        if(m_total > 1024) {
            total = m_total / 1024;
        }

//        if(m_localDebug) {
//            qDebug() << "  Valeur de a : " << a << " " << "Tps passé : "<< m_time.elapsed() <<" Tps restant : " << tempsrestant << " actuel : " << actuel << " total " << total << " vitesse " << vitesse;
//        }

        message = trUtf8("Temps restant : %1 %2 -- %3 sur %4 %5 (%6 Ko/s)")
                .arg(QString::number(tempsrestant))
                .arg(uniteTemps)
                .arg(QString::number(actuel))
                .arg(QString::number(total))
                .arg(unite)
                .arg(QString::number(vitesse));
    }
    else {
        message = trUtf8("Téléchargement en cours : %1 Ko/s").arg(QString::number(vitesse));
    }

    abeUpdateStatus(message);
}

void AbulEduDownloadProgressBar::slotLogicielUploadInProgress(qint64 a, qint64 b)
{
    QString message;
    int tempsrestant = 0;
    int total        = 0;

    /* Petite protection : au moins il faut un max -> b */
    if(b > 0){
        ui->AbulEduDownloadProgressBarProgressBar->setMaximum(b);
        ui->AbulEduDownloadProgressBarProgressBar->setValue(a);
    }
    QString unite      = trUtf8("Ko");
    QString uniteTemps = trUtf8("secondes");
    int actuel       = a/1024;
    int vitesse      = a/m_time.elapsed(); //En Ko/sec
    if(b > 0 && vitesse > 0) {
        total = b/1024;
        int reste = total - actuel;
        tempsrestant = reste / vitesse;
        if(tempsrestant > 60) {
            tempsrestant = reste / vitesse / 60;
            uniteTemps = trUtf8("minutes");
        }

        message = trUtf8("Temps restant : %1 %2 -- %3 sur %4 %5 (%6 Ko/s)")
                .arg(QString::number(tempsrestant))
                .arg(uniteTemps)
                .arg(QString::number(actuel))
                .arg(QString::number(total))
                .arg(unite)
                .arg(QString::number(vitesse));
    }
    else {
        message = trUtf8("Téléchargement en cours : %1 Ko/s")
                .arg(QString::number(vitesse));
    }

    abeUpdateStatus(message);
}

QByteArray AbulEduDownloadProgressBar::fileNameFromContentDisposition(const QByteArray &header)
{
    ABULEDU_LOG_TRACE() << "header = "<< header.data();

    foreach(const QByteArray item, header.split(';') )
    {
        int equalSignPos = item.indexOf('=');
        if (equalSignPos == -1)
            continue;
        QString key = item.left(equalSignPos).trimmed();
        if(key.compare("filename", Qt::CaseInsensitive) != 0)
            continue;

        QByteArray value = item.mid(equalSignPos+1).trimmed();
        if(value.startsWith('\"') && value.endsWith('\"'))
        {
            value = value.mid(1,value.length()-2);
        }
        return QByteArray::fromPercentEncoding(value);
    }
    return QByteArray();
}

bool AbulEduDownloadProgressBar::abeSetUploadBar(bool upload)
{
    ABULEDU_LOG_TRACE() << upload;

    if(upload) {
        //        disconnect(m_reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(slotLogicielDownloadInProgress(qint64,qint64)));
        connect(m_reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(slotLogicielUploadInProgress(qint64,qint64)),Qt::UniqueConnection);
    }
    else {
        //        disconnect(m_reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(slotLogicielUploadInProgress(qint64,qint64)));
        connect(m_reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(slotLogicielDownloadInProgress(qint64,qint64)),Qt::UniqueConnection);
    }

    return true;
}

void AbulEduDownloadProgressBar::on_AbulEduDownloadProgressBarBtnCancel_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On essaye de stopper le transfert en cours */
    m_reply->abort();
    this->close();
}
