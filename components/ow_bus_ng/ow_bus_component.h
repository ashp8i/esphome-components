#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>
// #include <cstdstring>

namespace esphome {
namespace ow_bus_ng {

class ESPHomeOneWireNGComponent : public esphome::Component {
 public:
  ESPHomeOneWireNGComponent() : pin_config_(OneWirePinConfig::SINGLE_PIN) {}

  ESPHomeOneWireNGComponent(InternalGPIOPin *pin) : pin_(pin), pin_config_(OneWirePinConfig::SINGLE_PIN) {}

  void setup() override;

  void dump_config() override;
  enum OneWirePinConfig { SINGLE_PIN, SPLIT_IO };
  void set_single_pin(InternalGPIOPin *pin);
  void set_split_io(InternalGPIOPin *input_pin, InternalGPIOPin *output_pin);

 protected:
  InternalGPIOPin *pin_{nullptr};
  InternalGPIOPin *input_pin_{nullptr};
  InternalGPIOPin *output_pin_{nullptr};
  OneWirePinConfig pin_config_;
};

};  // namespace ow_bus_ng
};  // namespace esphome
