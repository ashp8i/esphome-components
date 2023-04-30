#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

// using InputPin = InternalGPIOPin;
// using OutputPin = InternalGPIOPin;

class ESPHomeOneWireNGComponent : public Component {
 public:
  ESPHomeOneWireNGComponent();

  ESPHomeOneWireNGComponent(InternalGPIOPin *pin);

  // ESPHomeOneWireNGComponent(InputPin *input_pin, OutputPin *output_pin);

  void setup() override;
  void dump_config() override;
  bool perform_reset();

 protected:
  InternalGPIOPin *pin_{nullptr};
  // InputPin *input_pin_{nullptr};
  // OutputPin *output_pin_{nullptr};
  ESPHomeOneWireNGComponent *bitbang_single_pin_bus_;  // Single pin mode
  // ESPHomeOneWireNGComponent *bitbang_split_io_bus_;    // Split IO mode
};

}  // namespace ow_bus_ng
}  // namespace esphome
