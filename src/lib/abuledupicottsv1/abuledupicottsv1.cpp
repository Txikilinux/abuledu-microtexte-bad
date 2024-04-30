/**
  * Classe
  * @author 2011-2013 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2014 Ichal Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
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

#include "abuledupicottsv1.h"
#include "abuleduapplicationv1.h"

#ifndef DEBUG_ABULEDUPICOTTSV1
    #include "abuledudisableloggerv1.h"
#endif

/* adaptation layer defines */
#define PICO_MEM_SIZE       2500000
#define DummyLen 100000000

/* string constants */
#define MAX_OUTBUF_SIZE     128
#ifdef picolangdir
const char * PICO_LINGWARE_PATH             = picolangdir "/";
#else
  #if defined(Q_OS_ANDROID)
    const char * PICO_LINGWARE_PATH             = "/system/tts/lang_pico/";
  #elif defined(Q_OS_LINUX)
      const char * PICO_LINGWARE_PATH             = "/usr/share/pico/lang/";
  #elif defined(Q_WS_MAC)
      const char * PICO_LINGWARE_PATH             = "./data/picotts/";
  #elif defined(Q_OS_WIN)
      const char * PICO_LINGWARE_PATH             = "./data/picotts/";
  #endif
#endif
const char * PICO_VOICE_NAME                = "PicoVoice";

/* supported voices
   Pico does not seperately specify the voice and locale.   */
const char * picoSupportedLangIso3[]        = { "eng",              "eng",              "deu",              "spa",              "fra",              "ita" };
const char * picoSupportedCountryIso3[]     = { "USA",              "GBR",              "DEU",              "ESP",              "FRA",              "ITA" };
const char * picoSupportedLang[]            = { "en-US",            "en-GB",            "de-DE",            "es-ES",            "fr-FR",            "it-IT" };
const char * picoInternalLang[]             = { "en-US",            "en-GB",            "de-DE",            "es-ES",            "fr-FR",            "it-IT" };
const char * picoInternalTaLingware[]       = { "en-US_ta.bin",     "en-GB_ta.bin",     "de-DE_ta.bin",     "es-ES_ta.bin",     "fr-FR_ta.bin",     "it-IT_ta.bin" };
const char * picoInternalSgLingware[]       = { "en-US_lh0_sg.bin", "en-GB_kh0_sg.bin", "de-DE_gl0_sg.bin", "es-ES_zl0_sg.bin", "fr-FR_nk0_sg.bin", "it-IT_cm0_sg.bin" };
const char * picoInternalUtppLingware[]     = { "en-US_utpp.bin",   "en-GB_utpp.bin",   "de-DE_utpp.bin",   "es-ES_utpp.bin",   "fr-FR_utpp.bin",   "it-IT_utpp.bin" };
const int picoNumSupportedVocs              = 6;

/* adapation layer global variables */
void *          picoMemArea         = NULL;
pico_System     picoSystem          = NULL;
pico_Resource   picoTaResource      = NULL;
pico_Resource   picoSgResource      = NULL;
pico_Resource   picoUtppResource    = NULL;
pico_Engine     picoEngine          = NULL;
pico_Char *     picoTaFileName      = NULL;
pico_Char *     picoSgFileName      = NULL;
pico_Char *     picoUtppFileName    = NULL;
pico_Char *     picoTaResourceName  = NULL;
pico_Char *     picoSgResourceName  = NULL;
pico_Char *     picoUtppResourceName = NULL;
int             picoSynthAbort = 0;


/** Constructeur
    Langue = 0: "USA" 1: "GBR" 2: "DEU" 3: "ESP" 4: "FRA" 5: "ITA"
*/
AbulEduPicottsV1::AbulEduPicottsV1(AbulEduLang lang, QObject *parent, QMediaPlayer *player)
    : QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

#if QT_VERSION < 0x050000
    m_audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    m_mediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(m_mediaObject, m_audioOutput);
    connect(m_mediaObject,SIGNAL(finished()),this,SLOT(play_end()));
#else
    if(player) {
        ABULEDU_LOG_DEBUG() << "  j'ai un player";
        m_audioPlayer = player;
    }
    else {
        ABULEDU_LOG_DEBUG() << "  j'ai pas de player ... j'en créé un et je connecte sur le slot local de gestion";
        m_audioPlayer = new QMediaPlayer(this);
        connect(m_audioPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slotMediaStatusChanged(QMediaPlayer::MediaStatus)),Qt::UniqueConnection);
        m_audioPlayer->setVolume(100);
        ABULEDU_LOG_DEBUG() << m_audioPlayer->isAudioAvailable() << m_audioPlayer->isAvailable();
    }

