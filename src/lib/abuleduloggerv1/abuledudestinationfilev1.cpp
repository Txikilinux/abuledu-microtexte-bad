/** Classe AbulEduLoggingV1 : gestion des logs
  *
  * @author 2014 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013-2014 Icham Sirat <icham.sirat@ryxeo.com>
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

#include "abuledudestinationfilev1.h"

#include <QTextCodec>
#include <QDateTime>
#include <QtGlobal>
#include <iostream>
#include <QDebug>

const int AbulEduLoggingV1::SizeRotationStrategyV1::MaxBackupCount = 10;

AbulEduLoggingV1::RotationStrategyV1::~RotationStrategyV1()
{
}

AbulEduLoggingV1::SizeRotationStrategyV1::SizeRotationStrategyV1()
    : mCurrentSizeInBytes(0)
    , mMaxSizeInBytes(0)
    , mBackupsCount(0)
{
}

void AbulEduLoggingV1::SizeRotationStrategyV1::setInitialInfo(const QFile &file)
{
    mFileName = file.fileName();
    mCurrentSizeInBytes = file.size();
}

void AbulEduLoggingV1::SizeRotationStrategyV1::includeMessageInCalculation(const QString &message)
{
    mCurrentSizeInBytes += message.toUtf8().size();
}

bool AbulEduLoggingV1::SizeRotationStrategyV1::shouldRotate()
{
    return mCurrentSizeInBytes > mMaxSizeInBytes;
}

// Algorithm assumes backups will be named filename.X, where 1 <= X <= mBackupsCount.
// All X's will be shifted up.
void AbulEduLoggingV1::SizeRotationStrategyV1::rotate()
{
//    qDebug() << "ROTATION" << mBackupsCount << shouldRotate() ;
    if (!mBackupsCount) {
        if (!QFile::remove(mFileName))
            std::cerr << "AbulEduLoggingV1: backup delete failed " << qPrintable(mFileName);
        return;
    }

    /** @todo Stratégie d'archivage ? */
//    /* Test recherche logFile */
//    const QString logNamePattern1 = mFileName + QString::fromUtf8(".%1");
//    for (int i = 1;i <= mBackupsCount;++i) {
//        const QString backupFileName = logNamePattern1.arg(i);
//        if (QFile::exists(backupFileName)){
////            qDebug() << "Il existe : " << backupFileName << i;
////            if (i == 10){
////                qDebug() << "Nous sommes au max, on archive =)";
////            }
//        }
//        else
//            break;
//    }

    // 1. find the last existing backup than can be shifted up
    const QString logNamePattern = mFileName + QString::fromUtf8(".%1");
    int lastExistingBackupIndex = 0;
    for (int i = 1;i <= mBackupsCount;++i) {
        const QString backupFileName = logNamePattern.arg(i);
        if (QFile::exists(backupFileName))
            lastExistingBackupIndex = qMin(i, mBackupsCount - 1);
        else
            break;
    }

    // 2. shift up
    for (int i = lastExistingBackupIndex;i >= 1;--i) {
        const QString oldName = logNamePattern.arg(i);
        const QString newName = logNamePattern.arg(i + 1);
        QFile::remove(newName);
        const bool renamed = QFile::rename(oldName, newName);
        if (!renamed) {
            std::cerr << "AbulEduLoggingV1: could not rename backup " << qPrintable(oldName)
                      << " to " << qPrintable(newName);
        }
    }

    // 3. rename current log file
    const QString newName = logNamePattern.arg(1);
    if (QFile::exists(newName))
        QFile::remove(newName);
    if (!QFile::rename(mFileName, newName)) {
        std::cerr << "AbulEduLoggingV1: could not rename log " << qPrintable(mFileName)
                  << " to " << qPrintable(newName);
    }
}

QIODevice::OpenMode AbulEduLoggingV1::SizeRotationStrategyV1::recommendedOpenModeFlag()
{
    return QIODevice::Append;
}

void AbulEduLoggingV1::SizeRotationStrategyV1::setMaximumSizeInBytes(qint64 size)
{
    Q_ASSERT(size >= 0);
    mMaxSizeInBytes = size;
}

void AbulEduLoggingV1::SizeRotationStrategyV1::setBackupCount(int backups)
{
    Q_ASSERT(backups >= 0);
    mBackupsCount = qMin(backups, SizeRotationStrategyV1::MaxBackupCount);
}

AbulEduLoggingV1::FileDestinationV1::FileDestinationV1(const QString& filePath, RotationStrategyPtrV1 RotationStrategy)
    : mRotationStrategyV1(RotationStrategy)
{
    mFile.setFileName(filePath);
    if (!mFile.open(QFile::WriteOnly | QFile::Text | mRotationStrategyV1->recommendedOpenModeFlag()))
    {
        std::cerr << "AbulEduLoggingV1 : could not open log file " << qPrintable(filePath);
        return;
    }
    mOutputStream.setDevice(&mFile);
    mOutputStream.setCodec(QTextCodec::codecForName("UTF-8"));

    mRotationStrategyV1->setInitialInfo(mFile);
}

void AbulEduLoggingV1::FileDestinationV1::write(const QString& message, AbulEduLogLevelV1)
{
    mRotationStrategyV1->includeMessageInCalculation(message);
    if (mRotationStrategyV1->shouldRotate()) {
        mOutputStream.setDevice(NULL);
        mFile.close();
        mRotationStrategyV1->rotate();
        if (!mFile.open(QFile::WriteOnly | QFile::Text | mRotationStrategyV1->recommendedOpenModeFlag()))
            std::cerr << "QsLog: could not reopen log file " << qPrintable(mFile.fileName());
        mRotationStrategyV1->setInitialInfo(mFile);
        mOutputStream.setDevice(&mFile);
    }

    mOutputStream << message << endl;
    mOutputStream.flush();
}

bool AbulEduLoggingV1::FileDestinationV1::isValid()
{
    return mFile.isOpen();
}

