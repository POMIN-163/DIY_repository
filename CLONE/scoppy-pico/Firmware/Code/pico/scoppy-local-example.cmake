# Your configuration goes here
# scoppy-local.cmake is not tracked by git

# Create empty list of definitions
set(SCOPPY_COMPILE_DEFS "")

# The output GPIO for the PWM signal generator
#list(APPEND SCOPPY_COMPILE_DEFS SIG_GEN_PWM_GPIO=22)

# Status LED GPIO
#list(APPEND SCOPPY_COMPILE_DEFS STATUS_LED_GPIO=25)

# CH1 ADC GPIO
#list(APPEND SCOPPY_COMPILE_DEFS SCOPE_CH1_IN_GPIO=26)

# GPIOs for CH1 voltage range MSB (bit 1) and LSB (bit 0)
#list(APPEND SCOPPY_COMPILE_DEFS VR_CH1_1_GPIO=2)
#list(APPEND SCOPPY_COMPILE_DEFS VR_CH1_0_GPIO=3)

# CH2 ADC GPIO
#list(APPEND SCOPPY_COMPILE_DEFS SCOPE_CH2_IN_GPIO=27)

# GPIOs for CH2 voltage range MSB (bit 1) and LSB (bit 0)
#list(APPEND SCOPPY_COMPILE_DEFS VR_CH2_1_GPIO=4)
#list(APPEND SCOPPY_COMPILE_DEFS VR_CH2_0_GPIO=5)

# If AUTO_VOLTAGE_RANGE is set to non-zero (enabled), 
# the voltage range GPIOs will be set by the
# firmware and the hardware will need to set the appropriate gain of
# the input signal. The voltage range value to be used when setting the GPIOS
# is sent by the app to the firmware.
# When this is enabled, the 'Selected input voltage range' option will appear in the channel menu
#
# If undefined or 0, the firmware reads the value of the voltage range pins
# and sends this to the app. For example, a 2 pole switch could be used to
# simultaniously set the gain of the input signal and set the appropriate
# values of voltage range pins.
# When this is disabled, the 'Selected input voltage range' option will NOT appear in the channel menu
#list(APPEND SCOPPY_COMPILE_DEFS "AUTO_VOLTAGE_RANGE=0")

# The input voltage ranges ie. the values you see when tapping the Channel badge, then Settings and then
# Voltage ranges. Compiling these into the firmware saves us from having to enter the values manually in the app.
# These are uploaded to the app each time the pico is connected to the Android device.
# This will overwrite existing voltage range values in the app. There is a Maximum of 4 ranges per channel.
# The ranges should be in ordered from largest to smallest (required for the AUTO_VOLTAGE_RANGE to work correctly)
# Format: {channel_id, range_id, min_voltage_microvolts, max_voltage_microvolts}
# Note: The channel_ids for CH1 and CH2 are 0 and 1 respectively
#list(APPEND SCOPPY_COMPILE_DEFS "INPUT_VOLTAGE_RANGES=\
#{0, 0, -6309000, 6301000}, \
#{0, 1, -308000, 3501000},  \
#{1, 0, -3305000, 3398000}, \
#{1, 1, -304000, 3508000},  \
#")

# The first logic analyser GPIO
#list(APPEND SCOPPY_COMPILE_DEFS "LOGIC_GPIO_BASE=6")
# The number of logic analyser GPIOs
#list(APPEND SCOPPY_COMPILE_DEFS "LOGIC_GPIO_COUNT=8")

# Note: you can add multiple definitions like so:
# list(APPEND SCOPPY_COMPILE_DEFS 
#    "PULL_UNUSED_ADC_TO_GND=1" 
#    "BLAH=88"
#)
