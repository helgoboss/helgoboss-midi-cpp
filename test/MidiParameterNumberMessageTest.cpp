#include <catch.hpp>
#include <helgoboss-midi/MidiParameterNumberMessage.h>

using helgoboss::util::couldBePartOfParameterNumberMessage;

namespace helgoboss {
  SCENARIO("Empty parameter number messages") {
    GIVEN("Default-constructed or explicitly empty parameter number messages") {
      const MidiParameterNumberMessage defaultMsg{};
      const auto emptyMsg = MidiParameterNumberMessage::empty();
      THEN("should be empty") {
        REQUIRE(defaultMsg.isEmpty());
        REQUIRE(emptyMsg.isEmpty());
      }
      THEN("should be equal to each other") {
        REQUIRE(defaultMsg == emptyMsg);
      }
    }
  }

  SCENARIO("14-bit parameter number messages") {
    GIVEN("Registered 14-bit parameter number message") {
      const MidiParameterNumberMessage msg(0, 420, 15000, true, true);
      THEN("should not be empty") {
        REQUIRE_FALSE(msg.isEmpty());
      }
      THEN("should return correct values") {
        REQUIRE(msg.getChannel() == 0);
        REQUIRE(msg.getNumber() == 420);
        REQUIRE(msg.getValue() == 15000);
        REQUIRE(msg.is14bit());
        REQUIRE(msg.isRegistered());
      }
      WHEN("asked to build corresponding MIDI messages") {
        const auto midiMsgs = msg.buildMidiMessages();
        THEN("should return exactly 4 messages") {
          REQUIRE(midiMsgs.size() == 4);
        }
        THEN("should return correct messages") {
          REQUIRE(midiMsgs.at(0) == MidiMessage::controlChange(0, 101, 3));
          REQUIRE(midiMsgs.at(1) == MidiMessage::controlChange(0, 100, 36));
          REQUIRE(midiMsgs.at(2) == MidiMessage::controlChange(0, 38, 24));
          REQUIRE(midiMsgs.at(3) == MidiMessage::controlChange(0, 6, 117));
        }
      }
    }
  }

  SCENARIO("7-bit parameter number messages") {
    WHEN("trying to construct a 7-bit message with a too high value") {
      THEN("should complain") {
        REQUIRE_THROWS(MidiParameterNumberMessage(0, 420, 15000, false, false));
      }
    }
    GIVEN("Non-registered 7-bit parameter number message") {
      const MidiParameterNumberMessage msg(2, 421, 126, false, false);
      THEN("should not be empty") {
        REQUIRE(!msg.isEmpty());
      }
      THEN("should return correct values") {
        REQUIRE(msg.getChannel() == 2);
        REQUIRE(msg.getNumber() == 421);
        REQUIRE(msg.getValue() == 126);
        REQUIRE(!msg.is14bit());
        REQUIRE(!msg.isRegistered());
      }
      WHEN("asked to build corresponding MIDI messages") {
        const auto midiMsgs = msg.buildMidiMessages();
        THEN("should return exactly 4 messages") {
          REQUIRE(midiMsgs.size() == 4);
        }
        THEN("should return correct messages") {
          REQUIRE(midiMsgs.at(0) == MidiMessage::controlChange(2, 99, 3));
          REQUIRE(midiMsgs.at(1) == MidiMessage::controlChange(2, 98, 37));
          REQUIRE(midiMsgs.at(2) == MidiMessage::controlChange(2, 6, 126));
          REQUIRE(midiMsgs.at(3) == MidiMessage::empty());
        }
      }
    }
  }

  TEST_CASE("Could be part of parameter message") {
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(2, 99, 3)));
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(2, 98, 37)));
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(2, 6, 126)));
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 101, 3)));
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 100, 36)));
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 38, 24)));
    REQUIRE(couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 6, 117)));
    REQUIRE(!couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 5, 117)));
    REQUIRE(!couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 39, 117)));
    REQUIRE(!couldBePartOfParameterNumberMessage(MidiMessage::controlChange(0, 77, 2)));
  }
}