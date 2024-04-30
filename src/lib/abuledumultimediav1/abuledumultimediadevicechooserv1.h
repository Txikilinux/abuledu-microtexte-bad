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

#ifndef ABULEDUMULTIMEDIADEVICECHOOSERV1_H
#define ABULEDUMULTIMEDIADEVICECHOOSERV1_H

#include <QGroupBox>
#include <QRadioButton>
#include <QAudioDeviceInfo>
#include "abuleduloggerv1.h"
#include "abuledumultimediav1.h"

namespace Ui {
class AbulEduMultiMediaDeviceChooserV1;
}

class AbulEduMultiMediaDeviceChooserV1 : public QGroupBox
{
    Q_OBJECT
    Q_ENUMS(enumDeviceType)


public:
    enum enumDeviceType{abeDeviceInput,abeDeviceOutput};
    ///
    /// \brief Permet de choisir un périphérique d'entrée ou de sortie
    /// \param parent le parent de l'objet. Le passage d'un parent permet de n'avoir pas à détruire l'objet, il est détruit avec son parent.
    /// \param type l'indication d'une valeur de l'enum permet de choisir si on traite les périphériques d'entrée (choix par défaut) ou de sortie.
    /// \param isAlwaysVisible permet d'indiquer si on veut que cet objet soit visible de toutes les façons ou seulement si le périphérique par défaut
    ///        du système n'a pas été trouvé
    ///
    explicit AbulEduMultiMediaDeviceChooserV1(QWidget *parent = 0, enumDeviceType type = abeDeviceInput, bool isAlwaysVisible = false);
    inline enumDeviceType abeDeviceChooserGetDeviceType() {return m_deviceType;}
    void abeDeviceChooserSetDeviceType(enumDeviceType type);
    void abeDeviceChooserSetAlwaysVisible(bool isAlwaysVisible);
    void abeDeviceChooserSetMultimedia(AbulEduMultiMediaV1 *abeDeviceChooserGetMultimedia);

    ~AbulEduMultiMediaDeviceChooserV1();


private:
    Ui::AbulEduMultiMediaDeviceChooserV1 *ui;
    enumDeviceType m_deviceType;
    bool m_isAlwaysVisible;
    QList<QRadioButton*> m_buttons;
    AbulEduMultiMediaV1* m_multimedia;

private slots:
    void slotAbeDeviceChooserRbChecked(bool isChecked);

signals:
    void signalAbeDeviceChooseRbChecked(bool isChecked,QString buttonText);
};

#endif // ABULEDUMULTIMEDIADEVICECHOOSERV1_H
