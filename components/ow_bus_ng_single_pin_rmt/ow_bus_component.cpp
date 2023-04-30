#include "ow_bus_component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ow_bus_ng {

static const char *const TAG = "owbus.ng";

// Constructor definitions here
ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent() {}

ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(InternalGPIOPin *pin) {}

void ESPHomeOneWireNGComponent::setup() { 
  ESP_LOGCONFIG(TAG, "Setting up ESPHomeOneWireNGComponent...");
  ESP_LOGCONFIG(TAG, "Setting up Single Pin RMT/Bit Bang Mode...");
  if (this->pin_ != nullptr) {
  // Create owb_rmt_driver_info struct 
  owb_rmt_driver_info info;
  
  // Get RMT channels and GPIO from config
  gpio_num_t gpio = this->get_pin()->get_pin(); 
  rmt_channel_t tx_channel = get_tx_channel_config(); 
  rmt_channel_t rx_channel = get_rx_channel_config();
  
  // Initialize the bus
  OneWireBus *bus = owb_rmt_initialize(&info, gpio, tx_channel, rx_channel);
  
  // Scan for devices
  this->scan(bus); 
}

void scan(OneWireBus *bus) {
  // Call onewire_device_reset() and check for devices...
}

void ESPHomeOneWireNGComponent::setup() { 
  ESP_LOGCONFIG(TAG, "Setting up ESPHomeOneWireNGComponent...");
  ESP_LOGCONFIG(TAG, "Setting up Single Pin RMT/Bit Bang Mode...");
  if (this->pin_ != nullptr) {
    if (!perform_reset())
      return;  // No device present
      this->configure_rmt_(); 
  }
}

void RemoteTransmitterComponent::configure_rmt_() {
  rmt_config_t c{};

  this->config_rmt(c);
  c.rmt_mode = RMT_MODE_TX;
  c.gpio_num = gpio_num_t(this->pin_->get_pin());

  c.rx_config.enable = false;  // Disable RMT RX
  c.tx_config.loop_en = false;
  c.tx_config.carrier_en = true; 
  c.tx_config.idle_output_en = true;
  
  c.tx_config.carrier_freq_hz = 38000;  // 1-Wire frequency 
  c.tx_config.carrier_duty_percent = 50; 
   
  c.clk_div = 80; 
   
  c.tx_config.idle_output_en = true;
  c.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
  c.tx_config.idle_level = RMT_IDLE_LEVEL_LOW; 

  esp_err_t error = rmt_config(&c);
  if (error != ESP_OK) {
    this->error_code_ = error;
    this->mark_failed();
    return;
  }
  
  error = rmt_driver_install(this->channel_, &c.tx_config, 0); 
                               // Pass TX config only
  if (error != ESP_OK) {
    this->error_code_ = error;
    this->mark_failed();
    return;
  }
  this->initialized_ = true; 
}

// To receive:
rmt_driver_uninstall(this->channel_);  // Free GPIO pin  
uint8_t received = gpio_get_level(c.gpio_num);

rmt_driver_install(this->channel_, &c.tx_config, 0);

void setup() override {
  if (this->pin_ == nullptr) {
    this->pin_ = new InternalGPIOPin();
  }
  
  // Initialize RMT TX and RX channels for OneWire communication
  if (!onewire_rmt_init(this->pin_->get_pin(), /* TX channel */ 0, /* RX channel */ 1)) {
    ESP_LOGE("onewire", "Error initializing RMT for OneWire!");
  }
  
  // Attach RMT channels to the OneWire pin
  if (!onewire_rmt_attach_pin(this->pin_->get_pin())) {
    ESP_LOGE("onewire", "Error attaching RMT channels to OneWire pin!");
  }
  
  // Flush any pending RX data
  onewire_flush_rmt_rx_buf(); 
}

void ESPHomeOneWireNGComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "  Using bitbang single pin mode:");
  ESP_LOGCONFIG(TAG, "    Pin: %d", this->pin_->get_pin());
}

