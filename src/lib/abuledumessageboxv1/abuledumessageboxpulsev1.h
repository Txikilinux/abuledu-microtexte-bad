#ifndef ABULEDUMESSAGEBOXPULSEV1_H
#define ABULEDUMESSAGEBOXPULSEV1_H

#include <QTimer>
#include "abuledumessageboxv1.h"
#include "abuleduloggerv1.h"

class AbulEduMessageBoxPulseV1 : public AbulEduMessageBoxV1
{
    Q_OBJECT
public:
    /* le paramètre autoCloseTimeout permet de définir (en milisecondes) la durée au bout de laquelle la popup se ferme toute seule (pour un affichage d'information façon 'notification gnome' par exemple) */
    explicit AbulEduMessageBoxPulseV1(const QString &titre = trUtf8("Problème"),
                                      const QString &texte = trUtf8("Impossible de continuer"),
                                      bool doYouWantToClose = true,
                                      int autoCloseTimeout = 0,
                                      QWidget *parent = 0);
    ~AbulEduMessageBoxPulseV1();

private:
    QTimer *m_autoCloseTimer;
};

#endif // ABULEDUMESSAGEBOXPULSEV1_H
