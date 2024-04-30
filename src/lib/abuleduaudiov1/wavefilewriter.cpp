#include "wavefilewriter.h"

/**********************************************
 *
 *
 *
 *
 **********************************************/

WaveFileWriter::WaveFileWriter(QObject *parent)
    : QObject(parent)
    , m_dataLength(0)
    , m_makeFile(true)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

WaveFileWriter::~WaveFileWriter()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    close();
}

bool WaveFileWriter::open(const QString &fileName, const QAudioFormat &format)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_file.isOpen()){
        ABULEDU_LOG_DEBUG() <<"WaveFileWriter:: File is already open";
        return false;
    }// file already open

    if (format.codec() != "audio/pcm" || format.sampleType() != QAudioFormat::SignedInt){
        ABULEDU_LOG_DEBUG() << "WaveFileWriter:: Data Format not supported";
        return false; // data format is not supported
    }

    m_file.setFileName(fileName);
    if (!m_file.open(QIODevice::WriteOnly)){
        ABULEDU_LOG_DEBUG() << "WaveFileWriter:: Unable to open file for writing";
        return false; // unable to open file for writing
    }

    if(m_buffer.isOpen())
        return false;

    if(!m_buffer.open(QIODevice::WriteOnly))
        return false;

    m_format = format;

    if (!writeHeader(format))
        return false;

    return true;
}

bool WaveFileWriter::write(const QAudioBuffer &buffer)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if (buffer.format() != m_format)
        return false; // buffer format has changed

    qint64 written = 0;

    if(m_makeFile) {
        written = m_file.write((const char *)buffer.constData(), buffer.byteCount());
    }
    else {
        written = m_buffer.write((const char *)buffer.constData(), buffer.byteCount());
    }

    m_dataLength += written;
    return written == buffer.byteCount();
}

bool WaveFileWriter::close()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool result = false;

    if(m_makeFile) {
        if (m_file.isOpen()) {
            Q_ASSERT(m_dataLength < INT_MAX);
            result = writeDataLength();

            m_dataLength = 0;
            m_file.flush();
            m_file.close();
        }
    }
    else {
        if (m_buffer.isOpen()) {
            Q_ASSERT(m_dataLength < INT_MAX);
            result = writeDataLength();

            m_dataLength = 0;
            m_buffer.close();
        }
    }

    return result;
}

bool WaveFileWriter::isOpen() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_makeFile)
        return m_file.isOpen();
    else
        return m_buffer.isOpen();
}

const QPointer<QIODevice> WaveFileWriter::getDecodedFile()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_makeFile)
        return &m_file;
    else
        return &m_buffer;
}

bool WaveFileWriter::writeHeader(const QAudioFormat &format)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    // check if format is supported
    if (format.byteOrder() == QAudioFormat::BigEndian || format.sampleType() != QAudioFormat::SignedInt)
        return false;

    CombinedHeader header;
    memset(&header, 0, HeaderLength);

    if (format.byteOrder() == QAudioFormat::LittleEndian)
        strncpy(&header.riff.descriptor.id[0], "RIFF", 4);
    else
        strncpy(&header.riff.descriptor.id[0], "RIFX", 4);

    qToLittleEndian<quint32>(quint32(m_dataLength + HeaderLength - 8),
                             reinterpret_cast<unsigned char*>(&header.riff.descriptor.size));
    strncpy(&header.riff.type[0], "WAVE", 4);

    // WAVE header
    strncpy(&header.wave.descriptor.id[0], "fmt ", 4);
    qToLittleEndian<quint32>(quint32(16),
                             reinterpret_cast<unsigned char*>(&header.wave.descriptor.size));
    qToLittleEndian<quint16>(quint16(1),
                             reinterpret_cast<unsigned char*>(&header.wave.audioFormat));
    qToLittleEndian<quint16>(quint16(format.channelCount()),
                             reinterpret_cast<unsigned char*>(&header.wave.numChannels));
    qToLittleEndian<quint32>(quint32(format.sampleRate()),
                             reinterpret_cast<unsigned char*>(&header.wave.sampleRate));
    qToLittleEndian<quint32>(quint32(format.sampleSize() * format.channelCount() * format.sampleSize() / 8),
                             reinterpret_cast<unsigned char*>(&header.wave.byteRate));
    qToLittleEndian<quint16>(quint16(format.channelCount() * format.sampleSize() / 8),
                             reinterpret_cast<unsigned char*>(&header.wave.blockAlign));
    qToLittleEndian<quint16>(quint16(format.sampleSize()),
                             reinterpret_cast<unsigned char*>(&header.wave.bitsPerSample));

    // DATA header
    strncpy(&header.data.descriptor.id[0], "data", 4);
    qToLittleEndian<quint32>(quint32(m_dataLength),
                             reinterpret_cast<unsigned char*>(&header.data.descriptor.size));

    if(m_makeFile)
        return (m_file.write(reinterpret_cast<const char *>(&header), HeaderLength) == HeaderLength);
    else
        return (m_buffer.write(reinterpret_cast<const char *>(&header), HeaderLength) == HeaderLength);
}

bool WaveFileWriter::writeDataLength()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#ifndef Q_LITTLE_ENDIAN
    // only implemented for LITTLE ENDIAN
    return false;
#endif

    if(m_makeFile){
        if (m_file.isSequential()) {
            return false;
        }
        m_file.seek(40);
        unsigned char dataLengthLE[4];
        qToLittleEndian<quint32>(quint32(m_dataLength), dataLengthLE);
        return m_file.write(reinterpret_cast<const char *>(dataLengthLE), 4);
    }
    else
    {
        if(m_buffer.isSequential())
            return false;

        // seek to RIFF header size, see header.riff.descriptor.size above
        if (!m_buffer.seek(4))
            return false;

        quint32 length = m_dataLength + HeaderLength - 8;
        if (m_buffer.write(reinterpret_cast<const char *>(&length), 4) != 4)
            return false;

        // seek to DATA header size, see header.data.descriptor.size above
        if (!m_buffer.seek(40))
            return false;

        return m_buffer.write(reinterpret_cast<const char *>(&m_dataLength), 4) == 4;
    }
}
