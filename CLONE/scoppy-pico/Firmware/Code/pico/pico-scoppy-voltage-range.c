/*
 * This file is part of the scoppy-pico project.
 *
 * Copyright (C) 2021 FHDM Apps <scoppy@fhdm.xyz>
 * https://github.com/fhdm-dev
 *
 * scoppy-pico is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * scoppy-pico is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with scoppy-pico.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "pico/stdlib.h"
//
#include "pico-scoppy-pins.h"
#include "pico-scoppy-voltage-range.h"
#include "scoppy.h"

uint8_t pico_scoppy_get_voltage_range_id(int channel_id) {

#if AUTO_VOLTAGE_RANGE
    return scoppy.channels[channel_id].voltage_range;
#else
    // The channel ids for CH1 and CH2 are 0 and 1 respectively
    if (channel_id == 0) {
        return (gpio_get(VR_CH1_1_GPIO) ? 1u : 0u) << 1 | (gpio_get(VR_CH1_0_GPIO) ? 1u : 0u);
    } else if (channel_id == 1) {
        return (gpio_get(VR_CH2_1_GPIO) ? 1u : 0u) << 1 | (gpio_get(VR_CH2_0_GPIO) ? 1u : 0u);
    } else {
        return 0;
    }
#endif
}

#if AUTO_VOLTAGE_RANGE
void pico_scoppy_on_voltage_range_changed(uint8_t channel_id, uint8_t range_id) {

    // The channel ids for CH1 and CH2 are 0 and 1 respectively
    uint8_t bit1_gpio, bit0_gpio;
    if (channel_id == 0) {
        bit1_gpio = VR_CH1_1_GPIO;
        bit0_gpio = VR_CH1_0_GPIO;
    } else if (channel_id == 1) {
        bit1_gpio = VR_CH2_1_GPIO;
        bit0_gpio = VR_CH2_0_GPIO;
    }

    gpio_put(bit1_gpio, range_id & 0x02 ? true : false);
    gpio_put(bit0_gpio, range_id & 0x01 ? true : false);
}
#endif