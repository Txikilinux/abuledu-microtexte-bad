/** Classe AbulEduFileAboutV1
  * @see https://redmine.ryxeo.com/projects/
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

#include "abuledufileaboutv1.h"
#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduFileAboutV1::AbulEduFileAboutV1(QWidget *parent, const QString &fichierAbe) :
    QWidget(parent),
    ui(new Ui::AbulEduFileAboutV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
//    setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    m_abuleduFile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1(this), &QObject::deleteLater);
    abeSetFileName(fichierAbe);
}

AbulEduFileAboutV1::~AbulEduFileAboutV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    m_abuleduFile.clear();
}

bool AbulEduFileAboutV1::abeSetFileName(const QString &fichierAbe)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(fichierAbe != "") {
        QFileInfo fi(fichierAbe);
        if(fi.exists() && m_abuleduFile->abeFileOpen(fichierAbe)) {
            setWindowTitle(trUtf8("Propriétés du module %1").arg(fi.fileName()));
            p_afficheProprietes();
            return true;
        }
    }
    return false;
}

void AbulEduFileAboutV1::p_afficheProprietes()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QSharedPointer<AbulEduSCOLOMFrV1> scolom = m_abuleduFile->abeFileGetLOM();
    ui->lblTitreValue->setText(scolom->abeLOMgetGeneralTitle("fre"));

    //Auteurs
    if (scolom->abeLOMgetLifeCycleContributionRoles().size() > 0)
    {
        foreach(QString role, scolom->abeLOMgetLifeCycleContributionRoles())
        {
            foreach (QDate date,scolom->abeLOMgetLifeCycleContributionDates(role))
            {
                ui->teAuteurs->setPlainText(ui->teAuteurs->toPlainText() + scolom->abeLOMgetLifeCycleContributionEntities(role,date).first().getFullName() + trUtf8(" (%1, %2)\n").arg(role).arg(date.toString("dd.MM.yyyy")));
            }
        }
    }

    //Mots cles
    if (scolom->abeLOMgetGeneralKeywordKeys().count() > 0)
    {
        foreach(QString lang,scolom->abeLOMgetGeneralKeywordKeys())
        {
            foreach(QString keyword,scolom->abeLOMgetGeneralKeywords(lang))
            {
                ui->teMotsCles->setPlainText(ui->teMotsCles->toPlainText() + keyword + "\n");
            }
        }
    }

    ui->lblURIValue->setText(scolom->abeLOMgetGeneralIdentifierEntry());
    ui->lblTailleValue->setText(scolom->abeLOMgetTechnicalSize());
    ui->teResume->setPlainText(scolom->abeLOMgetGeneralDescription("fre").first());
    ui->lblIcone->setPixmap(m_abuleduFile->abeFileGetIcon());
}
