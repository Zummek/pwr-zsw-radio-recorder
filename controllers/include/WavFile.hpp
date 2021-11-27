#ifndef WAVFILE_H
#define WAVFILE_H

#include <SD.h>

class WavFile
{
private:

public:
    WavFile(SDLib::File& fileObj);
    void write(const byte* inBuff, int buffLen);
    void close();
    virtual ~WavFile();
};

#endif
