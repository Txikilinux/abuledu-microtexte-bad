/** Classe AbulEduMultiMediaDeviceChooserV1
  *
  * @author 2015 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
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

#include "abuledumultimediadevicechooserv1.h"
#include "ui_abuledumultimediadevicechooserv1.h"

AbulEduMultiMediaDeviceChooserV1::AbulEduMultiMediaDeviceChooserV1(QWidget *parent, enumDeviceType type, bool isAlwaysVisible) :
    QGroupBox(parent),
    ui(new Ui::AbulEduMultiMediaDeviceChooserV1)
{
    ui->setupUi(this);
    m_isAlwaysVisible = isAlwaysVisible;
    m_multimedia = 0;
    abeDeviceChooserSetDeviceType(type);
}

/** Attention : ça marche pour les input mais (visiblement) ça ne récupère pas les output si c'est le paramètre ->
 *  Le besoin était pour le moment sur les input donc, ça part comme ça !! */
void AbulEduMultiMediaDeviceChooserV1::abeDeviceChooserSetDeviceType(AbulEduMultiMediaDeviceChooserV1::enumDeviceType type)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<type;
    m_deviceType = type;
    QString msgLabel = trUtf8("Attention : aucun périphérique %1 n'a été automatiquement sélectionné pour le son.");
    QString deviceType;
    QAudioDeviceInfo defaultDevice;
    QList<QAudioDeviceInfo> detectedDevices;
    if(type == abeDeviceInput){
        setTitle(trUtf8("Choix du périphérique d'entrée"));
        detectedDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
        defaultDevice = QAudioDeviceInfo::defaultInputDevice();
        deviceType = trUtf8("d'entrée");
//        qDebug()<<"Entrée";
//        foreach(QAudioDeviceInfo d,detectedDevices){
//            qDebug()<<d.deviceName();
//        }
    }
    else {
        setTitle(trUtf8("Choix du périphérique de sortie"));
        detectedDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        defaultDevice = QAudioDeviceInfo::defaultOutputDevice();
        deviceType = trUtf8("de sortie");
//        qDebug()<<"Sortie";
//        foreach(QAudioDeviceInfo d,detectedDevices){
//            qDebug()<<d.deviceName();
//        }
    }
    foreach(QRadioButton* rb,m_buttons){
        delete rb;
    }

    if(detectedDevices.size() == 0){
        ui->abeDeviceChooserLblChoose->setVisible(false);
        ui->abeDeviceChooserLblWarning->setText(trUtf8("Aucun périphérique %1 n'a été détecté pour le son...").arg(deviceType));
    }
    else {
        ui->abeDeviceChooserLblWarning->setText(msgLabel.arg(deviceType));
    }
    foreach(QAudioDeviceInfo di,detectedDevices){
        QRadioButton* r = new QRadioButton(di.deviceName(),this);
        m_buttons << r;
        layout()->addWidget(r);
        connect(r, SIGNAL(toggled(bool)),this, SLOT(slotAbeDeviceChooserRbChecked(bool)),Qt::UniqueConnection);
        if(di == defaultDevice){
            ui->abeDeviceChooserLblWarning->setText(trUtf8("Le périphérique %1 par défaut été sélectionné pour le son.").arg(deviceType));
            r->setChecked(true);
            if(m_isAlwaysVisible){
                ui->abeDeviceChooserLblChoose->setVisible(false);
                ui->abeDeviceChooserLblWarning->setVisible(false);
            }
            else{
                setVisible(false);
            }
            ABULEDU_LOG_DEBUG()<<"Périphérique d'entrée son trouvé : "<<di.deviceName();
        }
    }
}

void AbulEduMultiMediaDeviceChooserV1::abeDeviceChooserSetAlwaysVisible(bool isAlwaysVisible)
{
     m_isAlwaysVisible = isAlwaysVisible;
     setVisible(true);
}

AbulEduMultiMediaDeviceChooserV1::~AbulEduMultiMediaDeviceChooserV1()
{
    delete ui;
}

void AbulEduMultiMediaDeviceChooserV1::abeDeviceChooserSetMultimedia(AbulEduMultiMediaV1 *multimedia)
{
    m_multimedia = multimedia;
}


void AbulEduMultiMediaDeviceChooserV1::slotAbeDeviceChooserRbChecked(bool isChecked)
{
    QRadioButton* radioButton = (QRadioButton*)sender();
    if(isChecked){
        QList<QAudioDeviceInfo> devicesList;
        if(m_deviceType == abeDeviceOutput){
            devicesList = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        }
        else {
            devicesList = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
        }
        bool found = false;
        int i = 0;
        while(i < devicesList.size() && !found){
            if(devicesList.at(i).deviceName() == radioButton->text()){
                if(m_deviceType == abeDeviceOutput){
                    abeApp->getAbeApplicationMultiMediaSettings()->abeMultiMediaSettingsSetAudioOutputDeviceInfo(devicesList.at(i));
                    if(m_multimedia){
                        m_multimedia->abeMultiMediaGetViewerWidget()->abeMediaViewerSetOutputDevice(devicesList.at(i));
                    }
                }
                else{
                    abeApp->getAbeApplicationMultiMediaSettings()->abeMultiMediaSettingsSetAudioInputDeviceInfo(devicesList.at(i));
                    if(m_multimedia){
                        m_multimedia->abeMultiMediaGetViewerWidget()->abeMediaViewerSetInputDevice(devicesList.at(i));
                    }
                }
                found = true;
            }
            i++;
        }
    }
    emit signalAbeDeviceChooseRbChecked(isChecked,radioButton->text());
    if(!m_isAlwaysVisible){
        setVisible(false);
    }
}
