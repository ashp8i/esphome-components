#include "onewire_component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace onewire_bus {

static const char *const TAG = "owbus.ng";

OneWireBusComponent::OneWireBusComponent(OneWireDriverBase& OneWireDriverBase) : OneWireDriverBase_(OneWireDriverBase) {}

void OneWireBusComponent::doSomething() {
  OneWireDriverBase_.init();
  OneWireDriverBase_.read();
  OneWireDriverBase_.write();
  
  // check if the driver is of type DriverB
  if (dynamic_cast<OneWireBitbangDriver*>(&OneWireDriverBase_) != nullptr) {
    OneWireBitbangDriver& OneWireBitbangDriver = dynamic_cast<OneWireBitbangDriver&>(OneWireDriverBase_);
    OneWireBitbangDriver.setupB();
    OneWireBitbangDriver.modeB();
    // do something specific to driver B
  }
  
  // do something common to all drivers
}

void ESPHomeOneWireNGComponent::setup() {

  ESP_LOGCONFIG(TAG, "Setting up ESPHomeOneWireNGComponent...");
  this->pin_->setup();

  // Initialize 1-Wire bus and search for devices
  // ...
}

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Configuration:");
  // Config Output
}

}  // namespace onewire_bus
}  // namespace esphome