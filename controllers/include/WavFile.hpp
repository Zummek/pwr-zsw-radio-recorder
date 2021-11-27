#ifndef WAVFILE_H
#define WAVFILE_H

#include <SD.h>

#define UINT_SIZE 4

class WavFile
{
private:
    File* file;
    size_t dataSize;
    byte wavSampleRate[UINT_SIZE];
    bool isObjectClosed;

    void writeHeader();
    void updateSizes();
public:
    WavFile(File* fileObj, unsigned int sampleRate);
    void write(const byte* inBuff, size_t buffSize);
    void close();
    virtual ~WavFile();
};

#endif
