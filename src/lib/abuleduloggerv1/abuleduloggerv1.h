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

#ifndef ABULEDULOGGERV1_H
#define ABULEDULOGGERV1_H

#include <QDebug>
#include <QString>

#include "abuleduloglevelv1.h"
#include "abuledulogdestinationv1.h"

namespace AbulEduLoggingV1 {

class AbulEduLogDestinationV1;
class AbulEduLoggerImplV1; // d pointer

class  AbulEduLoggerV1
{

public:
    static AbulEduLoggerV1& instance();
    static void destroyInstance();
    static AbulEduLogLevelV1 levelFromLogMessage(const QString& logMessage, bool* conversionSucceeded = 0);

    ~AbulEduLoggerV1();

    //! Ajoute une destinatination au log. Ne pas ajouter de destination nulle. @todo a protéger
    void addDestination(DestinationPtrV1 destination);
    //! Logging at a level < 'newLevel' will be ignored
    void setLoggingLevel(const AbulEduLogLevelV1 &newLevel);
    //! Le niveau par défaut est INFO
    AbulEduLogLevelV1 loggingLevel() const;

    //! The helper forwards the streaming to QDebug and builds the final
    //! log message.
    class Helper
    {
    public:
        explicit Helper(AbulEduLogLevelV1 logLevel);
        ~Helper();
        QDebug& stream(){ return qtDebug; }

    private:
        void writeToLog();

        AbulEduLogLevelV1 level;
        QString buffer;
        QDebug qtDebug;
    };

private:
    AbulEduLoggerV1();
    AbulEduLoggerV1(const AbulEduLoggerV1&);            // not available
    AbulEduLoggerV1& operator=(const AbulEduLoggerV1&); // not available

    void enqueueWrite(const QString& message, AbulEduLogLevelV1 level);
    void write(const QString& message, AbulEduLogLevelV1 level);

    AbulEduLoggerImplV1 *d;

    friend class AbulEduLogWriterRunnableV1;
};

} /* Namespace end */

//! Logging macros: define ABULEDU_LOG_LINE_NUMBERS to get the file and line number
//! in the log output.
#ifndef ABULEDU_LOG_LINE_NUMBERS
#define ABULEDU_LOG_TRACE() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::TraceLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::TraceLevel).stream()
#define ABULEDU_LOG_DEBUG() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::DebugLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::DebugLevel).stream()
#define ABULEDU_LOG_INFO()  \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::InfoLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::InfoLevel).stream()
#define ABULEDU_LOG_WARN()  \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::WarnLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::WarnLevel).stream()
#define ABULEDU_LOG_ERROR() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::ErrorLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::ErrorLevel).stream()
#define ABULEDU_LOG_FATAL() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::FatalLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::FatalLevel).stream()
#else
#define ABULEDU_LOG_TRACE() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::TraceLevel) {} \
    else  AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::TraceLevel).stream() << __FILE__ << '@' << __LINE__
#define ABULEDU_LOG_DEBUG() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::DebugLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::DebugLevel).stream() << __FILE__ << '@' << __LINE__
#define ABULEDU_LOG_INFO()  \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::InfoLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::InfoLevel).stream() << __FILE__ << '@' << __LINE__
#define ABULEDU_LOG_WARN()  \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::WarnLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::WarnLevel).stream() << __FILE__ << '@' << __LINE__
#define ABULEDU_LOG_ERROR() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::ErrorLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::ErrorLevel).stream() << __FILE__ << '@' << __LINE__
#define ABULEDU_LOG_FATAL() \
    if (AbulEduLoggingV1::AbulEduLoggerV1::instance().loggingLevel() > AbulEduLoggingV1::FatalLevel) {} \
    else AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::FatalLevel).stream() << __FILE__ << '@' << __LINE__
#endif


#ifdef ABULEDU_LOG_DISABLE
    #include "abuledudisableloggerv1.h"
#endif

#endif // ABULEDULOGGERV1_H
