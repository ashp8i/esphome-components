#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace ow_bus_ng {

static const char *const TAG = "owbus.ng";

#define OWB_ROM_SEARCH        0xF0
#define OWB_ROM_READ          0x33
#define OWB_ROM_MATCH         0x55
#define OWB_ROM_SKIP          0xCC
#define OWB_ROM_SEARCH_ALARM  0xEC
#define OWB_ROM_CODE_STRING_LENGTH (17)

// Forward declarations
class OneWireBusDevice;
class OneWireBusDriverInfo;
class OneWireBusImplementor;
class OneWireBusLowLevelComponent; 
class OneWireBusRMT;
class OneWireBusGPIO;
class OneWireBusUtils;

// Shared enums and typedefs
enum class OneWireBusStatus {
    NOT_SET = -1,
    OK = 0,                 ///< Operation succeeded
    ERROR,                  ///< Unspecified error
    NO_DEVICES_FOUND,       ///< No 1-Wire devices found
    NOT_INITIALIZED,        ///< Unspecified error
    PARAMETER_NULL,         ///< Unspecified error
    DEVICE_NOT_RESPONDING,  ///< 1-Wire device disconnected during operation
    CRC_FAILED,             ///< CRC Mis-match
    TOO_MANY_BITS,          ///< Incorrect Bits
    HW_ERROR                ///< H/W Error
 };


struct OneWireBusROMCode {
  union {
    struct {
      uint8_t family[1];
      uint8_t serial_number[6];
      uint8_t crc[1];
    } fields;
    uint8_t bytes[8];
  };
};

struct OneWireBusSearchState {
  OneWireBusROMCode rom_code;
  int last_discrepancy;
  int last_family_discrepancy;
  int last_device_flag;
};

struct OneWireBus {
  const struct _OneWireBus_Timing* timing;
  bool use_crc;
  bool use_parasitic_power;
  gpio_num_t strong_pullup_gpio;
  const struct OneWireBusDriver* driver;  
};

class OneWireBusDriver {
 public:
  const char* name;
  
  OneWireBusStatus initialize();
  // (Re-named from uninitialize() for ESPHome style)
  // Initialize the driver and any hardware resources. Return OK on success.  
    
  OneWireBusStatus reset(const OneWireBus* bus, bool* is_present);  
  // Perform a bus reset and check if any devices are present.
  // Set *is_present to true if any devices are detected, false otherwise.
  
  OneWireBusStatus write_bits(const OneWireBus* bus, uint8_t out, 
                              int number_of_bits_to_write);
  // Write number_of_bits_to_write bits from the out byte to the bus.
  // Return OK on success. 
  
  OneWireBusStatus read_bits(const OneWireBus* bus, uint8_t* in, 
                             int number_of_bits_to_read);
  // Read number_of_bits_to_read bits from the bus and store in the in byte.
  // Return OK on success. 
  OneWireBusStatus set_crc_enabled(bool use_crc); 
  
  OneWireBusStatus set_parasitic_power_enabled(bool use_parasitic_power);
  OneWireBusStatus set_strong_pullup_gpio(gpio_num_t gpio);
  
  OneWireBusStatus read_rom(const OneWireBus *bus, OneWireBusROMCode *rom_code);
  OneWireBusStatus verify_rom(const OneWireBus *bus, OneWireBusROMCode rom_code, bool *is_present); 
  OneWireBusStatus reset(const OneWireBus *bus, bool *is_present);
  
  OneWireBusStatus read_bit(const OneWireBus *bus, uint8_t *out);
  OneWireBusStatus read_byte(const OneWireBus *bus, uint8_t *out);
  
  OneWireBusStatus write_bit(const OneWireBus *bus, uint8_t bit);
  OneWireBusStatus write_byte(const OneWireBus *bus, uint8_t data); 
  
  OneWireBusStatus write_rom_code(const OneWireBus* bus, OneWireBusROMCode rom_code);

  uint8_t crc8_byte(uint8_t crc, uint8_t data);   
  uint8_t crc8_bytes(uint8_t crc, const uint8_t* data, size_t len);
  OneWireBusStatus search_first(const OneWireBus* bus, OneWireBusSearchState* state, bool* found_device);
  OneWireBusStatus search_next(const OneWireBus* bus, OneWireBusSearchState* state, bool* found_device);  
  char* string_from_rom_code(OneWireBusROMCode rom_code, char* buffer, size_t len);
  OneWireBusStatus set_strong_pullup(const OneWireBus* bus, bool enable);
};

typedef struct OneWireBusDevice OneWireBusDevice;

// Abstract device class 
class OneWireBusDevice {
public:
  virtual OneWireBusStatus read_scratchpad(uint8_t *buf, size_t len) = 0;
  virtual OneWireBusStatus write_scratchpad(/* ... */) = 0;
  // ...
};

// Base classes 
class OneWireBusDriverInfo { /* ... */ };  

