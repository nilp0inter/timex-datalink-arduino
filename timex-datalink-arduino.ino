// This is a working Arduino Uno based replacement for the Notebook Adapter kit.
// It's compatible with the original software, and eliminates the need for a CRT monitor.
// Connect Arduino Uno with RS232 TTL hat to its TX on pin 9, RX on 8. Connect GND and appropriate voltage on VCC.
// Use 1k resistor with red LED between its GND and connect pin 12 to the LED's postive lead.

// Modern lightbulbs and even monitors can interfere with the sync signal. Try to shield it or even turn out the lights.

// Special thanks to Antti Huhtala and excsniper for their info and troubleshooting help!

// Go to Sketch, Include Library, Manage Libraries to search for and add DigitialIO 1.0.0, AltSoftSerial 1.4.0, and SafeString 4.1.15 (millisDelay). 
// TODO: See if libraries can be removed.
#include "led_blaster.h" // Fast digital IO library.
#include "millisDelay.h" // For resetting handshake.

#include "DigiCDC.h"

boolean stophandshake = false; // Handshake will only occur on initial boot and after serial inactivity. 
millisDelay reset_handshake_timer; // Timer library for resetting handshake after inactivity. 
int handshake_payload[] = {0x31, 0x32, 0x36, 0x0D, 0x20, 0x4D, 0x37, 0x36, 0x34, 0x20, 0x72, 0x65, 0x76, 0x20, 0x37, 0x36, 0x34, 0x30, 0x30, 0x32, (byte)0};


void setup() {
  SerialUSB.begin(); 
  LedBlaster::setup();
}

void loop()
{
  if (SerialUSB.available()) {
    if (reset_handshake_timer.justFinished()) {
      stophandshake = false; // Handshake needed again after inactivity.
    }
    int receivedChar = SerialUSB.read();
    if (receivedChar != -1)
    {
      LedBlaster::emit_byte(receivedChar);
      if (receivedChar == 'U') // Handshake requirement is over whenever U received.
      {
        stophandshake = true; 
        SerialUSB.write('U'); // Echo 'U'.
        reset_handshake_timer.start(1000); // Keep resetting inactivity timer after test pattern activity.
      }
      else if (receivedChar == 0x3F) // If ASCII '?' received...
      {
        SerialUSB.write(0x3F); // Always at least echo '?'...
        delay(20);
        if (stophandshake == false) { // But send only the following during initial handshake...
          for (int i = 0; i < 21; i++) { // Send one at a time, because the software is picky about how it's sent.
            SerialUSB.write(handshake_payload[i]); // Reply with "126\r M764 rev 764002(null)" during handshake.
            delay(20);
          }
        }
      }
      else {
        SerialUSB.write(receivedChar); // Always echo any other characters received, or transmission fails.
        reset_handshake_timer.start(1000); // Keep resetting inactivity timer after sync mode activity.
      }
    }
  }
}
