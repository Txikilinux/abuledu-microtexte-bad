/** Classe AbulEduControlAudioV1
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2014 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 3 of the License, or
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

#include "abuleducontrolaudiov1.h"
#include "ui_abuleducontrolaudiov1.h"
#include "abuledumultimediav1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduControlAudioV1::AbulEduControlAudioV1(QWidget *parent, enumControlType type) :
    ui(new Ui::AbulEduControlAudioV1),
    m_parent(parent),
    m_type(type),
    m_state(Stopped),
    m_direction(QBoxLayout::LeftToRight),
    m_isSpeedControlVisible(false)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setObjectName("AbulEduControlAudioV1");
    ui->setupUi(this);
    ui->btnAbulEduControlAudioHide->setEnabled(true);
    ui->btnAbulEduControlAudioRecord->setVisible(false);
#if defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    ui->btnAbulEduControlAudioToggleOutput->setVisible(false);
#endif
    setEnabled(false);
    ui->wgAbulEduControlAudioLevelMeter->setVisible(false);
    m_content = children();
    for(int i = 0;i < m_content.count();i++){
        if(m_content[i]->inherits("QLayout")){
            m_content.removeOne(m_content[i]);
        }
    }
    if(abeApp->getAbeApplicationMultiMediaSettings()->abeMultiMediaSettingsGetHeadPhone().isNull()){
        ui->btnAbulEduControlAudioToggleOutput->setEnabled(false);
    }

    abeControlAudioSetSpeedControlVisible(false);
}

AbulEduControlAudioV1::~AbulEduControlAudioV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduControlAudioV1::abeControlAudioSetType(AbulEduControlAudioV1::enumControlType type)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_type = type;
    abeControlAudioSetState(Stopped);
}

void AbulEduControlAudioV1::abeControlAudioSetState(enumControlAudioState state)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< state << " sender : " << sender();
    m_state = state;
    AbulEduMultiMediaV1* multimediaParent = (AbulEduMultiMediaV1*) m_parent;
    if(multimediaParent->abeMultiMediaGetTTSlang() == AbulEduPicottsV1::None){
        if(multimediaParent->abeMultiMediaGetCurrentMediaMedia().isEmpty()){
            ABULEDU_LOG_DEBUG() <<" Aucun contenu dans l'AbulEduMultiMediaV1";
            ui->btnAbulEduControlAudioPause->setEnabled(false);
            ui->btnAbulEduControlAudioNext->setEnabled(false);
            ui->btnAbulEduControlAudioPlay->setEnabled(false);
            ui->btnAbulEduControlAudioPrevious->setEnabled(false);
            ui->btnAbulEduControlAudioStop->setEnabled(false);
            ui->btnAbulEduControlAudioRecord->setEnabled(false);
            ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(false);
        }
        else {
            if(m_type == PlayerType)
            {
                ABULEDU_LOG_DEBUG() <<"  PlayerType : "<< "Player";
                switch (m_state)
                {
                case Playing:
                    ABULEDU_LOG_DEBUG() <<"  Playing";
                    ui->btnAbulEduControlAudioPause->setEnabled(true);
                    ui->btnAbulEduControlAudioStop->setEnabled(true);
                    ui->btnAbulEduControlAudioPlay->setEnabled(false);
                    ui->btnAbulEduControlAudioRecord->setEnabled(false);
                    ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(true);
                    break;
                case Stopped:
                    ABULEDU_LOG_DEBUG() <<"  Stopped";
                    ui->btnAbulEduControlAudioPause->setEnabled(false);
                    ui->btnAbulEduControlAudioStop->setEnabled(false);
                    if(!QFile(multimediaParent->abeMultiMediaGetCurrentMediaMedia().abeMediaMediaGetSound()).exists()){
                        ui->btnAbulEduControlAudioPlay->setEnabled(false);
                    }
                    else {
                        ABULEDU_LOG_DEBUG() <<" Je mets le bouton Play accessible car je peux lire le fichier "<<multimediaParent->abeMultiMediaGetCurrentMediaMedia().abeMediaMediaGetSound();
                        ui->btnAbulEduControlAudioPlay->setEnabled(true);
                    }
                    ui->btnAbulEduControlAudioNext->setEnabled(false);
                    ui->btnAbulEduControlAudioPrevious->setEnabled(false);
                    ui->btnAbulEduControlAudioRecord->setEnabled(true);
                    ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(false);
                    break;
                case Paused:
                    ABULEDU_LOG_DEBUG() <<"  Paused";
                    ui->btnAbulEduControlAudioPause->setEnabled(false);
                    ui->btnAbulEduControlAudioStop->setEnabled(true);
                    ui->btnAbulEduControlAudioPlay->setEnabled(true);
                    break;
                case Recording: // Oups !!! On ne devrait pas être là !!!
                    ABULEDU_LOG_DEBUG() <<"  Recording";
                    ui->btnAbulEduControlAudioPause->setEnabled(false);
                    ui->btnAbulEduControlAudioNext->setEnabled(false);
                    ui->btnAbulEduControlAudioPlay->setEnabled(false);
                    ui->btnAbulEduControlAudioPrevious->setEnabled(false);
                    ui->btnAbulEduControlAudioStop->setEnabled(true);
                    ui->btnAbulEduControlAudioRecord->setEnabled(false);
                    ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(true);
                default:
                    break;
                }
            }
            else
            {
                ABULEDU_LOG_DEBUG() <<"  PlayerType : "<< "Recorder";
                /* On est dans le cas du recorder */
                switch (m_state)
                {
                case Playing:
                    ABULEDU_LOG_DEBUG() <<"  Playing";
                    ui->btnAbulEduControlAudioPause->setEnabled(true);
                    ui->btnAbulEduControlAudioStop->setEnabled(true);
                    ui->btnAbulEduControlAudioPlay->setEnabled(false);
                    ui->btnAbulEduControlAudioRecord->setEnabled(false);
                    ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(true);
                    break;
                case Stopped:
                    ABULEDU_LOG_DEBUG() <<"  Stopped";
                    if(!QFile(multimediaParent->abeMultiMediaGetCurrentMediaMedia().abeMediaMediaGetSound()).exists()){
                        ui->btnAbulEduControlAudioPlay->setEnabled(false);
                    }
                    else {
                        ABULEDU_LOG_DEBUG() << " Je mets le bouton Play accessible car je peux lire le fichier "<<multimediaParent->abeMultiMediaGetCurrentMediaMedia().abeMediaMediaGetSound();
                        ui->btnAbulEduControlAudioPlay->setEnabled(true);
                    }
                    ui->btnAbulEduControlAudioPause->setEnabled(false);
                    ui->btnAbulEduControlAudioStop->setEnabled(false);
                    ui->btnAbulEduControlAudioNext->setEnabled(false);
                    ui->btnAbulEduControlAudioPrevious->setEnabled(false);
                    ui->btnAbulEduControlAudioRecord->setEnabled(true);
                    ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(false);
                    break;
                case Paused:
                    ABULEDU_LOG_DEBUG() <<"  Paused";
                    ui->btnAbulEduControlAudioPause->setEnabled(false);
                    ui->btnAbulEduControlAudioStop->setEnabled(true);
                    if(ui->btnAbulEduControlAudioRecord->isEnabled())
                    {
                        ui->btnAbulEduControlAudioPlay->setEnabled(false);
                        ui->btnAbulEduControlAudioRecord->setEnabled(true);
                    }
                    else
                    {
                        ui->btnAbulEduControlAudioPlay->setEnabled(true);
                        ui->btnAbulEduControlAudioRecord->setEnabled(false);
                    }
                    break;
                case Recording:
                    ABULEDU_LOG_DEBUG() <<"  Recording";
                    ui->btnAbulEduControlAudioPause->setEnabled(true);
                    ui->btnAbulEduControlAudioNext->setEnabled(false);
                    ui->btnAbulEduControlAudioPlay->setEnabled(false);
                    ui->btnAbulEduControlAudioPrevious->setEnabled(false);
                    ui->btnAbulEduControlAudioStop->setEnabled(true);
                    ui->btnAbulEduControlAudioRecord->setEnabled(false);
                    ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(true);
                    break;

                default:
                    break;
                }
            }
        }
    }
    else {
        ABULEDU_LOG_DEBUG() <<"  TTS";
        switch (m_state)
        {
        case Playing:
            ABULEDU_LOG_DEBUG() <<"  Playing";
            ui->btnAbulEduControlAudioPause->setEnabled(true);
            ui->btnAbulEduControlAudioStop->setEnabled(true);
            ui->btnAbulEduControlAudioPlay->setEnabled(false);
            ui->btnAbulEduControlAudioRecord->setEnabled(false);
            ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(true);
            break;
        case Stopped:
            ABULEDU_LOG_DEBUG() <<"  Stopped";
            ui->btnAbulEduControlAudioPause->setEnabled(false);
            ui->btnAbulEduControlAudioStop->setEnabled(false);
            ui->btnAbulEduControlAudioPlay->setEnabled(true);
            ui->btnAbulEduControlAudioNext->setEnabled(false);
            ui->btnAbulEduControlAudioPrevious->setEnabled(false);
            ui->btnAbulEduControlAudioRecord->setEnabled(true);
            ui->wgAbulEduControlAudioLevelMeter->slotAbulEduAudioLevelWidgetSetActivate(false);
            break;
        case Paused:
            ABULEDU_LOG_DEBUG() <<"  Paused";
            ui->btnAbulEduControlAudioPause->setEnabled(false);
            ui->btnAbulEduControlAudioStop->setEnabled(true);
            ui->btnAbulEduControlAudioPlay->setEnabled(true);
            break;
        case Recording:
            break;
        }
    }
}

