/*
Copyright 2018 Mattia Dal Ben <matthewdibi@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#undef RGBLIGHT_LED_COUNT
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_LED_COUNT 14
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8

/* Reverse direction of each LED strip. */
/* The LEDs on my Redox as wired by Falbatech run from the centre towards the
   outside edge on both halves. I want gradients/animations etc to run from
   far-left to far-right across the whole keyboard, so both segments are reversed
   using the LED map.
   Depending on the direction your LEDs are wired (or your lighting preferences),
   you may not need this line. */
#define RGBLIGHT_LED_MAP { 6, 5, 4, 3, 2, 1, 0, 13, 12, 11, 10, 9, 8, 7 }

/* Old default behavior of mod-taps
   This allows for rolling keystrokes within the TAPPING_TERM */
#define HOLD_ON_OTHER_KEY_PRESS
