#pragma once

#include "MidiParameterNumberMessage.h"

namespace helgoboss {

  class MidiParameterNumberMessageParser {
  private:
    class ParserForOneChannel {
    private:
      int numberMsb_ = -1;
      int numberLsb_ = -1;
      bool isRegistered_ = false;
      int valueLsb_ = -1;
    public:
      MidiParameterNumberMessage feed(const MidiMessage& msg);
      void reset();
    private:
      MidiParameterNumberMessage processNumberLsb(int numberLsb, bool isRegistered);
      MidiParameterNumberMessage processNumberMsb(int numberMsb, bool isRegistered);
      MidiParameterNumberMessage processValueLsb(int valueLsb);
      MidiParameterNumberMessage processValueMsb(int channel, int valueMsb);
      void resetValue();
    };

    ParserForOneChannel parserByChannel_[32];
  public:
    MidiParameterNumberMessage feed(const MidiMessage& msg);
    void reset();
  };
}


