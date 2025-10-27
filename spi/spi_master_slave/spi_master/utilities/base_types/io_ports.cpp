#include "io_ports.h"

InputPort::InputPort(uint8_t port) : port_(port)
{
  gpio_init(port);
  gpio_set_dir(port, GPIO_IN);
}

bool InputPort::getState() const
{
  return gpio_get(port_);
}

uint8_t InputPort::getPort()
{
  return port_;
}


OutputPort::OutputPort(uint8_t port) : port_(port)
{
  gpio_init(port);
  gpio_set_dir(port, GPIO_OUT);
}

void OutputPort::enable()
{
  gpio_put(port_, true);
}

void OutputPort::disable()
{
  gpio_put(port_, false);
}

void OutputPort::blink()
{
  gpio_put(port_, true);
  sleep_us(10);
  gpio_put(port_, false);
}

uint8_t OutputPort::getPort() const
{
  return port_;
}