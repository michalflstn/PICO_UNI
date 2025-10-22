#pragma once
#ifndef PICO_EXAMPLES_SPI_HPP
#define PICO_EXAMPLES_SPI_HPP

#include "hardware/spi.h"
#include "hardware/dma.h"
#include "pico/stdlib.h"

class Spi
{
private:
 spi_cpol_t _pol_val;
 spi_cpha_t _pha_val;  
 uint8_t _databitsval;
 spi_order_t   _first;
public:
  //static uint dma_chan; 
 // static const uint CS_PIN = 1; // Chip Select pin
  Spi(uint databitsval, spi_cpol_t pol_val, spi_cpha_t pha_val,spi_order_t  first);

  void setProperties();

  void setProperties(uint databitsval, spi_cpol_t pol_val, spi_cpha_t pha_val,spi_order_t first);

  static int  write(const uint8_t *, size_t length);

  static int  read(const uint8_t *inB, uint8_t *buf, size_t length);
 
 // static void spi_read_dma(uint32_t dma_chan, spi_inst_t* spi, uint32_t cs_pin, uint16_t* rx_buffer, const uint16_t* dummy_bytes, size_t length);
};


#endif
