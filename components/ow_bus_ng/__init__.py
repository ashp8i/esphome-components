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
        cv.Required(CONF_PIN): pins.gpio_input_pin_schema.extend(
            {
                "type": "multi_pin",
                "pins": {
                    "input_pin": pins.gpio_input_pin_schema,
                    "output_pin": pins.gpio_output_pin_schema,
                },
            }
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())

    if isinstance(config[CONF_PIN], str):
        # Single pin mode
        pin = await cg.gpio_pin_expression(config[CONF_PIN])
        cg.add(var.set_single_pin(pin))
    elif isinstance(config[CONF_PIN], dict):
        # Split IO mode
        in_pin = await cg.gpio_pin_expression(config[CONF_PIN]["input_pin"])
        out_pin = await cg.gpio_pin_expression(config[CONF_PIN]["output_pin"])
        cg.add(var.set_split_io(in_pin, out_pin))

    await cg.register_component(var, config)
