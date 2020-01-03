#pragma once

#include <array>
#include "MidiMessage.h"

namespace helgoboss {
  namespace util {
    bool couldBePartOf14BitCcMessage(const MidiMessage& msg);
  }

  class Midi14BitCcMessage {
  private:
    int channel_ = -1;
    int msbControllerNumber_ = -1;
    int value_ = -1;
  public:
    static constexpr Midi14BitCcMessage empty() {
      return Midi14BitCcMessage();
    }
    constexpr Midi14BitCcMessage() = default;
    Midi14BitCcMessage(int channel, int msbControllerNumber, int value);
    bool isEmpty() const;
    int getChannel() const;
    int getMsbControllerNumber() const;
    int getLsbControllerNumber() const;
    int getValue() const;
    std::array<MidiMessage, 2> buildMidiMessages() const;
    friend bool operator==(const Midi14BitCcMessage& lhs, const Midi14BitCcMessage& rhs);
    friend bool operator!=(const Midi14BitCcMessage& lhs, const Midi14BitCcMessage& rhs);
  };
}


