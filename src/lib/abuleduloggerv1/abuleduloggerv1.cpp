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

#include "abuleduloggerv1.h"

#ifdef ABULEDU_LOG_SEPARATE_THREAD
    #include <QThreadPool>
    #include <QRunnable>
#endif
#include <QMutex>
#include <QVector>
#include <QDateTime>
#include <QtGlobal>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

namespace AbulEduLoggingV1 {

typedef QVector<DestinationPtrV1> DestinationList;

static const char TraceString[] = "[T]";
static const char DebugString[] = "[D]";
static const char InfoString[]  = "[I]";
static const char WarnString[]  = "[W]";
static const char ErrorString[] = "[E]";
static const char FatalString[] = "[F]";

/* Ne pas utiliser Qt::ISODate (pas de millisecondes)  */
static const QString fmtDateTime("yyyy-MM-ddThh:mm:ss.zzz");

static AbulEduLoggerV1* INSTANCE = 0;

static const char* LevelToText(const AbulEduLogLevelV1 &theLevel)
{
    switch (theLevel) {
    case TraceLevel:
        return TraceString;
    case DebugLevel:
        return DebugString;
    case InfoLevel:
        return InfoString;
    case WarnLevel:
        return WarnString;
    case ErrorLevel:
        return ErrorString;
    case FatalLevel:
        return FatalString;
    case OffLevel:
        return "";
    default: {
        assert(!"bad log level");
        return InfoString;
    }
    }
}

#ifdef ABULEDU_LOG_SEPARATE_THREAD
class AbulEduLogWriterRunnableV1 : public QRunnable
{
public:
    AbulEduLogWriterRunnableV1(const QString &message, const AbulEduLogLevelV1 &level);
    virtual void run();

private:
    QString mMessage;
    AbulEduLogLevelV1 mLevel;
};
#endif

class AbulEduLoggerImplV1
{
public:
    AbulEduLoggerImplV1();

#ifndef Q_OS_ANDROID
    QMutex logMutex;
#endif
    AbulEduLogLevelV1 level;
    DestinationList destList;
};

#ifdef ABULEDU_LOG_SEPARATE_THREAD
AbulEduLogWriterRunnableV1::AbulEduLogWriterRunnableV1(const QString &message, const AbulEduLogLevelV1 &level)
    : QRunnable()
    , mMessage(message)
    , mLevel(level)
{
}

void AbulEduLogWriterRunnableV1::run()
{
    AbulEduLoggerV1::instance().write(mMessage, mLevel);
}
#endif


AbulEduLoggerImplV1::AbulEduLoggerImplV1()
    : level(InfoLevel)
{
    /* On reserve 2 destinations (fichier + console) */
    destList.reserve(2);
#ifdef ABULEDU_LOG_SEPARATE_THREAD
    QThreadPool::globalInstance()->setMaxThreadCount(1);
    QThreadPool::globalInstance()->setExpiryTimeout(-1);
#endif
}

AbulEduLoggerV1 &AbulEduLoggingV1::AbulEduLoggerV1::instance()
{
    if (!INSTANCE)
        INSTANCE = new AbulEduLoggerV1;

    return *INSTANCE;
}

void AbulEduLoggerV1::destroyInstance()
{
    delete INSTANCE;
}

//// Essaye d'extraire le niveau de debug depuis un message de log (string)
//// Si ok, conversionSucceeded contient le resultat de la converion.
AbulEduLogLevelV1 AbulEduLoggingV1::AbulEduLoggerV1::levelFromLogMessage(const QString &logMessage, bool *conversionSucceeded)
{
    if (conversionSucceeded)
        *conversionSucceeded = true;

    if (logMessage.startsWith(QLatin1String(TraceString)))
        return TraceLevel;
    if (logMessage.startsWith(QLatin1String(DebugString)))
        return DebugLevel;
    if (logMessage.startsWith(QLatin1String(InfoString)))
        return InfoLevel;
    if (logMessage.startsWith(QLatin1String(WarnString)))
        return WarnLevel;
    if (logMessage.startsWith(QLatin1String(ErrorString)))
        return ErrorLevel;
    if (logMessage.startsWith(QLatin1String(FatalString)))
        return FatalLevel;

    if (conversionSucceeded)
        *conversionSucceeded = false;
    return OffLevel;

}

AbulEduLoggerV1::~AbulEduLoggerV1()
{
#ifdef ABULEDU_LOG_SEPARATE_THREAD
    QThreadPool::globalInstance()->waitForDone();
    QThreadPool::globalInstance()->deleteLater();
#endif
    delete d;
    d = 0;
}

void AbulEduLoggerV1::addDestination(DestinationPtrV1 destination)
{
    assert(destination.data());
    d->destList.push_back(destination);
}

void AbulEduLoggerV1::setLoggingLevel(const AbulEduLogLevelV1 &newLevel)
{
    d->level = newLevel;
}

AbulEduLogLevelV1 AbulEduLoggerV1::loggingLevel() const
{
    return d->level;
}

AbulEduLoggerV1::AbulEduLoggerV1() :
    d(new AbulEduLoggerImplV1)
{
}

////! directs the message to the task queue or writes it directly
void AbulEduLoggerV1::enqueueWrite(const QString &message, AbulEduLogLevelV1 level)
{
#ifdef ABULEDU_LOG_SEPARATE_THREAD
    AbulEduLogWriterRunnableV1 *r = new AbulEduLogWriterRunnableV1(message, level);
    QThreadPool::globalInstance()->start(r);
#else
    write(message, level);
#endif
}

////! Sends the message to all the destinations. The level for this message is passed in case
void AbulEduLoggerV1::write(const QString &message, AbulEduLogLevelV1 level)
{
    ///@todo: Provoque un plantage sur android en Qt 5.2 ... sera peut-être amélioré un jour ?
#ifndef Q_OS_ANDROID
    QMutexLocker lock(&d->logMutex);
    for (DestinationList::iterator it = d->destList.begin(),
         endIt = d->destList.end();it != endIt;++it) {
        (*it)->write(message, level);
    }
#endif
}

AbulEduLoggerV1::Helper::Helper(AbulEduLogLevelV1 logLevel) :
    level(logLevel),
    qtDebug(&buffer) {}

AbulEduLoggerV1::Helper::~Helper()
{
    try {
        writeToLog();
    }
    catch(std::exception&) {
        // you shouldn't throw exceptions from a sink
        assert(!"exception in logger helper destructor");
        throw;
    }
}

////! Creation du message de log complet et passage au logger
void AbulEduLoggerV1::Helper::writeToLog()
{
    const char* const levelName = LevelToText(level);
    const QString completeMessage(QString("%1 %2 %3")
                                  .arg(levelName)
                                  .arg(QDateTime::currentDateTime().toString(fmtDateTime))
                                  .arg(buffer)
                                  );

    AbulEduLoggerV1::instance().enqueueWrite(completeMessage, level);
}

} /* Namespace End */
