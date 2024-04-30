#ifndef ABULEDUMULTIMEDIASETTINGSV1_H
#define ABULEDUMULTIMEDIASETTINGSV1_H

#include <QObject>
#include <QDebug>
#include <QAudioDeviceInfo>
#include "abuledusounddevices.h"
#include "abuledupicottsv1.h"
#include "abuleduloggerv1.h"

class AbulEduMultiMediaSettingsV1 : public QObject
{
    Q_OBJECT
    Q_ENUMS(enumPlayMode enumTTSLanguage)

public:
    /** Énumère les langues pour la synthèse vocale */
//    enum enumTTSLanguage{None = -1, us = 0,  eng = 1, ger = 2, spa = 3, fre = 4, ita = 5};

    /** Précise la façon dont les medias seront joués
     *  \li automatic -> démarrage manuel + enchainement automatique
     *  \li manual    -> enchainement manuel
     *  \li autostart -> démarrage automatique + enchainement automatique
     */
    enum enumPlayMode{Automatic, Manual, AutoStart};


    explicit AbulEduMultiMediaSettingsV1(QObject *parent = 0);

    inline AbulEduPicottsV1::AbulEduLang abeMultiMediaSettingsGetTTSLanguage() {return m_TTSLanguage;}
    inline void abeMultiMediaSettingsSetTTSLanguage(AbulEduPicottsV1::AbulEduLang v) {m_TTSLanguage = v;}

    AbulEduPicottsV1::AbulEduLang abeMultiMediaSettingsGetTTSLanguageFromIso6391(const QString& codeIso);
    AbulEduPicottsV1::AbulEduLang abeMultiMediaSettingsGetTTSLanguageFromIso6392(const QString& codeIso);
    QString abeMultiMediaSettingsGetIso6391FromTTSLanguage(AbulEduPicottsV1::AbulEduLang  codeEnum);
    QString abeMultiMediaSettingsGetIso6392FromTTSLanguage(AbulEduPicottsV1::AbulEduLang codeEnum);


    inline QAudioDeviceInfo abeMultiMediaSettingsGetAudioOutputDeviceInfo() {return m_defaultAudioOuput;}
    inline void abeMultiMediaSettingsSetAudioOutputDeviceInfo(QAudioDeviceInfo v) {m_defaultAudioOuput = v;}

    inline QAudioDeviceInfo abeMultiMediaSettingsGetAudioInputDeviceInfo() {return m_defaultAudioInput;}
    inline void abeMultiMediaSettingsSetAudioInputDeviceInfo(QAudioDeviceInfo v) {m_defaultAudioInput = v;}

    inline bool abeMultiMediaSettingsGetIsTextVisible() {return m_isTextVisible;}
    inline void abeMultiMediaSettingsSetIsTextVisible(bool v) { m_isTextVisible = v;}

    inline enumPlayMode abeMultiMediaSettingsGetPlayMode() {return m_playMode;}
    inline void abeMultiMediaSettingsSetPlayMode(enumPlayMode v) { m_playMode = v;}

    inline QAudioDeviceInfo abeMultiMediaSettingsGetHeadPhone() {return m_headPhone;}

    inline QAudioDeviceInfo abeMultiMediaSettingsGetLoudSpeaker() {return m_loudSpeaker;}

    /** La lecture va se faire dans un QAudioOutput, pour lequel le volume varie entre 0.0 et 1.0 */
    inline qreal abeMultiMediaSettingsGetAudioOutputLevel() const {return m_audioOutputLevel;}
    void abeMultiMediaSettingsSetAudioOutputLevel(const qreal &audioOutputLevel) {m_audioOutputLevel = audioOutputLevel;}

    /** L'enregistrement va se faire dans un QAudioRecorder, pour lequel le volume varie entre 0.0 et 100.0 */
    inline qreal abeMultiMediaSettingsGetAudioInputLevel() const {return m_audioInputLevel;}
    void abeMultiMediaSettingsSetAudioInputLevel(const qreal &audioInputLevel) {m_audioInputLevel = audioInputLevel;}
private:
    QAudioDeviceInfo  m_defaultAudioOuput;
    QAudioDeviceInfo  m_headPhone;
    QAudioDeviceInfo  m_loudSpeaker;
    QAudioDeviceInfo  m_defaultAudioInput;
    AbulEduPicottsV1::AbulEduLang   m_TTSLanguage;
    bool              m_isTextVisible;
    enumPlayMode      m_playMode;
    qreal             m_audioOutputLevel;
    qreal             m_audioInputLevel;
    /** QMap de correspondance entre les AbulEduPicottsV1::AbulEduLang et les codes ISO 639-1 et ISO 639-2 */
    QMap<AbulEduPicottsV1::AbulEduLang,QPair<QString,QString> > m_codeLanguages;
};

#endif // ABULEDUMULTIMEDIASETTINGSV1_H
