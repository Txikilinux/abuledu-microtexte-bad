/** Classe AbulEduLoggingV1 : gestion des logs
  *
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

#ifndef ABULEDUDESTINATIONFILEV1_H
#define ABULEDUDESTINATIONFILEV1_H
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QSharedPointer>

#include "abuledulogdestinationv1.h"

namespace AbulEduLoggingV1
{
class RotationStrategyV1
{
public:
    virtual ~RotationStrategyV1();

    virtual void setInitialInfo(const QFile &file) = 0;
    virtual void includeMessageInCalculation(const QString &message) = 0;
    virtual bool shouldRotate() = 0;
    virtual void rotate() = 0;
    virtual QIODevice::OpenMode recommendedOpenModeFlag() = 0;
};

// Never rotates file, overwrites existing file.
class NullRotationStrategyV1 : public RotationStrategyV1
{
public:
    virtual void setInitialInfo(const QFile &) {}
    virtual void includeMessageInCalculation(const QString &) {}
    virtual bool shouldRotate() { return false; }
    virtual void rotate() {}
    virtual QIODevice::OpenMode recommendedOpenModeFlag() { return QIODevice::Truncate; }
};

// Rotates after a size is reached, keeps a number of <= 10 backups, appends to existing file.
class SizeRotationStrategyV1 : public RotationStrategyV1
{
public:
    SizeRotationStrategyV1();
    static const int MaxBackupCount;

    virtual void setInitialInfo(const QFile &file);
    virtual void includeMessageInCalculation(const QString &message);
    virtual bool shouldRotate();
    virtual void rotate();
    virtual QIODevice::OpenMode recommendedOpenModeFlag();

    void setMaximumSizeInBytes(qint64 size);
    void setBackupCount(int backups);

private:
    QString mFileName;
    qint64 mCurrentSizeInBytes;
    qint64 mMaxSizeInBytes;
    int mBackupsCount;
};

typedef QSharedPointer<RotationStrategyV1> RotationStrategyPtrV1;

// file message sink
class FileDestinationV1 : public AbulEduLogDestinationV1
{
public:
    FileDestinationV1(const QString& filePath, RotationStrategyPtrV1 RotationStrategy);
    virtual void write(const QString& message, AbulEduLogLevelV1 level);
    virtual bool isValid();

private:
    QFile mFile;
    QTextStream mOutputStream;
    QSharedPointer<RotationStrategyV1> mRotationStrategyV1;
};

}


#endif // ABULEDUDESTINATIONFILEV1_H
