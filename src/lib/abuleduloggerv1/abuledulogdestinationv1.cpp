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

#include "abuledulogdestinationv1.h"
#include "abuledulogdestinationconsolev1.h"
#include "abuledudestinationfilev1.h"
#include <QString>
#include <QDebug>

namespace AbulEduLoggingV1
{

DestinationPtrV1 AbulEduLogDestinationFactoryV1::MakeFileDestination(const QString &filePath, LogRotationOption rotation, const MaxSizeBytes &sizeInBytesToRotateAfter, const MaxOldLogCount &oldLogsToKeep)
{
    if (EnableLogRotation == rotation) {
        QScopedPointer<SizeRotationStrategyV1> logRotation(new SizeRotationStrategyV1);
        logRotation->setMaximumSizeInBytes(sizeInBytesToRotateAfter.size);
        logRotation->setBackupCount(oldLogsToKeep.count);

        return DestinationPtrV1(new FileDestinationV1(filePath, RotationStrategyPtrV1(logRotation.take())));
    }

    return DestinationPtrV1(new FileDestinationV1(filePath, RotationStrategyPtrV1(new NullRotationStrategyV1)));
}

DestinationPtrV1 AbulEduLogDestinationFactoryV1::MakeDebugOutputDestination()
{
    return DestinationPtrV1(new DebugOutputDestination);
}

} // end namespace

