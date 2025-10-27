#include "Spi.hpp"
#include "pico/stdlib.h"
#include <cstdio>
#include <string>
#include "../../utilities/base_types/decoder.hpp"
#include "../../loop/common_data/device_variables.hpp"

Spi::Spi(uint8_t port, uint8_t databitsval, spi_cpol_t pol_val, spi_cpha_t pha_val, spi_order_t first)
{
  _port=port;
  _pol_val= pol_val;
  _pha_val= pha_val;
  _databitsval=databitsval;
  _first=first;
//  bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, 
// spi_set_format(spi_default,_databitsval, _pol_val, _pha_val,_first);// SPI_MSB_FIRST);//8  251008
}
 void Spi::SetSpiPinCS_N(int8_t level)
{
  gpio_put(PICO_DEFAULT_SPI_CSN_PIN, level);
}
 
void Spi::Activate()
{
  decoder.activePort(_port);
  spi_set_format(spi_default,_databitsval,_pol_val,_pha_val,_first);
}
void Spi::deActivate()
{
  decoder.activePort(port_None);
}
int Spi::write(const uint8_t *buf, size_t length)
{
  return spi_write_blocking(spi_default, buf, length);
}

int Spi::read(const uint8_t *inB, uint8_t *buf, size_t length)
{
  while ( !spi_is_readable(spi_default)) //?????   251022
  { 
   tight_loop_contents();
  }
  if (spi_is_readable(spi_default))
  {
    spi_write_read_blocking(spi_default, inB, buf, length);
  }
    return length;
}

// Read SPI using DMA on demand
/*
void Spi::spi_read_dma(uint32_t dma_chan, spi_inst_t* spi, uint32_t cs_pin, uint16_t* rx_buffer, const uint16_t* dummy_bytes, size_t length) {
     
 // uint16_t dummy_bytes[length] = {0}; // Dummy bytes to generate clock   gpio_put(cs_pin, 0); // Select device
    // Configure DMA transfer for 'length' bytes
    gpio_put(cs_pin, 0); // Select device
    dma_channel_set_read_addr(dma_chan, &spi_get_hw(spi)->dr, false);
    dma_channel_set_write_addr(dma_chan, rx_buffer, false);
    dma_channel_set_trans_count(dma_chan, length, false);
    dma_channel_start(dma_chan);
    // Send dummy bytes to generate SPI clock and read data
  hardware->conv->disable();
  sleep_us(10);
  conv->enable();

    spi_write16_blocking(spi, dummy_bytes, length);
    dma_channel_wait_for_finish_blocking(dma_chan);
    gpio_put(cs_pin, 1); // Deselect device
    // rx_buffer now holds received data
  std::string afc; 
   afc.clear();
  afc = "code"+std::to_string(16)+" SPI read DMA "+std::to_string(rx_buffer[0]);
  afc +="\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}
*/

