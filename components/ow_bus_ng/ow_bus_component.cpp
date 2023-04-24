#include "ow_bus_component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ow_bus_ng {

static const char *const TAG = "owbus.ng";

void ESPHomeOneWireNGComponent::set_bitbang_single_pin(InternalGPIOPin *pin) {
  this->pin_ = pin;
  this->setup_methods_.push_back(ONEWIRE_SETUP_BITBANG_SINGLE_PIN);
}

void ESPHomeOneWireNGComponent::set_bitbang_split_io(InternalGPIOPin *input_pin, InternalGPIOPin *output_pin) {
  this->input_pin_ = rx_pin;
  this->output_pin_ = tx_pin;
  this->setup_methods_.push_back(ONEWIRE_SETUP_BITBANG_SPLIT_IO);
}

// void ESPHomeOneWireNGComponent::set_uart_half_duplex(UARTComponent *uart, GPIOPin *rx_pin, GPIOPin *tx_pin) {
//   this->uart_ = uart;
//   this->tx_pin_ = tx_pin;
//   this->setup_methods_.push_back(ONEWIRE_SETUP_UART_HALF_DUPLEX);
// }

void ESPHomeOneWireNGComponent::set_modbus_half_duplex(ModbusComponent *modbus, GPIOPin *tx_pin) {
  this->modbus_ = modbus;
  this->tx_pin_ = tx_pin;
  this->setup_methods_.push_back(ONEWIRE_SETUP_MODBUS_HALF_DUPLEX);
}

void ESPHomeOneWireNGComponent::set_uart_full_duplex(UARTComponent *uart, GPIOPin *rx_pin, GPIOPin *tx_pin) {
  this->uart_ = uart;
  this->rx_pin_ = rx_pin;
  this->tx_pin_ = tx_pin;
  this->setup_methods_.push_back(ONEWIRE_SETUP_UART_FULL_DUPLEX);
}

// Constructor definitions here
ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent() : pin_config_(OneWirePinConfig::SINGLE_PIN) {}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(InternalGPIOPin *pin)
    : pin_(pin), pin_config_(OneWirePinConfig::SINGLE_PIN) {}

void ESPHomeOneWireNGComponent::setup() {
  Component::setup();  // Call parent class setup()

  ESP_LOGCONFIG(TAG, "Setting up ESPHomeOneWireNGComponent...");

  for (OneWireSetupMethod method : this->setup_methods_) {
    switch (method) {
      case ONEWIRE_SETUP_BITBANG_SINGLE_PIN:
        if (this->pin_ != nullptr) {
          this->pin_->setup();
        }
        break;
      case ONEWIRE_SETUP_BITBANG_SPLIT_IO:
        if (this->input_pin_ != nullptr && this->output_pin_ != nullptr) {
          this->input_pin_->setup();
          this->output_pin_->setup();
        }
        break;
      case ONEWIRE_SETUP_MODBUS_HALF_DUPLEX:
        if (this->modbus_ != nullptr && this->tx_pin_ != nullptr) {
          this->modbus_->setup();
          this->tx_pin_->setup();
        }
        break;
      case ONEWIRE_SETUP_UART_FULL_DUPLEX:
        if (this->uart_ != nullptr && this->rx_pin_ != nullptr && this->tx_pin_ != nullptr) {
          this->uart_->setup();
          this->rx_pin_->setup();
          this->tx_pin_->setup();
        }
        break;
    }
  }

  // Initialize 1-Wire bus and search for devices
  // ...
}

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Configuration:");

  for (OneWireSetupMethod method : this->setup_methods_) {
    switch (method) {
      case ONEWIRE_SETUP_BITBANG_SINGLE_PIN:
        ESP_LOGCONFIG(TAG, "  bitbang_single_pin");
        if (this->pin_ != nullptr) {
          ESP_LOGCONFIG(TAG, "  pin: %d", this->pin_->get_pin());
        }
        break;
      case ONEWIRE_SETUP_BITBANG_SPLIT_IO:
        ESP_LOGCONFIG(TAG, "  bitbang_split_io");
        if (this->input_pin_ != nullptr && this->output_pin_ != nullptr) {
          ESP_LOGCONFIG(TAG, "  input_pin: %d", this->input_pin_->get_pin());
          ESP_LOGCONFIG(TAG, "  output_pin: %d", this->output_pin_->get_pin());
        }
        break;
      case ONEWIRE_SETUP_UART_FULL_DUPLEX:
        ESP_LOGCONFIG(TAG, "  uart_full_duplex");
        if (this->uart_ != nullptr && this->rx_pin_ != nullptr && this->tx_pin_ != nullptr) {
          ESP_LOGCONFIG(TAG, "  uart: %s", this->uart_->get_name().c_str());
          ESP_LOGCONFIG(TAG, "  rx_pin: %d", this->rx_pin_->get_pin());
          ESP_LOGCONFIG(TAG, "  tx_pin: %d", this->tx_pin_->get_pin());
        }
        break;
      case ONEWIRE_SETUP_MODBUS_HALF_DUPLEX:
        ESP_LOGCONFIG(TAG, "  modbus_half_duplex");
        if (this->modbus_ != nullptr && this->tx_pin_ != nullptr) {
          ESP_LOGCONFIG(TAG, "  modbus: %s", this->modbus_->get_name().c_str());
          ESP_LOGCONFIG(TAG, "  tx_pin: %d", this->tx_pin_->get_pin());
        }
        break;
    }
  }
}

}  // namespace ow_bus_ng
}  // namespace esphome
