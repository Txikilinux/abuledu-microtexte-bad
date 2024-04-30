/**
  * Classe
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

#include "frmdisquelocal.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

frmDisqueLocal::frmDisqueLocal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDisqueLocal)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);

    // Icone mode + setsizeIcon + setSpacing ---> le rendu visuel est meilleur; p-ê qu'il faut gerer iconSize
    // par rapport à un ration :)
    ui->lwSimple->setDragEnabled(true);
    ui->lwSimple->setViewMode(QListView::IconMode);
    ui->lwSimple->setIconSize(QSize(120, 120));
    ui->lwSimple->setSpacing(10);
}

frmDisqueLocal::~frmDisqueLocal()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void frmDisqueLocal::setListeFichiers(QStringList liste)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << liste.count();
    m_listeFichiers = liste;
}

void frmDisqueLocal::on_btnRecherche_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /** CODE ERIC

    QRegExp r;
    r.setPattern(".*" + ui->leRecherche->text().trimmed() + ".*");
    r.setPatternSyntax(QRegExp::Wildcard);

    int premiere = m_listeFichiers.indexOf(QRegExp(),0);
    int derniere = m_listeFichiers.lastIndexOf(r);
    while(premiere > 0 && premiere <= derniere) {
            qDebug() << "On trouve " << premiere;
            //Creation des itemwidgets
            QFileInfo ficInfo(m_listeFichiers.at(premiere));

            // =================================================== la liste en icones
            //Pour tests on affiche toutes les vignettes d'un répertoire
            QListWidgetItem *itemIcone = new QListWidgetItem(ficInfo.fileName());

//            itemIcone->setData(Qt::UserRole+1,docId);
//            itemIcone->setData(Qt::UserRole+2,URI);
//            itemIcone->setData(Qt::UserRole+3,ficInfo.absoluteFilePath());
//            itemIcone->setData(Qt::UserRole+4,contributeur);
//            itemIcone->setData(Qt::UserRole+5,note);
//            itemIcone->setData(Qt::UserRole+6,resume);
//            itemIcone->setData(Qt::UserRole+7,motscles);
//            itemIcone->setData(Qt::UserRole+8,md5);
//            itemIcone->setData(Qt::UserRole+9,docFileName);

            //On cache le texte ... c'est rapide et le moins pire que j'ai trouvé
            QFont invisible("arial",1);
            itemIcone->setFont(invisible);
            QFileInfo ficone;

            QByteArray nomFichier = QString("file://%1").arg(ficInfo.absoluteFilePath()).toLatin1();
            QByteArray hashData = QCryptographicHash::hash(nomFichier,QCryptographicHash::Md5);

            ficone.setFile(abeApp->getAbeHomepath() + "/.thumbnails/normal/" + hashData.toHex() + ".png");
            itemIcone->setIcon(QIcon(ficone.absoluteFilePath()));
            ui->lwSimple->insertItem(0,itemIcone);

            premiere = m_listeFichiers.indexOf(r, premiere+1);
    }
*/
    /// TEST Icham 30/08/2012 ----> OK ça fonctionne =)

    ui->lwSimple->clear();

    QString recherche(ui->leRecherche->text().trimmed());
    ABULEDU_LOG_DEBUG() << "On recherche le mot :" << recherche;

    QRegExp rx(".*"+ recherche +".*");
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    for (int i =0 ; i< m_listeFichiers.count(); i++)
    {
        if (m_listeFichiers.at(i).contains(rx))
        {
            ABULEDU_LOG_DEBUG() << " TROUVER  a la position : " << i << "et le chemin est : " << m_listeFichiers.at(i) ;
            QFileInfo ficInfo(m_listeFichiers.at(i));

            QListWidgetItem *itemIcone = new QListWidgetItem(ficInfo.fileName());
            //On cache le texte ... c'est rapide et le moins pire que j'ai trouvé
            QFont invisible("andika",1);
            itemIcone->setFont(invisible);
            QFileInfo ficone;

            QByteArray nomFichier = QString("file://%1").arg(ficInfo.absoluteFilePath()).toLatin1();
            QByteArray hashData = QCryptographicHash::hash(nomFichier,QCryptographicHash::Md5);

            ficone.setFile(abeApp->getAbeHomepath() + "/.thumbnails/normal/" + hashData.toHex() + ".png");
            itemIcone->setIcon(QIcon(ficone.absoluteFilePath()));

            ui->lwSimple->insertItem(0,itemIcone);
        }
    }

}
