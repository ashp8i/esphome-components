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
        cv.Optional("pin"): pins.gpio_input_pin_schema,
        cv.Optional("input_pin"): pins.gpio_input_pin_schema,
        cv.Optional("output_pin"): pins.gpio_output_pin_schema,
        cv.Optional("mode", default="single_pin_rmt"): cv.enum(
            {
                "SINGLE_PIN_BITBANG": "single_pin_bitbang",
                "SINGLE_PIN_RMT": "single_pin_rmt",
                "SPLIT_IO_BITBANG": "split_io_bitbang",
                "SPLIT_IO_RMT": "split_io_rmt",
            },
            upper=True,
        ),
    }
)

async def setup_single_pin_bitbang(var, config):
    pin = await cg.gpio_pin_expression(config["pin"])
    cg.add(var.set_single_pin_bitbang(pin))

async def setup_single_pin_rmt(var, config):
    pin = await cg.gpio_pin_expression(config["pin"])
    cg.add(var.set_single_pin_rmt(pin))

async def setup_split_io_bitbang(var, config):
    in_pin = await cg.gpio_pin_expression(config["input_pin"])
    out_pin = await cg.gpio_pin_expression(config["output_pin"])
    cg.add(var.set_split_io_bitbang(in_pin, out_pin))

async def setup_split_io_rmt(var, config):
    in_pin = await cg.gpio_pin_expression(config["input_pin"])
    out_pin = await cg.gpio_pin_expression(config["output_pin"])
    cg.add(var.set_split_io_rmt(in_pin, out_pin))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())
    mode = config["mode"]
    if mode == "SINGLE_PIN_BITBANG":
        cg.add_define("USE_SINGLE_PIN_BITBANG")
    elif mode == "SINGLE_PIN_RMT":
        cg.add_define("USE_SINGLE_PIN_RMT")
    elif mode == "SPLIT_IO_BITBANG":
        cg.add_define("USE_SPLIT_IO_BITBANG")
    elif mode == "SPLIT_IO_RMT":
        cg.add_define("USE_SPLIT_IO_RMT")
    else:
        _LOGGER.error("Invalid mode for ow_bus_ng: %s", mode)
        return

    await cg.register_component(var, config)
