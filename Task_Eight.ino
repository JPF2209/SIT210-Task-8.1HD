#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");  // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int blue = 2;
const int green = 3;
const int yellow = 4;
const int red = 5;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  digitalWrite(blue, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to control.");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        int distance = switchCharacteristic.value();
        Serial.println(distance);
        
        if (distance <= 15) 
        {
          digitalWrite(blue, LOW);
          digitalWrite(green, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(red, HIGH);
        } 
        else if (distance <= 30) 
        {
          digitalWrite(blue, LOW);
          digitalWrite(green, LOW);
          digitalWrite(yellow, HIGH);
          digitalWrite(red, LOW);
        } 
        else if (distance <= 60) 
        {
          digitalWrite(blue, LOW);
          digitalWrite(green, HIGH);
          digitalWrite(yellow, LOW);
          digitalWrite(red, LOW);
        } 
        else 
        {
          digitalWrite(blue, HIGH);
          digitalWrite(green, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(red, LOW);
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from control."));
    Serial.println(central.address());
  }
}
