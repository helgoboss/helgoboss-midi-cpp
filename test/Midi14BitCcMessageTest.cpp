#include <catch.hpp>
#include <helgoboss-midi/Midi14BitCcMessage.h>

using helgoboss::util::couldBePartOf14BitCcMessage;

namespace helgoboss {
  SCENARIO("Empty 14-bit CC messages") {
    GIVEN("Default-constructed or explicitly empty 14-bit CC messages") {
      const Midi14BitCcMessage defaultMsg{};
      const auto emptyMsg = Midi14BitCcMessage::empty();
      THEN("should be empty") {
        REQUIRE(defaultMsg.isEmpty());
        REQUIRE(emptyMsg.isEmpty());
      }
      THEN("should be equal to each other") {
        REQUIRE(defaultMsg == emptyMsg);
      }
    }
  }

  SCENARIO("Filled 14-bit CC messages") {
    GIVEN("Typical 14-bit CC message") {
      const Midi14BitCcMessage msg(5, 2, 1057);
      THEN("should not be empty") {
        REQUIRE(!msg.isEmpty());
      }
      THEN("should return correct values") {
        REQUIRE(msg.getChannel() == 5);
        REQUIRE(msg.getMsbControllerNumber() == 2);
        REQUIRE(msg.getLsbControllerNumber() == 34);
        REQUIRE(msg.getValue() == 1057);
      }
      WHEN("asked to build corresponding MIDI messages") {
        const auto midiMsgs = msg.buildMidiMessages();
        THEN("should return exactly 2 messages") {
          REQUIRE(midiMsgs.size() == 2);
        }
        THEN("should return correct messages") {
          REQUIRE(midiMsgs.at(0) == MidiMessage::controlChange(5, 2, 8));
          REQUIRE(midiMsgs.at(1) == MidiMessage::controlChange(5, 34, 33));
        }
      }
    }
  }

  TEST_CASE("Could be part of 14-bit CC message") {
    REQUIRE(couldBePartOf14BitCcMessage(MidiMessage::controlChange(5, 2, 8)));
    REQUIRE(couldBePartOf14BitCcMessage(MidiMessage::controlChange(5, 34, 33)));
    REQUIRE(!couldBePartOf14BitCcMessage(MidiMessage::controlChange(5, 67, 8)));
  }
}