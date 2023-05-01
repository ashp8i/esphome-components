#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ds18b20 {

class DS18B20 : public OneWireBusDevice {
 public:
  DS18B20(OneWireBusComponent *bus, OneWireBus_ROMCode rom_code);
  virtual owb_status read_scratchpad(uint8_t *buffer, size_t len) override;
  virtual owb_status write_scratchpad(const uint8_t *buffer, size_t len) override;
  virtual owb_status copy_scratchpad() override;
  virtual owb_status recall_eeprom() override;
  virtual owb_status read_power_supply(bool *is_parasitic) override;
 private:
  OneWireBusComponent *bus_;
  OneWireBus_ROMCode rom_code_;
};

}  // namespace ds18b20
}  // namespace esphome
