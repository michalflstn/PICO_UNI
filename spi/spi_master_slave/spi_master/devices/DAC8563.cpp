//#include <cstring> //240503
#include "DAC8563.hpp"
#include "../utilities/base_types/Spi.hpp"
#include "../loop/common_data/device_variables.hpp"

DAC8563::DAC8563(int modeA1B1)
{
  mode = modeA1B1;
}

void DAC8563::DAC_WR_REG(uint8_t cmd_byte, uint16_t data_byte)
{
  Spi::setProperties(8, 0, 1);
  uint8_t array[] = {cmd_byte, (uint8_t) (data_byte >> 8), (uint8_t) ((data_byte << 8) >> 8)};
  decoder.activePort(port_);
  spi_write_blocking(spi_default, array, 3);
  decoder.activePort(7);
}

void DAC8563::outPutValue(uint8_t cmd_byte, uint16_t input)
{
  uint8_t inputMid = ((input << 8) >> 8);
  uint8_t inputLast = (input >> 8);
//  unsigned int t = (input >> 8) & 0xFF;
  writeValue(cmd_byte, (inputLast), (inputMid));
}

void DAC8563::writeVoltage(int input)
{
  writeA(input);
  writeB(input);
}

void DAC8563::writeA(int input)
{
  outPutValue(CMD_SETA_UPDATEA, input);
}

void DAC8563::writeB(int input)
{
  outPutValue(CMD_SETB_UPDATEB, input);
}

void DAC8563::writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last)
{
  uint8_t array[] = {cmd_byte, mid, last};
  setSpiProps();
  spi_write_blocking(spi_default, array, 3);
  decoder.activePort(7);
}

void DAC8563::initialize(int port)  //DAC
{
  port_ = port;
  DAC_WR_REG(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);     // reset
  DAC_WR_REG(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);           // power up
  DAC_WR_REG(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN); // enable internal reference
  if (mode == 1)
  {
    DAC_WR_REG(CMD_GAIN, DATA_GAIN_B2_A2);
  }
  else
  if (mode == 2)
  {
    DAC_WR_REG(CMD_GAIN, DATA_GAIN_B1_A1);          // set multiplier TODO DAC 1 [-10..0]V
   // set multiplier TODO DAC 1 [-10..0]V
  }
  DAC_WR_REG(CMD_LDAC_DIS, DATA_LDAC_DIS);          // update the caches
}

void DAC8563::setSpiProps()
{
  decoder.activePort(port_);
  Spi::setProperties(8, 0, 1);
}
