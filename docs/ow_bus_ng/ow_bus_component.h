#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

enum class OneWireBusStatus {
  OK,
  ERROR,
  NO_DEVICES_FOUND,
  DEVICE_DISCONNECTED,
  // ...
};

class OneWireBusComponent : public Component {
 public:
  virtual void setup() override {}
  virtual void dump_config() override {}
  virtual OneWireBusStatus initialize() = 0;
  virtual OneWireBusStatus uninitialize() = 0;
  virtual OneWireBusStatus reset(bool *is_present) = 0;
  virtual OneWireBusStatus read_bit(uint8_t *out) = 0;
  virtual OneWireBusStatus read_byte(uint8_t *out) = 0;
  virtual OneWireBusStatus read_bytes(uint8_t *buffer, unsigned int len) = 0;
  virtual OneWireBusStatus write_bit(uint8_t bit) = 0;
  virtual OneWireBusStatus write_byte(uint8_t data) = 0;
  virtual OneWireBusStatus write_bytes(const uint8_t *buffer, size_t len) = 0;
  virtual OneWireBusStatus search_first(OneWireBus_SearchState *state, bool *found_device) = 0;
  virtual OneWireBusStatus search_next(OneWireBus_SearchState *state, bool *found_device) = 0;
};

class OneWireBusRMT : public OneWireBusComponent {
 public:
  OneWireBusRMT(owb_rmt_driver_info driver_info);
  virtual OneWireBusStatus initialize() override;
  virtual OneWireBusStatus uninitialize() override;
  virtual OneWireBusStatus reset(bool *is_present) override;
  virtual OneWireBusStatus read_bit(uint8_t *out) override;
  virtual OneWireBusStatus read_byte(uint8_t *out) override;
  virtual OneWireBusStatus read_bytes(uint8_t *buffer, unsigned int len) override;
  virtual OneWireBusStatus write_bit(uint8_t bit) override;
  virtual OneWireBusStatus write_byte(uint8_t data) override;
  virtual OneWireBusStatus write_bytes(const uint8_t *buffer, size_t len) override;
  virtual OneWireBusStatus search_first(OneWireBus_SearchState *state, bool *found_device) override;
  virtual OneWireBusStatus search_next(OneWireBus_SearchState *state, bool *found_device) override;
private:
    owb_rmt_driver_info driver_info_;
};

class OneWireBusGPIO : public OneWireBusComponent {
 public:
  OneWireBusGPIO(owb_gpio_driver_info driver_info);
  virtual OneWireBusStatus initialize() override;
  virtual OneWireBusStatus uninitialize() override;
  virtual OneWireBusStatus reset(bool *is_present) override;
  virtual OneWireBusStatus read_bit(uint8_t *out) override;
  virtual OneWireBusStatus read_byte(uint8_t *out) override;
  virtual OneWireBusStatus read_bytes(uint8_t *buffer, unsigned int len) override;
  virtual OneWireBusStatus write_bit(uint8_t bit) override;
  virtual OneWireBusStatus write_byte(uint8_t data) override;
  virtual OneWireBusStatus write_bytes(const uint8_t *buffer, size_t len) override;
  virtual OneWireBusStatus search_first(OneWireBus_SearchState *state, bool *found_device) override;
  virtual OneWireBusStatus search_next(OneWireBus_SearchState *state, bool *found_device) override;
 private:
  owb_gpio_driver_info driver_info_;
};

class OneWireBusDevice {
 public:
  virtual OneWireBusStatus read_scratchpad(uint8_t *buffer, size_t len) = 0;
  virtual OneWireBusStatus write_scratchpad(const uint8_t *buffer, size_t len) = 0;
  virtual OneWireBusStatus copy_scratchpad() = 0;
  virtual OneWireBusStatus recall_eeprom() = 0;
  virtual OneWireBusStatus read_power_supply(bool *is_parasitic) = 0;
};

}  // namespace ow_bus_ng
}  // namespace esphome
