/** Classe MainWindow : Exemple utilisation librairie AbulEduAudioV1
  *
  * @author 2014 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>

#include "waveheaderreader.h"
#include "wavetoogg.h"

using namespace  AbulEduAudioV1;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_fileDialogButton_clicked();
    void slotWaveEncoderErrors(WaveToOgg::Status, WaveHeaderReader::Status, const QString&);
    void slotDisplayWaveInformations(WaveHeaderReader::WaveInformations);
    void slotDisplayProgression(float);

    void on_convertButton_clicked();

private:
    Ui::MainWindow *ui;

    AbulEduAudioV1::WaveToOgg *waveEncoder;
};

#endif // MAINWINDOW_H
