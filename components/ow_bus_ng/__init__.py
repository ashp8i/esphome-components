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

pin_schema = vol.Schema(
    {
        vol.Required("input_pin"): pins.gpio_input_pin_schema,
        vol.Required("output_pin"): pins.gpio_output_pin_schema,
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPHomeOneWireNGComponent),
        cv.Required(CONF_PIN): vol.Any(
            [pins.gpio_input_pin_schema, pin_schema],
            msg="The 'pin' option is required (as marked Required in the config schema) for ow_bus_ng component instance ${instance_number}. Please provide a GPIO pin number or pin object with 'input_pin' and 'output_pin'.",
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())

    conf_pin = config[CONF_PIN]
    if isinstance(conf_pin, dict):
        # Split IO mode
        in_pin = await cg.gpio_pin_expression(conf_pin["input_pin"])
        out_pin = await cg.gpio_pin_expression(conf_pin["output_pin"])
        cg.add(var.set_split_io(in_pin, out_pin))
    else:
        # Single pin mode
        pin = await cg.gpio_pin_expression(conf_pin)
        cg.add(var.set_single_pin(pin))

    await cg.register_component(var, config)
