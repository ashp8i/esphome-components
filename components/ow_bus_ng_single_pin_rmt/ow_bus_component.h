// #pragma once

// #include "esphome/core/hal.h"
// #include "esphome/core/component.h"
// #include <vector>

// namespace esphome {
// namespace ow_bus_ng {

// // using InputPin = InternalGPIOPin;
// // using OutputPin = InternalGPIOPin;

// class ESPHomeOneWireNGComponent : public Component {
//  public:
//   ESPHomeOneWireNGComponent();

//   ESPHomeOneWireNGComponent(InternalGPIOPin *pin);

//   // ESPHomeOneWireNGComponent(InputPin *input_pin, OutputPin *output_pin);

//   void setup() override;
//   void dump_config() override;
//   bool perform_reset();

//  protected:
//   InternalGPIOPin *pin_{nullptr};
//   // InputPin *input_pin_{nullptr};
//   // OutputPin *output_pin_{nullptr};
//   ESPHomeOneWireNGComponent *bitbang_single_pin_bus_;  // Single pin mode
//   // ESPHomeOneWireNGComponent *bitbang_split_io_bus_;    // Split IO mode
// };

// }  // namespace ow_bus_ng
// }  // namespace esphome

// #pragma once

// #include "esphome/core/hal.h"
// #include "esphome/core/component.h"
// #include <vector>

// namespace esphome {
// namespace ow_bus_ng {

// class OneWireRMT {
//   // Low-level RMT methods
// }

// class OneWireBus {
//   // OneWire bus methods like reset, read bit, write bit, etc.  
// }

// class OneWireDevice {
//   // Methods for interacting with OneWire devices/ROMs
//   uint8_t family_code;
//   std::vector<uint8_t> rom_code;
  
//   void set_rom_code(const std::vector<uint8_t> &rom);
//   std::vector<uint8_t> get_rom_code() const;
// };

// class ESPHomeOneWireNGComponent : public Component {
//  public:
//   OneWireRMTRMT rmt;
//   OneWireBus bus; 
//   std::vector<OneWireDevice> devices;
  
//   ESPHomeOneWireNGComponent();
//   ESPHomeOneWireNGComponent(InternalGPIOPin *pin);
  
//   void setup() override;
//   void dump_config() override;
  
//   // Methods to expose OneWire functionality 
// };

// }  // namespace ow_bus_ng
// }  // namespace esphome

#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

class ESPHomeOneWireNGComponent : public Component {
 public:
  ESPHomeOneWireNGComponent();

  ESPHomeOneWireNGComponent(InternalGPIOPin *pin);

  void setup() override;
  void dump_config() override;
  bool reset();
  uint8_t crc8(const uint8_t *rom, int len);
  void target_setup(const uint8_t family_code);
  bool next(uint8_t *rom, int mode);
  void select(const uint8_t *rom);
  void skip();
  void reset_search();
  uint64_t search()
  std::vector<uint64_t> search_vec();
  bool read_bit();
  uint8_t read();
  void read_bytes(uint8_t *buf, int len);
  void write_bit(int bit);
  void write(const uint8_t data);
  void write_bytes(const uint8_t *buf, int len);

 protected:
  InternalGPIOPin *pin_{nullptr}; 
  uint8_t last_discrepancy_{0};
  bool last_device_flag_{false};
  uint64_t rom_number_{0};
  /// Helper to get the internal 64-bit unsigned rom number as a 8-bit integer pointer.
  inline uint8_t *rom_number8_();
};

}  // namespace ow_bus_ng
}  // namespace esphome
