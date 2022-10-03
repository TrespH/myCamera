#include <SPI.h>
#include <BLEPeripheral.h>

#define REQ 3    // Nano: 3 Uno: 10
#define RDY 2     
#define RST 9
#define X_pin A4
#define Y_pin A3

BLEPeripheral blePeripheral = BLEPeripheral(REQ, RDY, RST);
BLEService padService = BLEService("BBB0"); //1801
BLEIntCharacteristic xxCharacteristic = BLEIntCharacteristic("BBB1", BLERead | BLENotify); //2A58
BLEDescriptor xxDescriptor = BLEDescriptor("2901", "xx");
BLEIntCharacteristic yyCharacteristic = BLEIntCharacteristic("BBB2", BLERead | BLENotify);
BLEDescriptor yyDescriptor = BLEDescriptor("2901", "yy");
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

unsigned long previousMillis = 0;
unsigned short interval = 100;
long x_pad, y_pad;
int rightSpeed, leftSpeed;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  blePeripheral.setLocalName("PadAnalog");
  blePeripheral.setAdvertisedServiceUuid(padService.uuid());
  blePeripheral.addAttribute(padService);
  blePeripheral.addAttribute(xxCharacteristic);
  blePeripheral.addAttribute(xxDescriptor);
  blePeripheral.addAttribute(yyCharacteristic);
  blePeripheral.addAttribute(yyDescriptor);
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  blePeripheral.begin();

  Serial.println("BLE Pad Sensor Test");
}

void loop() {
  blePeripheral.poll();
  if (millis() - previousMillis > interval) {
    setPadCharacteristicValue();
    previousMillis = millis();
  }
}

void setPadCharacteristicValue() {
  x_pad = map(analogRead(X_pin), 0, 1023, 0, 511) - 255;
  y_pad = map(analogRead(Y_pin), 0, 1023, 0, 511) - 255;
  Serial.print(x_pad);
  Serial.print("\t");
  Serial.print(y_pad);
  Serial.print("\t");
  adaptSpeeds();
  Serial.print(leftSpeed);
  Serial.print("\t");
  Serial.println(rightSpeed);
  xxCharacteristic.setValue(rightSpeed);
  yyCharacteristic.setValue(leftSpeed);
}

void adaptSpeeds() { // SET DELLE VELOCITA' NEI 4 QUADRANTI
  if (y_pad > 10 && x_pad > 10) {
    if (y_pad > x_pad) {
      leftSpeed = y_pad;
      rightSpeed = 0;
    }
    else {
      leftSpeed = x_pad;
      rightSpeed = abs(x_pad - y_pad);
    }
  }
  //2
  else if (y_pad < -10 && x_pad > 10) {
    if (abs(y_pad) > x_pad) {
      rightSpeed = abs(y_pad);
      leftSpeed = 0;
    }
    else {
      rightSpeed = x_pad;
      leftSpeed = abs(x_pad + y_pad);
    }
  }
  //3
  else if (y_pad < -10 && x_pad < -10) {
    rightSpeed = abs(y_pad) - abs(x_pad);
    if (abs(y_pad) >  abs(x_pad)) leftSpeed = 0;
    else leftSpeed = x_pad - y_pad;
  }
  //4
  else if (y_pad > 10 && x_pad < -10) {
    leftSpeed = y_pad - abs(x_pad);
    if (y_pad > abs(x_pad)) rightSpeed = 0;
    else rightSpeed = y_pad + x_pad;
  }
  
  else {
    rightSpeed = x_pad;
    leftSpeed = rightSpeed;
  }
  
  
  if (x_pad < 10 && x_pad > -10) {
    if (y_pad > 10) {
      rightSpeed = 0;
      leftSpeed = abs(y_pad);
    }
    else if (y_pad < -10) {
      rightSpeed = abs(y_pad);
      leftSpeed = 0;
    }
    else leftSpeed = 0;
  }
  /*if (rightSpeed >= 0 && rightSpeed % 2 != 0) rightSpeed--; 
  else if (rightSpeed < 0 && rightSpeed % 2 == 0) rightSpeed--; 
  if (leftSpeed >= 0 && leftSpeed % 2 != 0) leftSpeed--; 
  else if (leftSpeed < 0 && leftSpeed % 2 == 0) leftSpeed--;*/
  rightSpeed = (rightSpeed + 255) / 2;
  leftSpeed = (leftSpeed + 255) / 2;
  rightSpeed = abs(rightSpeed);
  leftSpeed = abs(leftSpeed);
}
void blePeripheralConnectHandler(BLECentral& central) {
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}
