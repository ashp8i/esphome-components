# # ashp8i's ESPHome External Components

This Repo contains components that I'm currently working on for [ESPHome](https://esphome.io/).

## 1. Usage

Use latest [ESPHome](https://esphome.io/) with external components and add this to your `.yaml` definition:

```yaml
external_components:
  - source: github://ashp8i/esphome-components
```

## 2. Components

### 2.1. `owng_bus`

Component to create a 1-wire master by bit banging GPIO's, I may also consider whether I can create a UART version of this component but then supportable pins will be limited, Currently as it stands this is a dummy component and has empty functions. it is very similar to concept as dallas ds18b20 Temperature sensor Component but in progress to allow easy support of new 1-Wire devices. Specifically I'm also working on a ds28e17 I2C Bus component that allows use of I2C devices tunneled through a 1-Wire Bridge.

#### 2.1.1. Example

```yaml
# configure bus as shown below
# set pin_option to specify to use single pin or
# split RX/TX pins

# Single Pin 1-Wire Bus
ow_bus_ng:

pin: 'GPIO18'

pin_option: 'single'

Split Pin 1-Wire Bus
ow_bus_ng:

pin_option: 'split_io'

input_pin: 'GPIO19'

output_pin: 'GPIO20'
```

## 3. Author & License

ashp8i, GPL, 2023
