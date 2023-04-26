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
  // Get TX pin from UART bus
  this->tx_pin_ = this->uart_->get_tx_pin();
  if (this->uart_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
    // uart_bus_ = new ESPHomeOneWireNGComponent(uart);
    // uart_bus_->begin();
  }
}

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "  Using UART mode:");
  // ESP_LOGCONFIG(TAG, "    UART bus: %s", this->uart_->get_name().c_str());
  ESP_LOGCONFIG(TAG, "    UART bus: %s", this->uart_->get_id().c_str());
}

bool ESPHomeOneWireNGComponent::perform_reset() {
  uart_->pause();  // Temporarily pause UART component

  tx_pin_->set_level(false);
  delay(30);
  tx_pin_->set_level(true);

  uart_->resume();  // Resume UART component
}

}  // namespace ow_bus_ng
}  // namespace esphome
