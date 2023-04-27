#include "ow_bus_component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ow_bus_ng {

static const char *const TAG = "owbus.ng";

// Constructor definitions here
ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent() {}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(InternalGPIOPin *pin) {
  if (pin->get_pin_mode() != OUTPUT_OPEN_DRAIN) {
    ESP_LOGE(TAG, "1-Wire pin %d must be in open-drain mode!", pin->get_pin());
    return;
  }
  this->pin_ = pin;
}

void ESPHomeOneWireNGComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ESPHomeOneWireNGComponent...");
  ESP_LOGCONFIG(TAG, "Setting up bitbang single pin mode...");
  if (this->pin_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
    single_pin_bus_ = new ESPHomeOneWireNGComponent(this->pin_->get_pin());
  }
}

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "  Using bitbang single pin mode:");
  ESP_LOGCONFIG(TAG, "    Pin: %d", this->pin_->get_pin());
}

bool ESPHomeOneWireNGComponent::perform_reset() {
  if (this->pin_ != nullptr) {
    this->pin_->digital_write(LOW);
    delayMicroseconds(480);
    this->pin_->digital_write(HIGH);
    delayMicroseconds(70);
  }
}

}  // namespace ow_bus_ng
}  // namespace esphome
