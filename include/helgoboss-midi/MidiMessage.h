#pragma once

namespace helgoboss {
  // DONE-rust
  // The most low-level type of a MIDI message
  enum class MidiMessageType : unsigned char {
    // Channel messages = channel voice messages + channel mode messages (given value represents channel 0 status byte)
        NoteOff = 0x80,
    NoteOn = 0x90,
    PolyphonicKeyPressure = 0xa0,
    ControlChange = 0xb0,
    ProgramChange = 0xc0,
    ChannelPressure = 0xd0,
    PitchBendChange = 0xe0,
    // System exclusive messages
        SystemExclusiveStart = 0xf0,
    // System common messages
        MidiTimeCodeQuarterFrame = 0xf1,
    SongPositionPointer = 0xf2,
    SongSelect = 0xf3,
    TuneRequest = 0xf6,
    SystemExclusiveEnd = 0xf7,
    // System real-time messages (given value represents the complete status byte)
        TimingClock = 0xf8,
    Start = 0xfa,
    Continue = 0xfb,
    Stop = 0xfc,
    ActiveSensing = 0xfe,
    SystemReset = 0xff
  };

  // A somewhat mid-level type of a MIDI message
  enum class MidiMessageSuperType {
    // In this enum we don't distinguish between channel voice and channel mode messages because this difference
    // doesn't solely depend on the MidiMessageType (channel mode messages are just particular ControlChange messages).
        Channel,
    SystemCommon,
    SystemRealTime,
    SystemExclusive
  };

  // At the highest level MIDI messages are put into two categories
  enum class MidiMessageMainCategory {
    Channel,
    System
  };

  namespace util {
    MidiMessageSuperType getMidiMessageSuperType(MidiMessageType type);
    MidiMessageMainCategory getMidiMessageMainCategory(MidiMessageSuperType superType);
  }

  class MidiMessage {
  private:
    // DONE-rust
    unsigned char statusByte_ = 0;
    unsigned char dataByte1_ = 0;
    unsigned char dataByte2_ = 0;
  public:
    // DONE-rust
    static constexpr MidiMessage empty() {
      return MidiMessage();
    }
    static MidiMessage noteOn(int channel, int keyNumber, int velocity);
    static MidiMessage noteOff(int channel, int keyNumber, int velocity);
    static MidiMessage controlChange(int channel, int controllerNumber, int controlValue);
    static MidiMessage programChange(int channel, int programNumber);
    static MidiMessage polyphonicKeyPressure(int channel, int keyNumber, int pressureAmount);
    static MidiMessage channelPressure(int channel, int pressureAmount);
    static MidiMessage pitchBendChange(int channel, int pitchBendValue);
    static MidiMessage timingClock();
    static MidiMessage start();
    static MidiMessage continueMessage();
    static MidiMessage stop();
    static MidiMessage activeSensing();
    static MidiMessage systemReset();

    constexpr MidiMessage() = default;
    // DONE-rust
    MidiMessage(unsigned char statusByte, unsigned char dataByte1, unsigned char dataByte2);
    // DONE-rust
    bool isEmpty() const;
    // DONE-rust
    unsigned char getStatusByte() const;
    // DONE-rust
    unsigned char getDataByte1() const;
    // DONE-rust
    unsigned char getDataByte2() const;
    // DONE-rust
    MidiMessageType getType() const;
    MidiMessageSuperType getSuperType() const;
    MidiMessageMainCategory getMainCategory() const;
    int getChannel() const;
    // Returns false if the message type is NoteOn but the velocity is 0
    // DONE-rust
    bool isNoteOn() const;
    // Also returns true if the message type is NoteOn but the velocity is 0
    bool isNoteOff() const;
    bool isNote() const;
    int getKeyNumber() const;
    // DONE-rust
    int getVelocity() const;
    int getControllerNumber() const;
    int getControlValue() const;
    int getProgramNumber() const;
    int getPressureAmount() const;
    int getPitchBendValue() const;
    friend bool operator==(const MidiMessage& lhs, const MidiMessage& rhs);
    friend bool operator!=(const MidiMessage& lhs, const MidiMessage& rhs);
  private:
    static MidiMessage createChannelMessage(MidiMessageType type, int channel, int data1, int data2);
    static MidiMessage createSystemRealTimeMessage(MidiMessageType type);
  };
}