void AbulEduControlAudioV1::abeControlAudioSetVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setVisible(visible);
}

void AbulEduControlAudioV1::abeControlAudioSetButtonVisible(flagControlButtons buttons, bool visible)
{

    if(buttons.testFlag(BtnPlay))
    {
       ui->btnAbulEduControlAudioPlay->setVisible(visible);
    }
    if(buttons.testFlag(BtnPause))
    {
       ui->btnAbulEduControlAudioPause->setVisible(visible);
    }
    if(buttons.testFlag(BtnStop))
    {
       ui->btnAbulEduControlAudioStop->setVisible(visible);
    }
    if(buttons.testFlag(BtnPrevious))
    {
       ui->btnAbulEduControlAudioPrevious->setVisible(visible);
    }
    if(buttons.testFlag(BtnNext))
    {
       ui->btnAbulEduControlAudioNext->setVisible(visible);
    }
    if(buttons.testFlag(BtnMagnifyingGlass))
    {
       ui->btnAbulEduControlAudioMagnifyingGlass->setVisible(visible);
    }
    if(buttons.testFlag(BtnHide))
    {
       ui->btnAbulEduControlAudioHide->setVisible(visible);
    }
    if(buttons.testFlag(BtnRecord))
    {
       ui->btnAbulEduControlAudioRecord->setVisible(visible);
    }
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    if(buttons.testFlag(BtnToggleOutput))
    {
       ui->btnAbulEduControlAudioToggleOutput->setVisible(visible);
    }
#endif
}

