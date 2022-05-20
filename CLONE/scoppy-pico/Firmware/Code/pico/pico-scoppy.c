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

//
#include <stdio.h>
//
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
//
#include "pico-scoppy-ctx.h"
#include "pico-scoppy-pwm-sig-gen.h"
#include "pico-scoppy-samples.h"
#include "pico-scoppy-util.h"
#include "pico-scoppy.h"
#include "scoppy_usb.h"

static void init_smps() {
#if SMPS_LOW_POWER
    // this is the default. do nothing
#else
        gpio_init(SMPS_GPIO);
        gpio_set_dir(SMPS_GPIO, GPIO_OUT);
        gpio_put(SMPS_GPIO, true);

        // ignore. for testing only
        // gpio_init(15);
        // gpio_set_dir(15, GPIO_OUT);
        // gpio_put(15, true);
#endif
}

static void init_voltage_range_gpio(uint i) {
    if (i >= 0) {
#if AUTO_VOLTAGE_RANGE
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
        gpio_put(i, false);
#else        
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_down(i);
#endif        
    } else {
        // This voltage range pin is not used
    }
}

int main() {

    sleep_ms(200);

    gpio_init(STATUS_LED_GPIO);
    gpio_set_dir(STATUS_LED_GPIO, GPIO_OUT);
    gpio_put(STATUS_LED_GPIO, 1);

    LOG_PRINT("Initialising SMPS\n");
    init_smps();

    init_voltage_range_gpio(VR_CH1_1_GPIO);
    init_voltage_range_gpio(VR_CH1_0_GPIO);
    init_voltage_range_gpio(VR_CH2_1_GPIO);
    init_voltage_range_gpio(VR_CH2_0_GPIO);

    LOG_PRINT("Initialising stdio\n");
    stdio_init_all();

    LOG_PRINT("Initialising USB\n");
    scoppy_usb_init();

    struct scoppy_context *ctx = pico_scoppy_get_context();

    LOG_PRINT("Initialising ADC\n");
    pico_scoppy_init_samplers();

    LOG_PRINT("Starting PWM\n");
    pwm_sig_gen_init();

    LOG_PRINT("Starting\n");
    gpio_put(STATUS_LED_GPIO, 0);
    sleep_ms(100);

    DEBUG_PRINT("... launching core1\n");
    multicore_launch_core1(pico_scoppy_sampling_loop);
    multicore_fifo_push_blocking((uint32_t)ctx);

    DEBUG_PRINT("... starting scoppy_main\n");
    scoppy_main(ctx);

    return 0;
}
