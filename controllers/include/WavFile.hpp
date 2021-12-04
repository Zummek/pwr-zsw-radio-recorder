#ifndef WAVFILE_H
#define WAVFILE_H

#include <Arduino.h>
#include <SD.h>

#define WAV_UINT_SIZE 4

class WavFile
{
private:
    File file;
    size_t dataSize;
    byte wavSampleRate[WAV_UINT_SIZE];
    bool isObjectClosed;

    void writeHeader();
    void updateSizes();
public:
    WavFile(File fileObj, uint16_t sampleRate);
    void write(const byte* inBuff, size_t buffSize);
    void close();
    virtual ~WavFile();
};

#endif
