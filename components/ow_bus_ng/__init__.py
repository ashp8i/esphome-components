import voluptuous as vol
import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID, CONF_PIN
from voluptuous import Required, Optional, Any

_LOGGER = logging.getLogger(__name__)

CODEOWNERS = ["@ashp8i"]
MULTI_CONF = True

ow_bus_ng_ns = cg.esphome_ns.namespace("ow_bus_ng")
ESPHomeOneWireNGComponent = ow_bus_ng_ns.class_(
    "ESPHomeOneWireNGComponent", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPHomeOneWireNGComponent),
        cv.Optional("pin"): pins.gpio_input_pin_schema,
        cv.Optional("input_pin"): pins.gpio_input_pin_schema,
        cv.Optional("output_pin"): pins.gpio_output_pin_schema,
        cv.Optional("mode", default="bitbang_single"): vol.All(
            vol.Lower,
            vol.In(
                [
                    "BITBANG_SINGLE",
                    "BITBANG_SPLIT_IO",
                    "MODBUS_HALF_DUPLEX",
                    "UART_FULL_DUPLEX",
                ]
            ),
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())

    if "input_pin" in config and "output_pin" in config:
        in_pin = await cg.gpio_pin_expression(config["input_pin"])
        out_pin = await cg.gpio_pin_expression(config["output_pin"])
        mode = config.get("mode", "bitbang_single")
        if mode == "bitbang_split_io":
            cg.add(var.set_split_io(in_pin, out_pin))
        else:
            _LOGGER.error("Invalid mode for split I/O pins: %s", mode)
            return

    else:  
        # Single pin mode
        pin = await cg.gpio_pin_expression(config["pin"])
        mode = config.get("mode", "bitbang_single")
        if mode == "bitbang_single":
            cg.add(var.set_single_pin(pin))
        elif mode == "modbus_half_duplex":
            cg.add(var.set_modbus_half_duplex(pin))
        elif mode == "uart_full_duplex":
            cg.add(var.set_uart_full_duplex(pin))
        else:
            _LOGGER.error("Invalid mode for single pin: %s", mode)
            return

    await cg.register_component(var, config)
