#pragma once

#include "Midi14BitCcMessage.h"

namespace helgoboss {

  class Midi14BitCcMessageParser {
  private:
    class ParserForOneChannel {
    private:
      int msbControllerNumber_ = -1;
      int valueMsb_ = -1;
    public:
      Midi14BitCcMessage feed(const MidiMessage& msg);
      void reset();
    private:
      Midi14BitCcMessage processValueMsb(int msbControllerNumber, int valueMsb);
      Midi14BitCcMessage processValueLsb(int channel, int lsbControllerNumber, int valueLsb);
    };

    ParserForOneChannel parserByChannel_[32];
  public:
    Midi14BitCcMessage feed(const MidiMessage& msg);
    void reset();
  };
}


