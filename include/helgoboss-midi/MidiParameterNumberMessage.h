#pragma once

#include <array>
#include "MidiMessage.h"

namespace helgoboss {
  namespace util {
    bool couldBePartOfParameterNumberMessage(const MidiMessage& msg);
  }
  class MidiParameterNumberMessage {
  private:
    int channel_ = -1;
    int number_ = -1;
    int value_ = -1;
    bool isRegistered_ = false;
    bool is14bit_ = false;
  public:
    static constexpr MidiParameterNumberMessage empty() {
      return MidiParameterNumberMessage();
    }
    constexpr MidiParameterNumberMessage() = default;
    MidiParameterNumberMessage(int channel, int number, int value, bool isRegistered, bool is14bit);
    bool isEmpty() const;
    int getChannel() const;
    int getNumber() const;
    int getValue() const;
    bool is14bit() const;
    bool isRegistered() const;
    // If not 14-bit, this returns only 3 messages (the last one is empty)
    std::array<MidiMessage, 4> buildMidiMessages() const;
    friend bool operator==(const MidiParameterNumberMessage& lhs, const MidiParameterNumberMessage& rhs);
    friend bool operator!=(const MidiParameterNumberMessage& lhs, const MidiParameterNumberMessage& rhs);
  };
}
