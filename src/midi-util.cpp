#include "midi-util.h"
#include <gsl/gsl>

namespace helgoboss::util {
  unsigned char extractLowNibbleFromByte(unsigned char byte) {
    return static_cast<unsigned char>(byte & 0x0f);
  }
  unsigned char extractHighNibbleFromByte(unsigned char byte) {
    return static_cast<unsigned char>((byte >> 4) & 0x0f);
  }
  unsigned char withLowNibbleAdded(unsigned char byte, unsigned char lowNibble) {
    Expects(lowNibble <= 0xf);
    return byte | lowNibble;
  }
  unsigned char buildByteFromNibbles(unsigned char highNibble, unsigned char lowNibble) {
    Expects(highNibble <= 0xf);
    Expects(lowNibble <= 0xf);
    return (highNibble << 4) | lowNibble;
  }
  unsigned char extractLowByteFrom14BitValue(int value) {
    Expects(value < 16384);
    return static_cast<unsigned char>(value & 0x7f);
  }
  unsigned char extractHighByteFrom14BitValue(int value) {
    Expects(value < 16384);
    return static_cast<unsigned char>((value >> 7) & 0x7f);
  }
  int build14BitValueFromTwoBytes(unsigned char highByte, unsigned char lowByte) {
    Expects(highByte <= 0x7f);
    Expects(lowByte <= 0x7f);
    return (highByte << 7) | lowByte;
  }
}