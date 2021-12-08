#ifndef I2CBUS_H
#define I2CBUS_H

#define I2C_MAX_ARG_SIZE 32

namespace I2CCommands
{
    enum class audioController
    {
        startRecording,
        cancelRecording,
        stopRecording,
    };
};

enum class I2CAddresses
{
    audioController = 0x02,
};

#endif
