#include "ow_bus_component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace ow_bus_ng {

static const char *const TAG = "owbus.ng";

// Constructor definitions here
ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent() {}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(InternalGPIOPin *pin) {
  this->pin_ = pin;
  // Set bitbang single pin mode
}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(InputPin *input_pin, OutputPin *output_pin) {
  this->input_pin_ = input_pin;
  this->output_pin_ = output_pin;
  // Set bitbang split IO mode
}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(UARTComponent *uart) {
  this->uart_ = uart;
  // Set UART mode
}

void ESPHomeOneWireNGComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ESPHomeOneWireNGComponent...");
  if (this->pin_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
    single_pin_bus_ = new OneWire(this->pin_->get_pin());
  }

  if (this->input_pin_ != nullptr && this->output_pin_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
    split_io_bus_ = new OneWire(this->input_pin_->get_pin(), this->output_pin_->get_pin());
  }

  if (this->uart_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
    uart_bus_ = new OneWire(uart);
    uart_bus_->begin();
  }
}

  // if (this->uart_ != nullptr) this->uart_->begin();     // Initialize UART

  // if (!perform_reset()) return;  // Check for any connected devices

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "ESPHomeOneWireNGComponent:");

  if (this->pin_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Using bitbang single pin mode:");
    ESP_LOGCONFIG(TAG, "    Pin: %d", this->pin_->get_pin());
  } else if (this->input_pin_ != nullptr && this->output_pin_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Using bitbang split IO mode:");
    ESP_LOGCONFIG(TAG, "    Input pin: %d", this->input_pin_->get_pin());
    ESP_LOGCONFIG(TAG, "    Output pin: %d", this->output_pin_->get_pin());
  } else if (this->uart_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Using UART mode:");
    ESP_LOGCONFIG(TAG, "    UART bus: %s", this->uart_->get_name().c_str());
  }
}

bool ESPHomeOneWireNGComponent::perform_reset() {
  if (this->pin_ != nullptr) {  // Single pin mode
    this->pin_->digital_write(LOW);
    delayMicroseconds(480);
    this->pin_->digital_write(HIGH);
    delayMicroseconds(70);
  } else if (this->input_pin_ != nullptr && this->output_pin_ != nullptr) {
    // Split I/O - set output low
    this->output_pin_->digital_write(LOW);
    delayMicroseconds(480);
    // Set output high
    this->output_pin_->digital_write(HIGH);
    // Check input pin
    return this->input_pin_->digital_read() == LOW;
  }

  if (this->uart_ != nullptr) {
    this->uart_->transmit_break();      // Send 1-Wire reset pulse
    while (this->uart_->peek() == 0) { /* wait */ }
    return true;                       // Presence pulse detected!
  }

  return false;
}

  // For UART just call uart_->transmit_break()
  return false;  // If no pin defined
}

}  // namespace ow_bus_ng
}  // namespace esphome
