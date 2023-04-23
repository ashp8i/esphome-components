import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID

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
        cv.Optional("input_pin"): pins.gpio_input_pin_schema,
        cv.Optional("output_pin"): pins.gpio_output_pin_schema,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())

    if "input_pin" in config and "output_pin" in config:
        in_pin = await cg.gpio_pin_expression(config["input_pin"])
        out_pin = await cg.gpio_pin_expression(config["output_pin"])
        if in_pin is None or out_pin is None:
            raise cv.Invalid("input_pin and output_pin are both required for split IO!")

        cg.add(ESPHomeOneWireNGComponent(in_pin, out_pin))

        core.LOG.info("Setting up OneWire split IO")

    elif "input_pin" in config or "output_pin" in config:
        raise cv.Invalid("Only input_pin OR output_pin, not alone")

    await cg.register_component(var, config)
