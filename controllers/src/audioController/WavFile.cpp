#include "WavFile.hpp"

#define WAV_ID "RIFF"
#define WAV_ID_OFF 0
#define WAV_ID_LEN 4

#define WAV_SIZE "\x00\x00\x00\x00"
#define WAV_SIZE_OFF WAV_ID_LEN
#define WAV_SIZE_LEN 4

#define WAV_FORMAT "WAVE"
#define WAV_FORMAT_OFF WAV_SIZE_OFF + WAV_SIZE_LEN
#define WAV_FORMAT_LEN 4

#define WAV_FMT_ID "fmt "
#define WAV_FMT_ID_OFF WAV_FORMAT_OFF + WAV_FORMAT_LEN
#define WAV_FMT_ID_LEN 4

#define WAV_FMT_SIZE "\x10\x00\x00\x00"
#define WAV_FMT_SIZE_OFF WAV_FMT_ID_OFF + WAV_FMT_ID_LEN
#define WAV_FMT_SIZE_LEN 4

#define WAV_FMT_FORMAT "\x01\x00"
#define WAV_FMT_FORMAT_OFF WAV_FMT_SIZE_OFF + WAV_FMT_SIZE_LEN
#define WAV_FMT_FORMAT_LEN 2

#define WAV_FMT_CHANNELS "\x01\x00"
#define WAV_FMT_CHANNELS_OFF WAV_FMT_FORMAT_OFF + WAV_FMT_FORMAT_LEN
#define WAV_FMT_CHANNELS_LEN 2

#define WAV_FMT_SAMPLE_RATE_OFF WAV_FMT_CHANNELS_OFF + WAV_FMT_CHANNELS_LEN
#define WAV_FMT_SAMPLE_RATE_LEN 4

#define WAV_FMT_BYTE_RATE_OFF WAV_FMT_SAMPLE_RATE_OFF + WAV_FMT_SAMPLE_RATE_LEN
#define WAV_FMT_BYTE_RATE_LEN 4

#define WAV_FMT_BLOCK_ALIGN "\x01\x00"
#define WAV_FMT_BLOCK_ALIGN_OFF WAV_FMT_BYTE_RATE_OFF + WAV_FMT_BYTE_RATE_LEN
#define WAV_FMT_BLOCK_ALIGN_LEN 2

#define WAV_FMT_BPS "\x08\x00"
#define WAV_FMT_BPS_OFF WAV_FMT_BLOCK_ALIGN_OFF + WAV_FMT_BLOCK_ALIGN_LEN
#define WAV_FMT_BPS_LEN 2

#define WAV_DATA_ID "data"
#define WAV_DATA_ID_OFF WAV_FMT_BPS_OFF + WAV_FMT_BPS_LEN
#define WAV_DATA_ID_LEN 4

#define WAV_DATA_SIZE "\x00\x00\x00\x00"
#define WAV_DATA_SIZE_OFF WAV_DATA_ID_OFF + WAV_DATA_ID_LEN
#define WAV_DATA_SIZE_LEN 4

#define WAV_DATA_DATA_OFF WAV_DATA_SIZE_OFF + WAV_DATA_SIZE_LEN

#define WRITE_WAV(VAL_NAME)    \
    file.seek(VAL_NAME##_OFF); \
    file.write(VAL_NAME, VAL_NAME##_LEN);

#define WRITE_WAV_NO_OFF(VAL_NAME) \
    file.write(VAL_NAME, VAL_NAME##_LEN);

#define WRITE_WAV_VAL(VAL_NAME, VAL) \
    file.seek(VAL_NAME##_OFF);       \
    file.write(VAL, VAL_NAME##_LEN);

#define WRITE_WAV_VAL_NO_OFF(VAL_NAME, VAL) \
    file.write(VAL, VAL_NAME##_LEN);

void uint32ToBytes(uint32_t input, byte *output);

WavFile::WavFile(File& fileObj, uint16_t sampleRate)
    : file(fileObj), dataSize(0), isObjectClosed(false)
{
    uint32ToBytes(sampleRate, wavSampleRate);
}

void WavFile::writeHeader()
{
    WRITE_WAV(WAV_ID);
    WRITE_WAV_NO_OFF(WAV_SIZE);
    WRITE_WAV_NO_OFF(WAV_FORMAT);
    WRITE_WAV_NO_OFF(WAV_FMT_ID);
    WRITE_WAV_NO_OFF(WAV_FMT_SIZE);
    WRITE_WAV_NO_OFF(WAV_FMT_FORMAT);
    WRITE_WAV_NO_OFF(WAV_FMT_CHANNELS);
    WRITE_WAV_VAL_NO_OFF(WAV_FMT_SAMPLE_RATE, wavSampleRate);
    WRITE_WAV_VAL_NO_OFF(WAV_FMT_BYTE_RATE, wavSampleRate);
    WRITE_WAV_NO_OFF(WAV_FMT_BLOCK_ALIGN);
    WRITE_WAV_NO_OFF(WAV_FMT_BPS);
    WRITE_WAV_NO_OFF(WAV_DATA_ID);
    WRITE_WAV_NO_OFF(WAV_DATA_SIZE);
}

void WavFile::begin()
{
    writeHeader();
}

void WavFile::write(const byte *inBuff, uint8_t buffSize)
{
    if (isObjectClosed) return;
    file.write(inBuff, buffSize);
    dataSize += buffSize;
}

void WavFile::close()
{
    if (isObjectClosed) return;
    updateSizes();
    file.close();
    isObjectClosed = true;
}

void WavFile::updateSizes()
{
    byte wavDataSize[WAV_UINT_SIZE];
    uint32ToBytes(dataSize, wavDataSize);
    WRITE_WAV_VAL(WAV_DATA_SIZE, wavDataSize);

    byte wavSize[WAV_UINT_SIZE];
    uint32ToBytes(dataSize + (WAV_DATA_DATA_OFF - WAV_FORMAT_OFF), wavSize);
    WRITE_WAV_VAL(WAV_SIZE, wavSize);
}

void uint32ToBytes(uint32_t input, byte *output)
{
    output[0] = (byte)(input & 0xFF);
    output[1] = (byte)((input >> 8) & 0xFF);
    output[2] = (byte)((input >> 16) & 0xFF);
    output[3] = (byte)((input >> 24) & 0xFF);
}
