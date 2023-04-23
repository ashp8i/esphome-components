#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

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

class ESPHomeOneWireNGComponent : public Component {
 public:
  OneWire() : OneWire(nullptr, OneWire::SINGLE_PIN) {}  // Default to single pin

  OneWire(InternalGPIOPin *pin) : OneWire(pin, OneWire::SINGLE_PIN) {}

  OneWire(InternalGPIOPin *input_pin, InternalGPIOPin *output_pin)
      : pin_config_(OneWire::SPLIT_IO), input_pin_(input_pin), output_pin_(output_pin) {}

  void setup() override {
    switch (this->pin_config_) {
      case OneWire::SINGLE_PIN:
        if (this->pin_ != nullptr) {
          this->pin_->setup();
          // Initialize bus and search for devices
        }
        break;
      case OneWire::SPLIT_IO:
        this->input_pin_->setup();
        this->output_pin_->setup();
        // Initialize bus and search for devices
        break;
    }
  }

  enum PinConfig { SINGLE_PIN, SPLIT_IO };

 private:
  PinConfig pin_config_;
  InternalGPIOPin *pin_;         // For SINGLE_PIN config
  InternalGPIOPin *input_pin_;   // For SPLIT_IO config
  InternalGPIOPin *output_pin_;  // For SPLIT_IO config
};

}  // namespace ow_bus_ng
}  // namespace esphome
