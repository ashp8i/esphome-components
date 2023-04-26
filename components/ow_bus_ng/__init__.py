import voluptuous as vol
import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
import esphome.components.uart as uart
from esphome.const import CONF_ID, CONF_PIN
from voluptuous import Required, Optional, Any, All, Lower, In

DEPENDENCIES = ["uart"]

_LOGGER = logging.getLogger(__name__)

CODEOWNERS = ["@ashp8i"]
MULTI_CONF = True

ow_bus_ng_ns = cg.esphome_ns.namespace("ow_bus_ng")
ESPHomeOneWireNGComponent = ow_bus_ng_ns.class_(
    "ESPHomeOneWireNGComponent", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPHomeOneWireNGComponent),
        vol.Optional(CONF_PIN): pins.gpio_input_pin_schema,
        vol.Optional("input_pin"): pins.gpio_input_pin_schema,
        vol.Optional("output_pin"): pins.gpio_output_pin_schema,
        vol.Optional("mode", default="bitbang_single"): vol.In(
            [
                "bitbang_single",
                "bitbang_split_io",
                # "uart_half_duplex",
                "uart_full_duplex",
            ]
        ),
        vol.Optional("uart"): uart.UART_DEVICE_SCHEMA,
    }
)


async def setup_bitbang_single(var, config):
    pin = await cg.gpio_pin_expression(config["pin"])
    cg.add(var.set_single_pin(pin))


async def setup_bitbang_split_io(var, config):
    in_pin = await cg.gpio_pin_expression(config["input_pin"])
    out_pin = await cg.gpio_pin_expression(config["output_pin"])
    cg.add(var.set_split_io(in_pin, out_pin))


async def setup_uart_full_duplex(var, config):
    await uart.register_uart_device(var, config["uart"])
    uart_conf = config["uart"]
    cg.add(
        var.set_uart(uart_conf["tx_pin"], uart_conf["rx_pin"], uart_conf["baud_rate"])
    )


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], ESPHomeOneWireNGComponent())

    if "uart" in config:
        await setup_uart_full_duplex(var, config)
    else:
        mode = config.get("mode", "bitbang_single")
        if mode == "bitbang_single":
            await setup_bitbang_single(var, config)
        elif mode == "bitbang_split_io":
            await setup_bitbang_split_io(var, config)
        elif mode == "uart_full_duplex":
            await setup_uart_full_duplex(var, config)
        else:
            _LOGGER.error("Invalid mode for single pin: %s", mode)
            return

    await cg.register_component(var, config)
