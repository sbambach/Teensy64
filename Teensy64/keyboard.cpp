/*
  Copyright Frank Bösing, 2017

  This file is part of Teensy64.

    Teensy64 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Teensy64 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Teensy64.  If not, see <http://www.gnu.org/licenses/>.

    Diese Datei ist Teil von Teensy64.

    Teensy64 ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

    Teensy64 wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.

*/

#include "Teensy64.h"
#include "keyboard.h"


#if USBHOST
#include "usb_USBHost.h"

USBHost myusb;
//USBHub hub1, hub2, hub3;
KeyboardController keyboard;

#endif

const char * hotkeys[] = {"\x45RUN", //F12
                          "\x44LOAD\"$\"\rLIST\r",  //F11
                          "\x43LOAD\"$\",8", //F10
                          "\x42LOAD\"*\",8", //F9
                          NULL
                         };

char * _sendString = NULL;

void sendKey(char key) {

  while (true) {
    noInterrupts();
    if (cpu.RAM[198] == 0) break;
    interrupts();
    delay(1);
    //asm volatile("wfi");
  }

  cpu.RAM[631] = key;
  cpu.RAM[198] = 1;
  interrupts();
  return;

}

void do_sendString() {
  if (_sendString == NULL) return;
  char ch = *_sendString++;
  if (ch != 0)
    sendKey(ch);
  else
    _sendString = NULL;
}

void sendString(const char * p) {
  _sendString = (char *) p;

  Serial.print("Send String:");
  Serial.println(p);
}

static int hotkey(char ch) {
  if (_sendString != NULL ) return 1;

  unsigned i = 0;
  while (hotkeys[i] != NULL) {
    if (*hotkeys[i] == ch) {
      sendString(hotkeys[i] + 1);
      return 1;
    }
    i++;
  }
  return 0;
}

/*******************************************************************************************************************/
/*******************************************************************************************************************/
/*******************************************************************************************************************/

//Array values are keyboard-values returned from USB
//LeftShift, RightShift, Control, and Commodore are special codes, represented here with 0xfc..0xff
static const uint8_t ktab[8][8] = {
  {0x2a, 0x28, 0x4f, 0x40, 0x3a, 0x3c, 0x3e, 0x51}, //DEL, Return, Cursor Right, F7, F1, F3, F5, Cursor Down
  {0x20, 0x1a, 0x04, 0x21, 0x1d, 0x16, 0x08, 0xff}, //3, W, A, 4, Z, S, E, LeftShift
  {0x22, 0x15, 0x07, 0x23, 0x06, 0x09, 0x17, 0x1b}, //5, R, D, 6, C, F, T, X
  {0x24, 0x1c, 0x0a, 0x25, 0x05, 0x0B, 0x18, 0x19}, //7, Y, G, 8, B, H, U, V
  {0x26, 0x0c, 0x0D, 0x27, 0x10, 0x0E, 0x12, 0x11}, //9, I, J, 0, M, K, O, N
  {0x57, 0x13, 0x0F, 0x56, 0x37, 0x33, 0x2f, 0x36}, //+(Keypad), P, L, -(Keypad), ",", ":", "@", ","
  {0x49, 0x55, 0x34, 0x4A, 0xfe, 0x32, 0x4b, 0x54}, //Pound(ins), *(Keypad), ";", HOME (Pos1), RightShift, =, UP Arrow (Bild hoch), /(Keypad)
  {0x1e, 0x4e, 0xfd, 0x1f, 0x2c, 0xfc, 0x14, 0x29} //1,LEFT ARROW(Bild runter) , CTRL, 2, Space, Commodore, Q, RUN/STOP(ESC)
};


