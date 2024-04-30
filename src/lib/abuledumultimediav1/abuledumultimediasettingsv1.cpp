#include "abuledumultimediasettingsv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMultiMediaSettingsV1::AbulEduMultiMediaSettingsV1(QObject *parent) :
    QObject(parent),
    m_headPhone(QAudioDeviceInfo()),
    m_loudSpeaker(QAudioDeviceInfo()),
    m_defaultAudioInput(QAudioDeviceInfo::defaultInputDevice()),
    m_TTSLanguage(AbulEduPicottsV1::None),
    m_isTextVisible(true),
    m_playMode(Automatic),
    m_audioOutputLevel(1.0),
    m_audioInputLevel(50.0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    foreach(QAudioDeviceInfo di,QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)){
        ABULEDU_LOG_DEBUG() << di.deviceName();
    }
    if(QAudioDeviceInfo::defaultOutputDevice().deviceName().contains("usb",Qt::CaseInsensitive)){
        m_headPhone = QAudioDeviceInfo::defaultOutputDevice();
    }
    else{
        m_loudSpeaker = QAudioDeviceInfo::defaultOutputDevice();
    }

    /* Boucle de liste des peripheriques audio de sortie pour prendre l'usb par defaut */
    QMap<QString, QAudioDeviceInfo> listoutputDevices = abeGetOutputDevices();
    QMapIterator<QString, QAudioDeviceInfo> it(listoutputDevices);
    bool notFound = true;
    /* On va prendre le premier outputdevice USB trouvé (Sera-ce le bon ? )*/
    while(it.hasNext() && notFound)
    {
        it.next();
        QString outputDeviceName = it.value().deviceName();
        if(outputDeviceName.contains("USB",Qt::CaseInsensitive) || outputDeviceName.contains("Plantronics"))
        {
            m_defaultAudioOuput = it.value();
            m_headPhone = it.value();
            notFound = false;
            ABULEDU_LOG_DEBUG() << "m_headPhone mis à "<<it.value().deviceName();
        }
    }
    it.toFront();
    notFound = true;
    /** On va prendre le premier outputdevice USB trouvé (Sera-ce le bon ? )*/
    while(it.hasNext() && notFound)
    {
        it.next();
        if(it.value().deviceName().contains("pci",Qt::CaseInsensitive))
        {
            m_loudSpeaker = it.value();
            if(m_defaultAudioOuput.isNull()){
                m_defaultAudioOuput = m_loudSpeaker;
            }
            notFound = false;
            ABULEDU_LOG_DEBUG() <<"m_loudSpeaker mis à "<<it.value().deviceName();
        }
    }

    /* Boucle de liste des peripheriques audio d'entrée pour prendre l'usb par defaut */
    /** @todo encore faut-il le passer au QAudioRecoder */
    QMap<QString, QAudioDeviceInfo> listinputDevices = abeGetInputDevices();
    QMapIterator<QString, QAudioDeviceInfo> it2(listinputDevices);
    notFound = true;
    while(it2.hasNext() && notFound){
        it2.next();
        QString inputDeviceName = it2.value().deviceName();
        /* Parce que sous Windows 7 le deviceName est coupé à 31 caractères on prévoit le cas où le casque que nous recommandons est branché */
        if(inputDeviceName.contains("USB") || inputDeviceName.contains("Plantronics"))
        {
            m_defaultAudioInput = it2.value();
            notFound = false;
        }
    }

    /* Construction de la QMap m_codeLanguages
     * Il y a un souci du fait de la nom différenciation en ISO 639 de l'anglais et de l'américain alors que Picotts fait une différence */
    m_codeLanguages.clear();
    m_codeLanguages.insert(AbulEduPicottsV1::None,QPair<QString,QString>("Unset","Unset"));
    m_codeLanguages.insert(AbulEduPicottsV1::us,QPair<QString,QString>("en","eng"));
    m_codeLanguages.insert(AbulEduPicottsV1::en,QPair<QString,QString>("en","eng"));
    m_codeLanguages.insert(AbulEduPicottsV1::de,QPair<QString,QString>("de","ger"));
    m_codeLanguages.insert(AbulEduPicottsV1::es,QPair<QString,QString>("es","spa"));
    m_codeLanguages.insert(AbulEduPicottsV1::fr,QPair<QString,QString>("fr","fre"));
    m_codeLanguages.insert(AbulEduPicottsV1::it,QPair<QString,QString>("it","ita"));
}

AbulEduPicottsV1::AbulEduLang AbulEduMultiMediaSettingsV1::abeMultiMediaSettingsGetTTSLanguageFromIso6391(const QString &codeIso)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduPicottsV1::AbulEduLang codeEnum = AbulEduPicottsV1::None;
    QMapIterator<AbulEduPicottsV1::AbulEduLang,QPair<QString,QString> > it(m_codeLanguages);
    while(it.hasNext() && codeEnum == AbulEduPicottsV1::None){
        QPair<QString,QString> p = it.next().value();
        if(p.first == codeIso){
            codeEnum = m_codeLanguages.key(p);
        }
    }
    return codeEnum;
}

AbulEduPicottsV1::AbulEduLang AbulEduMultiMediaSettingsV1::abeMultiMediaSettingsGetTTSLanguageFromIso6392(const QString &codeIso)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduPicottsV1::AbulEduLang codeEnum = AbulEduPicottsV1::None;
    QMapIterator<AbulEduPicottsV1::AbulEduLang,QPair<QString,QString> > it(m_codeLanguages);
    while(it.hasNext() && codeEnum == AbulEduPicottsV1::None){
        QPair<QString,QString> p = it.next().value();
        if(p.second == codeIso){
            codeEnum = m_codeLanguages.key(p);
        }
    }
    return codeEnum;
}

QString AbulEduMultiMediaSettingsV1::abeMultiMediaSettingsGetIso6391FromTTSLanguage(AbulEduPicottsV1::AbulEduLang codeEnum)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_codeLanguages.contains(codeEnum)){
        return m_codeLanguages.value(codeEnum).first;
    }
    else
        return QString();
}

QString AbulEduMultiMediaSettingsV1::abeMultiMediaSettingsGetIso6392FromTTSLanguage(AbulEduPicottsV1::AbulEduLang codeEnum)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString codeIso = QString();
    QMapIterator<AbulEduPicottsV1::AbulEduLang,QPair<QString,QString> > it(m_codeLanguages);
    while(it.hasNext() && codeIso.isNull()){
        AbulEduPicottsV1::AbulEduLang e = it.next().key();
        if(e == codeEnum){
            codeIso = m_codeLanguages.value(e).second;
        }
    }
    return codeIso;
}
