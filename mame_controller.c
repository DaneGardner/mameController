/*
 * This file is part of the MAME Controller Project
 * Copyright (C) 2011 Dane Gardner
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"

//#define EXAMPLE
#define MAME1
//#define MAME2

uint8_t keymap[25] = {
#if defined(EXAMPLE)
                       KEY_A,               // Pin 01
                       KEY_B,               // Pin 02
                       KEY_C,               // Pin 03
                       KEY_D,               // Pin 04
                       KEY_E,               // Pin 05
                       KEY_F,               // Pin 06
                       KEY_G,               // Pin 07
                       KEY_H,               // Pin 08
                       KEY_I,               // Pin 09
                       KEY_J,               // Pin 10
                       KEY_K,               // Pin 11
                       KEY_L,               // Pin 12
                       KEY_M,               // Pin 13
                       KEY_N,               // Pin 14
                       KEY_O,               // Pin 15
                       KEY_P,               // Pin 16
                       KEY_Q,               // Pin 17
                       KEY_R,               // Pin 18
                       KEY_S,               // Pin 19
                       KEY_T,               // Pin 20
                       KEY_U,               // Pin 21
                       KEY_V,               // Pin 22
                       KEY_W,               // Pin 23
                       KEY_X,               // Pin 24
                       KEY_Y                // Pin 25
#elif defined(MAME1)
                       KEY_1,               // Pin 01 (Player 1 Start)
                       KEY_5,               // Pin 02 (Player 1 Coin/Select)
                       KEY_RIGHT,           // Pin 03 (Player 1 Right)
                       KEY_LEFT,            // Pin 04 (Player 1 Left)
                       KEY_UP,              // Pin 05 (Player 1 Up)
                       KEY_DOWN,            // Pin 06 (Player 1 Down)
                       KEY_LEFT_CTRL,       // Pin 07 (Player 1 Button 1)
                       KEY_LEFT_ALT,        // Pin 08 (Player 1 Button 2)
                       KEY_SPACE,           // Pin 09 (Player 1 Button 3)
                       KEY_LEFT_SHIFT,      // Pin 10 (Player 1 Button 4)
                       KEY_Z,               // Pin 11 (Player 1 Button 5)
                       KEY_X,               // Pin 12 (Player 1 Button 6)
                       KEY_C,               // Pin 13 (Player 1 Button 7)
                       KEY_V,               // Pin 14 (Player 1 Button 8)
                       KEY_NONE,            // Pin 15
                       KEY_NONE,            // Pin 16
                       KEY_NONE,            // Pin 17
                       KEY_NONE,            // Pin 18
                       KEY_NONE,            // Pin 19
                       KEY_ENTER,           // Pin 20 (Enter)
                       KEY_ESC,             // Pin 21 (Esc)
                       KEY_TAB,             // Pin 22 (Tab)
                       KEY_NONE,            // Pin 23
                       KEY_NONE,            // Pin 24
                       KEY_NONE             // Pin 25
#elif defined(MAME2)
                       KEY_2,               // Pin 01 (Player 2 Start)
                       KEY_6,               // Pin 02 (Player 2 Coin/Select)
                       KEY_G,               // Pin 03 (Player 2 Right)
                       KEY_D,               // Pin 04 (Player 2 Left)
                       KEY_R,               // Pin 05 (Player 2 Up)
                       KEY_F,               // Pin 06 (Player 2 Down)
                       KEY_A,               // Pin 07 (Player 2 Button 1)
                       KEY_S,               // Pin 08 (Player 2 Button 2)
                       KEY_Q,               // Pin 09 (Player 2 Button 3)
                       KEY_W,               // Pin 10 (Player 2 Button 4)
                       KEY_I,               // Pin 11 (Player 2 Button 5)
                       KEY_K,               // Pin 12 (Player 2 Button 6)
                       KEY_J,               // Pin 13 (Player 2 Button 7)
                       KEY_L,               // Pin 14 (Player 2 Button 8)
                       KEY_NONE,            // Pin 15
                       KEY_NONE,            // Pin 16
                       KEY_NONE,            // Pin 17
                       KEY_NONE,            // Pin 18
                       KEY_NONE,            // Pin 19
                       KEY_ENTER,           // Pin 20 (Enter)
                       KEY_ESC,             // Pin 21 (Esc)
                       KEY_TAB,             // Pin 22 (Tab)
                       KEY_NONE,            // Pin 23
                       KEY_NONE,            // Pin 24
                       KEY_NONE             // Pin 25
#endif
                      };

int main(void)
{
	uint8_t b_prev=0xFF, c_prev=0xFF, d_prev=0xFF, e_prev=0xFF, f_prev=0xFF;

	// set for 16 MHz clock
	CLKPR = 0x80, CLKPR = 0;

	// Configure all port B and port D pins as inputs with pullup resistors.
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	DDRE = 0x00;
	DDRF = 0x00;
	PORTB = 0xFF;
	PORTC = 0xC0;
	PORTD = 0xFF;
	PORTE = 0x40;
	PORTF = 0xF3;

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port, this will wait forever.
	usb_init();
	while(!usb_configured());

	// Wait an extra second for the PC's operating system to load drivers
	_delay_ms(1000);

	while(1) {
    uint8_t b = PINB;
    uint8_t c = (PINC & 0xC0);
    uint8_t d = PIND;
    uint8_t e = (PINE & 0x40);
    uint8_t f = (PINF & 0xF3);

    if(b != b_prev || c != c_prev || d != d_prev || e != e_prev || f != f_prev) {
      uint8_t keycount = 0;

      if(!(b & 0x01)) { keyboard_keys[keycount++] = keymap[0]; }
      if(!(b & 0x02)) { keyboard_keys[keycount++] = keymap[1]; }
      if(!(b & 0x04)) { keyboard_keys[keycount++] = keymap[2]; }
      if(!(b & 0x08)) { keyboard_keys[keycount++] = keymap[3]; }
      if(!(b & 0x80)) { keyboard_keys[keycount++] = keymap[4]; }
      if(!(d & 0x01)) { keyboard_keys[keycount++] = keymap[5]; }
      if(!(d & 0x02)) { keyboard_keys[keycount++] = keymap[6]; }
      if(!(d & 0x04)) { keyboard_keys[keycount++] = keymap[7]; }
      if(!(d & 0x08)) { keyboard_keys[keycount++] = keymap[8]; }
      if(!(c & 0x40)) { keyboard_keys[keycount++] = keymap[9]; }
      if(!(c & 0x80)) { keyboard_keys[keycount++] = keymap[10]; }
      if(!(d & 0x40)) { keyboard_keys[keycount++] = keymap[11]; }
      if(!(d & 0x80)) { keyboard_keys[keycount++] = keymap[12]; }
      if(!(b & 0x10)) { keyboard_keys[keycount++] = keymap[13]; }
      if(!(b & 0x20)) { keyboard_keys[keycount++] = keymap[14]; }
      if(!(b & 0x40)) { keyboard_keys[keycount++] = keymap[15]; }
      if(!(f & 0x80)) { keyboard_keys[keycount++] = keymap[16]; }
      if(!(f & 0x40)) { keyboard_keys[keycount++] = keymap[17]; }
      if(!(f & 0x20)) { keyboard_keys[keycount++] = keymap[18]; }
      if(!(f & 0x10)) { keyboard_keys[keycount++] = keymap[19]; }
      if(!(f & 0x02)) { keyboard_keys[keycount++] = keymap[20]; }
      if(!(f & 0x01)) { keyboard_keys[keycount++] = keymap[21]; }
      if(!(d & 0x10)) { keyboard_keys[keycount++] = keymap[22]; }
      if(!(d & 0x20)) { keyboard_keys[keycount++] = keymap[23]; }
      if(!(e & 0x40)) { keyboard_keys[keycount++] = keymap[24]; }

      while(keycount < sizeof(keyboard_keys)) {
        keyboard_keys[keycount++] = KEY_NONE;
      }

      usb_keyboard_send();

      b_prev = b;
      c_prev = c;
      d_prev = d;
      e_prev = e;
      f_prev = f;
    }
    _delay_ms(2);   // Debounce
	}
}