//Autogenerated with C64_keyboardtool.ino :
#if USBHOST
static const uint8_t keymatrixmap[2][256] = {
  //Rows:
  // 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F
  { 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x04, 0x04, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, //0x00
    0x10, 0x10, 0x10, 0x20, 0x80, 0x04, 0x02, 0x04, 0x08, 0x08, 0x02, 0x04, 0x08, 0x02, 0x80, 0x80, //0x10
    0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x01, 0x80, 0x01, 0x00, 0x80, 0x00, 0x00, 0x20, //0x20
    0x00, 0x00, 0x40, 0x20, 0x40, 0x00, 0x20, 0x20, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, //0x30
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x00, 0x00, 0x80, 0x01, //0x40
    0x00, 0x01, 0x00, 0x00, 0x40, 0x40, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x60
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x70
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x02
  }, //0xF0
  //Columns:
  // 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F
  { 0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10, 0x04, 0x40, 0x20, 0x04, 0x20, 0x02, 0x04, 0x20, 0x04, //0x00
    0x10, 0x80, 0x40, 0x02, 0x40, 0x02, 0x20, 0x40, 0x40, 0x80, 0x02, 0x80, 0x02, 0x10, 0x01, 0x08, //0x10
    0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0x02, 0x80, 0x01, 0x00, 0x10, 0x00, 0x00, 0x40, //0x20
    0x00, 0x00, 0x20, 0x20, 0x04, 0x00, 0x80, 0x10, 0x00, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, //0x30
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x40, 0x00, 0x00, 0x02, 0x04, //0x40
    0x00, 0x80, 0x00, 0x00, 0x80, 0x02, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x60
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x70
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x10, 0x80
  }
}; //0xF0

#elif PS2KEYBOARD
static const uint8_t keymatrixmap[2][256] = {
  //Rows:
  // 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F
  { 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x00
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x80, 0x00, //0x10
    0x00, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x00, 0x00, 0x80, 0x08, 0x04, 0x04, 0x04, 0x04, 0x00, //0x20
    0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x00, 0x00, 0x00, 0x10, 0x10, 0x08, 0x08, 0x08, 0x00, //0x30
    0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0x00, 0x00, //0x40
    0x00, 0x00, 0x40, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, //0x50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, //0x60
    0x40, 0x00, 0x01, 0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x20, 0x80, 0x20, 0x40, 0x40, 0x00, 0x00, //0x70
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x02
  }, //0xF0
  //Columns:
  // 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F
  { 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x00
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x00, 0x10, 0x20, 0x04, 0x02, 0x08, 0x00, //0x10
    0x00, 0x10, 0x80, 0x04, 0x40, 0x08, 0x01, 0x00, 0x00, 0x10, 0x80, 0x20, 0x40, 0x02, 0x01, 0x00, //0x20
    0x00, 0x80, 0x10, 0x20, 0x04, 0x02, 0x08, 0x00, 0x00, 0x00, 0x10, 0x04, 0x40, 0x01, 0x08, 0x00, //0x30
    0x00, 0x80, 0x20, 0x02, 0x40, 0x08, 0x01, 0x00, 0x00, 0x00, 0x80, 0x04, 0x20, 0x02, 0x00, 0x00, //0x40
    0x00, 0x00, 0x04, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x20, 0x00, 0x00, //0x50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, //0x60
    0x01, 0x00, 0x80, 0x00, 0x04, 0x00, 0x80, 0x00, 0x00, 0x01, 0x02, 0x08, 0x02, 0x40, 0x00, 0x00, //0x70
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x10, 0x80
  }
}; //0xF0
#endif


struct {
  union {
    uint32_t kv;
    struct {
      uint8_t ke,   //Extratasten SHIFT, STRG, ALT...
              kdummy,
              k,    //Erste gedrückte Taste
              k2;   //Zweite gedrückte Taste
    };
  };
  uint32_t lastkv;
  uint8_t shiftLock;
} kbdData = {0, 0, 0};




/*
  Die Initialisierung der I/O-Ports erfolgt bei $FDA3:
        Die Portbits PB0-PB7 des CIA 1 werden als Eingänge programmiert ($DC03=0).
        Ohne Tastendruck liegt auf den Eingängen wegen der im Portbaustein integrierten Pull-Up-Widerstände ein High-Pegel.
        Die Portbits PA0-PA7 des CIA 1 werden als Ausgänge programmiert ($DC02=255).
  Die eigentliche Keyscan-Routine bei $EA87 macht folgendes:
        Es wird eine 0 (Low-Pegel) durch die Portbits PA0-PA7 geschoben ($DC00=254,253,251,247,239..., siehe KERNAL $EAA3 und $EAD6).
        Werden Nullen bei den Eingangs-Portbits PB0-PB7/$DC01 festgestellt (KERNAL $EAAB), so sind die entsprechende(n) Taste(n) gedrückt.
*/

