// #pragma once

// #include "esphome/core/hal.h"
// #include "esphome/core/component.h"
// #include <vector>

// namespace esphome {
// namespace ow_bus_ng {

// enum class OneWireBusPinConfig {
//   ONEWIRE_BUS_PIN_SINGLE,
//   ONEWIRE_BUS_PIN_SPLIT
// };

// class ESPHomeOneWireNGComponent : public Component {
//  public:
//   ESPHomeOneWireNGComponent(OneWireBusPinConfig pin_config, InternalGPIOPin *input_pin, InternalGPIOPin *output_pin,
//                             InternalGPIOPin *pin)
//       : pin_{pin}, input_pin_{input_pin}, output_pin_{output_pin}, split_io_{pin_config} {}
//   virtual ~ESPHomeOneWireNGComponent();
//   void setup() override;
//   void dump_config() override;
//   void update();

//   void set_pin(InternalGPIOPin *pin);
//   void set_in_pin(InternalGPIOPin *in_pin);
//   void set_out_pin(InternalGPIOPin *out_pin);
//   void set_split_io(OneWireBusPinConfig split_io);

//   void issue_reset();
//   void issue_skip_rom();
//   void issue_command(uint8_t);

//  protected:
//   OneWireBusPinConfig split_io_;
//   InternalGPIOPin *pin_{nullptr};
//   InternalGPIOPin *input_pin_{nullptr};
//   InternalGPIOPin *output_pin_{nullptr};

//   void InitOWBus();
// };

#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

class ESPHomeOneWireNGComponent : public Component {
 public:
  ESPHomeOneWireNGComponent() : pin_config_(OneWirePinConfig::SINGLE_PIN) {}

  ESPHomeOneWireNGComponent(InternalGPIOPin *pin) : pin_(pin), pin_config_(OneWirePinConfig::SINGLE_PIN) {}

  ESPHomeOneWireNGComponent(InternalGPIOPin *input_pin, InternalGPIOPin *output_pin)
      : input_pin_(input_pin), output_pin_(output_pin), pin_config_(OneWirePinConfig::SPLIT_IO) {}

  enum OneWirePinConfig { SINGLE_PIN, SPLIT_IO };

 protected:
  InternalGPIOPin *pin_{nullptr};
  InternalGPIOPin *input_pin_{nullptr};
  InternalGPIOPin *output_pin_{nullptr};
  OneWirePinConfig pin_config_;
};

}  // namespace ow_bus_ng
}  // namespace esphome
