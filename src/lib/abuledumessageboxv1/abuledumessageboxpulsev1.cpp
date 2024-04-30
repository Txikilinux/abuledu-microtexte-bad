#include "abuledumessageboxpulsev1.h"

#ifndef DEBUG_ABULEDUMESSAGEBOXV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMessageBoxPulseV1::AbulEduMessageBoxPulseV1(const QString &titre, const QString &texte, bool doYouWantToClose, int autoCloseTimeout, QWidget *parent) :
    AbulEduMessageBoxV1(titre, texte, doYouWantToClose, parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_autoCloseTimer = new QTimer(this);
    if(autoCloseTimeout > 0) {
        connect(m_autoCloseTimer, SIGNAL(timeout()), this, SLOT(close()));
        m_autoCloseTimer->start(autoCloseTimeout);
    }
}

AbulEduMessageBoxPulseV1::~AbulEduMessageBoxPulseV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_autoCloseTimer->deleteLater();
}
