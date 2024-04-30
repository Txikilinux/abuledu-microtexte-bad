#ifndef ABULEDUSOUNDDEVICES_H
#define ABULEDUSOUNDDEVICES_H

#include <QString>
#include <QMap>
#include <QAudioDeviceInfo>
/** QMap des output devices : nom du device, deviceInfo */
static inline QMap<QString, QAudioDeviceInfo> abeGetOutputDevices()
{
    QMap<QString, QAudioDeviceInfo> mapDeviceNames;
    QList<QAudioDeviceInfo> listDeviceInfo;
    listDeviceInfo = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    foreach (QAudioDeviceInfo device, listDeviceInfo) {
        mapDeviceNames.insert(device.deviceName(), device);
    }
    return mapDeviceNames;
}

/** QMap des input devices : nom du device, deviceInfo */
static inline QMap<QString, QAudioDeviceInfo> abeGetInputDevices()
{
    QMap<QString, QAudioDeviceInfo> mapDeviceNames;
    QList<QAudioDeviceInfo> listDeviceInfo;
    listDeviceInfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo device, listDeviceInfo) {
        mapDeviceNames.insert(device.deviceName(), device);
    }
    return mapDeviceNames;
}

#endif // ABULEDUSOUNDDEVICES_H
