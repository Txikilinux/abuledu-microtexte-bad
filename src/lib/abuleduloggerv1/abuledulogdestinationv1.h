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

#ifndef AbulEduLogDestinationV1_H
#define AbulEduLogDestinationV1_H


#include "abuleduloglevelv1.h"
#include <QSharedPointer>
#include <QtGlobal>
class QString;

namespace AbulEduLoggingV1
{

class AbulEduLogDestinationV1
{
public:
    virtual ~AbulEduLogDestinationV1(){}
    virtual void write(const QString& message, AbulEduLogLevelV1 level) = 0;
    virtual bool isValid() = 0; // returns whether the destination was created correctly
};
typedef QSharedPointer<AbulEduLogDestinationV1> DestinationPtrV1;


// a series of "named" paramaters, to make the file destination creation more readable
enum LogRotationOption
{
    DisableLogRotation = 0,
    EnableLogRotation  = 1
};

struct  MaxSizeBytes
{
    MaxSizeBytes() : size(0) {}
    MaxSizeBytes(qint64 size_) : size(size_) {}
    qint64 size;
};

struct  MaxOldLogCount
{
    MaxOldLogCount() : count(0) {}
    MaxOldLogCount(int count_) : count(count_) {}
    int count;
};


//! Creates logging destinations/sinks. The caller shares ownership of the destinations with the logger.
//! After being added to a logger, the caller can discard the pointers.
class  AbulEduLogDestinationFactoryV1
{
public:
    static DestinationPtrV1 MakeFileDestination(const QString& filePath,
        LogRotationOption rotation = DisableLogRotation,
        const MaxSizeBytes &sizeInBytesToRotateAfter = MaxSizeBytes(),
        const MaxOldLogCount &oldLogsToKeep = MaxOldLogCount());

    static DestinationPtrV1 MakeDebugOutputDestination();
};

} // end namespace

#endif // AbulEduLogDestinationV1_H
