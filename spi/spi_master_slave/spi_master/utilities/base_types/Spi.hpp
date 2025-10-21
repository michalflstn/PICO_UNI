#pragma once
#ifndef PICO_EXAMPLES_SPI_HPP
#define PICO_EXAMPLES_SPI_HPP

#include "hardware/spi.h"
#include "hardware/dma.h"
#include "pico/stdlib.h"

class Spi
{
public:
  //static uint dma_chan; 
 // static const uint CS_PIN = 1; // Chip Select pin
  Spi();

  static void setProperties(int, int, int, int first = static_cast<spi_order_t>(1));

  static int  write(const uint8_t *, size_t length);

  static int  read(const uint8_t *inB, uint8_t *buf, size_t length);
 
 // static void spi_read_dma(uint32_t dma_chan, spi_inst_t* spi, uint32_t cs_pin, uint16_t* rx_buffer, const uint16_t* dummy_bytes, size_t length);
};


#endif
