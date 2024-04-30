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

#ifndef ABULEDUDISABLELOGGERV1_H
#define ABULEDUDISABLELOGGERV1_H

#include <QtDebug>
// When included AFTER QsLog.h, this file will disable logging in that C++ file. When included
// before, it will lead to compiler warnings or errors about macro redefinitions.

#undef ABULEDU_LOG_TRACE
#undef ABULEDU_LOG_DEBUG
#undef ABULEDU_LOG_INFO
#undef ABULEDU_LOG_WARN
#undef ABULEDU_LOG_ERROR
#undef ABULEDU_LOG_FATAL

#define ABULEDU_LOG_TRACE() if (1) {} else qDebug()
#define ABULEDU_LOG_DEBUG() if (1) {} else qDebug()
#define ABULEDU_LOG_INFO()  if (1) {} else qDebug()
#define ABULEDU_LOG_WARN()  if (1) {} else qDebug()
#define ABULEDU_LOG_ERROR() if (1) {} else qDebug()
#define ABULEDU_LOG_FATAL() if (1) {} else qDebug()

#endif // ABULEDUDISABLELOGGERV1_H
