#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>
#include "one_wire_driver_base.h"
#include "one_wire_bitbang_driver.h"
#include "one_wire_rmt_driver.h"

namespace esphome {
namespace onewire_bus {

static const char *const TAG = "onewire.bus";

class OneWireBusComponent : public esphome::Component {
 public:
  OneWireBusComponent();
  void setup() override;
  void dump_config() override;
  void update() override;
  void loop() override;
  void OneWireBitbangDriver::init() {
    // implementation for initializing driver A
  }
  void OneWireBitbangDriver::read() {
    // implementation for reading from driver A
  }
  void OneWireBitbangDriver::write() {
    // implementation for writing to driver A
  }
  void OneWireBitbangDriver::setupA() {
    // implementation for setting up driver A setup method A
  }
  void OneWireBitbangDriver::setupB() {
    // implementation for setting up driver B setup method B
  }
  void OneWireBitbangDriver::modeA() {
    // implementation for driver A mode A
  }
  void OneWireBitbangDriver::modeB() {
    // implementation for driver A mode B
  }
  void OneWireRMTDriver::init() {
    // implementation for initializing driver B
  }
  void OneWireRMTDriver::read() {
    // implementation for reading from driver B
  }
  void OneWireRMTDriver::write() {
    // implementation for writing to driver B
  }
  void OneWireRMTDriver::setupA() {
    // implementation for setting up driver B setup method A
  }
  void OneWireRMTDriver::setupB() {
    // implementation for setting up driver B setup method B
  }
  void OneWireRMTDriver::modeA() {
    // implementation for driver B mode A
  }
  void OneWireRMTDriver::modeB() {
    // implementation for driver B mode B
  }

 protected:
  InternalGPIOPin *pin_{nullptr};

};

}  // namespace onewire_bus
}  // namespace esphome