/** Classe AbulEduSearchKeywordsLineEdit
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/
  *
  * Cette classe permet de faire des recherches sur la base de mots clés de notre thésaurus (RAMEAU)
  * C'est un dérivé de QLineEdit qui lance une recherche toute seule au bout de 2secondes d'inactivité
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  */

 /* This program is free software; you can redistribute it and/or modify
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

#ifndef ABULEDUSEARCHKEYWORDSLINEEDIT_H
#define ABULEDUSEARCHKEYWORDSLINEEDIT_H

#include <QLineEdit>
#include <QCompleter>
#include <QRegExpValidator>
#include <QPointer>
#include <QTimer>
#include "abuledunetworkaccessmanagerv1.h"
#include "maiaXmlRpcClient.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

class AbulEduSearchKeywordsLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit AbulEduSearchKeywordsLineEdit(QWidget *parent = 0);

    virtual ~AbulEduSearchKeywordsLineEdit();

    /**
      * Permet de lancer une recherche depuis un objet externe, retourne une liste de réponses
      * correspondantes au mot clé en question, comme il faut la clé du mot, on passe par la
      * structure de données m_answers
      */
    void abeSearchDirectTerms(QString search);

signals:
    /**
      * Signal lancé quand la réponse est prête
      */
    void abeSignalKeywordsDownloaded();

public slots:
    /**
      * public pour pouvoir connecter qqchose et récupérer le résultat de la recherche
      */
    QStringList abeGetKeywordsDownloaded();

    /**
      * permet d'activer la recherche automatique au bout d'un certain temps
      */
    void abeSetAutoSearch(bool on, int delay);

private slots:
    void abeTextChanged(const QString & search);
    void abeReturnPressed();
    void keywordsDownloaded();
    void rpcFault(int code, const QString &errorMessage);
    void delayReached();

protected:

private:
    QPointer<AbulEduNetworkAccessManagerV1> m_nam; //connexion réseau
    QPointer<QTimer>                        m_delayRequest; //
    QNetworkReply                          *m_networkReplyDownloadKeywords; //
    bool                                    m_isSearchInProgress; //Flag pour éviter de lancer plusieurs recherches en mm temps
    QString                                 m_search;  //Le mot dont la recherche est en cours
    QMap<QString,QString>                   m_answers; //La liste des réponses -> permet de faire les recherches sur les mots voisins
    bool                                    m_isAutoSearch;
    int                                     m_delayAutoSearch;
};

#endif // ABULEDUSEARCHKEYWORDSLINEEDIT_H
