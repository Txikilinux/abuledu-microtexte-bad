#ifndef ABULEDUWIDGETTAXON_H
#define ABULEDUWIDGETTAXON_H

#include <QGroupBox>
#include "ui_abuleduwidgettaxon.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduWidgetTaxon;
}

class AbulEduWidgetTaxon : public QGroupBox
{
    Q_OBJECT
    
public:
    explicit AbulEduWidgetTaxon(QWidget *parent = 0,int rank = 1);
    ~AbulEduWidgetTaxon();
    void setLEsourceText(QString text);
    void setLEidText(QString text);
    void setLEentryText(QString text);
    QString getLEsourceText();
    QString getLEidText();
    QString getLEentryText();
    QList<QPair<QString,QWidget*> > getInfoTaxon();

private slots:
    void on_btnPlusLOM9_2_2_clicked();

private:
    Ui::AbulEduWidgetTaxon *ui;
    int m_numberTaxon;
    int m_rank;
    QList<QPair<QString,QWidget*> > m_addedWidgetsTaxon;
};

#endif // ABULEDUWIDGETTAXON_H
