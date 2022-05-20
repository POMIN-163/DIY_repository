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

#pragma once

#ifndef SIG_GEN_PWM_GPIO
    #define SIG_GEN_PWM_GPIO 22
#endif

// PWM signal generator output gpio is configured in CMakeLists.txt
// default is 22
#if SIG_GEN_PWM_GPIO <= 1
    #error invalid SIG_GEN_PWM_GPIO - conflict with stdio uart
#endif

#if SIG_GEN_PWM_GPIO >= 6 && SIG_GEN_PWM_GPIO <= 13
    #error invalid SIG_GEN_PWM_GPIO - conflict with logic analyzer gpio
#endif

#if SIG_GEN_PWM_GPIO >= 26 && SIG_GEN_PWM_GPIO <= 27
    #error invalid SIG_GEN_PWM_GPIO - conflict with adc gpio
#endif

// Input pins used to deterimine the currently selected voltage range
// The first pin for each channel can be configured in CMakeLists.txt and the rest are assumed
// to be on consecutive pins
// default is 2
// #if VOLTAGE_RANGE_START_GPIO <= 1
//     #error invalid VOLTAGE_RANGE_START_GPIO - conflict with stdio uart
// #endif

// #if VOLTAGE_RANGE_START_GPIO >= 3 && VOLTAGE_RANGE_START_GPIO <= 13
//     #error invalid VOLTAGE_RANGE_START_GPIO - conflict with logic analyzer gpio
// #endif

// #if VOLTAGE_RANGE_START_GPIO >= 23 && VOLTAGE_RANGE_START_GPIO >= 27
//     #error invalid VOLTAGE_RANGE_START_GPIO - conflict with adc gpio
// #endif

// #if VOLTAGE_RANGE_START_GPIO >= (SIG_GEN_PWM_GPIO - 3) && VOLTAGE_RANGE_START_GPIO <= SIG_GEN_PWM_GPIO
//     #error invalid VOLTAGE_RANGE_START_GPIO - conflict with signal generator output
// #endif

#ifndef SCOPE_CH1_IN_GPIO
    #define SCOPE_CH1_IN_GPIO 26
#endif

#ifndef VR_CH1_1_GPIO
    #define VR_CH1_1_GPIO 2
#endif

#ifndef VR_CH1_0_GPIO
    #define VR_CH1_0_GPIO 3
#endif

#ifndef SCOPE_CH2_IN_GPIO
    #define SCOPE_CH2_IN_GPIO 27
#endif

#ifndef VR_CH2_1_GPIO
    #define VR_CH2_1_GPIO 4
#endif

#ifndef VR_CH2_0_GPIO
    #define VR_CH2_0_GPIO 5
#endif


#ifndef STATUS_LED_GPIO
    #define STATUS_LED_GPIO PICO_DEFAULT_LED_PIN
#endif

#ifndef SMPS_GPIO
    #define SMPS_GPIO 23
#endif