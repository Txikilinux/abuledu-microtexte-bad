/** Classe AbulEduSearchKeywordsLineEdit
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "abuledusearchkeywordslineedit.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduSearchKeywordsLineEdit::AbulEduSearchKeywordsLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //http://data.abuledu.org/vocabRameau/vocab/services.php?task=search&arg=papillon
    m_isAutoSearch = false;
    m_delayAutoSearch = 1250;

    m_nam = abeApp->getAbeNetworkAccessManager();
    m_isSearchInProgress = false;
    m_delayRequest = new QTimer();
    connect(m_delayRequest, SIGNAL(timeout()), this, SLOT(delayReached()), Qt::UniqueConnection);

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(abeTextChanged(QString)), Qt::UniqueConnection);
    connect(this, SIGNAL(returnPressed()), this, SLOT(abeReturnPressed()), Qt::UniqueConnection);

//    setStyleSheet("background: url(:/abuleduboxfilemanagerv1/loupe) no-repeat right;"
//                  "height: 25px;"
//                  "border: black;"
    //                  );
}

AbulEduSearchKeywordsLineEdit::~AbulEduSearchKeywordsLineEdit()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    delete m_delayRequest;
}

void AbulEduSearchKeywordsLineEdit::abeSearchDirectTerms(QString search)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< search;
    //Il nous faut son index, on utilise notre QMap pour ça
    ABULEDU_LOG_DEBUG() << search
                        << "recherche index de " << search << " --> " << m_answers.value(search)
                        << " ou " << m_answers.key(search);
    QString k = m_answers.key(search);
    if(k.isEmpty()) {
        return;
    }
    //http://data.abuledu.org/vocabRameau/vocab/services.php?task=fetchDirectTerms&arg=214491

    QUrl url("http://data.abuledu.org/vocabRameau/vocab/services.php?task=fetchDirectTerms&arg=" + k);
    m_networkReplyDownloadKeywords = m_nam->get(QNetworkRequest(url));
    m_networkReplyDownloadKeywords->setObjectName("downloadKeywords");

    connect(m_networkReplyDownloadKeywords, SIGNAL(finished()), this, SLOT(keywordsDownloaded()),Qt::UniqueConnection);
    //On affiche le sablier le temps que ça télécharge ...
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

QStringList AbulEduSearchKeywordsLineEdit::abeGetKeywordsDownloaded()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_answers.values();
}

void AbulEduSearchKeywordsLineEdit::abeSetAutoSearch(bool on, int delay)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_isAutoSearch = on;
    m_delayAutoSearch = delay;
}

void AbulEduSearchKeywordsLineEdit::abeTextChanged(const QString &search)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << search;
    m_search = search;

    //On ne lance pas de recherche s'il y en a déjà une en cours
    if ((m_search.size() > 2) && (m_isSearchInProgress == false) && (m_isAutoSearch == true))
    {
        ABULEDU_LOG_DEBUG() << "lancement du timer";
        //On remet le timer à zéro -> permet de faire une recherche un peu moins rapide
        m_delayRequest->stop();
        m_delayRequest->start(m_delayAutoSearch);
    }
    else {
        ABULEDU_LOG_DEBUG() << "recherche déjà en cours";
    }
}

void AbulEduSearchKeywordsLineEdit::abeReturnPressed()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_isAutoSearch == false) {
        //On lance la recherche "comme si" on était le timer :)
        delayReached();
    }
}

void AbulEduSearchKeywordsLineEdit::keywordsDownloaded()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< sender()->objectName();
    QApplication::restoreOverrideCursor();

    if("downloadKeywords" != sender()->objectName()) {
        return;
    }
    disconnect(m_networkReplyDownloadKeywords, SIGNAL(finished()), this, SLOT(keywordsDownloaded()));

    QDomDocument m_dom;
    m_dom.setContent(m_networkReplyDownloadKeywords->readAll());

    ABULEDU_LOG_DEBUG() << m_dom.firstChild().nodeName()
                        << m_dom.elementsByTagName("term").at(0).nodeName();

    m_answers.clear();
    //Parcours des réponses
    //    QStringList liste;
    for(int i = 0; i < m_dom.elementsByTagName("term").count(); i++) {
        QString v = m_dom.elementsByTagName("term").at(i).firstChildElement("string").text();
        QString k = m_dom.elementsByTagName("term").at(i).firstChildElement("term_id").text().remove("|");
        QString vv = m_dom.elementsByTagName("term").at(i).firstChildElement("no_term_string").text();
        //        qDebug() << " * pour " << k << "on a " << v << " mais on a aussi " << vv;
        m_answers.insert(k,v);
        if(vv != "")
            m_answers.insert(k+"b",vv);
        //        liste << v << vv;
        ABULEDU_LOG_DEBUG() << k << " : " << v;
    }
    m_isSearchInProgress=false;
    //On affiche la liste triée, c'est mieux pour l'utilisateur :)
    //    liste.sort();

    m_networkReplyDownloadKeywords->deleteLater();

    emit abeSignalKeywordsDownloaded();
}

void AbulEduSearchKeywordsLineEdit::delayReached()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_delayRequest->stop();

    //Si on a déjà une recherche en cours on la laisse finir
    if(m_isSearchInProgress) {
        return;
    }

    QUrl url("http://data.abuledu.org/vocabRameau/vocab/services.php?task=search&arg=" + this->text());
    m_networkReplyDownloadKeywords = m_nam->get(QNetworkRequest(url));
    m_networkReplyDownloadKeywords->setObjectName("downloadKeywords");

    m_isSearchInProgress = true;
    connect(m_networkReplyDownloadKeywords, SIGNAL(finished()), this, SLOT(keywordsDownloaded()),Qt::UniqueConnection);
    //On affiche le sablier le temps que ça télécharge ...
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void AbulEduSearchKeywordsLineEdit::rpcFault(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << code << errorMessage;
    QApplication::restoreOverrideCursor();
    m_isSearchInProgress=false;
}