void AbulEduControlAudioV1::abeControlAudioSetSpeedControlVisible(bool visible)
{
    m_isSpeedControlVisible = visible;
    ui->frmSpeed->setVisible(visible);
}

void AbulEduControlAudioV1::abeControlAudioSetButtonEnabled(flagControlButtons buttons, bool enabled)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << buttons;

    AbulEduMultiMediaV1* multimediaParent = (AbulEduMultiMediaV1*) m_parent;
    if(buttons.testFlag(BtnPlay))
    {
        /* On interdit que les boutons Play et Record soient accessibles s'il n'y a pas de chemin pour le son */
        if(multimediaParent->abeMultiMediaGetCurrentMediaMedia().abeMediaMediaGetSound().isEmpty())
        {
            ui->btnAbulEduControlAudioPlay->setEnabled(false);
        }
        else {
            ui->btnAbulEduControlAudioPlay->setEnabled(enabled);
        }
    }
    if(buttons.testFlag(BtnPause))
    {
       ui->btnAbulEduControlAudioPause->setEnabled(enabled);
    }
    if(buttons.testFlag(BtnStop))
    {
       ui->btnAbulEduControlAudioStop->setEnabled(enabled);
    }
    if(buttons.testFlag(BtnPrevious))
    {
       ui->btnAbulEduControlAudioPrevious->setEnabled(enabled);
    }
    if(buttons.testFlag(BtnNext))
    {
       ui->btnAbulEduControlAudioNext->setEnabled(enabled);
    }
    if(buttons.testFlag(BtnMagnifyingGlass))
    {
       ui->btnAbulEduControlAudioMagnifyingGlass->setEnabled(enabled);
    }
    if(buttons.testFlag(BtnHide))
    {
        ui->btnAbulEduControlAudioHide->setEnabled(enabled);
    }
    if(buttons.testFlag(BtnRecord))
    {
        /* On interdit que les boutons Play et Record soient accessibles s'il n'y a pas de chemin pour le son */
        if(multimediaParent->abeMultiMediaGetCurrentMediaMedia().abeMediaMediaGetSound().isEmpty())
        {
            ui->btnAbulEduControlAudioRecord->setEnabled(false);
        }
        else {
            ui->btnAbulEduControlAudioRecord->setEnabled(enabled);
        }
    }
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    if(buttons.testFlag(BtnToggleOutput))
    {
        if(abeApp->getAbeApplicationMultiMediaSettings()->abeMultiMediaSettingsGetHeadPhone().isNull()){
            ui->btnAbulEduControlAudioToggleOutput->setEnabled(false);
        }
        else {
            ui->btnAbulEduControlAudioToggleOutput->setEnabled(enabled);
        }
    }
