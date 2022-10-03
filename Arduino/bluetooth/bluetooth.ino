#include <SPI.h>
#include "Adafruit_BLE_UART.h"

#define ADAFRUITBLE_REQ 3    // Nano: 3 Uno: 10
#define ADAFRUITBLE_RDY 2     // interrupt pin
#define ADAFRUITBLE_RST 9
#define SW_pin 7
#define X_pin A4
#define Y_pin A3

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void setup(void) {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));
  BTLEserial.setDeviceName("BLeTest"); /* 7 characters max! */
  BTLEserial.begin();
}

void loop() {
  BTLEserial.pollACI();
  aci_evt_opcode_t status = BTLEserial.getState();
  if (status != laststatus) {
    if (status == ACI_EVT_DEVICE_STARTED) Serial.println(F("* Advertising started"));
    if (status == ACI_EVT_CONNECTED) Serial.println(F("* Connected!"));
    if (status == ACI_EVT_DISCONNECTED) Serial.println(F("* Disconnected or advertising timed out"));
    laststatus = status;
  }

  if (status == ACI_EVT_CONNECTED) {
    int sw = digitalRead(SW_pin);
    int xx = analogRead(X_pin);
    int yy = analogRead(Y_pin);
    Serial.print("Switch:  ");
    Serial.print(sw);
    Serial.print("\t");
    Serial.print(xx);
    Serial.print("\t");
    Serial.println(yy);
    String s = String(sw)
               + " - " + String(xx)
               + " - " + String(yy);
    /*uint8_t sendbuffer[20];
    s.getBytes(sendbuffer, 20);
    char sendbuffersize = min(20, s.length());
    */
    delay(1000);
    Serial.print(F("*Sending->\"")); Serial.print(xx); Serial.println("\"");
    BTLEserial.print(xx);
    Serial.println("Fatto");  
    /*if (BTLEserial.available()) {
      Serial.print("* ");
      Serial.print(BTLEserial.available());
      Serial.println(F(" bytes available from BTLE"));
    }
    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }

    if (Serial.available()) {
      Serial.setTimeout(100);
      String s = Serial.readString();
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      BTLEserial.write(sendbuffer, sendbuffersize);
    }*/
  }
  delay(500);
}
