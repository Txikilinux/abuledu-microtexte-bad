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

#ifndef ABULEDUVCARDCOMBOBOX_H
#define ABULEDUVCARDCOMBOBOX_H

#include <QComboBox>
#include <QCompleter>
#include <QRegExpValidator>
#include <QPointer>
#include <QTimer>
#include "abuledunetworkaccessmanagerv1.h"
#include "vcard.h"
#include "maiaXmlRpcClient.h"
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"

class AbulEduVCardComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AbulEduVCardComboBox(QWidget *parent = 0);
    virtual ~AbulEduVCardComboBox();
    vCard abeGetVCard(QString name);
    vCard abeGetCurrentVCard();
    bool abeAddVCard(vCard card);

signals:

public slots:
    void abeEditText(const QString & entity);
    void rpcGetSearchVCard(QVariant& v, QNetworkReply* reply);
    void completerActivated(QString value);
    void delayReached();

private slots:
    /** Permet de detecter un changement de domaine et donc de changer l'uri de webadminxml */
    void slotSSOStatusChanged(const int code);

    void rpcFault(int code, const QString &errorMessage);

protected:

private:
    QPointer<MaiaXmlRpcClient>              m_rpc;
    QPointer<AbulEduNetworkAccessManagerV1> m_nam;
    QPointer<QCompleter> m_completer;
    QPointer<QCompleter> m_initialCompleter;
    QPointer<QRegExpValidator> m_validator;
    QList<vCard> m_vCardList;
    QPointer<QTimer> m_delayRequest;
    QString m_entity;
    bool m_isSearchInProgress;
    bool m_isCompleterActivated;

};

#endif // ABULEDUVCARDCOMBOBOX_H
