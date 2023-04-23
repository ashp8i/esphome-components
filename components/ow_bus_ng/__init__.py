import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID, CONF_PIN

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
        cv.Optional(CONF_PIN): pins.gpio_output_pin_schema,
        cv.Optional("input_pin"): pins.gpio_input_pin_schema,
        cv.Optional("output_pin"): pins.gpio_output_pin_schema,
    }
)


async def to_code(config):
    var = cg.Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())
    if CONF_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_PIN])
        cg.add(ESPHomeOneWireNGComponent(pin))
    elif "input_pin" in config and "output_pin" in config:
        in_pin = await cg.gpio_pin_expression(config["input_pin"])
        out_pin = await cg.gpio_pin_expression(config["output_pin"])
        cg.add(ESPHomeOneWireNGComponent(in_pin, out_pin))
    await cg.register_component(var, config)
