/**
  * Classe de base en version une pour les applications AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * #AbulEduApplicationV1
  * include(lib/abuleduapplicationv1/abuleduapplicationv1.pri)
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


/** =========================================== VERSION FAKE POUR PLUGIN QT CREATOR ===========================================  */

#ifndef ABULEDUAPPLICATIONV1_H
#define ABULEDUAPPLICATIONV1_H

#ifndef abeApp
#define abeApp ((AbulEduApplicationV1 *) qApp)
#endif

#include <stdio.h>

#include <QtGui/QApplication>
#include <QPluginLoader>
#include <QNetworkProxy>
#include <QSharedMemory>
#include <QMessageBox>
#include <QtXml/QtXml>
#include <QSettings>
#include <QHostInfo>
#include <QWidget>
#include <QThread>
#include <QString>
#include <QDebug>
#include <QEvent>
#include <QDate>
#include <QHash>
#include <QDir>
#include <QFontDatabase>

#include "abuleduconstantesv1.h"
#include "abuleduproxypassworddialog.h"
#include "abuledunetworkthreadv1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "abuledumessageboxv1.h"
#include "abuleduidentitesv1.h"


/** =========================================== VERSION FAKE POUR PLUGIN QT CREATOR ===========================================  */

class AbulEduApplicationV1: public QObject
{
    Q_OBJECT

public:
    QString applicationName() { return "plugin-qtcreator"; }
    QString applicationVersion() { return "0.1.0"; }
    QString organizationName() { return "ryxeo"; }
    QFont   font();
    void setActiveWindow (QWidget * active) {};


    AbulEduApplicationV1(int &argc, char *argv[], QString appName, QString appVersion, QString organizationDomain = "", QString organizationName = "", QString unitVersion = "1.0");
    ~AbulEduApplicationV1();
    QString getAbeApplicationLongName();
    void setAbeApplicationLongName(QString s);
    void setAbeApplicationDecorRatio(float ratio);
    float getAbeApplicationDecorRatio();
    QString getAbeApplicationThisOS() {return os();}
    AbulEduNetworkAccessManagerV1* getAbeNetworkAccessManager();
    AbulEduIdentitesV1* getAbeIdentite();
    QString getAbeHomepath();
    virtual void sendSignalDownloaded(QString s);
    QString uniqueUID();
    QString getAbeUserLogin();
    void setAbeApplicationVirtualKeyboard(bool enabled);

private slots:
    void slotOnlineUpdateRequest(QNetworkReply*);
    void slotReplyError(QNetworkReply::NetworkError code);

private:
    AbulEduNetworkAccessManagerV1*   m_nam;
    QNetworkReply*                   m_networkReply;
    QSharedMemory                    m_sharedMemory;
    int                              m_localDebug;
    QString                          m_applicationLongName;
    float                            m_decorRatio;
    bool                             m_browserLaunched;
    QString                          m_linuxPackageFormat;
    QString                          m_linuxDistrib;
    QString                          m_homepath;
    void p_setUserName();
    QString                          m_username;
    void detach();
    void loadPlugins();
    QString linuxPackageFormat();
    QString linuxDistrib();
    void linuxAnalyse();
    QString os();
    int execAge();
    AbulEduIdentitesV1*   m_identite;

signals:
    void signalApplicationReady();
    void signalDownloaded(QString s);
    void signalApplicationMessageStart();
    void signalApplicationMessageEnd();
};

#endif //end ABULEDUAPPLICATIONV1_H
