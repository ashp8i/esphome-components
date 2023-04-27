Components have been split up to modules to make development easier

- ow_bus_ng - a unified library for 1-Wire Bus Master using bitbanging and RMT for reliable operation.
- ow_bus_ng_bitbang_single - same library as above but the core Implementation will be built here for easier testing and troubleshooting
- ow_bus_ng_bitbang_single_parasitic_test - as previous a work in progress for support of parasitic power
- ow_bus_ng_bitbang_split_io - same as (ow_bus_ng_bitbang_single) but using separate pins for scenarios where isolation circuits with diodes are used.
- ow_bus_ng_rmt_single - same library as ow_bus_ng_bitbang_single above but the core Implementation uses RMT internally for signal generation rather than bitbang GPIO.
- ow_bus_ng_rmt_split_io - same as (ow_bus_ng_rmt_single) but using separate pins for scenarios where isolation circuits with diodes are used.

All the versions will be built and tested as much as possible to provide a robust way of 1-wire signalling, the working libraries will be unified and common code will be folded/reused to enable maximum implementation of 1-wire in software with lowest footprint as possible. this will extend support for the following to be interfaced using 1-Wire Master Bus provided by ow_bus_ng or it's basic blocks: -

- Dallas DS18B20 Temperature Sensor
- Maxim Integrated DS28E17 1-Wire-to-I2C Master Bridge