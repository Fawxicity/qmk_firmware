/* Copyright 2022 splitkb.com <support@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"

// The first four layers gets a name for readability, which is then used in the OLED below.
enum layers
{
  LAYER_HOME,
  LAYER_FN,
  LAYER_NAVNUM,
  LAYER_MOUSE
};

#ifdef OLED_ENABLE
// NOTE: Most of the OLED code was originally written by Soundmonster for the Corne,
// and has been copied directly from `crkbd/soundmonster/keymap.c`

oled_rotation_t oled_init_kb(oled_rotation_t rotation)
{
    return OLED_ROTATION_270;
}

void render_logo(void)
{
    static const char PROGMEM logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0};

    oled_write_P(logo, false);
}

void render_space(int rows)
{
    for (size_t i = 0; i < rows; i++)
    {
        oled_write_P(PSTR("     "), false);
    }
}

void render_kb_LED_state(void)
{
    // Host Keyboard LED Status
    led_t led_usb_state = host_keyboard_led_state();
    led_usb_state.num_lock ? oled_write_P( PSTR("N"), true) : oled_write_P( PSTR("N"), false);
    oled_write_P( PSTR(" "), false);
    led_usb_state.caps_lock ? oled_write_P( PSTR("C"), true) : oled_write_P( PSTR("C"), false);
    oled_write_P( PSTR(" "), false);
    led_usb_state.scroll_lock ? oled_write_P( PSTR("S"), true) : oled_write_P( PSTR("S"), false);
}

void render_layer_state(void)
{
    static const char PROGMEM home_layer_l[] = {
        0x8F, 0x90, 0x91, 0x92, 0x01,
        0xAF, 0xB0, 0xB1, 0xB2, 0x03,
        0xCF, 0xD0, 0xD1, 0xD2, 0x05, 0};
    static const char PROGMEM home_layer_r[] = {
        0x01, 0x8F, 0x90, 0x91, 0x92,
        0x03, 0xAF, 0xB0, 0xB1, 0xB2,
        0x05, 0xCF, 0xD0, 0xD1, 0xD2, 0};

    static const char PROGMEM functions_symbols_l[] = {
        0x93, 0x94, 0x95, 0x96, 0x02,
        0xB3, 0xB4, 0xB5, 0xB6, 0x03,
        0xD3, 0xD4, 0xD5, 0xD6, 0x05, 0};
    static const char PROGMEM functions_symbols_r[] = {
        0x02, 0x93, 0x94, 0x95, 0x96,
        0x03, 0xB3, 0xB4, 0xB5, 0xB6,
        0x05, 0xD3, 0xD4, 0xD5, 0xD6, 0};

    static const char PROGMEM nav_numpad_l[] = {
        0x97, 0x98, 0x99, 0x9A, 0x01,
        0xB7, 0xB8, 0xB9, 0xBA, 0x04,
        0xD7, 0xD8, 0xD9, 0xDA, 0x05, 0};
    static const char PROGMEM nav_numpad_r[] = {
        0x01, 0x97, 0x98, 0x99, 0x9A,
        0x04, 0xB7, 0xB8, 0xB9, 0xBA, 
        0x05, 0xD7, 0xD8, 0xD9, 0xDA, 0};

    static const char PROGMEM mouse_media_rgb_l[] = {
        0x9B, 0x9C, 0x9D, 0x9E, 0x01,
        0xBB, 0xBC, 0xBD, 0xBE, 0x03,
        0xDB, 0xDC, 0xDD, 0xDE, 0x06, 0};
    static const char PROGMEM mouse_media_rgb_r[] = {
        0x01, 0x9B, 0x9C, 0x9D, 0x9E, 
        0x03, 0xBB, 0xBC, 0xBD, 0xBE, 
        0x06, 0xDB, 0xDC, 0xDD, 0xDE, 0};

    bool is_left = is_keyboard_left();
    if(layer_state_is(LAYER_HOME))
        oled_write_P(is_left ? home_layer_l : home_layer_r, false);
    else if(layer_state_is(LAYER_FN))
        oled_write_P(is_left ? functions_symbols_l : functions_symbols_r, false);
    else if(layer_state_is(LAYER_NAVNUM))
        oled_write_P(is_left ? nav_numpad_l : nav_numpad_r, false);
    else
        oled_write_P(is_left ? mouse_media_rgb_l : mouse_media_rgb_r, false);
}

bool hit = false;

void render_bongocat(bool hit, bool master)
{
    static const char PROGMEM paws_up[] = {
        0x8A, 0x8B, 0x8C, 0x8D, 0x20,
        0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0
    };

    static const char PROGMEM paws_down[] = {
        0x8A, 0x8B, 0x8C, 0x8D, 0x20,
        0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0
    };

    static const char PROGMEM paws_up_flipped[] = {
        0x20, 0x86, 0x87, 0x88, 0x89,
        0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0
    };

    static const char PROGMEM paws_down_flipped[] = {
        0x20, 0x86, 0x87, 0x88, 0x89,
        0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0
    };

    bool left_is_master = is_keyboard_left() && master;
    if(hit)
        oled_write_P(left_is_master ? paws_down : paws_down_flipped, false);
    else
        oled_write_P(left_is_master ? paws_up : paws_up_flipped, false);
}

void render_wpm(void)
{
    uint8_t n = get_current_wpm();
    char    wpm_str[6];
    wpm_str[5] = '\0';
    wpm_str[4] = ' ';
    wpm_str[3] = '0' + n % 10;
    wpm_str[2] = '0' + (n /= 10) % 10;
    wpm_str[1] = '0' + n / 10;
    wpm_str[0] = ' ';

    oled_write(" WPM ", false);
    oled_write(wpm_str, false);
}

void render(void)
{
    render_logo();
    render_space(1);
    render_kb_LED_state();
    render_space(3);
    oled_write("Layer", false);
    render_layer_state();
    render_space(2);
    bool is_master = is_keyboard_master();
    if(is_master)
        render_bongocat(hit, is_master);
    else
        render_wpm();
}

bool oled_task_kb(void)
{
    if (!oled_task_user())
        return false;

    render();

    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed)
        hit = true;
    else
        hit = false;

    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed)
        hit = true;
    else
        hit = false;
}

#endif