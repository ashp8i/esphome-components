import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.const import CONF_ID, CONF_PIN, CONF_BAUD_RATE

DEPENDENCIES = ["uart"]

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
        cv.Required("conf_pin"): pins.gpio_input_pin_schema,
        # cv.Optional("input_pin"): pins.gpio_input_pin_schema,
        # cv.Optional("output_pin"): pins.gpio_output_pin_schema,
    }
)

async def setup_bitbang_single(var, config):
    pin = await cg.gpio_pin_expression(config["conf_pin"])
    cg.add(var.set_single_pin(pin))

# async def setup_bitbang_split_io(var, config):
#     in_pin = await cg.gpio_pin_expression(config["input_pin"])
#     out_pin = await cg.gpio_pin_expression(config["output_pin"])
#     cg.add(var.set_split_io(in_pin, out_pin))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())
    mode = config["mode"]
    if mode == "BITBANG_SINGLE":
        cg.add_define("USE_BITBANG_SINGLE")
    # elif mode == "BITBANG_SPLIT_IO":
    #     cg.add_define("USE_BITBANG_SPLIT_IO")
    else:
        _LOGGER.error("Invalid mode for ow_bus_ng: %s", mode)
        return

    await cg.register_component(var, config)
