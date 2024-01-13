#include "led_blaster.h"
#include "notebook_adapter.h"
#include "DigiCDC.h"


void setup() {
  SerialUSB.begin(); 
  LedBlaster::setup();
}

void loop() {
  if (SerialUSB.available()) {
    int serial_byte = SerialUSB.read();

    if (serial_byte != -1) {
      NotebookAdapter::emulate(serial_byte);
      LedBlaster::emit_byte(serial_byte);
    }
  }
}
