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
#include <QString>
#include <QtGlobal>

#if defined(Q_OS_WIN)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
void AbulEduDebugOutputV1::output( const QString& message )
{
   OutputDebugStringW(reinterpret_cast<const WCHAR*>(message.utf16()));
   OutputDebugStringW(L"\n");
}
#elif defined(Q_OS_UNIX)
#include <cstdio>
void AbulEduDebugOutputV1::output( const QString& message )
{
   fprintf(stderr, "%s\n", qPrintable(message));
   fflush(stderr);
}
#endif

void AbulEduLoggingV1::DebugOutputDestination::write(const QString& message, AbulEduLogLevelV1)
{
    AbulEduDebugOutputV1::output(message);
}

bool AbulEduLoggingV1::DebugOutputDestination::isValid()
{
    return true;
}
