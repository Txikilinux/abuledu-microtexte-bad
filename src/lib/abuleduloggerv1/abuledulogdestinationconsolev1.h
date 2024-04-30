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

#ifndef ABULEDULOGDESTINATIONCONSOLEV1_H
#define ABULEDULOGDESTINATIONCONSOLEV1_H


#include "abuledulogdestinationv1.h"

class QString;

class AbulEduDebugOutputV1
{
public:
   static void output(const QString& a_message);
};

namespace AbulEduLoggingV1
{

// debugger sink
class DebugOutputDestination : public AbulEduLogDestinationV1
{
public:
    virtual void write(const QString& message, AbulEduLogLevelV1 level);
    virtual bool isValid();
};

} /* end namespace */



#endif // ABULEDULOGDESTINATIONCONSOLEV1_H