#endif
    m_langIndex = lang;
    m_fileTemp = 0;
    m_speedLevel = 100;
}

AbulEduPicottsV1::~AbulEduPicottsV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete m_fileTemp;
#if QT_VERSION < 0x050000
    delete m_audioOutput;
    delete m_mediaObject;
#endif
}

AbulEduPicottsV1::AbulEduLang AbulEduPicottsV1::abePicottsGetAbulEduLangFromIso6391(QString codeIso6391)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QMap<AbulEduLang,QString> codeLanguages;
    codeLanguages.insert(None,"Unset");
    codeLanguages.insert(us,"us");
    codeLanguages.insert(en,"en");
    codeLanguages.insert(de,"de");
    codeLanguages.insert(es,"es");
    codeLanguages.insert(fr,"fr");
    codeLanguages.insert(it,"it");
    AbulEduLang codeEnum = None;
    QMapIterator<AbulEduLang,QString> iter(codeLanguages);
    while(iter.hasNext() && codeEnum == None){
        QString p = iter.next().value();
        if(p == codeIso6391){
            codeEnum = codeLanguages.key(p);
        }
    }
    return codeEnum;
}
int AbulEduPicottsV1::abePicottsGetSpeedLevel() const
{
    return m_speedLevel;
}

void AbulEduPicottsV1::abePicottsSetSpeedLevel(int speedLevel)
{
    m_speedLevel = speedLevel;
}


void AbulEduPicottsV1::abePicoPlay(const QString &texte)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /** @warning si m_langIndex est égal à -1 plantage sur :
    strcat((char *) picoTaFileName,   (const char *) picoInternalTaLingware[m_langIndex]); */
    if(m_langIndex == -1){
        ABULEDU_LOG_WARN() << "Index de langue non conforme (-1). Pas de synthèse vocale";
        emit endOfPlay();
        return;
    }

    //Pour l'instant (08.01.2013) ça ne marche que sous linux ... toute aide est la bienvenue !
    //Update 12.06.2013 ajout du support windows
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#ifdef Q_OS_ANDROID
    m_fileTemp = new QFile(abeApp->getAbeHomepath() + "/lt-XXXXXXX.wav", this);
    if(!m_fileTemp->open(QIODevice::WriteOnly)) {
        ABULEDU_LOG_DEBUG() << "Impossible de créer un fichier !";
        return;
    }
#else
    m_fileTemp = new QTemporaryFile(QDir::tempPath() + "/lt-XXXXXXX.wav", this);
    if(!m_fileTemp->open()) {
        ABULEDU_LOG_DEBUG() << "Impossible de créer un fichier !";
        return;
    }