uint8_t cia1PORTA(void) {

  uint8_t v;

  v = ~cpu.cia1.R[0x02] | (cpu.cia1.R[0x00] & cpu.cia1.R[0x02]);

  if ( gpioRead(PIN_JOY2_1) == 0 ) v &= 0xFE;
  if ( gpioRead(PIN_JOY2_2) == 0 ) v &= 0xFD;
  if ( gpioRead(PIN_JOY2_3) == 0 ) v &= 0xFB;
  if ( gpioRead(PIN_JOY2_4) == 0 ) v &= 0xF7;
  if ( gpioRead(PIN_JOY2_BTN) == 0 ) v &= 0xEF;

/*
    if ( digitalReadFast(PIN_JOY2_1) == 0 ) v &= 0xFE;
  if ( digitalReadFast(PIN_JOY2_2) == 0 ) v &= 0xFD;
  if ( digitalReadFast(PIN_JOY2_3) == 0 ) v &= 0xFB;
  if ( digitalReadFast(PIN_JOY2_4) == 0 ) v &= 0xF7;
  if ( digitalReadFast(PIN_JOY2_BTN) == 0 ) v &= 0xEF;
*/  
  if (!kbdData.kv) return v; //Keine Taste gedrückt

  uint8_t filter = ~cpu.cia1.R[0x01] & cpu.cia1.R[0x03];
  if (kbdData.k) {
    if ( keymatrixmap[1][kbdData.k] & filter)  v &= ~keymatrixmap[0][kbdData.k];
  }

  if (kbdData.ke) {
    if (kbdData.ke & 0x02) { //Shift-links
      if ( keymatrixmap[1][0xff] & filter) v &= ~keymatrixmap[0][0xff];
    }
    if (kbdData.ke & 0x20) { //Shift-rechts
      if ( keymatrixmap[1][0xfe] & filter) v &= ~keymatrixmap[0][0xfe];
    }
    if (kbdData.ke & 0x11) { //Control
      if ( keymatrixmap[1][0xfd] & filter) v &= ~keymatrixmap[0][0xfd];
    }
    if (kbdData.ke & 0x88) { //Windows (=> Commodore)
      if ( keymatrixmap[1][0xfc] & filter) v &= ~keymatrixmap[0][0xfc];
    }
  }
  return v;

}


uint8_t cia1PORTB(void) {

  uint8_t v;

  v = ~cpu.cia1.R[0x03] | (cpu.cia1.R[0x00] & cpu.cia1.R[0x02]) ;

  if ( gpioRead(PIN_JOY1_1) == 0 ) v &= 0xFE;
  if ( gpioRead(PIN_JOY1_2) == 0 ) v &= 0xFD;
  if ( gpioRead(PIN_JOY1_3) == 0 ) v &= 0xFB;
  if ( gpioRead(PIN_JOY1_4) == 0 ) v &= 0xF7;
  if ( gpioRead(PIN_JOY1_BTN) == 0 ) v &= 0xEF;
/*
  if ( digitalReadFast(PIN_JOY1_1) == 0 ) v &= 0xFE;
  if ( digitalReadFast(PIN_JOY1_2) == 0 ) v &= 0xFD;
  if ( digitalReadFast(PIN_JOY1_3) == 0 ) v &= 0xFB;
  if ( digitalReadFast(PIN_JOY1_4) == 0 ) v &= 0xF7;
  if ( digitalReadFast(PIN_JOY1_BTN) == 0 ) v &= 0xEF;
*/  
  if (!kbdData.kv) return v; //Keine Taste gedrückt

  uint8_t filter = ~cpu.cia1.R[0x00] & cpu.cia1.R[0x02];
  if (kbdData.k) {
    if ( keymatrixmap[0][kbdData.k] & filter) v &= ~keymatrixmap[1][kbdData.k];
  }

  if (kbdData.ke) {
    if (kbdData.ke & 0x02) { //Shift-links
      if ( keymatrixmap[0][0xff] & filter) v &= ~keymatrixmap[1][0xff];
    }
    if (kbdData.ke & 0x20) { //Shift-rechts
      if ( keymatrixmap[0][0xfe] & filter) v &= ~keymatrixmap[1][0xfe];
    }
    if (kbdData.ke & 0x11) { //Control
      if ( keymatrixmap[0][0xfd] & filter) v &= ~keymatrixmap[1][0xfd];
    }
    if (kbdData.ke & 0x88) { //Windows (=> Commodore)
      if ( keymatrixmap[0][0xfc] & filter) v &= ~keymatrixmap[1][0xfc];
    }
  }

  return v;
}

