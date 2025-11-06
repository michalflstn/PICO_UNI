#pragma once
#ifndef PICO_EXAMPLES_IO_PORTS_H
#define PICO_EXAMPLES_IO_PORTS_H

#include "pico/stdlib.h"


class InputPort
{
public:
  explicit InputPort(uint8_t port);

  bool getLevel() const;

  uint8_t getPort();

private:
  uint8_t port_;
};

class OutputPort
{
public:
  explicit OutputPort(uint8_t port);

  void enable();

  void disable();

  void blink();

  uint8_t getPort() const;

private:
  uint8_t port_;
};


#endif