// Utility class with shared logic  
class OneWireBusUtils {
 public:
  static OneWireBusStatus readBit(OneWireBusLowLevelComponent *bus, 
                                  uint8_t *out);
  static bool _is_init(const OneWireBus *bus);
  
  static uint8_t _calc_crc(uint8_t crc, uint8_t data);
  
  static uint8_t _calc_crc_block(uint8_t crc, const uint8_t *buffer, size_t len);
  
  static OneWireBusStatus read_bit(OneWireBusLowLevelComponent *bus, 
                                   uint8_t *out);
                                   
  static uint8_t calculate_crc8(const uint8_t *data, size_t len);
  {
    uint8_t crc = 0;
    return _calc_crc_block(crc, data, len); 
  }
};

/* Usage Tips
// Calculate CRC8 of input 
uint8_t crc = OneWireBusUtils::calculate_crc8(input, len);

// Check if bus initialized
if (OneWireBusUtils::_is_init(bus)) {
  // Use bus...  
} else {
  // Handle error  
}

// Read bit from bus 
OneWireBusStatus status = OneWireBusUtils::read_bit(bus, &bit); 
*/

// Base classes 
class OneWireBusLowLevelComponent { 
 public:
  virtual OneWireBusStatus doReadBit(uint8_t *out) = 0;
};

class OneWireBusComponent : public Component, protected OneWireBusLowLevelComponent {};

// Shared base class for concrete implementations  
class OneWireBusImplementor {
public:
  OneWireBusImplementor(OneWireBusDriverInfo info);
protected:
  OneWireBusDriverInfo driver_info_; 
};  

class OneWireBusRMT : public OneWireBusComponent, public OneWireBusImplementor  {
 public: 
  OneWireBusRMT(OneWireBusDriverInfo info);
  OneWireBusStatus doReadBit(uint8_t *out) override; 
};

class OneWireBusBitbang : public OneWireBusComponent, public OneWireBusImplementor {
public: 
  OneWireBusBitbang(OneWireBusDriverInfo info);
  OneWireBusStatus doReadBit(uint8_t *out) override;  
};

}  // namespace ow_bus_ng
}  // namespace esphome

// /** @file one_wire_bus.h
//  *  @brief Unified C++ header for 1-Wire bus communication.
//  */

// #pragma once

// #include "esphome/core/hal.h"
// #include "esphome/core/component.h"
// #include <vector>

// namespace esphome {
// namespace ow_bus_ng {

// /** @brief Status/error codes for 1-Wire bus operations. */ 
// enum class OneWireBusStatus {
//   OK,                ///< Operation succeeded
//   ERROR,             ///< Unspecified error
//   NO_DEVICES_FOUND,  ///< No 1-Wire devices found
//   DEVICE_DISCONNECTED, ///< 1-Wire device disconnected during operation
// };

// /** @brief Holds the ROM code and search state for a 1-Wire device. */
// struct OneWireBusDevice {
//   uint8_t family;    ///< Device family code
//   uint8_t serial[6]; ///< Serial number
//   uint8_t crc;       ///<CRC checksum byte
// };

// /** @brief Driver for interfacing with a 1-Wire bus. */
// class OneWireBusComponent {
//  public:
//   /** @brief Initializes the 1-Wire bus.
//    *  @return OneWireBusStatus::OK on success.
//    */
//   virtual OneWireBusStatus initialize() = 0;

//   /** @brief Uninitializes the 1-Wire bus.
//    *  @return OneWireBusStatus::OK on success. 
//    */
//   virtual OneWireBusStatus uninitialize() = 0;

//   // ... same methods as in sample C++ code ... 
// };

// /** @brief Implementation of OneWireBusComponent using the RMT peripheral. */ 
// class OneWireBusRMT : public OneWireBusComponent {
//   // ...
// };

// /** @brief Implementation of OneWireBusComponent using GPIO. */  
// class OneWireBusGPIO : public OneWireBusComponent {
//   // ...  
// };

// /** @brief Interacts with a 1-Wire device on a bus. */ 
// class OneWireBusDevice {
//   // ... same methods as in sample C++ code ...
// };

// #pragma once

// #include "esphome/core/component.h"
// #include "esphome/core/hal.h"

// namespace esphome {
// namespace ow_bus_ng {

// /** @brief Status/error codes for 1-Wire bus operations. */  
// enum class OneWireBusStatus {/*...*/};  

// /** @brief Holds the ROM code and search state for a 1-Wire device. */  
// typedef struct OneWireBusDevice OneWireBusDevice;

// /** @brief Driver information for a 1-Wire bus implementor. */
// class OneWireBusDriverInfo { /* ... */ };  

// /** @brief Shared logic/utilities for 1-Wire bus communication. */ 
// class OneWireBusUtils {
//  public:
//   /** @brief Reads a bit from the 1-Wire bus using the provided implementor.
//    *  @param bus The 1-Wire bus implementor to use.
//    *  @param[out] out The read bit value. 
//    *  @return OneWireBusStatus indicating the operation result.
//    */
//   static OneWireBusStatus readBit(OneWireBusLowLevelComponent *bus, 
//                                   uint8_t *out);
// }; 

