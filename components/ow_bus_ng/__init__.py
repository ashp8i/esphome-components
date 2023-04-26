import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.const import CONF_ID, CONF_PIN

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
        cv.Optional(CONF_PIN): pins.gpio_input_pin_schema,
        cv.Optional("input_pin"): pins.gpio_input_pin_schema,
        cv.Optional("output_pin"): pins.gpio_output_pin_schema,
        cv.Optional("mode", default="bitbang_single"): cv.enum(
            {
                "BITBANG_SINGLE": "bitbang_single",
                "BITBANG_SPLIT_IO": "bitbang_split_io",
                #   "UART_BUS_SINGLE_PIN": "uart_bus_single_pin",
                "UART_BUS": "uart_bus",
            },
            upper=True,
        ),
        cv.Optional("uart_id"): cv.int_,
        cv.Optional("uart"): cv.Schema(
            {
                cv.Required("tx_pin"): pins.gpio_output_pin_schema,
                cv.Required("rx_pin"): pins.gpio_input_pin_schema,
                cv.Optional("baud_rate", default="115200bps"): baud_rate_schema,
            }
        ),
    }
)


async def setup_bitbang_single(var, config):
    pin = await cg.gpio_pin_expression(config["pin"])
    cg.add(var.set_single_pin(pin))


async def setup_bitbang_split_io(var, config):
    in_pin = await cg.gpio_pin_expression(config["input_pin"])
    out_pin = await cg.gpio_pin_expression(config["output_pin"])
    cg.add(var.set_split_io(in_pin, out_pin))


# async def setup_uart_half_duplex(var, config):
#   if "uart" in config:
#     await uart.register_uart_device(var, config["uart"])
#     uart_conf = config["uart"]
#     cg.add(var.set_uart(uart_conf["tx_pin"], uart_conf["rx_pin"], uart_conf["baud_rate"]))
#   elif "uart_id" in config:
#     var.uart_bus = uart.ESP_PLATFORM.get_uart_instance(config["uart_id"])
#   else:
#     uart_buses = uart.ESP_PLATFORM.get_uart_instances()
#     var.uart_bus = uart_buses[0]


async def setup_uart_full_duplex(var, config):
    if "uart" in config:
        await uart.register_uart_device(var, config["uart"])
        uart_conf = config["uart"]
        cg.add(
            var.set_uart(
                uart_conf["tx_pin"], uart_conf["rx_pin"], uart_conf["baud_rate"]
            )
        )
    elif "uart_id" in config:
        var.uart_bus = uart.ESP_PLATFORM.get_uart_instance(config["uart_id"])
    else:
        uart_buses = uart.ESP_PLATFORM.get_uart_instances()
        var.uart_bus = uart_buses[0]


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())

    mode = config.get("mode", "bitbang_single")
    if mode == "bitbang_single":
        await setup_bitbang_single(var, config)
    elif mode == "bitbang_split_io":
        await setup_bitbang_split_io(var, config)
    #   elif mode == "uart_bus_single_pin":
    #     await setup_uart_half_duplex(var, config)
    elif mode == "uart_bus":
        await setup_uart_full_duplex(var, config)
    else:
        _LOGGER.error("Invalid mode for single pin: %s", mode)
        return

    await cg.register_component(var, config)
