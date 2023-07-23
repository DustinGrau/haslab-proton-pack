/**
 *   gpstar Proton Pack - Ghostbusters Proton Pack & Neutrona Wand.
 *   Copyright (C) 2023 Michael Rajotte <michael.rajotte@gmail.com>
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

/**
 * Colors are based on the RGB pattern; for GRB the Red/Green values should be switched.
 * Brightness is based on varying the intensity of each (0-255) using a relative value.
 *
 * However, colors based on Hue use a color degree, a Saturation, and Value (brightness).
*/

enum colors {
  C_BLACK,
  C_WHITE,
  C_PINK,
  C_RED,
  C_RED2,
  C_RED3,
  C_RED4,
  C_RED5,
  C_ORANGE,
  C_YELLOW,
  C_GREEN,
  C_MINT,
  C_AQUA,
  C_BLUE,
  C_PURPLE,
  C_REDGREEN,
  C_ORANGEPURPLE,
  C_PASTEL,
  C_RAINBOW
};

int getBrightness(uint8_t i_percent = 100){
  // Brightness here is a percentage, to be convered to a range 0-255.
  if (i_percent > 100) {
    i_percent = 100;
  }
  return (int) ((255 * i_percent) / 100);
}

// Special values for color cycles: current hue (color) and when to change color.
uint8_t i_curr_color = 0;
uint8_t i_count = 0;

uint8_t getDeviceColor(uint8_t i_device, uint8_t i_firing_mode, bool b_toggle) {
  if(b_toggle == true) {
    switch(i_firing_mode) {
      case PROTON:
        return C_RED;
      break;

      case SLIME:
        return C_GREEN;
      break;

      case STASIS:
        return C_BLUE;
      break;

      case MESON:
        return C_ORANGE;
      break;

      case VENTING:
        return C_RED;
      break;

      case SETTINGS:
        return C_WHITE;
      break;

      default:
        return C_BLUE;
      break;
    }
  } else {
    return C_BLUE;
  }
  switch(i_device) {
    case POWERCELL:
      return C_BLUE;
    break;

    case CYCLOTRON_OUTER:
    case CYCLOTRON_INNER:
      return C_RED;
    break;

    case VENT_LIGHT:
      return C_AQUA;
    break;

    default:
      return C_WHITE;
    break;
  }
}

CHSV getHue(uint8_t i_color, uint8_t i_brightness = 255, uint8_t i_saturation = 255, uint8_t i_cycle = 2) {
  // Brightness here is a value from 0-255 as limited by byte (uint8_t) type.
  // Note that for color cycles, i_cycle indicates how often to change color.

  // Returns a CHSV object with a hue (color), full saturation, and stated brightness.
  switch (i_color) {
    case C_BLACK:
      return CHSV(0, 0, 0); // Overrides brightness.
    break;

    case C_WHITE:
      return CHSV(100, 0, i_brightness);
    break;

    case C_PINK:
      return CHSV(244, i_saturation, i_brightness);
    break;

    case C_RED:
      return CHSV(0, i_saturation, i_brightness);
    break;

    case C_RED2:
      return CHSV(5, i_saturation, i_brightness);
    break;

    case C_RED3:
      return CHSV(10, i_saturation, i_brightness);
    break;

    case C_RED4:
      return CHSV(15, i_saturation, i_brightness);
    break;

    case C_RED5:
      return CHSV(20, i_saturation, i_brightness);
    break;

    case C_ORANGE:
      return CHSV(32, i_saturation, i_brightness);
    break;

    case C_YELLOW:
      return CHSV(64, i_saturation, i_brightness);
    break;

    case C_GREEN:
      return CHSV(96, i_saturation, i_brightness);
    break;

    case C_MINT:
      return CHSV(112, 120, i_brightness);
    break;

    case C_AQUA:
      return CHSV(128, i_saturation, i_brightness);
    break;

    case C_BLUE:
      return CHSV(160, i_saturation, i_brightness);
    break;

    case C_PURPLE:
      return CHSV(192, i_saturation, i_brightness);
    break;

    case C_REDGREEN:
      // Alternate between red (0) and green (96).
      if (i_curr_color != 0 && i_curr_color != 96) {
        i_curr_color = 0; // Reset if out of range.
      }
      i_count++;
      if (i_count % i_cycle == 0) {
        if (i_curr_color == 0) {
          i_curr_color = 96;
        } else {
          i_curr_color = 0;
        }
      }
      return CHSV(i_curr_color, 255, i_brightness);
    break;

    case C_ORANGEPURPLE:
      // Alternate between orange (32) and purple (192).
      if (i_curr_color != 32 && i_curr_color != 192) {
        i_curr_color = 32; // Reset if out of range.
      }
      i_count++;
      if (i_count % i_cycle == 0) {
        if (i_curr_color == 32) {
          i_curr_color = 192;
        } else {
          i_curr_color = 32;
        }
      }
      return CHSV(i_curr_color, 255, i_brightness);
    break;

    case C_PASTEL:
      // Cycle through all colors (0-255) at half saturation.
      i_count++;
      if (i_count % i_cycle == 0) {
        i_curr_color = (i_curr_color + 5) % 255;
      }
      return CHSV(i_curr_color, 128, i_brightness);
    break;

    case C_RAINBOW:
      // Cycle through all colors (0-255) at full saturation.
      i_count++;
      if (i_count % i_cycle == 0) {
        i_curr_color = (i_curr_color + 5) % 255;
      }
      return CHSV(i_curr_color, 255, i_brightness);
    break;

    default:
      return CHSV(100, 0, i_brightness); // White on no match.
    break;
  }
}

CRGB getHueAsRGB(uint8_t i_color, uint8_t i_brightness = 255, bool b_grb = false) {
  // Brightness here is a value from 0-255 as limited by byte (uint8_t) type.

  // Get the initial color using the HSV scheme.
  CHSV hsv = getHue(i_color, i_brightness);
  CRGB rgb;

  hsv2rgb_rainbow(hsv, rgb); // Convert from HSV to RGB.

  if (b_grb) {
    // Swap red/green values.
    return CRGB(rgb[1], rgb[0], rgb[2]);
  } else {
    return rgb; // Return RGB.
  } 
}

CRGB getHueAsGRB(uint8_t i_color, uint8_t i_brightness = 255) {
  return getHueAsRGB(i_color, i_brightness, true);
}
