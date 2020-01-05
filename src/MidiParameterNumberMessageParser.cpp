#include <helgoboss-midi/MidiParameterNumberMessageParser.h>
#include "midi-util.h"

namespace helgoboss {
  MidiParameterNumberMessage MidiParameterNumberMessageParser::feed(const MidiMessage& msg) {
    return parserByChannel_[msg.getChannel()].feed(msg);
  }
  void MidiParameterNumberMessageParser::reset() {
    for (auto& i : parserByChannel_) {
      i.reset();
    }
  }
  MidiParameterNumberMessage MidiParameterNumberMessageParser::ParserForOneChannel::feed(const MidiMessage& msg) {
    if (msg.getType() != MidiMessageType::ControlChange) {
      return MidiParameterNumberMessage::empty();
    }
    switch (msg.getControllerNumber()) {
      case 98: return processNumberLsb(msg.getDataByte2(), false);
      case 99: return processNumberMsb(msg.getDataByte2(), false);
      case 100: return processNumberLsb(msg.getDataByte2(), true);
      case 101: return processNumberMsb(msg.getDataByte2(), true);
      case 38: return processValueLsb(msg.getDataByte2());
      case 6: return processValueMsb(msg.getChannel(), msg.getDataByte2());
      default: return MidiParameterNumberMessage::empty();
    }
  }
  void MidiParameterNumberMessageParser::ParserForOneChannel::reset() {
    numberMsb_ = -1;
    numberLsb_ = -1;
    isRegistered_ = false;
    resetValue();
  }
  MidiParameterNumberMessage MidiParameterNumberMessageParser::ParserForOneChannel::processNumberLsb(
      int numberLsb, bool isRegistered) {
    resetValue();
    numberLsb_ = numberLsb;
    isRegistered_ = isRegistered;
    return MidiParameterNumberMessage::empty();
  }
  MidiParameterNumberMessage MidiParameterNumberMessageParser::ParserForOneChannel::processNumberMsb(
      int numberMsb, bool isRegistered) {
    resetValue();
    numberMsb_ = numberMsb;
    isRegistered_ = isRegistered;
    return MidiParameterNumberMessage::empty();
  }
  MidiParameterNumberMessage MidiParameterNumberMessageParser::ParserForOneChannel::processValueLsb(int valueLsb) {
    valueLsb_ = valueLsb;
    return MidiParameterNumberMessage::empty();
  }
  MidiParameterNumberMessage MidiParameterNumberMessageParser::ParserForOneChannel::processValueMsb(
      int channel, int valueMsb) {
    if (numberLsb_ == -1 || numberMsb_ == -1) {
      return MidiParameterNumberMessage::empty();
    }
    const int number = util::build14BitValueFromTwoBytes(
        static_cast<unsigned char>(numberMsb_),
        static_cast<unsigned char>(numberLsb_)
    );
    const int value = valueLsb_ == -1 ? valueMsb : util::build14BitValueFromTwoBytes(
        static_cast<unsigned char>(valueMsb),
        static_cast<unsigned char>(valueLsb_)
    );
    return MidiParameterNumberMessage(channel, number, value, isRegistered_, valueLsb_ != -1);
  }
  void MidiParameterNumberMessageParser::ParserForOneChannel::resetValue() {
    valueLsb_ = -1;
  }
}