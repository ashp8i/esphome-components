#include "ds18b20.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ow_bus_ng {

class DS18B20Component : public Component, public Sensor {
 public:
  void set_bus(OneWireBusComponent *bus) { bus_ = bus; }
  void set_rom_code(OneWireBus_ROMCode rom_code) { rom_code_ = rom_code; }
  void set_resolution(uint8_t resolution) { resolution_ = resolution; }
  void set_update_interval(uint32_t update_interval) { update_interval_ = update_interval; }
  void setup() override;
  void update() override;
 private:
  OneWireBusComponent *bus_;
  OneWireBus_ROMCode rom_code_;
  uint8_t resolution_ = 9;
  uint32_t update_interval_ = 60000;
  unsigned long last_update_ = 0;
};

void DS18B20Component::setup() {
  if (!bus_) {
    ESP_LOGW(TAG, "No OneWireBusComponent specified");
    return;
  }
  bus_->reset(nullptr);
  bus_->search_first(nullptr, nullptr);
  bus_->search_next(nullptr, nullptr);
}

void DS18B20Component::update() {
  if (!bus_) {
    ESP_LOGW(TAG, "No OneWireBusComponent specified");
    return;
  }
  unsigned long current_time = millis();
  if (current_time - last_update_ < update_interval_) {
    return;
  }
  last_update_ = current_time;

  float temperature;
  uint8_t scratchpad[9];
  if (bus_->reset(nullptr) == OWB_STATUS_OK &&
      bus_->select_device(&rom_code_) == OWB_STATUS_OK &&
      bus_->read_scratchpad(&scratchpad[0], 9) == OWB_STATUS_OK) {
    DS18B20_ConvertT(&scratchpad[0], resolution_);
    temperature = DS18B20_ReadT(&scratchpad[0]);
    publish_state(temperature);
  } else {
    ESP_LOGW(TAG, "Failed to read temperature");
  }
}

}  // namespace ds18b20
}  // namespace esphome
