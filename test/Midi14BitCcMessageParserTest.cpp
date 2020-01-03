#include <catch.hpp>
#include <helgoboss-midi/Midi14BitCcMessageParser.h>

namespace helgoboss {
  SCENARIO("14-bit CC message parser") {
    GIVEN("A default-constructed parser") {
      Midi14BitCcMessageParser parser;
      WHEN("fed with a few non-14-bit-contributing MIDI messages") {
        THEN("should return empty 14-bit messages") {
          REQUIRE(parser.feed(MidiMessage::noteOn(0, 100, 100)).isEmpty());
          REQUIRE(parser.feed(MidiMessage::noteOn(0, 100, 120)).isEmpty());
          REQUIRE(parser.feed(MidiMessage::controlChange(0, 80, 1)).isEmpty());
        }
      }
      WHEN("fed with 14-bit-contributing MIDI messages") {
        THEN("should return 14-bit result message on second (LSB) MIDI message") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(5, 2, 8));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(5, 34, 33));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(!resultTwo.isEmpty());
          REQUIRE(resultTwo.getChannel() == 5);
          REQUIRE(resultTwo.getMsbControllerNumber() == 2);
          REQUIRE(resultTwo.getLsbControllerNumber() == 34);
          REQUIRE(resultTwo.getValue() == 1057);
        }
      }
      WHEN("fed with 14-bit-contributing MIDI messages interleaved on separate channels") {
        THEN("should process different channels independently") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(5, 2, 8));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(6, 3, 8));
          const auto resultThree = parser.feed(MidiMessage::controlChange(5, 34, 33));
          const auto resultFour = parser.feed(MidiMessage::controlChange(6, 35, 34));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(resultThree.getChannel() == 5);
          REQUIRE(resultThree.getMsbControllerNumber() == 2);
          REQUIRE(resultThree.getValue() == 1057);
          REQUIRE(resultFour.getChannel() == 6);
          REQUIRE(resultFour.getLsbControllerNumber() == 35);
          REQUIRE(resultFour.getValue() == 1058);
        }
      }
      WHEN("fed with mixed 14-bit-contributing and non-contributing MIDI messages") {
        THEN("should just ignore non-contributing MIDI messages") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(5, 2, 8));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(5, 77, 9));
          const auto resultThree = parser.feed(MidiMessage::controlChange(5, 34, 33));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(resultThree.getChannel() == 5);
          REQUIRE(resultThree.getMsbControllerNumber() == 2);
          REQUIRE(resultThree.getValue() == 1057);
        }
      }
      WHEN("fed with interleaved 14-bit-contributing MIDI messages on one channel") {
        THEN("should only consider last incoming MSB") {
          const auto resultOne = parser.feed(MidiMessage::controlChange(5, 2, 8));
          const auto resultTwo = parser.feed(MidiMessage::controlChange(5, 3, 8));
          const auto resultThree = parser.feed(MidiMessage::controlChange(5, 34, 33));
          const auto resultFour = parser.feed(MidiMessage::controlChange(5, 35, 34));
          REQUIRE(resultOne.isEmpty());
          REQUIRE(resultTwo.isEmpty());
          REQUIRE(resultThree.isEmpty());
          REQUIRE(resultFour.getChannel() == 5);
          REQUIRE(resultFour.getLsbControllerNumber() == 35);
          REQUIRE(resultFour.getValue() == 1058);
        }
      }
    }
  }
}