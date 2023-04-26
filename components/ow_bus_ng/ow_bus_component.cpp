#include "ow_bus_component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ow_bus_ng {

static const char *const TAG = "owbus.ng";

// Constructor definitions here
ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent() {}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(uart::UARTComponent *uart) {
  this->uart_ = uart;
}

void ESPHomeOneWireNGComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up UART mode...");
  if (this->uart_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
    // uart_bus_ = new ESPHomeOneWireNGComponent(uart);
    // uart_bus_->begin();
  }
}

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "  Using UART mode:");
  ESP_LOGCONFIG(TAG, "    UART bus: %s", this->uart_->get_name().c_str());
}

bool ESPHomeOneWireNGComponent::perform_reset() {
  if (this->uart_ != nullptr) {
    this->uart_->enable_tx_pin();
    this->uart_->set_tx_pin_level(false);
    delay(30);
    this->uart_->disable_tx_pin();
  }
  return true;
}

}  // namespace ow_bus_ng
}  // namespace esphome
