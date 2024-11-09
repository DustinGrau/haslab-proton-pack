/**
 *   GPStar External - Ghostbusters Proton Pack & Neutrona Wand.
 *   Copyright (C) 2023-2024 Michael Rajotte <michael.rajotte@gpstartechnologies.com>
 *                         & Dustin Grau <dustin.grau@gmail.com>
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

void debug(String message) {
  // Writes a debug message to the serial console.
  #if defined(DEBUG_SEND_TO_CONSOLE)
    Serial.println(message); // Print to serial console.
  #endif
  #if defined(DEBUG_SEND_TO_WEBSOCKET)
    ws.textAll(message); // Send a copy to the WebSocket.
  #endif
}

// Obtain a list of partitions for this device.
void printPartitions() {
  const esp_partition_t *partition;
  esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

  if (iterator == nullptr) {
    Serial.println(F("No partitions found."));
    return;
  }

  Serial.println(F("Partitions:"));
  while (iterator != nullptr) {
    partition = esp_partition_get(iterator);
    Serial.printf("Label: %s, Size: %lu bytes, Address: 0x%08lx\n",
                  partition->label,
                  partition->size,
                  partition->address);
    iterator = esp_partition_next(iterator);
  }

  esp_partition_iterator_release(iterator);  // Release the iterator once done
}

void ledsOff() {
  // Turn off the RGB pins.
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, LOW);
  digitalWrite(LED_B_PIN, LOW);
  fill_solid(device_leds, DEVICE_NUM_LEDS, CRGB::Black);
}

void animateLights() {
  static uint16_t wavePosition = 0;

  // Update timer interval in case i_power changes
  if (ms_anim_change.justFinished()) {
    if(b_firing) {
      // Speed up animation only when firing.
      ms_anim_change.start(i_animation_duration / ((i_power + 1) * 2));
    }
    else {
      // Otherwise return to normal speed.
      ms_anim_change.start(i_animation_duration);
    }

    for (int i = 0; i < DEVICE_NUM_LEDS; i++) {
      device_leds[i] = CHSV((wavePosition + i * 20) % 255, 255, 255);  // Wave effect
    }

    wavePosition += 5; // Move the wave position
  }
}

void blinkLights() {
  static bool b_restart = true;
  static uint8_t i_current = 0;
  static uint16_t i_delay_total;
  static uint16_t i_delay_led;

  if(b_firing) {
    // Only perform blinking if firing.
    if (i_current == 0) {
      b_restart = true; // Blink when sequence is complete.
    }
    else {
      b_restart = false; // Otherwise the animation is ongoing.
    }

    // Increment the count for the animation sequence.
    i_current++;
    if (i_current > 1) {
      // Ensure we cycle back around in the sequence.
      i_current = i_current % DEVICE_NUM_LEDS;
    }

    if(ms_anim_change.remaining() < 1) {
      if(i_power > 0) {
        // Speed up the blink with the power level.
        i_delay_total = i_animation_time / i_power;
        
      }
      else {
        // Handle case where power is unset.
        i_delay_total = i_animation_time;
      }
      ms_anim_change.start(i_delay_total);
      i_delay_led = i_delay_total / DEVICE_NUM_LEDS;
    }

    if(b_restart) {
      ledsOff(); // Turn off LEDs when flag is true.
    }
    else {
      // Turn on LED's according to the firing mode.

      //device_leds[i_current] = getHueAsGBR(PRIMARY_LED, C_WHITE);
      device_leds[i_current] = getHueAsRGB(PRIMARY_LED, C_WHITE);

      switch(STREAM_MODE) {
        case PROTON:
          // Red
          digitalWrite(LED_R_PIN, HIGH);
        break;
        case SLIME:
          // Green
          digitalWrite(LED_G_PIN, HIGH);
        break;
        case STASIS:
          // Blue
          digitalWrite(LED_B_PIN, HIGH);
        break;
        case MESON:
          // Orange
          digitalWrite(LED_R_PIN, HIGH);
          digitalWrite(LED_G_PIN, HIGH);
        break;
        default:
          // White
          digitalWrite(LED_R_PIN, HIGH);
          digitalWrite(LED_G_PIN, HIGH);
          digitalWrite(LED_B_PIN, HIGH);
        break;
      }
    }
  }
  else {
    ledsOff(); // Turn off the RGB LED's
    b_restart = true; // Mark animation as reset.
    i_current = 0; // Reset the LED sequence.
  }
}
