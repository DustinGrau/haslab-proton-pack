/**
 *   GPStar Ghost Trap - Ghostbusters Props, Mods, and Kits.
 *   Copyright (C) 2025 Michael Rajotte <michael.rajotte@gpstartechnologies.com>
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

#pragma once

/*
 * Control debug messages for various actions during normal operation.
 * Uncomment the desired line(s) to output messages when and where you
 * expect to see them. Using the console should be reserved for active
 * debugging, while the websocket will help with confirming operations
 * while using the device (post-setup for wireless).
 */
//#define DEBUG_WIRELESS_SETUP   // Output debugs related to the WiFi/network setup.
//#define DEBUG_PERFORMANCE      // Send debug messages for CPU/memory to the (USB) console.
#define DEBUG_SEND_TO_CONSOLE  // Send any general messages to the serial (USB) console.
//#define DEBUG_TASK_TO_CONSOLE  // Send any task messages to the serial (USB) console.
#define DEBUG_SEND_TO_WEBSOCKET  // Send any messages to connected WebSocket clients.

/*
 * Force the use of default SSID and password for wireless capabilities.
 * Uncomment and upload to device, then perform a reset of your password
 * to a new and known value. When completed, flash the latest version of
 * the software which has this line commented out.
 */
//#define RESET_AP_SETTINGS

/*
 * Used to reflect the last build date for the binary.
 */
String build_date = "20250126161853";

/*
 * Preferred WiFi Network Defaults
 * Directly provides information for an external WiFi network for the device to join.
 */
String user_wifi_ssid = ""; // Preferred network SSID for external WiFi
String user_wifi_pass = ""; // Preferred network password for external WiFi
