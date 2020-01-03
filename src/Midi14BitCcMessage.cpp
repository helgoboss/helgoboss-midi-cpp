#include <helgoboss-midi/Midi14BitCcMessage.h>
#include <helgoboss-midi/midi-util.h>
#include <gsl/gsl>

using std::array;

namespace helgoboss {
  namespace util {
    bool couldBePartOf14BitCcMessage(const MidiMessage& msg) {
      return msg.getType() == MidiMessageType::ControlChange && msg.getControllerNumber() < 64;
    }
  }
  Midi14BitCcMessage::Midi14BitCcMessage(int channel, int msbControllerNumber, int value)
      : channel_(channel), msbControllerNumber_(msbControllerNumber), value_(value) {
    Expects(channel < 16);
    Expects(msbControllerNumber < 128);
    Expects(value < 16384);
  }
  bool Midi14BitCcMessage::isEmpty() const {
    return value_ == -1;
  }
  int Midi14BitCcMessage::getChannel() const {
    return channel_;
  }
  int Midi14BitCcMessage::getMsbControllerNumber() const {
    return msbControllerNumber_;
  }
  int Midi14BitCcMessage::getLsbControllerNumber() const {
    return msbControllerNumber_ + 32;
  }
  int Midi14BitCcMessage::getValue() const {
    return value_;
  }
  std::array<MidiMessage, 2> Midi14BitCcMessage::buildMidiMessages() const {
    return array<MidiMessage, 2>{
        MidiMessage::controlChange(channel_, getMsbControllerNumber(), util::extractHighByteFrom14BitValue(value_)),
        MidiMessage::controlChange(channel_, getLsbControllerNumber(), util::extractLowByteFrom14BitValue(value_))
    };
  }
  bool operator==(const Midi14BitCcMessage& lhs, const Midi14BitCcMessage& rhs) {
    return lhs.channel_ == rhs.channel_ &&
        lhs.msbControllerNumber_ == rhs.msbControllerNumber_ &&
        lhs.value_ == rhs.value_;
  }
  bool operator!=(const Midi14BitCcMessage& lhs, const Midi14BitCcMessage& rhs) {
    return !(rhs == lhs);
  }
}