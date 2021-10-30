#ifndef I2CBUS_h
#define I2CBUS_h

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