bool ESPHomeOneWireNGComponent::reset() {
  if (this->pin_ != nullptr) {
  
  // Wait for bus idle 
  uint8_t retries = 125;
  do { /* ... */ } while (/* bus not idle */);
  
  // Send reset pulse (480 us low)
  RMT.write_items(this->ow_instance->tx_channel, {
    {0, 480}, // 480 us low
    {1, 0}    // End high
  }, 2, true);
  
  // Hold time (70 us) and sample 
  delayMicroseconds(70);
  bool present = !this->pin_->digital_read();
  
  // Remaining time (410 us)
  delayMicroseconds(410); 
  
  return present; 
}
}

bool ESPHomeOneWireNGComponent::read_bit() {
  // Start RX channel
  this->ow_instance->rx_channel->start(); 
  
  // Try to receive item within 5ms
  rmt_item32_t *item;
  size_t item_size;
  if (!rmt_rx_read(this->ow_instance->rx_channel, &item, &item_size, 5)) {
    // Timeout occurred
    return false; 
  }
  
  // Check if first item is 0 (bit = 0) or 1 (bit = 1)
  bool bit = item->level0 == 0;
  
  // Return received item
  rmt_rx_return_item(item);
  
  // Stop RX channel
  this->ow_instance->rx_channel->stop();
  
  return bit;
}

uint8_t ESPHomeOneWireNGComponentS::read_byte() {
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++) {
    bool bit = this->read_bit();
    byte = (byte >> 1) | (bit << 7);
  }
  return byte;
}

uint8_t ESPHomeOneWireNGComponent::crc8(const uint8_t *rom, int len);
void ESPHomeOneWireNGComponent::target_setup(const uint8_t family_code);
bool ESPHomeOneWireNGComponent::next(uint8_t *rom, int mode);
void ESPHomeOneWireNGComponent::select(const uint8_t *rom);
void ESPHomeOneWireNGComponent::skip();
void ESPHomeOneWireNGComponent::reset_search();
uint64_t ESPHomeOneWireNGComponent::search()
std::vector<uint64_t> ESPOneWire::search_vec()
bool ESPHomeOneWireNGComponent::read_bit();
uint8_t ESPHomeOneWireNGComponent::read();
void ESPHomeOneWireNGComponent::read_bytes(uint8_t *buf, int len);
void ESPHomeOneWireNGComponent::write_bit(int bit);
void ESPHomeOneWireNGComponent::write(const uint8_t data);
void ESPHomeOneWireNGComponent::write_bytes(const uint8_t *buf, int len);

}  // namespace ow_bus_ng
}  // namespace esphome

