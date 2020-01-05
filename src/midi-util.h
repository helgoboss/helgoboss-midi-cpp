#pragma once

namespace helgoboss::util {
  unsigned char extractLowNibbleFromByte(unsigned char byte);
  unsigned char extractHighNibbleFromByte(unsigned char byte);
  unsigned char withLowNibbleAdded(unsigned char byte, unsigned char lowNibble);
  unsigned char buildByteFromNibbles(unsigned char highNibble, unsigned char lowNibble);
  unsigned char extractLowByteFrom14BitValue(int value);
  unsigned char extractHighByteFrom14BitValue(int value);
  int build14BitValueFromTwoBytes(unsigned char highByte, unsigned char lowByte);
}