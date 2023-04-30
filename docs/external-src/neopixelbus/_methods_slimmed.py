from dataclasses import dataclass
from typing import Any
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_CHANNEL,
    CONF_METHOD,
    CONF_PIN,
)
from esphome.components.esp32 import get_esp32_variant
from esphome.components.esp32.const import (
    VARIANT_ESP32,
    VARIANT_ESP32C3,
    VARIANT_ESP32S2,
    VARIANT_ESP32S3,
)
from esphome.core import CORE
from .const import (
    CONF_ASYNC,
    CONF_BUS,
)

METHOD_BIT_BANG = "bit_bang"
METHOD_ESP32_RMT = "esp32_rmt"

CHANNEL_DYNAMIC = "dynamic"
BUS_DYNAMIC = "dynamic"


def _esp32_rmt_default_channel():
    return {
        VARIANT_ESP32C3: 1,
        VARIANT_ESP32S2: 1,
        VARIANT_ESP32S3: 1,
    }.get(get_esp32_variant(), 6)


def _validate_esp32_rmt_channel(value):
    if isinstance(value, str) and value.lower() == CHANNEL_DYNAMIC:
        value = CHANNEL_DYNAMIC
    else:
        value = cv.int_(value)
    variant_channels = {
        VARIANT_ESP32: [0, 1, 2, 3, 4, 5, 6, 7, CHANNEL_DYNAMIC],
        VARIANT_ESP32C3: [0, 1, CHANNEL_DYNAMIC],
        VARIANT_ESP32S2: [0, 1, 2, 3, CHANNEL_DYNAMIC],
        VARIANT_ESP32S3: [0, 1, 2, 3, CHANNEL_DYNAMIC],
    }
    variant = get_esp32_variant()
    if variant not in variant_channels:
        raise cv.Invalid(f"{variant} does not support the rmt method")
    if value not in variant_channels[variant]:
        raise cv.Invalid(f"{variant} does not support rmt channel {value}")
    return value


def _esp32_i2s_default_bus():
    return {
        VARIANT_ESP32: 1,
        VARIANT_ESP32S2: 0,
    }.get(get_esp32_variant(), 0)


neo_ns = cg.global_ns


def _bit_bang_to_code(config, chip: str, inverted: bool):
    # https://github.com/Makuna/NeoPixelBus/blob/master/src/internal/NeoEspBitBangMethod.h
    # Some chips are only aliases
    chip = {
        CHIP_WSXXXX: CHIP_WSXXXZ,
    }.get(chip, chip)

    lookup = {
        CHIP_WS2811: (neo_ns.NeoEspBitBangSpeedWs2811, False),
    }
    # For tm variants opposite of inverted is needed
    speed, pinset_inverted = lookup[chip]
    pinset = {
        False: neo_ns.NeoEspPinset,
        True: neo_ns.NeoEspPinsetInverted,
    }[inverted != pinset_inverted]
    return neo_ns.NeoEspBitBangMethodBase.template(speed, pinset)


def _bit_bang_extra_validate(config):
    pin = config[CONF_PIN]
    if CORE.is_esp8266 and not (0 <= pin <= 15):
        # Due to use of w1ts
        raise cv.Invalid("Bit bang only supports pins GPIO0-GPIO15 on ESP8266")
    if CORE.is_esp32 and not (0 <= pin <= 31):
        raise cv.Invalid("Bit bang only supports pins GPIO0-GPIO31 on ESP32")


def _esp32_rmt_to_code(config, chip: str, inverted: bool):
    # https://github.com/Makuna/NeoPixelBus/blob/master/src/internal/NeoEsp32RmtMethod.h
    channel = {
        0: neo_ns.NeoEsp32RmtChannel0,
        1: neo_ns.NeoEsp32RmtChannel1,
        2: neo_ns.NeoEsp32RmtChannel2,
        3: neo_ns.NeoEsp32RmtChannel3,
        4: neo_ns.NeoEsp32RmtChannel4,
        5: neo_ns.NeoEsp32RmtChannel5,
        6: neo_ns.NeoEsp32RmtChannel6,
        7: neo_ns.NeoEsp32RmtChannel7,
        CHANNEL_DYNAMIC: neo_ns.NeoEsp32RmtChannelN,
    }[config[CONF_CHANNEL]]
    # Some chips are only aliases
    chip = {
        CHIP_WSXXXX: CHIP_WSXXXXZ,
    }.get(chip, chip)

    lookup = {
        (CHIP_WS2811, False): neo_ns.NeoEsp32RmtSpeedWs2811,
    }
    speed = lookup[(chip, inverted)]
    return neo_ns.NeoEsp32RmtMethodBase.template(speed, channel)


@dataclass
class MethodDescriptor:
    method_schema: Any
    to_code: Any
    supported_chips: list[str]
    extra_validate: Any = None


METHODS = {
    METHOD_BIT_BANG: MethodDescriptor(
        method_schema={},
        to_code=_bit_bang_to_code,
        extra_validate=_bit_bang_extra_validate,
        supported_chips=ONE_WIRE_CHIPS,
    ),
    METHOD_ESP32_RMT: MethodDescriptor(
        method_schema=cv.All(
            cv.only_on_esp32,
            {
                cv.Optional(
                    CONF_CHANNEL, default=_esp32_rmt_default_channel
                ): _validate_esp32_rmt_channel,
            },
        ),
        to_code=_esp32_rmt_to_code,
        supported_chips=ONE_WIRE_CHIPS,
    ),
}
