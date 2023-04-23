
// ESPHomeOneWireNGComponent::ESPHomeOneWireNGComponent(OneWireBusPinConfig pin_config,
//                                InternalGPIOPin *input_pin,
//                                InternalGPIOPin *output_pin,
//                                InternalGPIOPin *pin) {
//   if (pin_config == ONEWIRE_BUS_PIN_SINGLE && pin != nullptr) {
//     this->split_io_ = ONEWIRE_BUS_PIN_SINGLE;
//   }

//   else if (pin_config == ONEWIRE_BUS_PIN_SPLIT && input_pin != nullptr && output_pin != nullptr) {
//     this->split_io_ = ONEWIRE_BUS_PIN_SPLIT;
//     this->input_pin_ = input_pin;
//     this->output_pin_ = output_pin;
//   }

//   if (this->split_io_ == ONEWIRE_BUS_PIN_SINGLE) {
//     this->pin_ = pin;
//   }
// }