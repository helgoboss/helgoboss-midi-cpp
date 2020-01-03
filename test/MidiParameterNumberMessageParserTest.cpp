#include <catch.hpp>
#include <helgoboss-midi/MidiParameterNumberMessageParser.h>

namespace helgoboss {
  SCENARIO("Parameter number message parser") {
    GIVEN("A default-constructed parser") {
      MidiParameterNumberMessageParser parser;
      WHEN("fed with a few non-contributing MIDI messages") {
        THEN("should return empty parameter number messages") {
          REQUIRE(parser.feed(MidiMessage::noteOn(0, 100, 100)).isEmpty());
          REQUIRE(parser.feed(MidiMessage::noteOn(0, 100, 120)).isEmpty());
          REQUIRE(parser.feed(MidiMessage::controlChange(0, 80, 1)).isEmpty());
        }
      }
      WHEN("fed with 14-bit-registered-contributing MIDI messages") {
        THEN("should return parameter number result message on fourth MIDI message") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(0, 101, 3));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(0, 100, 36));
          const auto resultThree = parser.feed(MidiMessage::controlChange(0, 38, 24));
          const auto resultFour = parser.feed(MidiMessage::controlChange(0, 6, 117));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(resultThree.isEmpty());
          REQUIRE(!resultFour.isEmpty());
          REQUIRE(resultFour.getChannel() == 0);
          REQUIRE(resultFour.getNumber() == 420);
          REQUIRE(resultFour.getValue() == 15000);
          REQUIRE(resultFour.isRegistered());
          REQUIRE(resultFour.is14bit());
        }
      }
      WHEN("fed with 7-bit-non-registered-contributing MIDI messages") {
        THEN("should return parameter number result message on third MIDI message") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(2, 99, 3));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(2, 98, 37));
          const auto resultThree = parser.feed(MidiMessage::controlChange(2, 6, 126));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(!resultThree.isEmpty());
          REQUIRE(resultThree.getChannel() == 2);
          REQUIRE(resultThree.getNumber() == 421);
          REQUIRE(resultThree.getValue() == 126);
          REQUIRE(!resultThree.isRegistered());
          REQUIRE(!resultThree.is14bit());
        }
      }
      WHEN("fed with contributing MIDI messages interleaved on separate channels") {
        THEN("should process different channels independently") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(0, 101, 3));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(2, 99, 3));
          const auto resultThree = parser.feed(MidiMessage::controlChange(0, 100, 36));
          const auto resultFour = parser.feed(MidiMessage::controlChange(2, 98, 37));
          const auto resultFive = parser.feed(MidiMessage::controlChange(0, 38, 24));
          const auto resultSix = parser.feed(MidiMessage::controlChange(2, 6, 126));
          const auto resultSeven = parser.feed(MidiMessage::controlChange(0, 6, 117));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultThree.isEmpty());
          REQUIRE(resultFive.isEmpty());
          REQUIRE(!resultSeven.isEmpty());
          REQUIRE(resultSeven.getChannel() == 0);
          REQUIRE(resultSeven.getNumber() == 420);
          REQUIRE(resultSeven.getValue() == 15000);
          REQUIRE(resultSeven.isRegistered());
          REQUIRE(resultSeven.is14bit());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(resultFour.isEmpty());
          REQUIRE(!resultSix.isEmpty());
          REQUIRE(resultSix.getChannel() == 2);
          REQUIRE(resultSix.getNumber() == 421);
          REQUIRE(resultSix.getValue() == 126);
          REQUIRE(!resultSix.isRegistered());
          REQUIRE(!resultSix.is14bit());
        }
      }
      WHEN("fed with mixed contributing and non-contributing MIDI messages") {
        THEN("should just ignore non-contributing MIDI messages") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(2, 99, 3));
          parser.feed(MidiMessage::controlChange(2, 34, 5));
          parser.feed(MidiMessage::noteOn(2, 100, 105));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(2, 98, 37));
          parser.feed(MidiMessage::controlChange(2, 50, 6));
          const auto resultThree = parser.feed(MidiMessage::controlChange(2, 6, 126));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(!resultThree.isEmpty());
          REQUIRE(resultThree.getChannel() == 2);
          REQUIRE(resultThree.getNumber() == 421);
          REQUIRE(resultThree.getValue() == 126);
          REQUIRE(!resultThree.isRegistered());
          REQUIRE(!resultThree.is14bit());
        }
      }
    }
  }
}