// /** @brief Low-level interface for a 1-Wire bus hardware implementation. */  
// class OneWireBusLowLevelComponent {  
//  public:
//   /** @brief Reads a bit from the 1-Wire bus. 
//    *  @param[out] out The read bit value.  
//    *  @return OneWireBusStatus indicating the operation result. 
//    */
//   virtual OneWireBusStatus doReadBit(uint8_t *out) = 0;
// };

// /** @brief Base class for a 1-Wire bus component. */
// class OneWireBusComponent : public Component, protected OneWireBusLowLevelComponent {};

// // ... class definitions for OneWireBusRMT, OneWireBusGPIO,  OneWireBusDevice ...

// }  // namespace ow_bus_ng
// }  // namespace esphome


// #pragma once

// #include "esphome/core/hal.h"
// #include "esphome/core/component.h"
// #include <vector>

// namespace esphome {
// namespace ow_bus_ng {

// enum class OneWireBusStatus {
//   OK,
//   ERROR,
//   NO_DEVICES_FOUND,
//   DEVICE_DISCONNECTED,
//   // ...
// };

// class OneWireBusComponent : public Component {
//  public:
//   virtual void setup() override {}
//   virtual void dump_config() override {}
//   virtual OneWireBusStatus initialize() = 0;
//   virtual OneWireBusStatus uninitialize() = 0;
//   virtual OneWireBusStatus reset(bool *is_present) = 0;
//   virtual OneWireBusStatus read_bit(uint8_t *out) = 0;
//   virtual OneWireBusStatus read_byte(uint8_t *out) = 0;
//   virtual OneWireBusStatus read_bytes(uint8_t *buffer, unsigned int len) = 0;
//   virtual OneWireBusStatus write_bit(uint8_t bit) = 0;
//   virtual OneWireBusStatus write_byte(uint8_t data) = 0;
//   virtual OneWireBusStatus write_bytes(const uint8_t *buffer, size_t len) = 0;
//   virtual OneWireBusStatus search_first(OneWireBus_SearchState *state, bool *found_device) = 0;
//   virtual OneWireBusStatus search_next(OneWireBus_SearchState *state, bool *found_device) = 0;
// };

// class OneWireBusRMT : public OneWireBusComponent {
//  public:
//   OneWireBusRMT(owb_rmt_driver_info driver_info);
//   virtual OneWireBusStatus initialize() override;
//   virtual OneWireBusStatus uninitialize() override;
//   virtual OneWireBusStatus reset(bool *is_present) override;
//   virtual OneWireBusStatus read_bit(uint8_t *out) override;
//   virtual OneWireBusStatus read_byte(uint8_t *out) override;
//   virtual OneWireBusStatus read_bytes(uint8_t *buffer, unsigned int len) override;
//   virtual OneWireBusStatus write_bit(uint8_t bit) override;
//   virtual OneWireBusStatus write_byte(uint8_t data) override;
//   virtual OneWireBusStatus write_bytes(const uint8_t *buffer, size_t len) override;
//   virtual OneWireBusStatus search_first(OneWireBus_SearchState *state, bool *found_device) override;
//   virtual OneWireBusStatus search_next(OneWireBus_SearchState *state, bool *found_device) override;
// private:
//     owb_rmt_driver_info driver_info_;
// };

// class OneWireBusGPIO : public OneWireBusComponent {
//  public:
//   OneWireBusGPIO(owb_gpio_driver_info driver_info);
//   virtual OneWireBusStatus initialize() override;
//   virtual OneWireBusStatus uninitialize() override;
//   virtual OneWireBusStatus reset(bool *is_present) override;
//   virtual OneWireBusStatus read_bit(uint8_t *out) override;
//   virtual OneWireBusStatus read_byte(uint8_t *out) override;
//   virtual OneWireBusStatus read_bytes(uint8_t *buffer, unsigned int len) override;
//   virtual OneWireBusStatus write_bit(uint8_t bit) override;
//   virtual OneWireBusStatus write_byte(uint8_t data) override;
//   virtual OneWireBusStatus write_bytes(const uint8_t *buffer, size_t len) override;
//   virtual OneWireBusStatus search_first(OneWireBus_SearchState *state, bool *found_device) override;
//   virtual OneWireBusStatus search_next(OneWireBus_SearchState *state, bool *found_device) override;
//  private:
//   owb_gpio_driver_info driver_info_;
// };

// class OneWireBusDevice {
//  public:
//   virtual OneWireBusStatus read_scratchpad(uint8_t *buffer, size_t len) = 0;
//   virtual OneWireBusStatus write_scratchpad(const uint8_t *buffer, size_t len) = 0;
//   virtual OneWireBusStatus copy_scratchpad() = 0;
//   virtual OneWireBusStatus recall_eeprom() = 0;
//   virtual OneWireBusStatus read_power_supply(bool *is_parasitic) = 0;
// };

// }  // namespace ow_bus_ng
// }  // namespace esphome