#endif
    ABULEDU_LOG_DEBUG() << "++++++++++++++ FILE [" << m_fileTemp->fileName() << "]";

    void * buffer;
    size_t bufferSize = 1024;

    buffer = malloc( bufferSize );

    ABULEDU_LOG_DEBUG() <<"++++++++++++++ BUFF [" << buffer << "]";

    int ret;
    int getstatus;
    pico_Char * inp;// = NULL;
    pico_Char * local_text;// = NULL;
    short       outbuf[MAX_OUTBUF_SIZE/2];

    pico_Int16      bytes_sent;
    pico_Int16      bytes_recv;
    pico_Int16      text_remaining;
    pico_Int16      out_data_type;
    pico_Retstring  outMessage;

    picoSynthAbort = 0;

    picoMemArea = malloc( PICO_MEM_SIZE );
    ABULEDU_LOG_DEBUG() << "++++++++++++++ PicoMemArea [" << picoMemArea << "]";

    if((ret = pico_initialize( picoMemArea, PICO_MEM_SIZE, &picoSystem ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot initialize pico (%i): %s\n", ret, outMessage);
        if (picoSystem) {
            pico_terminate(&picoSystem);
            picoSystem = NULL;
        }
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot initialize pico [" << ret << outMessage << "]";

        return;
    }

    /* Load the text analysis Lingware resource file.   */
    picoTaFileName      = (pico_Char *) malloc( PICO_MAX_DATAPATH_NAME_SIZE + PICO_MAX_FILE_NAME_SIZE );

    ABULEDU_LOG_DEBUG() << "++++++++++++++ picoTaFileName [" << picoTaFileName<< "]";
    strcpy((char *) picoTaFileName,   PICO_LINGWARE_PATH);
    strcat((char *) picoTaFileName,   (const char *) picoInternalTaLingware[m_langIndex]);
    if((ret = pico_loadResource( picoSystem, picoTaFileName, &picoTaResource ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot load text analysis resource file (%i): %s\n", ret, outMessage);
        if (picoTaResource) {
            pico_unloadResource( picoSystem, &picoTaResource );
            picoTaResource = NULL;
        }
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot load text analysis resource file [" << ret << outMessage << "]";
        return;
    }


    /* Load the signal generation Lingware resource file.   */
    picoSgFileName      = (pico_Char *) malloc( PICO_MAX_DATAPATH_NAME_SIZE + PICO_MAX_FILE_NAME_SIZE );
    strcpy((char *) picoSgFileName,   PICO_LINGWARE_PATH);
    strcat((char *) picoSgFileName,   (const char *) picoInternalSgLingware[m_langIndex]);
    if((ret = pico_loadResource( picoSystem, picoSgFileName, &picoSgResource ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot load signal generation Lingware resource file (%i): %s\n", ret, outMessage);
        if (picoSgResource) {
            pico_unloadResource( picoSystem, &picoSgResource );
            picoSgResource = NULL;
        }
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot load signal generation Lingware resource file [" << ret << outMessage << "]";
        return;
    }

    /* Load the utpp Lingware resource file if exists - NOTE: this file is optional
       and is currently not used. Loading is only attempted for future compatibility.
       If this file is not present the loading will still succeed.                      //
    picoUtppFileName      = (pico_Char *) malloc( PICO_MAX_DATAPATH_NAME_SIZE + PICO_MAX_FILE_NAME_SIZE );
    strcpy((char *) picoUtppFileName,   PICO_LINGWARE_PATH);
    strcat((char *) picoUtppFileName,   (const char *) picoInternalUtppLingware[m_langIndex]);
    ret = pico_loadResource( picoSystem, picoUtppFileName, &picoUtppResource );
    pico_getSystemStatusMessage(picoSystem, ret, outMessage);
    printf("pico_loadResource: %i: %s\n", ret, outMessage);
    */

    /* Get the text analysis resource name.     */
    picoTaResourceName  = (pico_Char *) malloc( PICO_MAX_RESOURCE_NAME_SIZE );
    if((ret = pico_getResourceName( picoSystem, picoTaResource, (char *) picoTaResourceName ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot get the text analysis resource name (%i): %s\n", ret, outMessage);
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot get the text analysis resource name [" << ret << outMessage << "]";
        return;
//        goto unloadUtppResource;
    }

    /* Get the signal generation resource name. */
    picoSgResourceName  = (pico_Char *) malloc( PICO_MAX_RESOURCE_NAME_SIZE );
    if((ret = pico_getResourceName( picoSystem, picoSgResource, (char *) picoSgResourceName ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot get the signal generation resource name (%i): %s\n", ret, outMessage);
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot get the signal generation resource name [" << ret << outMessage << "]";
        return;
//        goto unloadUtppResource;
    }


    /* Create a voice definition.   */
    if((ret = pico_createVoiceDefinition( picoSystem, (const pico_Char *) PICO_VOICE_NAME ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot create voice definition (%i): %s\n", ret, outMessage);
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot create voice definition  [" << ret << outMessage << "]";
        return;
//        goto unloadUtppResource;
    }

    /* Add the text analysis resource to the voice. */
    if((ret = pico_addResourceToVoiceDefinition( picoSystem, (const pico_Char *) PICO_VOICE_NAME, picoTaResourceName ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot add the text analysis resource to the voice (%i): %s\n", ret, outMessage);
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot add the text analysis resource to the voice [" << ret << outMessage << "]";
        return;
     }

    /* Add the signal generation resource to the voice. */
    if((ret = pico_addResourceToVoiceDefinition( picoSystem, (const pico_Char *) PICO_VOICE_NAME, picoSgResourceName ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot add the signal generation resource to the voice (%i): %s\n", ret, outMessage);
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot add the signal generation resource to the voice [" << ret << outMessage << "]";
        return;
//        goto unloadUtppResource;
    }

    /* Create a new Pico engine. */
    if((ret = pico_newEngine( picoSystem, (const pico_Char *) PICO_VOICE_NAME, &picoEngine ))) {
        pico_getSystemStatusMessage(picoSystem, ret, outMessage);
        fprintf(stderr, "Cannot create a new pico engine (%i): %s\n", ret, outMessage);
        if (picoEngine) {
            pico_disposeEngine( picoSystem, &picoEngine );
            pico_releaseVoiceDefinition( picoSystem, (pico_Char *) PICO_VOICE_NAME );
            picoEngine = NULL;
        }
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot create a new pico engine [" << ret << outMessage << "]";
        return;
    }

    picoos_Common common = (picoos_Common) pico_sysGetCommon(picoSystem);

    picoos_SDFile sdOutFile = NULL;

    picoos_bool done = TRUE;
    if(TRUE != (done = picoos_sdfOpenOut(common, &sdOutFile,
//                                         (picoos_char *) wavefile, SAMPLE_FREQ_16KHZ, PICOOS_ENC_LIN)))
                                         (picoos_char *) m_fileTemp->fileName().toStdString().c_str(), SAMPLE_FREQ_16KHZ, PICOOS_ENC_LIN)))
    {
        fprintf(stderr, "Cannot open output wave file\n");
        ret = 1;
        if (picoEngine) {
            pico_disposeEngine( picoSystem, &picoEngine );
            pico_releaseVoiceDefinition( picoSystem, (pico_Char *) PICO_VOICE_NAME );
            picoEngine = NULL;
        }
        ABULEDU_LOG_DEBUG() <<"++++++++++++++ Cannot open output wave file [" << ret << outMessage << "]";
        return;
    }

    //Decoupage via QString sans doute mieux que via pico char*
    QStringList phrases = texte.split(".");
    foreach (const QString &str, phrases) {
        const QByteArray ba1("<pitch level=\"100\"><speed level=\"");
        const QByteArray ba2(QString::number(m_speedLevel).toUtf8());
        const QByteArray ba3("\">");
        const QByteArray ba = ba1 + ba2 + ba3 + str.toUtf8() + "</speed></pitch>";
        local_text = (pico_Char *) ba.constData();
        text_remaining = strlen((const char *) local_text) + 1;

        inp = (pico_Char *) local_text;

        size_t bufused = 0;

        /* synthesis loop   */
        while (text_remaining) {

            /* Feed the text into the engine.   */
            if((ret = pico_putTextUtf8( picoEngine, inp, text_remaining, &bytes_sent ))) {
                pico_getSystemStatusMessage(picoSystem, ret, outMessage);
                fprintf(stderr, "Cannot put Text (%i): %s\n", ret, outMessage);
                if (picoEngine) {
                    pico_disposeEngine( picoSystem, &picoEngine );
                    pico_releaseVoiceDefinition( picoSystem, (pico_Char *) PICO_VOICE_NAME );
                    picoEngine = NULL;
                }
                ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot put Text [" << ret << outMessage << "]";
                return;
            }

            text_remaining -= bytes_sent;
            inp += bytes_sent;

            do {
                if (picoSynthAbort) {
                    if (picoEngine) {
                        pico_disposeEngine( picoSystem, &picoEngine );
                        pico_releaseVoiceDefinition( picoSystem, (pico_Char *) PICO_VOICE_NAME );
                        picoEngine = NULL;
                    }
                    return;
                }
                /* Retrieve the samples and add them to the buffer. */
                getstatus = pico_getData( picoEngine, (void *) outbuf,
                                          MAX_OUTBUF_SIZE, &bytes_recv, &out_data_type );
                if((getstatus !=PICO_STEP_BUSY) && (getstatus !=PICO_STEP_IDLE)){
                    pico_getSystemStatusMessage(picoSystem, getstatus, outMessage);
                    fprintf(stderr, "Cannot get Data (%i): %s\n", getstatus, outMessage);
                    if (picoEngine) {
                        pico_disposeEngine( picoSystem, &picoEngine );
                        pico_releaseVoiceDefinition( picoSystem, (pico_Char *) PICO_VOICE_NAME );
                        picoEngine = NULL;
                    }
                    ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot get Data [" << getstatus << outMessage << "]";
                    return;
                }
                if (bytes_recv) {
                    if ((bufused + bytes_recv) <= bufferSize) {
                        memcpy(buffer+bufused, (qint8 *) outbuf, bytes_recv);
                        bufused += bytes_recv;
                    } else {
                        done = picoos_sdfPutSamples(
                                    sdOutFile,
                                    bufused / 2,
                                    (picoos_int16*) (buffer));
                        bufused = 0;
                        memcpy(buffer, (qint8 *) outbuf, bytes_recv);
                        bufused += bytes_recv;
                    }
                }
            } while (PICO_STEP_BUSY == getstatus);
            /* This chunk of synthesis is finished; pass the remaining samples. */
            if (!picoSynthAbort) {
                done = picoos_sdfPutSamples(
                            sdOutFile,
                            bufused / 2,
                            (picoos_int16*) (buffer));
            }
            picoSynthAbort = 0;
        }
    }
    if(TRUE != (done = picoos_sdfCloseOut(common, &sdOutFile)))
    {
        fprintf(stderr, "Cannot close output wave file\n");
        ABULEDU_LOG_DEBUG() << "++++++++++++++ Cannot close output wave file ";
        ret = 1;
        if (picoEngine) {
            pico_disposeEngine( picoSystem, &picoEngine );
            pico_releaseVoiceDefinition( picoSystem, (pico_Char *) PICO_VOICE_NAME );
            picoEngine = NULL;
        }
        return;
    }

    QApplication::restoreOverrideCursor();

#if QT_VERSION < 0x050000
    m_mediaObject->setCurrentSource(fi.absoluteFilePath());
    //ABULEDU_LOG_DEBUG() << "AbulEduPicottsV1:: lecture du son ..." << fi.absoluteFilePath();
    m_mediaObject->play();
    emit beginOfPlay();

#else
    ABULEDU_LOG_DEBUG() << "++++++++++++++ setMedia " << m_fileTemp->fileName();

    /* Windows ne gère pas trop bien les URL locales */
#ifdef Q_OS_WIN
    m_audioPlayer->setMedia(0, m_fileTemp);
#else
    m_audioPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(m_fileTemp->fileName())));
#endif // Q_OS_WIN

#endif //QT_VERSION < 0x050000

#endif
}

void AbulEduPicottsV1::abePicoPause()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
  #if QT_VERSION < 0x050000
    m_mediaObject->pause();
  #else
    m_audioPlayer->pause();
  #endif //QT_VERSION < 0x050000
#endif
}

void AbulEduPicottsV1::abePicoResume()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
  #if QT_VERSION < 0x050000
    m_mediaObject->play();
  #else
    m_audioPlayer->play();
  #endif //QT_VERSION < 0x050000
#endif
}

void AbulEduPicottsV1::abePicoStop()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
  #if QT_VERSION < 0x050000
    m_mediaObject->stop();
  #else
    m_audioPlayer->stop();
    m_audioPlayer->setMedia(QMediaContent());
  #endif //QT_VERSION < 0x050000
#endif
}

void AbulEduPicottsV1::slotAbePicoSetLang(const QString &strLang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    us = 0, en, de, es, fr, it
    //    Langue = 0: "USA" 1: "GBR" 2: "DEU" 3: "ESP" 4: "FRA" 5: "ITA"
    int indexOfAbulEduLangEnum = staticMetaObject.indexOfEnumerator("AbulEduLang");
    switch (staticMetaObject.enumerator(indexOfAbulEduLangEnum).keysToValue(strLang.toUtf8())) {
    case us:
        ABULEDU_LOG_DEBUG() << "Langue pico = us";
        m_langIndex = AbulEduPicottsV1::us;
        break;
    case en:
        ABULEDU_LOG_DEBUG() << "Langue pico = en";
        m_langIndex = AbulEduPicottsV1::en;
        break;
    case de:
        ABULEDU_LOG_DEBUG() << "Langue pico = de";
        m_langIndex = AbulEduPicottsV1::de;
        break;
    case es:
        ABULEDU_LOG_DEBUG() << "Langue pico = es";
        m_langIndex = AbulEduPicottsV1::es;
        break;
    case fr:
        ABULEDU_LOG_DEBUG() << "Langue pico = fr";
        m_langIndex = AbulEduPicottsV1::fr;
        break;
    case it:
        ABULEDU_LOG_DEBUG() << "Langue pico = it";
        m_langIndex = AbulEduPicottsV1::it;
        break;
    }
}

void AbulEduPicottsV1::play_end()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    m_fileTemp->remove();
#endif
    emit endOfPlay();
}

#if QT_VERSION >= 0x050000
void AbulEduPicottsV1::slotMediaStatusChanged(const QMediaPlayer::MediaStatus &status)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch (status) {
    case QMediaPlayer::LoadedMedia:
        m_audioPlayer->play();
        emit beginOfPlay();
        break;

    case QMediaPlayer::EndOfMedia:
        m_audioPlayer->stop();
        m_audioPlayer->setMedia(QMediaContent());
        m_fileTemp = 0;
        emit endOfPlay();
        break;

    default:
        break;
    }
}
#endif
