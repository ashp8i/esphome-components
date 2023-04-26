#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

using InputPin = esphome::gpio::GPIOInputPin;
using OutputPin = esphome::gpio::GPIOOutputPin;

enum OneWireSetupMethod {
  ONEWIRE_SETUP_BITBANG_SINGLE_PIN,
  ONEWIRE_SETUP_BITBANG_SPLIT_IO,
  //  ONEWIRE_SETUP_HARDWARE_UART_HALF_DUPLEX, // No such thing, not supported by UART Component
  ONEWIRE_SETUP_UART_FULL_DUPLEX,
};

class ESPHomeOneWireNGComponent : public esphome::Component {
 public:
  ESPHomeOneWireNGComponent();

  ESPHomeOneWireNGComponent(std::initializer_list<OneWireSetupMethod> setup_methods);

  ESPHomeOneWireNGComponent(InternalGPIOPin *pin);

  ESPHomeOneWireNGComponent(InputPin *input_pin, OutputPin *output_pin);

  // ESPHomeOneWireNGComponent(UARTComponent *uart, GPIOPin *tx_pin); // not yet supported, or ever

  ESPHomeOneWireNGComponent(UARTComponent *uart, GPIOPin *rx_pin, GPIOPin *tx_pin);

  void set_bitbang_single_pin(InternalGPIOPin *pin);

  void set_bitbang_split_io(InternalGPIOPin *input_pin, InternalGPIOPin *output_pin);

  // void set_uart_half_duplex(UARTComponent *uart, GPIOPin *tx_pin);

  void set_uart_full_duplex(UARTComponent *uart, GPIOPin *rx_pin, GPIOPin *tx_pin);

  void setup() override;
  void dump_config() override;

 protected:
  InternalGPIOPin *pin_{nullptr};
  InternalGPIOPin *input_pin_{nullptr};
  InternalGPIOPin *output_pin_{nullptr};
  UARTComponent *uart_{nullptr};
  GPIOPin *rx_pin_{nullptr};
  GPIOPin *tx_pin_{nullptr};
  std::vector<OneWireSetupMethod> setup_methods_;
};

}  // namespace ow_bus_ng
}  // namespace esphome
