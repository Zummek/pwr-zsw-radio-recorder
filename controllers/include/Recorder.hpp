#ifndef RECORDER_H
#define RECORDER_H

#include <SdFat.h>

namespace Recorder
{
    void setInputPin(uint8_t pin);
    void setSampleRate(uint16_t sampleRate);
    bool startRecording(File& outputFile);
    void stopRecording();
}

#endif