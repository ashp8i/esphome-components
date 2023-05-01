OneWireBus : Structure containing 1-Wire bus information relevant to a single instance
OneWireBus_ROMCode : Represents a 1-Wire ROM Code. This is a sequence of eight bytes, where the first byte is the family number, then the following 6 bytes form the serial number. The final byte is the CRC8 check byte
fields : Provides access via field names
OneWireBus_SearchState : Represents the state of a device search on the 1-Wire bus
owb_driver : 
owb_gpio_driver_info : GPIO driver information
owb_rmt_driver_info : RMT driver information 

bus : owb_gpio_driver_info , owb_rmt_driver_info
gpio : owb_gpio_driver_info , owb_rmt_driver_info
rx_channel : owb_rmt_driver_info
tx_channel : owb_rmt_driver_info
rb : owb_rmt_driver_info

name : owb_driver
read_bits : owb_driver
reset : owb_driver
uninitialize : owb_driver
write_bits : owb_driver

driver : OneWireBus
timing : OneWireBus
strong_pullup_gpio : OneWireBus
use_crc : OneWireBus
use_parasitic_power : OneWireBus

rom_code : OneWireBus_SearchState
last_device_flag : OneWireBus_SearchState
last_discrepancy : OneWireBus_SearchState
last_family_discrepancy : OneWireBus_SearchState

bytes : OneWireBus_ROMCode
fields : OneWireBus_ROMCode

crc : OneWireBus_ROMCode::fields
family : OneWireBus_ROMCode::fields
serial_number : OneWireBus_ROMCode::fields

struct mgos_rmt_onewire* onewire_rmt_create(int pin, int rmt_rx, int rmt_tx);
void onewire_rmt_close(struct mgos_rmt_onewire *ow);

bool onewire_rmt_reset(struct mgos_rmt_onewire *ow);
uint8_t onewire_rmt_crc8(const uint8_t *rom, int len);
void onewire_rmt_target_setup(struct mgos_rmt_onewire *ow, const uint8_t family_code);

bool onewire_rmt_next(struct mgos_rmt_onewire *ow, uint8_t *rom, int mode);
void onewire_rmt_select(struct mgos_rmt_onewire *ow, const uint8_t *rom);
void onewire_rmt_skip(struct mgos_rmt_onewire *ow);
void onewire_rmt_search_clean(struct mgos_rmt_onewire *ow);

bool onewire_rmt_read_bit(struct mgos_rmt_onewire *ow);
uint8_t onewire_rmt_read(struct mgos_rmt_onewire *ow);
void onewire_rmt_read_bytes(struct mgos_rmt_onewire *ow, uint8_t *buf, int len);

void onewire_rmt_write_bit(struct mgos_rmt_onewire *ow, int bit);
void onewire_rmt_write(struct mgos_rmt_onewire *ow, const uint8_t data);
void onewire_rmt_write_bytes(struct mgos_rmt_onewire *ow, const uint8_t *buf, int len);

// Strong pull-up aka power mode is implemented by the pad's push-pull driver.
// Open-drain configuration is used for normal operation.
// power bus by disabling open-drain:
#define OW_POWER(g) GPIO.pin[g].pad_driver = 0
// de-power bus by enabling open-drain:
#define OW_DEPOWER(g) GPIO.pin[g].pad_driver = 1