#if USBHOST
#include "usb_USBHost.h"

void usbKeyboardmatrix(void * keys) { //Interrupt
  kbdData.kv = *(uint32_t*) keys;//Use only the first 4 bytes


  if (kbdData.kv != kbdData.lastkv) {

    kbdData.lastkv = kbdData.kv;
    if (!kbdData.kv ) return;
    if (hotkey(kbdData.k)) return;

    //Serial.printf("0x%x 0x%x\n", kbdData.ke, kbdData.k);

    //Special Keys
    //RESET
    if (kbdData.ke == 0x05 && kbdData.k == 0x4c) {
      //resetExternal();
      resetMachine();
    }

    else if (kbdData.k == 0x46) { //RESTORE - "Druck"
      kbdData.k = kbdData.k2;
      kbdData.k2 = 0;
      cpu_nmi();
      return;
    }
    else if (kbdData.k2 == 0x46) { //RESTORE - "Druck"
      kbdData.k2 = 0;
      cpu_nmi();
      return;
    }

    //Shift Lock
    if ( kbdData.k == 0x39 ) {
      kbdData.kv = 0;
      kbdData.shiftLock = ~kbdData.shiftLock;
      if (kbdData.shiftLock) {
        //Serial.println("ShiftLock: ON");
		//  keyboard.LEDS(0x02);
      } else {
        //Serial.println("ShiftLock: OFF");
		//  keyboard.LEDS(0x00);
      }
      return;
    }
    if (kbdData.shiftLock) kbdData.ke |= 0x20; //Apply shift-lock by pressing right shift

    //Sondertasten
    //Cursor -> kein Shift
    if ( (kbdData.k == 0x4f) || (kbdData.k == 0x51) ) {
      kbdData.ke &= ~0x22;  //Shift entfernen
      return;
    }
    //Cursor Links => Shift und Cursor Rechts
    else if ( kbdData.k == 0x50 ) {
      kbdData.ke |= 0x20;   //Shift Rechts
      kbdData.k = 0x4f;   //Cursor Rechts
      return;
    }
    //Cursor Hoch => Shift und Cursor Runter
    else if ( kbdData.k == 0x52 ) {
      kbdData.ke |= 0x20;
      kbdData.k = 0x51;   //Cursor runter
      return;
    }
    //F2 => SHIFT + F1, F4 => SHIFT + F3, F6 => SHIFT + F5, F8 => SHIFT + F7
    else if ( kbdData.k == 0x3b || kbdData.k == 0x3d || kbdData.k == 0x3f || kbdData.k == 0x41 ) {
      kbdData.ke |= 0x20;
      kbdData.k -= 1;
      return;
    }

  }

}
#endif

#if PS2KEYBOARD
#define PS2BREAK     0x01
#define PS2MODIFIER  0x02
#define PS2SHIFT_L   0x04
#define PS2SHIFT_R   0x08
#define PS2ALTGR     0x10
#define PS2WINDOWS   0x20
#define PS2CTRL   0x40
#define PS2ALT       0x80

