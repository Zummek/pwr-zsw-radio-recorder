#ifndef I2CBUS_H
#define I2CBUS_H

namespace WireCommands
{
  enum audioController
  {
    startRecording,
    stopRecording,
  };
};

enum WireAddresses
{
  audioController = 0x02,
};

#endif
