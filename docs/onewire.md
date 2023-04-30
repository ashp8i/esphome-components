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

