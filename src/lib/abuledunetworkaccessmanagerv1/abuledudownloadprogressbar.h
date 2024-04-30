/** Classe AbulEduDownloadProgressBar : popup de téléchargement
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

#ifndef ABULEDUDOWNLOADPROGRESSBAR_H
#define ABULEDUDOWNLOADPROGRESSBAR_H

#include <QNetworkReply>
#include <QWidget>
#include <QTime>
#include <QPointer>
#include <QDesktopWidget>
#include "abuleduloggerv1.h"
#include "ui_abuledudownloadprogressbar.h"

namespace Ui {
class AbulEduDownloadProgressBar;
}


class AbulEduDownloadProgressBar : public QWidget
{
    Q_OBJECT

public:
    /* Il faut passer un QNetworkReply */
    explicit AbulEduDownloadProgressBar(QNetworkReply *reply, QWidget *parent = 0);
    virtual ~AbulEduDownloadProgressBar();
    void abeUpdateStatus(const QString& message);
    QByteArray fileNameFromContentDisposition(const QByteArray &header);
    /* On peut "renverser" la downloadprogressbar en uploadprogressbar :) */
    bool abeSetUploadBar(bool upload);

private slots:
    void slotLogicielDownloadInProgress(qint64 a, qint64 b);
    void slotLogicielUploadInProgress(qint64 a, qint64 b);
    void on_AbulEduDownloadProgressBarBtnCancel_clicked();

private:
    Ui::AbulEduDownloadProgressBar  *ui;
    QNetworkReply                   *m_reply;
    QTime                           m_time;
    int                             m_total;
};

#endif // ABULEDUDOWNLOADPROGRESSBAR_H
