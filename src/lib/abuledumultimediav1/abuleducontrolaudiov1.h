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

#ifndef ABULEDUCONTROLAUDIOV1_H
#define ABULEDUCONTROLAUDIOV1_H

//#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QBoxLayout>
#include "abuleduaudiolevelwidgetv1.h"
#include "abuleduloggerv1.h"
#include "abuleduflatboutonv1.h"

namespace Ui {
class AbulEduControlAudioV1;
}
///
/// \brief La classe AbulEduControlAudioV1 implémente un widget de télécommande interagissant avec AbulEduMultiMediaV1
///
///  \li Cette classe propose les boutons Play, Stop, Pause, Suivant, Précédent, Enregistrer. Elle embarque aussi un vu-mètre.
///  \li Cette classe communique essentiellement avec des signaux.
///
class AbulEduControlAudioV1 : public QWidget
{
    Q_OBJECT
    Q_ENUMS(enumControlType)
    Q_ENUMS(enumControlAudioState)
    Q_ENUMS(enumControlButton)
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    Q_ENUMS(enumControlOutput)
#endif
    Q_FLAGS(flagControlButtons)
public:
    /// Énumère les types du widget
    /// \li TypePlayer le widget active les boutons correspondants à cet état : pas de bouton record
    /// \li TypeRecorder le widget active les boutons correspondants à cet état : bouton record visible
    enum enumControlType{PlayerType, RecorderType};

    /// Énumère les états du widget
    /// \li Playing le widget active les boutons correspondants à cet état : pause et stop activés, play désactivé
    /// \li Stopped le widget active les boutons correspondants à cet état : play activé, pause et stop désactivés
    /// \li Paused  le widget active les boutons correspondants à cet état : play et stop activés
    /// \li Recording le widget active les boutons correspondants à cet état : Stop activé, les autres désactivés
    enum enumControlAudioState{Playing, Stopped, Paused, Recording};

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    /// Énumère les sorties audio
    /// \li HeadPhones le casque
    /// \li LoudSpeakers les haut-parleurs
    enum enumControlOutput {HeadPhones, LoudSpeakers};
#endif

    /// Énumère les boutons de la télécommande
    enum enumControlButton{BtnPlay = 1,             BtnPause = 2,
                           BtnStop = 4,             BtnRecord = 8,
                           BtnPrevious = 16,        BtnNext = 32,
                           BtnMagnifyingGlass = 64, BtnHide = 128,
                           BtnToggleOutput = 256 };
    Q_DECLARE_FLAGS(flagControlButtons, enumControlButton)

    explicit AbulEduControlAudioV1(QWidget *parent, enumControlType type = PlayerType);
    ~AbulEduControlAudioV1();

    /// Passe le widget dans le type demandé
    /// @param type le type demandé
    void abeControlAudioSetType(enumControlType type);

    /// Passe le widget dans l'état demandé
    /// @param state l'état demandé
    void abeControlAudioSetState(enumControlAudioState state);

    /// Renvoi l'état dans lequel est le widget
    /// @param state l'état demandé
    inline enumControlAudioState abeControlAudioGetState() {return m_state;}

    /// Affiche ou masque le widget
    /// @param visible true: le widget est visible, false : le widget est masqué
    void abeControlAudioSetVisible(bool visible);

    /// Affiche/masque le ou les boutons passés en paramètre
    void abeControlAudioSetButtonVisible(flagControlButtons buttons, bool visible);

    void abeControlAudioSetSpeedControlVisible(bool visible);
    inline bool abeControlAudioGetSpeedControlVisible(){return m_isSpeedControlVisible;}

    /// Active/désactive le ou les boutons passés en paramètre
    void abeControlAudioSetButtonEnabled(AbulEduControlAudioV1::flagControlButtons buttons, bool enabled);

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    /// Marque le bouton toggleOutput checked ou non
    /// @param checked true : sortie casque activée, false : sortie Hp activée
    void abeControlAudioSetButtonToggleOutputDeviceChecked(bool checked);
#endif

    /// Retourne un pointeur vers le vu-mètre
    /// @return Pointeur vers le vu-mètre
    AbulEduAudioLevelWidgetV1 *abeControlAudioGetAudioLevelMeter();

    /// Active/désactive le widget de manière globale (ré-implémenté)
    /// @param enabled Le widget et ces "enfants" sont activés ou désactivés
    void setEnabled(bool enabled);

    ///
    /// \brief abeControlAudioSetDirection permet de choisir d'avoir le control audio horizontal comme il est créé par défaut ou vertical
    /// \param direction est une enum de QBoxLayout; les deux seuls valeurs traitées sont TopToBottom et LeftToRight
    /// \return true si les conditions sont remplies pour que ça ait pu fonctionner : le param est une des deux valeurs prises en compte et n'est pas la direction courante
    ///
    bool abeControlAudioSetDirection(QBoxLayout::Direction direction);

    ///
    /// \brief Met à jour l'affichage de la vitesse
    /// \param speed
    ///
    void abeControlAudioSetSpeedValue(int speed);

    ///
    /// \brief Getteur du QLabel où est écrit le mot "Vitesse". Permet de modifier en particulier son styleSheet
    /// \return un pointeur vers le QLabel
    ///
    QFrame *abeControlAudioGetFrameSpeed();

private:
    Ui::AbulEduControlAudioV1 *ui;
    QWidget* m_parent;
    enumControlType          m_type;
    enumControlAudioState    m_state;
    QBoxLayout::Direction    m_direction;
    bool m_isSpeedControlVisible;
    QObjectList              m_content;

private slots:
    void on_btnAbulEduControlAudioPrevious_clicked();

    void on_btnAbulEduControlAudioPause_clicked();

    void on_btnAbulEduControlAudioPlay_clicked();

    void on_btnAbulEduControlAudioStop_clicked();

    void on_btnAbulEduControlAudioNext_clicked();

    void on_btnAbulEduControlAudioHide_clicked();

    void on_btnAbulEduControlAudioRecord_clicked();

    void on_btnAbulEduControlAudioMagnifyingGlass_clicked();

#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    void on_btnAbulEduControlAudioToggleOutput_toggled(bool checked);
#endif

    void on_btnAbulEduControlAudioIncreaseSpeed_clicked();

    void on_btnAbulEduControlAudioDecreaseSpeed_clicked();

signals:
    void signalAbeControlAudioPlayClicked();
    void signalAbeControlAudioPauseClicked();
    void signalAbeControlAudioStopClicked();
    void signalAbeControlAudioNextClicked();
    void signalAbeControlAudioPreviousClicked();
    void signalAbeControlAudioBtnHideClicked();
    void signalAbeControlAudioRecordClicked();
    void signalAbeControlAudioMagnifyingGlassClicked();
#if !defined (Q_OS_WIN) || defined(Q_OS_ANDROID)
    void signalAbeControlAudioToggleOutput(bool);
#endif
    void signalAbeControlAudioSpeedChanged(int);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(AbulEduControlAudioV1::flagControlButtons)
#endif // ABULEDUCONTROLAUDIOV1_H
