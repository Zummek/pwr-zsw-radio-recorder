#ifndef WAVFILE_H
#define WAVFILE_H

#include <Arduino.h>
#include <SdFat.h>

#define WAV_UINT_SIZE 4

class WavFile
{
private:
    File* file;
    uint32_t dataSize;
    byte wavSampleRate[WAV_UINT_SIZE];

    void writeHeader();
    void updateSizes();
public:
    WavFile();
    WavFile(File& fileObj, uint16_t sampleRate);
    void begin();
    bool isOpened();
    void write(const byte* inBuff, uint8_t buffSize);
    void close();
};

#endif
