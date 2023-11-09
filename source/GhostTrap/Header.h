/**
 *   gpstar GhostTrap - Ghostbusters Proton Pack & Neutrona Wand.
 *   Copyright (C) 2023 Michael Rajotte <michael.rajotte@gpstartechnologies.com>
 *                    & Dustin Grau <dustin.grau@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, see <https://www.gnu.org/licenses/>.
 *
 */

/*
 * Pin for Addressable LEDs.
 */
#define DEVICE_LED_PIN 23
#define DEVICE_NUM_LEDS 1
CRGB device_leds[DEVICE_NUM_LEDS];

/*
 * LED Devices
 */
enum device {
  PRIMARY_LED
};

/*
 * Delay for fastled to update the addressable LEDs.
 * 0.03 ms to update 1 LED, and this device contains 2.
 * Just setting to 3 which should be sufficient.
 */
millisDelay ms_fast_led;
const uint8_t i_fast_led_delay = 3;

/*
 * Pin for test LED.
 */
#define TEST_LED_PIN 5