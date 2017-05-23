#include <SoftwareSerial.h>
#include <Arduino.h>

#ifndef HM10_h
#define HM10_h

enum ModuleType { HM10, CC41, MLT_BT05, Unknown };

#define BLE_BAUD 9600
#define BLE_TIMEOUT 250

class GenericBluetooth{
  public:
	GenericBluetooth(uint8_t rxPin, uint8_t txPin, uint8_t statePin);
 	~GenericBluetooth();
    ModuleType identifyDevice();
	void doCommandAndEchoResult(const char * command, const __FlashStringHelper * meaning = NULL);
	bool determineConnectionState();
	void displayMainSettings();
	void setName(String val);
	void setPin(String val);
	void setPower(uint8_t dbm);
	bool hasBytes();
	void notify(byte notify);
	SoftwareSerial * bleSerial;
	ModuleType moduleType;
  private:
	uint8_t rxPin,txPin,statePin;
};
#endif
