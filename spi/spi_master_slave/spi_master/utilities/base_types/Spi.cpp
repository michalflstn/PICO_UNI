#include "Spi.hpp"
#include "pico/stdlib.h"
#include <cstdio>
#include <string>
Spi::Spi( uint databitsval, spi_cpol_t pol_val, spi_cpha_t pha_val, spi_order_t first)
{
  _pol_val= pol_val;
  _pha_val= pha_val;
  _databitsval=databitsval;
  _first=first;
 /* 251022
  stdio_init_all();
  stdio_usb_init();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/spi_master example requires a board with SPI pins
  puts("Default SPI pins were not defined");
#else  // printf("SPI master example\n");
  spi_init(spi_default, 1000 * 1000);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
#endif
 */ 
//  bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
  spi_set_format(spi_default,_databitsval, _pol_val, _pha_val, SPI_MSB_FIRST);//8  251008
}
void Spi::setProperties( uint databitsval, spi_cpol_t pol_val, spi_cpha_t pha_val,spi_order_t first)
{
  spi_set_format(spi_default,databitsval, pol_val, pha_val, first);
}

void Spi::setProperties()
{
  spi_set_format(spi_default,_databitsval,_pol_val,_pha_val,_first);
}

int Spi::write(const uint8_t *buf, size_t length)
{
  return spi_write_blocking(spi_default, buf, length);
}

int Spi::read(const uint8_t *inB, uint8_t *buf, size_t length)
{

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

