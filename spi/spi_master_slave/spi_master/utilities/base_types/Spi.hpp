#ifndef PICO_EXAMPLES_SPI_HPP
#define PICO_EXAMPLES_SPI_HPP

#include "hardware/spi.h"

#define PDSPI                 PICO_DEFAULT_SPI 
#define PDSPI_SCK_PIN         2
#define PDSPI_TX_PIN          3
#define PDSPI_RX_PIN          0
#define PDSPI_CSN_PIN         1

class Spi
{
public:
  Spi();

  static void setProperties(int, int, int, int first = static_cast<spi_order_t>(1));

  static int write(const uint8_t *, size_t length);

  static int read(const uint8_t *inB, uint8_t *buf, size_t length);
};


#endif
