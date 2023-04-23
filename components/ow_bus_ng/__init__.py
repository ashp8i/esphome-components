import voluptuous as vol
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

pin_schema = vol.Schema({
    Required('input_pin'): pins.gpio_input_pin_schema,
    Required('output_pin'): pins.gpio_output_pin_schema 
})

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPHomeOneWireNGComponent),
        cv.Required(CONF_PIN): cv.any([
            pins.gpio_input_pin_schema, 
            pin_schema
        ])
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())
    
    conf_pin = config[CONF_PIN]
    if isinstance(conf_pin, dict):
        # Split IO mode
        input_pin = await cg.gpio_pin_expression(conf_pin['input_pin'])
        output_pin = await cg.gpio_pin_expression(conf_pin['output_pin'])
        cg.add(var.set_split_io(input_pin, output_pin))
    else:
        # Single pin mode
        pin = await cg.gpio_pin_expression(conf_pin)
        cg.add(var.set_single_pin(pin))
    
    await cg.register_component(var, config)