#endif
}
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
void AbulEduControlAudioV1::abeControlAudioSetButtonToggleOutputDeviceChecked(bool checked)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->btnAbulEduControlAudioToggleOutput->setChecked(checked);
    /* Je mets à jour le bouton */
    if(checked) {
        /* On veut passer sur le casque */
        ui->btnAbulEduControlAudioToggleOutput->setIcones(":/controlaudio/casque", ":/controlaudio/casque_hover", ":/controlaudio/casque_hover", ":/controlaudio/casque_gris");
    }
    else {
        /* On veut passer sur les haut-parleurs */
        ui->btnAbulEduControlAudioToggleOutput->setIcones(":/controlaudio/hp", ":/controlaudio/hp_hover", ":/controlaudio/hp_hover", ":/controlaudio/hp_gris");
    }
}
#endif

AbulEduAudioLevelWidgetV1 *AbulEduControlAudioV1::abeControlAudioGetAudioLevelMeter()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return ui->wgAbulEduControlAudioLevelMeter;
}

void AbulEduControlAudioV1::setEnabled(bool enabled)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeControlAudioSetButtonEnabled(BtnPlay | BtnPause | BtnStop | BtnNext | BtnPrevious | BtnRecord | BtnMagnifyingGlass, enabled );
    if(enabled)
    {
        abeControlAudioSetState(m_state);
    }
}

bool AbulEduControlAudioV1::abeControlAudioSetDirection(QBoxLayout::Direction direction)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour;
    if(m_direction == direction){
        retour = false;
    }
    else {
        QBoxLayout* generalLayout = (QBoxLayout*) layout();
        QBoxLayout* speedLayout = (QBoxLayout*) ui->frmSpeedLayout;
        speedLayout->setDirection(direction);
        generalLayout->setDirection(direction);
        m_direction = direction;
        retour = true;
    }
    if(m_parent){
        m_parent->adjustSize();
    }
    return retour;
}

void AbulEduControlAudioV1::abeControlAudioSetSpeedValue(int speed)
{
    ui->lblAbulEduControlAudioDisplay->setText(QString::number(speed));
}

QFrame *AbulEduControlAudioV1::abeControlAudioGetFrameSpeed()
{
    return ui->frmSpeed;
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioPrevious_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioPreviousClicked();
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioPause_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioPauseClicked();
//    abeControlAudioSetState(Paused);
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioPlay_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioPlayClicked();
//    abeControlAudioSetState(Playing);
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioStop_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioStopClicked();
//    abeControlAudioSetState(Stopped);
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioNext_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioNextClicked();
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioHide_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioBtnHideClicked();
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioRecord_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioRecordClicked();
//    abeControlAudioSetState(Recording);
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioMagnifyingGlass_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeControlAudioMagnifyingGlassClicked();
}

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
void AbulEduControlAudioV1::on_btnAbulEduControlAudioToggleOutput_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ABULEDU_LOG_DEBUG() <<"\n     émission signalAbeControlAudioToggleOutput "<<checked;

    if(checked) {
        // On veut passer sur le casque
        ui->btnAbulEduControlAudioToggleOutput->setIcones(":/controlaudio/casque", ":/controlaudio/casque_hover", ":/controlaudio/casque_hover", ":/controlaudio/casque_gris");
    }
    else {
        // On veut passer sur les haut-parleurs
        ui->btnAbulEduControlAudioToggleOutput->setIcones(":/controlaudio/hp", ":/controlaudio/hp_hover", ":/controlaudio/hp_hover", ":/controlaudio/hp_gris");
    }
    emit signalAbeControlAudioToggleOutput(checked);
}
#endif

void AbulEduControlAudioV1::on_btnAbulEduControlAudioIncreaseSpeed_clicked()
{
    int newSpeed = ui->lblAbulEduControlAudioDisplay->text().toInt()+10;
    ui->lblAbulEduControlAudioDisplay->setText(QString::number(newSpeed));
    emit signalAbeControlAudioSpeedChanged(newSpeed);
}

void AbulEduControlAudioV1::on_btnAbulEduControlAudioDecreaseSpeed_clicked()
{
    int newSpeed = ui->lblAbulEduControlAudioDisplay->text().toInt()-10;
    ui->lblAbulEduControlAudioDisplay->setText(QString::number(newSpeed));
    emit signalAbeControlAudioSpeedChanged(newSpeed);
}
