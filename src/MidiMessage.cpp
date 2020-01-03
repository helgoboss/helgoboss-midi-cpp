#include <helgoboss-midi/MidiMessage.h>
#include <helgoboss-midi/midi-util.h>
#include <gsl/gsl>

namespace helgoboss {
  MidiMessageSuperType util::getMidiMessageSuperType(MidiMessageType type) {
    switch (type) {
      case MidiMessageType::NoteOn:
      case MidiMessageType::NoteOff:
      case MidiMessageType::ChannelPressure:
      case MidiMessageType::PolyphonicKeyPressure:
      case MidiMessageType::PitchBendChange:
      case MidiMessageType::ProgramChange:
      case MidiMessageType::ControlChange: {
        return MidiMessageSuperType::Channel;
      }
      case MidiMessageType::TimingClock:
      case MidiMessageType::Start:
      case MidiMessageType::Continue:
      case MidiMessageType::Stop:
      case MidiMessageType::ActiveSensing:
      case MidiMessageType::SystemReset: {
        return MidiMessageSuperType::SystemRealTime;
      }
      case MidiMessageType::MidiTimeCodeQuarterFrame:
      case MidiMessageType::SongPositionPointer:
      case MidiMessageType::SongSelect:
      case MidiMessageType::TuneRequest:
      case MidiMessageType::SystemExclusiveEnd: {
        return MidiMessageSuperType::SystemCommon;
      }
      case MidiMessageType::SystemExclusiveStart: {
        return MidiMessageSuperType::SystemExclusive;
      }
      default:Expects(false);
    }
  }
  MidiMessageMainCategory util::getMidiMessageMainCategory(MidiMessageSuperType superType) {
    if (superType == MidiMessageSuperType::Channel) {
      return MidiMessageMainCategory::Channel;
    } else {
      return MidiMessageMainCategory::System;
    }
  }
  MidiMessage MidiMessage::timingClock() {
    return createSystemRealTimeMessage(MidiMessageType::TimingClock);
  }
  MidiMessage MidiMessage::start() {
    return createSystemRealTimeMessage(MidiMessageType::Start);
  }
  MidiMessage MidiMessage::continueMessage() {
    return createSystemRealTimeMessage(MidiMessageType::Continue);
  }
  MidiMessage MidiMessage::stop() {
    return createSystemRealTimeMessage(MidiMessageType::Stop);
  }
  MidiMessage MidiMessage::activeSensing() {
    return createSystemRealTimeMessage(MidiMessageType::ActiveSensing);
  }
  MidiMessage MidiMessage::systemReset() {
    return createSystemRealTimeMessage(MidiMessageType::SystemReset);
  }
  MidiMessage MidiMessage::noteOn(int channel, int keyNumber, int velocity) {
    return createChannelMessage(MidiMessageType::NoteOn, channel, keyNumber, velocity);
  }
  MidiMessage MidiMessage::noteOff(int channel, int keyNumber, int velocity) {
    return createChannelMessage(MidiMessageType::NoteOff, channel, keyNumber, velocity);
  }
  MidiMessage MidiMessage::controlChange(int channel, int controllerNumber, int controlValue) {
    return createChannelMessage(MidiMessageType::ControlChange, channel, controllerNumber, controlValue);
  }
  MidiMessage MidiMessage::programChange(int channel, int programNumber) {
    return createChannelMessage(MidiMessageType::ProgramChange, channel, programNumber, 0);
  }
  MidiMessage MidiMessage::polyphonicKeyPressure(int channel, int keyNumber, int pressureAmount) {
    return createChannelMessage(MidiMessageType::PolyphonicKeyPressure, channel, keyNumber, pressureAmount);
  }
  MidiMessage MidiMessage::channelPressure(int channel, int pressureAmount) {
    return createChannelMessage(MidiMessageType::ChannelPressure, channel, pressureAmount, 0);
  }
  MidiMessage MidiMessage::pitchBendChange(int channel, int pitchBendValue) {
    return createChannelMessage(MidiMessageType::PitchBendChange, channel, pitchBendValue & 0x7f, pitchBendValue >> 7);
  }
  MidiMessage MidiMessage::createChannelMessage(MidiMessageType type, int channel, int data1, int data2) {
    Expects(util::getMidiMessageMainCategory(util::getMidiMessageSuperType(type)) == MidiMessageMainCategory::Channel);
    Expects(channel < 16);
    Expects(data1 < 128);
    Expects(data2 < 128);
    return MidiMessage(
        util::withLowNibbleAdded(
            static_cast<unsigned char>(type),
            static_cast<unsigned char>(channel)
        ),
        static_cast<unsigned char>(data1),
        static_cast<unsigned char>(data2)
    );
  }
  MidiMessage MidiMessage::createSystemRealTimeMessage(MidiMessageType type) {
    Expects(util::getMidiMessageSuperType(type) == MidiMessageSuperType::SystemRealTime);
    return MidiMessage(static_cast<unsigned char>(type), 0, 0);
  }
  MidiMessage::MidiMessage(unsigned char statusByte, unsigned char dataByte1, unsigned char dataByte2)
      : statusByte_(statusByte), dataByte1_(dataByte1), dataByte2_(dataByte2) {
  }
  MidiMessageSuperType MidiMessage::getSuperType() const {
    return util::getMidiMessageSuperType(getType());
  }
  MidiMessageMainCategory MidiMessage::getMainCategory() const {
    return util::getMidiMessageMainCategory(getSuperType());
  }
  bool MidiMessage::isEmpty() const {
    return statusByte_ == 0;
  }
  unsigned char MidiMessage::getStatusByte() const {
    return statusByte_;
  }
  unsigned char MidiMessage::getDataByte1() const {
    return dataByte1_;
  }
  unsigned char MidiMessage::getDataByte2() const {
    return dataByte2_;
  }
  MidiMessageType MidiMessage::getType() const {
    const auto highStatusByteNibble = util::extractHighNibbleFromByte(statusByte_);
    if (highStatusByteNibble == 0xf) {
      // System message. The complete status byte makes up the type.
      return static_cast<MidiMessageType>(statusByte_);
    } else {
      // Channel message. Just the high nibble of the status byte makes up the type (low nibble encodes channel)
      return static_cast<MidiMessageType>(util::buildByteFromNibbles(highStatusByteNibble, 0));
    }
  }
  bool MidiMessage::isNoteOn() const {
    return getType() == MidiMessageType::NoteOn && getVelocity() > 0;
  }
  bool MidiMessage::isNoteOff() const {
    const auto type = getType();
    return type == MidiMessageType::NoteOff || (type == MidiMessageType::NoteOn && getVelocity() == 0);
  }
  bool MidiMessage::isNote() const {
    const auto type = getType();
    return type == MidiMessageType::NoteOn || type == MidiMessageType::NoteOff;
  }
  int MidiMessage::getKeyNumber() const {
    return dataByte1_;
  }
  int MidiMessage::getVelocity() const {
    return dataByte2_;
  }
  int MidiMessage::getChannel() const {
    return util::extractLowNibbleFromByte(statusByte_);
  }
  int MidiMessage::getControllerNumber() const {
    return dataByte1_;
  }
  int MidiMessage::getControlValue() const {
    return dataByte2_;
  }
  int MidiMessage::getProgramNumber() const {
    return dataByte1_;
  }
  int MidiMessage::getPressureAmount() const {
    if (getType() == MidiMessageType::PolyphonicKeyPressure) {
      return dataByte2_;
    } else {
      return dataByte1_;
    }
  }
  int MidiMessage::getPitchBendValue() const {
    return util::build14BitValueFromTwoBytes(dataByte2_, dataByte1_);
  }
  bool operator==(const MidiMessage& lhs, const MidiMessage& rhs) {
    return lhs.statusByte_ == rhs.statusByte_ &&
        lhs.dataByte1_ == rhs.dataByte1_ &&
        lhs.dataByte2_ == rhs.dataByte2_;
  }
  bool operator!=(const MidiMessage& lhs, const MidiMessage& rhs) {
    return !(rhs == lhs);
  }
}