

#ifndef ONE_WIRE_BUS_H
#define ONE_WIRE_BUS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


#define OWB_ROM_SEARCH        0xF0
#define OWB_ROM_READ          0x33
#define OWB_ROM_MATCH         0x55
#define OWB_ROM_SKIP          0xCC
#define OWB_ROM_SEARCH_ALARM  0xEC

#define OWB_ROM_CODE_STRING_LENGTH (17)

#ifndef GPIO_NUM_NC
#  define GPIO_NUM_NC (-1)
#endif

struct owb_driver;

typedef struct
{
    const struct _OneWireBus_Timing * timing;
    bool use_crc;
    bool use_parasitic_power;
    gpio_num_t strong_pullup_gpio;
    const struct owb_driver * driver;
} OneWireBus;

typedef union
{
    struct fields
    {
        uint8_t family[1];
        uint8_t serial_number[6];
        uint8_t crc[1];
    } fields;

    uint8_t bytes[8];

} OneWireBus_ROMCode;

typedef struct
{
    OneWireBus_ROMCode rom_code;
    int last_discrepancy;
    int last_family_discrepancy;
    int last_device_flag;
} OneWireBus_SearchState;

typedef enum
{
    OWB_STATUS_NOT_SET = -1,
    OWB_STATUS_OK = 0,
    OWB_STATUS_NOT_INITIALIZED,
    OWB_STATUS_PARAMETER_NULL,
    OWB_STATUS_DEVICE_NOT_RESPONDING,
    OWB_STATUS_CRC_FAILED,
    OWB_STATUS_TOO_MANY_BITS,
    OWB_STATUS_HW_ERROR
} owb_status;

struct owb_driver
{
    const char* name;

    owb_status (*uninitialize)(const OneWireBus * bus);

    owb_status (*reset)(const OneWireBus * bus, bool *is_present);

    owb_status (*write_bits)(const OneWireBus *bus, uint8_t out, int number_of_bits_to_write);

    owb_status (*read_bits)(const OneWireBus *bus, uint8_t *in, int number_of_bits_to_read);
};

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

owb_status owb_uninitialize(OneWireBus * bus);

owb_status owb_use_crc(OneWireBus * bus, bool use_crc);

owb_status owb_use_parasitic_power(OneWireBus * bus, bool use_parasitic_power);

owb_status owb_use_strong_pullup_gpio(OneWireBus * bus, gpio_num_t gpio);

owb_status owb_read_rom(const OneWireBus * bus, OneWireBus_ROMCode * rom_code);

owb_status owb_verify_rom(const OneWireBus * bus, OneWireBus_ROMCode rom_code, bool * is_present);

owb_status owb_reset(const OneWireBus * bus, bool * is_present);

owb_status owb_read_bit(const OneWireBus * bus, uint8_t * out);

owb_status owb_read_byte(const OneWireBus * bus, uint8_t * out);

owb_status owb_read_bytes(const OneWireBus * bus, uint8_t * buffer, unsigned int len);

owb_status owb_write_bit(const OneWireBus * bus, uint8_t bit);

owb_status owb_write_byte(const OneWireBus * bus, uint8_t data);

owb_status owb_write_bytes(const OneWireBus * bus, const uint8_t * buffer, size_t len);

owb_status owb_write_rom_code(const OneWireBus * bus, OneWireBus_ROMCode rom_code);

uint8_t owb_crc8_byte(uint8_t crc, uint8_t data);

uint8_t owb_crc8_bytes(uint8_t crc, const uint8_t * data, size_t len);

owb_status owb_search_first(const OneWireBus * bus, OneWireBus_SearchState * state, bool *found_device);

owb_status owb_search_next(const OneWireBus * bus, OneWireBus_SearchState * state, bool *found_device);

char * owb_string_from_rom_code(OneWireBus_ROMCode rom_code, char * buffer, size_t len);

owb_status owb_set_strong_pullup(const OneWireBus * bus, bool enable);


#include "owb_gpio.h"
#include "owb_rmt.h"

#ifdef __cplusplus
}
#endif

#endif
