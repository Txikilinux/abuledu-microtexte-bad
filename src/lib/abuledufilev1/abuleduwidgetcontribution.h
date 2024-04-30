#ifndef ABULEDUWIDGETCONTRIBUTION_H
#define ABULEDUWIDGETCONTRIBUTION_H

#include <QFrame>
#include "abuleduvcardcombobox.h"
#include "abuleduscolomfrv1.h"
#include "ui_abuleduwidgetcontribution.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduWidgetContribution;
}

class AbulEduWidgetContribution : public QFrame
{
    Q_OBJECT

public:
    explicit AbulEduWidgetContribution(QWidget *parent = 0);
    ~AbulEduWidgetContribution();

    /** Passe l'AbulEduSCOLOMFrV1 du widget dans lequel cet objet est instancié */
    void abeLOMsetABESCOLOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1);

    /** Enregistre les informations contenues dans ce widget dans le XML */
    void saveContribInformations();

    /** Ajoute une AbulEduVCardComboBox à la contribution
      * @param entity est la vCard éventuellement passée à la fonction
      * @note la fonction est appelée sans paramètre dans le cas d'une création ou d'un ajout, avec paramètre pour un affichage d'informations existantes
      */
    void addCBEntity(vCard entity = vCard());

    /** Met la QComboBox rôle à la valeur role */
    void setRole(QString role);

    /** Met la QDate date à la valeur date */
    void setDate(QDate date);

    /** Remet l'attribut m_numberEntities à zéro
      * @note utilisé avant de charger des entités depuis la structure de données ou (bientôt) directement le XML */
    void resetEntitiesNumber();

private slots:
    void on_btnPlusEntite_clicked();

private:
    Ui::AbulEduWidgetContribution *ui;

    /** Copie du pointeur du Widget depuis lequel cet objet est appelé vers l'attribut de type AbulEduSCOLOMFrV1 de son AbulEduFileV1 */
    QSharedPointer<AbulEduSCOLOMFrV1> m_abeScoLomFrV1;

    /** Nombre d'entités correspondant à cette contribution (rappel : 1 rôle et 1 date par contribution) */
    int m_numberEntities;

    /** Liste des AbulEduVCardComboBox créés à la volée pour stocker les entités quand il y en plus d'une */
    QList<QPair<QString,AbulEduVCardComboBox*> > m_addedComboBox;

};

#endif // ABULEDUWIDGETCONTRIBUTION_H
