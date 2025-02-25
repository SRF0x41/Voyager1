#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEServer.h>
#include <Wire.h>
#include <Arduino.h>

// BLE Server name
#define bleServerName "XIAOESP32S3_BLE"

// Create BLE characteristic pointer
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// BLE Server Callbacks
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    // Restart advertising when client disconnects
    BLEDevice::startAdvertising();
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Hello world");
  // Initialize BLE Device
  BLEDevice::init(bleServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  pServer->setSecurity(BLE_SEC_NONE);

  // Create BLE Service
  BLEService *pService = pServer->createService("12345678-1234-5678-1234-56789abcdef0");

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      BLEUUID("abcdef01-1234-5678-1234-56789abcdef0"),
                      BLECharacteristic::PROPERTY_READ | 
                      BLECharacteristic::PROPERTY_WRITE | 
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  // Set initial value
  pCharacteristic->setValue("Hello from XIAO ESP32S3!");

  // Start BLE Service
  pService->start();

  // Start Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE GATT Server Started!");
}

void loop() {
  if (deviceConnected) {
    // Send updated data to client
    String data = "Sensor Value: 42";
    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify(); // Send notification to client
    Serial.println("Sent: " + data);

    delay(1000);
  }
}
