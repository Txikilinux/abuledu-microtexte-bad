#include "abuleduwidgettaxon.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduWidgetTaxon::AbulEduWidgetTaxon(QWidget *parent,int rank) :
    QGroupBox(parent),
    ui(new Ui::AbulEduWidgetTaxon)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
//    m_addedWidgetsTaxon.clear();
    m_numberTaxon = 1;
    m_rank = rank;
    if (rank > 1)
        setTitle(title().append(" - "+QString::number(m_rank)));
    setObjectName("WidgetTaxon"+QString::number(rank));
}

AbulEduWidgetTaxon::~AbulEduWidgetTaxon()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduWidgetTaxon::setLEsourceText(QString text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->leSource->setText(text);
}

void AbulEduWidgetTaxon::setLEidText(QString text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->leId1->setText(text);
}

void AbulEduWidgetTaxon::setLEentryText(QString text)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->leEntry1->setText(text);
}

QString AbulEduWidgetTaxon::getLEsourceText()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return ui->leSource->text();
}

QString AbulEduWidgetTaxon::getLEidText()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return ui->leId1->text();
}

QString AbulEduWidgetTaxon::getLEentryText()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return ui->leEntry1->text();
}

QList<QPair<QString, QWidget *> > AbulEduWidgetTaxon::getInfoTaxon()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QList<QPair<QString, QWidget *> > listeInfo;
    listeInfo.append(QPair<QString, QWidget *>(ui->leSource->objectName(),ui->leSource));
    listeInfo.append(QPair<QString, QWidget *>(ui->leId1->objectName(),ui->leId1));
    listeInfo.append(QPair<QString, QWidget *>(ui->leEntry1->objectName(),ui->leEntry1));
    listeInfo.append(m_addedWidgetsTaxon);
    return listeInfo;
}

void AbulEduWidgetTaxon::on_btnPlusLOM9_2_2_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_numberTaxon++;
//    ui->leLOM9_2_1->setText("bouton dessous cliqué");
    QGroupBox* gbTaxon = new QGroupBox("9.2.2 Taxon - "+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
    QGridLayout* glTaxon = new QGridLayout();
    gbTaxon->setLayout(glTaxon);
    QLabel* lblId = new QLabel(trUtf8("9.2.2.1 ID - ")+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
            lblId->setObjectName("lblId"+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
            glTaxon->addWidget(lblId,0,0);
//            m_addedWidgetsTaxon.append(QPair<QString,QWidget*>(lblId->objectName(),lblId));
            lblId->show();
    QLineEdit* leId = new QLineEdit();
            leId->setObjectName("leId"+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
            glTaxon->addWidget(leId,0,1);
            m_addedWidgetsTaxon.append(QPair<QString,QWidget*>(leId->objectName(),leId));
            leId->show();
    QLabel* lblEntry = new QLabel(trUtf8("9.2.2.2 Entrée - ")+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
            lblEntry->setObjectName("lblEntry"+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
            glTaxon->addWidget(lblEntry,1,0);
//            m_addedWidgetsTaxon.append(QPair<QString,QWidget*>(lblEntry->objectName(),lblEntry));
            lblEntry->show();
    QLineEdit* leEntry = new QLineEdit();
            leEntry->setObjectName("leEntry"+QString::number(m_rank)+" - "+QString::number(m_numberTaxon));
            glTaxon->addWidget(leEntry,1,1);
            m_addedWidgetsTaxon.append(QPair<QString,QWidget*>(leEntry->objectName(),leEntry));
            leEntry->show();
    ui->vlThis->removeWidget(ui->btnPlusLOM9_2_2);
    ui->vlThis->addWidget(gbTaxon);
    ui->vlThis->addWidget(ui->btnPlusLOM9_2_2);
}
