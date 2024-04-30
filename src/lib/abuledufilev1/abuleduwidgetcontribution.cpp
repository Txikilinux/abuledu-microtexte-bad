#include "abuleduwidgetcontribution.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduWidgetContribution::AbulEduWidgetContribution(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AbulEduWidgetContribution)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    m_numberEntities = 0;
    setObjectName("WidgetContrib1");
}

AbulEduWidgetContribution::~AbulEduWidgetContribution()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    m_abeScoLomFrV1.clear();
}

void AbulEduWidgetContribution::abeLOMsetABESCOLOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << objectName()+"::abeLOMsetABESCOLOMFrV1";
    m_abeScoLomFrV1 = scoLomFrV1;
    ui->cbRole->addItems(m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoleVocabLOMFR().values());
    ui->cbRole->addItems(m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoleVocabLOM().values());
    ui->cbRole->blockSignals(true);
    ui->cbRole->setCurrentIndex(0);
    ui->cbRole->blockSignals(false);
    ui->deDate->setDate(QDate::currentDate());
    if (m_numberEntities == 0)
        addCBEntity();
}

void AbulEduWidgetContribution::saveContribInformations()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << m_addedComboBox;
    m_abeScoLomFrV1->abeLOMaddLifeCycleContributionRole(ui->cbRole->currentText(),
                                                        ui->cbEntite->abeGetVCard(ui->cbEntite->currentText()),
                                                        ui->deDate->date());
    for (int i=0; i < m_addedComboBox.count() ; i++)
    {
        AbulEduVCardComboBox* combo = m_addedComboBox[i].second;
        m_abeScoLomFrV1->abeLOMaddLifeCycleContributionRole(ui->cbRole->currentText(),
                                                            combo->abeGetVCard(combo->currentText()),
                                                            ui->deDate->date());
    }
}

void AbulEduWidgetContribution::addCBEntity(vCard entity)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << entity.getFullName();
    m_numberEntities++;
    int numLigne = 0;
    int numColonne = 0;
    if (m_numberEntities == 2)
    {
        numLigne = 1;
        numColonne = 2;
    }
    else
    {
        numLigne = m_numberEntities / 2 + m_numberEntities % 2;
        numColonne = 1 + (m_numberEntities+1) % 2;
    }

    QStringList contributors;//euh ?!
    int currentEntityIndex = 0;
    for (int i=0; i < m_abeScoLomFrV1->abeLOMgetAllVcards().size(); i++)
    {
        contributors << m_abeScoLomFrV1->abeLOMgetAllVcards()[i].getFullName();
        if (m_abeScoLomFrV1->abeLOMgetAllVcards()[i].getFullName() == entity.getFullName())
        {
            currentEntityIndex = i;
        }
    }

    if (entity.getFullName().isEmpty())
    {
        if (m_numberEntities == 1)
        {
            ABULEDU_LOG_DEBUG() << "AbulEduWidgetContribution::addCBEntity cas 1 : Null et 1";
        }
        else
        {
            AbulEduVCardComboBox* cbEntity = new AbulEduVCardComboBox();
            m_addedComboBox << QPair<QString,AbulEduVCardComboBox*>("QComboBoxEntity"+QString::number(m_numberEntities),cbEntity);
            cbEntity->addItems(contributors);
            cbEntity->setAttribute(Qt::WA_DeleteOnClose);
            cbEntity->blockSignals(true);
            cbEntity->setCurrentIndex(currentEntityIndex);
            cbEntity->blockSignals(false);
            cbEntity->setSizeAdjustPolicy(QComboBox::AdjustToContents);
            QHBoxLayout* lay = new QHBoxLayout();
            lay->setAlignment(Qt::AlignLeft);

            if (numColonne != 1)
            {
                QLabel* deCadix = new QLabel("");
                deCadix->setFixedWidth(15);
                lay->addWidget(deCadix);
                deCadix->show();
            }
            lay->addWidget(cbEntity);

            ui->glContribution->addLayout(lay,numLigne,numColonne);
            cbEntity->show();
            ABULEDU_LOG_DEBUG()<<"AbulEduWidgetContribution::addCBEntity cas 2 : Null et PAS 1";
        }
    }
    else
    {
        if (m_numberEntities == 1)
        {
            ui->cbEntite->clear();
            ui->cbEntite->addItems(contributors);
            ui->cbEntite->blockSignals(true);
            ui->cbEntite->setCurrentIndex(currentEntityIndex);
            ui->cbEntite->blockSignals(false);
            ABULEDU_LOG_DEBUG()<<"AbulEduWidgetContribution::addCBEntity cas 3 : PAS Null et 1";
        }
        else
        {
            AbulEduVCardComboBox* cbEntity = new AbulEduVCardComboBox();
            m_addedComboBox << QPair<QString,AbulEduVCardComboBox*>("QComboBoxEntity"+QString::number(m_numberEntities),cbEntity);
            cbEntity->addItems(contributors);
            cbEntity->setAttribute(Qt::WA_DeleteOnClose);
            cbEntity->blockSignals(true);
            cbEntity->setCurrentIndex(currentEntityIndex);
            cbEntity->blockSignals(false);
            cbEntity->setSizeAdjustPolicy(QComboBox::AdjustToContents);
            QHBoxLayout* lay = new QHBoxLayout();
            lay->setAlignment(Qt::AlignLeft);

            if (numColonne != 1)
            {
                QLabel* deCadix = new QLabel("");
                deCadix->setFixedWidth(15);
                lay->addWidget(deCadix);
                deCadix->show();
            }
            lay->addWidget(cbEntity);

            ui->glContribution->addLayout(lay,numLigne,numColonne);
            cbEntity->show();
            ABULEDU_LOG_DEBUG() <<"AbulEduWidgetContribution::addCBEntity cas 4 : PAS Null et PAS 1";
        }
    }
}

void AbulEduWidgetContribution::setRole(QString role)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << role;
    if (m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoleVocabLOMFR().contains(role))
    {
        role = m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoleVocabLOMFR().value(role);
    }
    else
    {
        if (m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoleVocabLOM().contains(role))
        {
            role = m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoleVocabLOM().value(role);
        }
    }

    int i = ui->cbRole->count();
    while (ui->cbRole->currentText() != role && i>0)
    {
        i--;
        ui->cbRole->blockSignals(true);
        ui->cbRole->setCurrentIndex(i);
        ui->cbRole->blockSignals(false);
    }
}

void AbulEduWidgetContribution::setDate(QDate date)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << date;
    ui->deDate->setDate(date);
}

void AbulEduWidgetContribution::resetEntitiesNumber()
{
    m_numberEntities = 0;
}

void AbulEduWidgetContribution::on_btnPlusEntite_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBEntity();
}