/*

__init__.py functions

def setup(self):
    # Create owb_rmt_driver_info struct 
    info = owb_rmt_driver_info()

    # Choose RMT channels and GPIO pin 
    tx_channel = CONFIG_TX_RMT_CHANNEL
    rx_channel = CONFIG_RX_RMT_CHANNEL
    gpio = CONFIG_ONEWIRE_PIN

    # Initialize the bus 
    self.onewire_bus = owb_rmt_initialize(info, gpio, tx_channel, rx_channel)

    # Do a first scan to detect devices 
    self.scan()

    This is a good example ESP-IDF app for using the OneWire bus and DS18B20 temperature sensors. Some key things it does:

It initializes the OneWire bus using the RMT timeslot driver. This allows precise timing control needed for the OneWire protocol.

It does an initial scan to find all devices on the bus and stores their ROM codes.

It checks if parasitic power is needed for the devices. If so, it enables it. It also optionally enables a strong pullup GPIO pin for extra power during temperature conversions.

It creates DS18B20_Info structs for each device, associating them with the OneWire bus and their ROM code.

It sets the resolution for all DS18B20 devices.

It can read temperatures either sequentially from all devices, or by starting a conversion on all devices at once and then reading the results. The latter is more efficient.

It counts any read errors for each device.

It restarts the app periodically.

Some improvements you could make:

Add temperature thresholds and alerts
Log temperature data
Add a web server to display the temperatures
Use FreeRTOS queues to read the temperatures in a separate task
Add other OneWire devices like DS2413 I/O expanders
Overall this is a great example of using the OneWire bus on ESP32 for reading multiple DS18B20 temperature sensors. Let me know if you have any other questions!

good response, how would this code look like in an esphome component?

Here's how this could look as an ESPHome component:

python
Copy
class DS18B20Component(Component):
    """Reads temperatures from DS18B20 OneWire temperature sensors."""

    def __init__(self, sensor_ids):
        self.sensors = []
        for id in sensor_ids:
            self.sensors.append(DS18B20Sensor(self, id))

    def setup(self):
        self.owb = owb_rmt_initialize()  # Initialize OneWire bus
        self.start_conversions()

    def start_conversions(self):
        """Start temperature conversions on all sensors."""
        # Start conversion on all sensors
        for sensor in self.sensors:
            sensor.start_conversion()

        # Delay for conversion time
        delay(750) 

        # Read temperatures
        for sensor in self.sensors:
            sensor.get_temperature()

    def dump_config(self):
        """Dump configuration to YAML."""
        return None

class DS18B20Sensor():
    """Class to represent a single DS18B20 sensor."""

    def __init__(self, parent, id):
        self.parent = parent
        self.id = id
        self.temperature = None

    def start_conversion(self):
        """Start a temperature conversion on this sensor."""
        pass  # Call owb_... function to start conversion for this sensor ID

    def get_temperature(self):
        """Get the temperature result from the last conversion."""
        # Call owb_... function to read temperature for this sensor ID
        # Update self.temperature with the result

# Example configuration:
ds18b20:
  - 28-000004847208  # ID of first sensor 
  - 28-00000484708b  # ID of second sensor
Some key points:

The DS18B20Component holds a list of DS18B20Sensor objects, one for each sensor ID.

In setup(), the component initializes the OneWire bus and starts an initial temperature conversion on all sensors.

The start_conversions() method starts a conversion on all sensors, waits, and then reads the temperatures. This is called periodically.

Each DS18B20Sensor represents one physical sensor. It can start a conversion for that sensor ID and read its temperature.

The sensor IDs are passed in the configuration.

The core logic for using the OneWire bus is abstracted away in the owb_rmt_initialize() and owb_... functions. The component simply calls these.

Let me know if this helps explain how to structure an ESPHome component for the DS18B20 and OneWire bus! I can provide more details if needed.

You make a good point. For an ESPHome component, the core logic should really be in C++, with Python mainly used for the configuration API.

A better approach would be:

Implement the DS18B20Component class in C++. This would handle:

Initializing the OneWire bus

Starting temperature conversions

Reading sensor temperatures

Storing sensor data

Have a Python class DS18B20Component that inherits from the C++ version:

python
Copy
class DS18B20Component(esphome.cpp_component.PolymorphicComponent):
    """Reads temperatures from DS18B20 OneWire temperature sensors."""

    def __init__(self, *args, **kwargs):
        super().__init__("DS18B20Component", *args, **kwargs)
The Python class handles the configuration (sensor IDs) and builds the C++ class.

So the full code would look something like this:

ds18b20_component.h:

cpp
Copy
#include "esphome/core/component.h"

namespace esphome {
namespace ds18b20 {

class DS18B20Component : public Component {
  public:
    void setup() override;
    void dump_config() override;
    float get_sensor_temperature(int id);
  protected:
    std::vector<std::string> sensor_ids_;
};

}  // namespace ds18b20
}  // namespace esphome
ds18b20_component.cpp:

cpp
Copy
#include "ds18b20_component.h"

// Implement setup(), dump_config() and get_sensor_temperature() here 
// Initialize OneWire, start conversions, read temperatures, etc. 
ds18b20.py:

python
Copy
from esphome import cpp_component
import esphome.codegen as cg

CODEOWNERS = ["@esphome/core"]
MULTI_CONF = True

ds18b20_ns = cg.esphome_ns.namespace("ds18b20")
DS18B20Component = ds18b20_ns.class_("DS18B20Component", cg.Component)

@cpp_component.register_component
class DS18B20Component(cpp_component.PolymorphicComponent):

    def __init__(self, *args, **kwargs):
        super().__init__("DS18B20Component", *args, **kwargs)

    def set_sensor_ids(self, ids):
        conf = self.get_config()
        conf["sensor_ids"] = ids
So in summary, for an ESPHome component:

Implement the core logic in C++ (header and source file)
Have a Python class inherit the C++ component and handle the configuration
Set any configuration values/structs from the Python class using get_config()
I hope this helps clarify structuring ESPHome components in C++! Let me know if you have any other questions.

Here's how you could structure an ESPHome onewire_rmt component:

onewire_rmt.h

cpp
Copy
#include "esphome/core/component.h"
#include "owb_rmt.h"

namespace esphome {
namespace onewire_rmt {

class OneWireRMTComponent : public Component {
  public:
    void setup() override;
    void dump_config() override;
    OneWireBus *get_bus();  // Return the OneWireBus
  protected:
    OneWireBus *bus_;
    bool use_parasitic_power_;
    bool use_strong_pullup_; 
};

}  // namespace onewire_rmt
}  // namespace esphome
onewire_rmt.cpp

cpp
Copy
#include "onewire_rmt.h"

void OneWireRMTComponent::setup() {
  // Initialize OneWire bus with RMT driver
  this->bus_ = owb_rmt_initialize(...);  
  
  this->bus_->use_parasitic_power(this->use_parasitic_power_);
  #ifdef USE_STRONG_PULLUP_PIN
    this->bus_->use_strong_pullup_pin(...); 
  #endif
}

void OneWireRMTComponent::dump_config() {
  // Log configuration  
}

OneWireBus *OneWireRMTComponent::get_bus() {
  return this->bus_; 
}
onewire_rmt.py

python
Copy
from esphome import pins
from esphome import cpp_component
import esphome.codegen as cg

CODEOWNERS = ["@esphome/core"]
multi_conf = True

onewire_rmt_ns = cg.esphome_ns.namespace("onewire_rmt")
OneWireRMTComponent = onewire_rmt_ns.class_(
    "OneWireRMTComponent", cg.Component
)

@cpp_component.register_component 
class OneWireRMTComponent(cpp_component.PolymorphicComponent):

  def __init__(self, *, pin, use_parasitic_power=False, strong_pullup_pin=None):
    super().__init__("OneWireRMTComponent")
    
    self.pin = cg.gpio_pin_expression(pin)
    self.use_parasitic_power = use_parasitic_power
    if strong_pullup_pin is not None:
        self.strong_pullup_pin = cg.gpio_pin_expression(strong_pullup_pin)

  def setup(self):
    conf = self.get_config()
    conf["pin"] = self.pin
    conf["use_parasitic_power"] = self.use_parasitic_power
    if hasattr(self, "strong_pullup_pin"):
        conf["strong_pullup_pin"] = self.strong_pullup_pin
The key points are:

The C++ OneWireRMTComponent class handles initializing the OneWire bus and its options like parasitic power.

The Python OneWireRMTComponent passes the pin, parasitic power option, and optional strong pullup pin to the C++ class through get_config().

The C++ component exposes a get_bus() method to get the OneWire bus instance, which other components can use.

The remainder of the logic for the OneWire bus and its drivers stays in the C++ code.

Let me know if this helps explain the multi-language component structure for ESPHome! I'd be happy to provide more examples.

You're right, I was mistaken in my previous explanation. The RMT peripheral is actually used for both transmitting and receiving with the OneWire bus.

Looking at that code, here's how it works:

Transmitting:

The RMT peripheral is used to generate precisely timed pulses to transmit bits over the OneWire bus pin.
It uses rmt_bytes_encoder to encode the bits to transmit, and rmt_copy_encoder to generate the reset pulse.
The RMT TX channel is used to actually transmit these pulses over the GPIO pin.
Receiving:

The RMT RX channel is used to detect pulses received on the OneWire bus pin.
When data is received, the rmt_rx_done_event_callbacks callback is called.
This callback reads the received pulse data from the RMT peripheral and decodes it into bits, which are then decoded into bytes.
The received bytes are pushed onto a queue to be read by the main program.
So you're right that the RMT peripheral and its channels are used for both transmitting and receiving with the OneWire bus. My previous explanation was mistaken.

The data flow is:

Transmitting:

Your code calls a function to transmit data over the OneWire bus (ex. write_bit())
The owb_rmt driver encodes the bit data into pulses using the RMT encoders
The RMT TX channel transmits those pulses over the GPIO pin
Receiving:

The RMT RX channel detects pulses received on the GPIO pin
When pulse data is received, the rmt_rx_done_event_callbacks callback is called
That callback decodes the pulse data into bits and bytes, and pushes the bytes onto a queue
Your code reads received bytes from that queue
So the RMT peripheral and its channels are used bidirectionally to communicate over the OneWire bus. My apologies for the incorrect information previously. Please let me know if you have any other questions!


can you provide be with what the setup(); would look like including the declaration of rmt parameters and constructors and instances like other esphome components do?

Here's what a setup() method for the OneWire bus using RMT may look like in ESPhome:

python
Copy
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor
from esphome.const import CONF_ID, CONF_PIN

DEPENDENCIES = ["rmt"]
MULTI_CONF = True

owb_rmt_ns = cg.esphome_ns.namespace("owb_rmt")
OneWireBusComponent = owb_rmt_ns.class_("OneWireBusComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(OneWireBusComponent),
    cv.Required(CONF_PIN): cv.All(
        cv.only_on_esp32,
        cv.uint8_t, cv.gpio_pin_input_pullup
    )
})

def setup_onewire_rmt(var, config):
    onewire = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(onewire, config)

    pin = yield cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(onewire.set_pin(pin))

@BINARY_SENSOR_REGISTRY.register("onewire_rmt", OneWireBusComponent, binary_sensor.BinarySensor)
async def onewire_rmt_binary_sensor_to_code(config, var, component):
    ...

@SENSOR_REGISTRY.register("onewire_rmt", OneWireBusComponent, sensor.Sensor)
async def onewire_rmt_sensor_to_code(config, var, component):
    ... 
This setup defines a OneWireBusComponent component. In the setup_onewire_rmt() coroutine function, it does the following:

Creates a new Pvariable for the component instance
Registers the component
Gets the GPIO pin expression for the configured pin
Sets that pin on the component using set_pin()
It also registers some binary_sensor and sensor platforms that can use this OneWireBusComponent.

The setup() method would look something like this:

python
Copy
onewire = setup_onewire_rmt(var, config[CONF_ONEWIRE_RMT_BUS]) 
This would set up the OneWireBusComponent and return the Pvariable for it, which could then be used in the binary_sensor and sensor platforms.

Let me know if you have any other questions!

I'm happy your helping be do the front facing part which is telling the build system what we are building how about the c++ cpp setup(); ?

Sure, here's what the C++ setup() method might look like for the OneWireBusComponent:

cpp
Copy
void OneWireBusComponent::setup() {
    // Create RMT configuration
    owb_rmt_config_t config;
    config.gpio_pin = this->pin_->get_pin();
    config.max_rx_bytes = 32; // Maximum bytes to receive
    
    // Create RMT OneWire bus
    esp_err_t err = owb_rmt_new_bus(&config, &this->bus_);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize OneWire bus: %s", esp_err_to_name(err));
        this->mark_failed();
        return;
    }
    
    // Start RMT channel
    err = rmt_enable(this->bus_->tx_channel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start OneWire RMT TX channel: %s", esp_err_to_name(err));
        this->mark_failed();
        return; 
    }
    err = rmt_enable(this->bus_->rx_channel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start OneWire RMT RX channel: %s", esp_err_to_name(err));
        this->mark_failed();
        return; 
    }
}
This setup() method does the following:

Creates an owb_rmt_config_t struct with the GPIO pin and max receive bytes
Calls owb_rmt_new_bus() to initialize the RMT OneWire bus. This sets up the TX and RX channels.
Enables (starts) the RMT TX and RX channels
Handles any errors by marking the component as failed
So this handles the RMT-specific initialization for the OneWire bus. The front-end setup() call you showed before would call this C++ setup() method to fully configure the component.

Let me know if you have any other questions!

*/
