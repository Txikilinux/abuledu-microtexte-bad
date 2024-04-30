#include "abuledumultimediawidgetcontainer.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMultiMediaWidgetContainer::AbulEduMultiMediaWidgetContainer(QWidget *parent) :
    QWidget(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

AbulEduMultiMediaWidgetContainer::~AbulEduMultiMediaWidgetContainer()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduMultiMediaV1* abeMedia = findChild<AbulEduMultiMediaV1*>();
    if(abeMedia){
        ABULEDU_LOG_DEBUG() << " déparentement de l'AbulEduMultiMediaV1";
        abeMedia->setParent(0);
    }
}
