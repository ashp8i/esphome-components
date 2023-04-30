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

# import esphome.codegen as cg
# import esphome.config_validation as cv
# from esphome import pins
# from esphome.const import CONF_ID, CONF_PIN, CONF_CARRIER_DUTY_PERCENT 

# CODEOWNERS = ["@ashp8i"]
# MULTI_CONF = True

# ow_bus_ng_ns = cg.esphome_ns.namespace("ow_bus_ng")
# ESPHomeOneWireNGComponent = ow_bus_ng_ns.class_(
#     "ESPHomeOneWireNGComponent", cg.Component
# )

# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.GenerateID(): cv.declare_id(ESPHomeOneWireNGComponent),
#         cv.Optional("pin"): pins.gpio_input_pin_schema,
#         cv.Optional("input_pin"): pins.gpio_input_pin_schema,
#         cv.Optional("output_pin"): pins.gpio_output_pin_schema,
#         cv.Optional("mode", default="single_pin_rmt"): cv.enum(
#             {
#                 "SINGLE_PIN_BITBANG": "single_pin_bitbang",
#                 "SINGLE_PIN_RMT": "single_pin_rmt",
#                 "SPLIT_IO_BITBANG": "split_io_bitbang",
#                 "SPLIT_IO_RMT": "split_io_rmt",
#             },
#             upper=True,
#         ),
#         cv.Optional(CONF_CARRIER_DUTY_PERCENT, default=50): cv.percentage_int,
#     }
# )

# # ... (setup methods same as before)

# async def to_code(config):
#     var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())
    
#     carrier_duty = config[CONF_CARRIER_DUTY_PERCENT]
#     cg.add(var.set_carrier_duty_percent(carrier_duty))
    
#     mode = config["mode"]
#     # ... (set mode same as before)
    
#     await cg.register_component(var, config)

# import esphome.codegen as cg
# import esphome.config_validation as cv
# from esphome import pins
# from esphome.components import remote_base
# from esphome.const import CONF_CARRIER_DUTY_PERCENT, CONF_ID, CONF_PIN, CONF_METHOD

# AUTO_LOAD = ["remote_base"]
# remote_transmitter_ns = cg.esphome_ns.namespace("remote_transmitter")
# RemoteTransmitterComponent = remote_transmitter_ns.class_(
#     "RemoteTransmitterComponent", remote_base.RemoteTransmitterBase, cg.Component
# )

# METHOD_BIT_BANG = "bit_bang"
# METHOD_ESP32_RMT = "esp32_rmt"

# METHODS = {
#     METHOD_BIT_BANG: {
#         "type": METHOD_BIT_BANG,
#     },
#     METHOD_ESP32_RMT: {
#         "type": METHOD_ESP32_RMT,
#     }
# }

# def _validate_method(value):
#     if isinstance(value, str):
#         value = value.upper()
#         if value == METHOD_BIT_BANG:
#             return {CONF_TYPE: METHOD_BIT_BANG}
#         if value == METHOD_ESP32_RMT:
#             return {CONF_TYPE: METHOD_ESP32_RMT}
#     raise cv.Invalid(f"invalid method {value}")

# MULTI_CONF = True
# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.GenerateID(): cv.declare_id(RemoteTransmitterComponent),
#         cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_CARRIER_DUTY_PERCENT): cv.All(
#             cv.percentage_int, cv.Range(min=1, max=100)
#         ),
#         cv.Optional(CONF_METHOD, default=METHOD_BIT_BANG): _validate_method
#     }
# ).extend(cv.COMPONENT_SCHEMA)

# async def to_code(config):
#     method = config[CONF_METHOD]
#     if method[CONF_TYPE] == METHOD_BIT_BANG:
#         # Use bit banging
#     elif method[CONF_TYPE] == METHOD_ESP32_RMT:
#         # Use ESP32 RMT
#     ...