#include <helgoboss-midi/Midi14BitCcMessageParser.h>
#include "midi-util.h"

namespace helgoboss {
  Midi14BitCcMessage Midi14BitCcMessageParser::feed(const MidiMessage& msg) {
    return parserByChannel_[msg.getChannel()].feed(msg);
  }
  void Midi14BitCcMessageParser::reset() {
    for (auto& i : parserByChannel_) {
      i.reset();
    }
  }
  Midi14BitCcMessage Midi14BitCcMessageParser::ParserForOneChannel::feed(const MidiMessage& msg) {
    if (msg.getType() != MidiMessageType::ControlChange) {
      return Midi14BitCcMessage::empty();
    }
    const int controllerNumber = msg.getControllerNumber();
    if (controllerNumber < 32) {
      return processValueMsb(controllerNumber, msg.getControlValue());
    } else if (controllerNumber < 64) {
      return processValueLsb(msg.getChannel(), controllerNumber, msg.getControlValue());
    } else {
      return Midi14BitCcMessage::empty();
    }
  }
  void Midi14BitCcMessageParser::ParserForOneChannel::reset() {
    msbControllerNumber_ = -1;
    valueMsb_ = -1;
  }
  Midi14BitCcMessage Midi14BitCcMessageParser::ParserForOneChannel::processValueMsb(
      int msbControllerNumber, int valueMsb) {
    msbControllerNumber_ = msbControllerNumber;
    valueMsb_ = valueMsb;
    return Midi14BitCcMessage::empty();
  }
  Midi14BitCcMessage Midi14BitCcMessageParser::ParserForOneChannel::processValueLsb(
      int channel, int lsbControllerNumber, int valueLsb) {
    if (msbControllerNumber_ != -1 && lsbControllerNumber == msbControllerNumber_ + 32) {
      const int value = util::build14BitValueFromTwoBytes(
          static_cast<unsigned char>(valueMsb_),
          static_cast<unsigned char>(valueLsb)
      );
      return Midi14BitCcMessage(channel, msbControllerNumber_, value);
    } else {
      return Midi14BitCcMessage::empty();
    }
  }
}