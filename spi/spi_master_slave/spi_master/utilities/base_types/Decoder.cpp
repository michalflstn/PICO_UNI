#include "Decoder.hpp"
#include <bitset>

Decoder::Decoder(uint8_t a, uint8_t b, uint8_t c) : portA(OutputPort(a)), portB(OutputPort(b)), portC(OutputPort(c))
{}

void Decoder::activePort(uint8_t port)
{
 /* std::string binary = std::bitset<3>(port).to_string();
  binary[0] == '1' ? portC.enable() : portC.disable();
  binary[1] == '1' ? portB.enable() : portB.disable();
  binary[2] == '1' ? portA.enable() : portA.disable();
  */
 std::bitset<3> binary = std::bitset<3>(port);//8
  binary[0] == 1 ? portC.enable() : portC.disable();
  binary[1] == 1 ? portB.enable() : portB.disable();
  binary[2] == 1 ? portA.enable() : portA.disable();
}