void Ps2Interrupt(void)
{
  /*
      Parts taken from PS2Keyboard library
        http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
  */
  static uint8_t bitcount = 0;
  static uint8_t incoming = 0;
  static uint32_t prev_ms = 0;
  uint32_t now_ms;
  uint8_t n, val;

  val = digitalReadFast(PIN_PS2DATA);
  now_ms = millis();
  if (now_ms - prev_ms > 250) {
    bitcount = 0;
    incoming = 0;
  }
  prev_ms = now_ms;
  n = bitcount - 1;
  if (n <= 7) {
    incoming |= (val << n);
  }
  bitcount++;
  if (bitcount == 11) {
#if 1
    Serial.print(incoming, HEX);
    Serial.println(" ");
#endif

    uint8_t s = incoming;
    bitcount = 0;
    incoming = 0;
    static uint64_t history = 0;

    if (s > 0) {

      history <<= 8;
      history |= s;

      static uint8_t state = 0;
      if (s == 0xF0) {
        state |= PS2BREAK;
        kbdData.k = 0;
        kbdData.ke = 0;
      } else if (s == 0xE0) {
        state |= PS2MODIFIER;
      } else {
        if (state & PS2BREAK) {
          if (s == 0x12) {
            state &= ~PS2SHIFT_L;
          } else if (s == 0x59) {
            state &= ~PS2SHIFT_R;
            //          } else if (s == 0x11 && (state & PS2MODIFIER)) {
            //            state &= ~PS2ALTGR;
            //          } else if (s == 0x11 && !(state & PS2MODIFIER)) {
            //            state &= ~PS2ALT;
          } else if (s == 0x14) { //Both CTRL keys
            state &= ~PS2CTRL;
          } else if (s == 0x1f || s == 0x27) { // Both GUI (Windows) keys
            state &= ~PS2WINDOWS;
          }
          // CTRL, ALT & WIN keys could be added
          // but is that really worth the overhead?
          state &= ~(PS2BREAK | PS2MODIFIER);
          return;
        }
        if (s == 0x12) {
          state |= PS2SHIFT_L;
          return;
        } else if (s == 0x59) {
          state |= PS2SHIFT_R;
          return;
          //        } else if (s == 0x11 && (state & PS2MODIFIER)) {
          //          state |= PS2ALTGR;
          //        } else if (s == 0x11 && !(state & PS2MODIFIER)) {
          //          state |= PS2ALT;
        } else if (s == 0x14) { //Both CTRL keys
          state |= PS2CTRL;
        } else if (s == 0x1f || s == 0x27) { // Both GUI (Windows) keys
          state |= PS2WINDOWS;
        } else if (s == 0x58) { //shift lock
          kbdData.shiftLock = ~kbdData.shiftLock;
        } else {


          switch (s) {
            case 0x71 : if (state == PS2CTRL | PS2ALT) {
                resetMachine();
              }; break;
            case 0x7c : if ((uint32_t)history == 0xE012E07C) {
                cpu_nmi();
                s = 0;
                Serial.println("Restore");
              } break; //RESTORE - "PrintScreen"   - PS2 Scancode is E0,12,E0,7C :-(
            case 0x75 : if (state & PS2MODIFIER) {
                s = 0x72;
                kbdData.ke |= 0x02;
              } break; //Cursor Hoch => Shift und Cursor Runter
            case 0x6b : if (state & PS2MODIFIER) {
                s = 0x74;
                kbdData.ke |= 0x02;
              } break; //Cursor Links => Shift und Cursor Rechts

            case 0x06 : s = 0x05; kbdData.ke |= 0x02; break;// F2 => SHIFT + F1
            case 0x0C : s = 0x04; kbdData.ke |= 0x02; break;// F4 => SHIFT + F3
            case 0x0B : s = 0x03; kbdData.ke |= 0x02; break;// F6 => SHIFT + F5
            case 0x0A : s = 0x83; kbdData.ke |= 0x02; break;// F8 => SHIFT + F7

          }

          if (s) {
			  if (!hotkey(s)) kbdData.k = s;
		  }
        }
        if (state) {
          if (state & PS2SHIFT_L) kbdData.ke |= 0x02;
          if (state & PS2SHIFT_R) kbdData.ke |= 0x20;
          if (state & PS2CTRL) kbdData.ke |= 0x11;
          if (state & PS2WINDOWS) kbdData.ke |= 0x88;
        }

        if (kbdData.shiftLock) kbdData.ke |= 0x20; //Apply shift-lock by pressing right shift

        state &= ~(PS2BREAK | PS2MODIFIER);

      }
    }
  }
}
#endif

void initJoysticks() {
  pinMode(PIN_JOY1_1, INPUT_PULLUP);
  pinMode(PIN_JOY1_2, INPUT_PULLUP);
  pinMode(PIN_JOY1_3, INPUT_PULLUP);
  pinMode(PIN_JOY1_4, INPUT_PULLUP);
  pinMode(PIN_JOY1_BTN, INPUT_PULLUP);

  pinMode(PIN_JOY2_1, INPUT_PULLUP);
  pinMode(PIN_JOY2_2, INPUT_PULLUP);
  pinMode(PIN_JOY2_3, INPUT_PULLUP);
  pinMode(PIN_JOY2_4, INPUT_PULLUP);
  pinMode(PIN_JOY2_BTN, INPUT_PULLUP);
}

void initKeyboard() {
#if USBHOST
  keyboard.attachC64(usbKeyboardmatrix);
 // keyboard.LEDS(0x07);
#elif PS2KEYBOARD
  pinMode(PIN_PS2DATA, INPUT_PULLUP);
  pinMode(PIN_PS2INT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PS2INT), Ps2Interrupt, FALLING);
#endif
}
