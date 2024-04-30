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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaEnum>

using namespace AbulEduAudioV1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->convertButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileDialogButton_clicked()
{
    /* Recupération wav */
    QString wavFilePath = QFileDialog::getOpenFileName(0, "Open WAV FILE", "", "WAV Files (*.wav)");
    if(wavFilePath != ""){
        ui->textEdit->append(QString::fromLatin1("Fichier choisi : %1").arg(wavFilePath));
    }
    else
    {
        ui->textEdit->append(QString::fromLatin1("No File..."));
        return;
    }

    waveEncoder = new AbulEduAudioV1::WaveToOgg(this);
    /* Connection sur le status */
    connect(waveEncoder, SIGNAL(signalWaveToOggStatus(WaveToOgg::Status,WaveHeaderReader::Status,QString)), this, SLOT(slotWaveEncoderErrors(WaveToOgg::Status,WaveHeaderReader::Status,QString)));
    /* Connection sur les informations */
    connect(waveEncoder, SIGNAL(signalWaveToOggInformationsAvailable(WaveHeaderReader::WaveInformations)), this, SLOT(slotDisplayWaveInformations(WaveHeaderReader::WaveInformations)));
    /* Connection sur les pourcentages */
    connect(waveEncoder, SIGNAL(signalWaveToOggProcessPercent(float)), this, SLOT(slotDisplayProgression(float)));
    waveEncoder->processWavFile(wavFilePath, QDir::tempPath() + "/test.ogg");
}

void MainWindow::slotWaveEncoderErrors(WaveToOgg::Status statusWaveToOgg, WaveHeaderReader::Status statusWaveHeaderReader, const QString &file)
{
    /* Recuperation name enum  WaveToOgg::Status */
    const QMetaObject &moWaveToOgg = WaveToOgg::staticMetaObject;
    QMetaEnum meWaveToOgg = moWaveToOgg.enumerator(moWaveToOgg.indexOfEnumerator("Status"));

    /* Recuperation name enum  WaveHeaderReader::Status */
    const QMetaObject &moWaveHeaderReader = WaveHeaderReader::staticMetaObject;
    QMetaEnum meWaveHeaderReader = moWaveHeaderReader.enumerator(moWaveHeaderReader.indexOfEnumerator("Status"));

    ui->textEdit->append(QString("Status Encoder : %1").arg(QString(meWaveToOgg.valueToKey(statusWaveToOgg))));
    ui->textEdit->append(QString("Status WaveReader : %1").arg(QString(meWaveHeaderReader.valueToKey(statusWaveHeaderReader))));
    ui->textEdit->append(QString("File : %1").arg(file));

    if(statusWaveToOgg == WaveToOgg::EncoderAvailable)
    {
        ui->convertButton->setEnabled(true);
    }
}

void MainWindow::slotDisplayWaveInformations(WaveHeaderReader::WaveInformations waveInfo)
{
    ui->labelBlockAlignValue->setText(QString::number(waveInfo.blockAlign));
    ui->labelBytesSampleValue->setText(QString::number(waveInfo.bitsPerSample));
    ui->labelChannelsValue->setText(QString::number(waveInfo.channels));
    ui->labelSampleRateValue->setText(QString::number(waveInfo.sampleRate));
    ui->labelAvgBytesSecondValue->setText(QString::number(waveInfo.avgBytesSecond));
}

void MainWindow::slotDisplayProgression(float progression)
{
    ui->textEdit->append(QString("Progression : %1").arg(progression));
}

void MainWindow::on_convertButton_clicked()
{
    waveEncoder->encodeWavFile();
    ui->convertButton->setEnabled(false);
}
