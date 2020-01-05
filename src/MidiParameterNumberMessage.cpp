#include <helgoboss-midi/MidiParameterNumberMessage.h>
#include "midi-util.h"
#include <gsl/gsl>

using std::array;

namespace helgoboss {
  namespace util {
    bool couldBePartOfParameterNumberMessage(const MidiMessage& msg) {
      if (msg.getType() != MidiMessageType::ControlChange) {
        return false;
      }
      switch (msg.getControllerNumber()) {
        case 98:
        case 99:
        case 100:
        case 101:
        case 38:
        case 6:return true;
        default:return false;
      }
    }
  }
  MidiParameterNumberMessage::MidiParameterNumberMessage(
      int channel,
      int number,
      int value,
      bool isRegistered,
      bool is14bit
  ) : channel_(channel),
      number_(number),
      value_(value),
      isRegistered_(isRegistered),
      is14bit_(is14bit) {
    Expects(channel < 16);
    Expects(number < 16384);
    Expects((is14bit && value < 16384) || (!is14bit && value < 128));
  }
  bool MidiParameterNumberMessage::isEmpty() const {
    return channel_ == -1;
  }
  int MidiParameterNumberMessage::getChannel() const {
    return channel_;
  }
  int MidiParameterNumberMessage::getNumber() const {
    return number_;
  }
  int MidiParameterNumberMessage::getValue() const {
    return value_;
  }
  bool MidiParameterNumberMessage::is14bit() const {
    return is14bit_;
  }
  bool MidiParameterNumberMessage::isRegistered() const {
    return isRegistered_;
  }
  std::array<MidiMessage, 4> MidiParameterNumberMessage::buildMidiMessages() const {
    array<MidiMessage, 4> messages;
    int i = 0;
    // Number MSB
    messages[i] = MidiMessage::controlChange(
        channel_,
        isRegistered_ ? 101 : 99,
        util::extractHighByteFrom14BitValue(number_)
    );
    i++;
    // Number LSB
    messages[i] = MidiMessage::controlChange(
        channel_,
        isRegistered_ ? 100 : 98,
        util::extractLowByteFrom14BitValue(number_)
    );
    i++;
    // Value LSB
    if (is14bit_) {
      messages[i] = MidiMessage::controlChange(
          channel_,
          38,
          util::extractLowByteFrom14BitValue(value_)
      );
      i++;
    }
    // Value MSB
    messages[i] = MidiMessage::controlChange(
        channel_,
        6,
        is14bit_ ? util::extractHighByteFrom14BitValue(value_) : value_
    );
    i++;
    // Add empty values if needed
    for (; i < 4; i++) {
      messages[i] = MidiMessage::empty();
    }
    return messages;
  }
  bool operator==(const MidiParameterNumberMessage& lhs, const MidiParameterNumberMessage& rhs) {
    return lhs.channel_ == rhs.channel_ &&
        lhs.number_ == rhs.number_ &&
        lhs.value_ == rhs.value_ &&
        lhs.isRegistered_ == rhs.isRegistered_ &&
        lhs.is14bit_ == rhs.is14bit_;
  }
  bool operator!=(const MidiParameterNumberMessage& lhs, const MidiParameterNumberMessage& rhs) {
    return !(rhs == lhs);
  }
}