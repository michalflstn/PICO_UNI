#ifndef PICO_EXAMPLES_DECODER_HPP
#define PICO_EXAMPLES_DECODER_HPP

#include "io_ports.h"


class Decoder
{
public:
  Decoder(uint8_t a, uint8_t b, uint8_t c);

  void activePort(uint8_t port = 0);

private:
  OutputPort portA;
  OutputPort portB;
  OutputPort portC;
};

#endif
