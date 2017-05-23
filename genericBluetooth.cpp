#include <genericBluetooth.h>
GenericBluetooth::GenericBluetooth(uint8_t rxPin,uint8_t txPin,uint8_t statePin)
{
  bleSerial = new SoftwareSerial(rxPin, txPin);
  bleSerial->begin(BLE_BAUD);
  bleSerial->setTimeout(BLE_TIMEOUT);
  rxPin = rxPin;
  txPin= txPin;
  pinMode(statePin, INPUT);
}

GenericBluetooth::~GenericBluetooth()
{

}

bool GenericBluetooth::determineConnectionState(){
    int state = digitalRead(statePin);
    unsigned long p1 = pulseIn(statePin, HIGH, 2000000);
    unsigned long p2 = pulseIn(statePin, LOW, 2000000);
    if (p1 == p2 && p1 == 0)
    {
      if (state == HIGH)
      {
         // The signal on the state pin is HIGH. DEVICE CONNECTED
        return true;
      }
      else {
        // The signal on the state pin is LOW. DEVICE NOT CONNECTED
      }
    }
    else
    {
       // blinking
  }
  return false;
}


void GenericBluetooth::displayMainSettings(){
  if (moduleType == HM10)
  {
    doCommandAndEchoResult(("AT+HELP?"));
    doCommandAndEchoResult(("AT+VERS?"));
    doCommandAndEchoResult(("AT+NAME?"));
    doCommandAndEchoResult(("AT+PASS?"));
    doCommandAndEchoResult(("AT+ADDR?"));
    doCommandAndEchoResult(("AT+ROLE?"), F("Peripheral=0, Central=1"));
    doCommandAndEchoResult(("AT+POWE?"), F("0 = -23dbm, 1 = -6dbm, 2 = 0dbm, 3 = 6dbm"));
    doCommandAndEchoResult(("AT+MODE?"), F("Transmission Mode=0, PIO collection Mode=1, Remote Control Mode=2"));
    doCommandAndEchoResult(("AT+PIO1?"), F("Behavior of state pin, Blink on disconnect=0, Off on disconnect=1"));
  }
  else if (moduleType == CC41)
  {
    doCommandAndEchoResult(("AT+HELP"));
    doCommandAndEchoResult(("AT+VERSION"));
    doCommandAndEchoResult(("AT+NAME"));
    doCommandAndEchoResult(("AT+PASS"));
    doCommandAndEchoResult(("AT+ADDR"));
    doCommandAndEchoResult(("AT+ROLE"));
    doCommandAndEchoResult(("AT+POWE"), F("0 = -23dbm, 1 = -6dbm, 2 = 0dbm, 3 = 6dbm"));
  }
  else if (moduleType == MLT_BT05)
  {
    doCommandAndEchoResult(("AT+HELP"));
    doCommandAndEchoResult(("AT+VERSION"));
    doCommandAndEchoResult(("AT+NAME"));
    doCommandAndEchoResult(("AT+PIN"));
    doCommandAndEchoResult(("AT+LADDR"));
    doCommandAndEchoResult(("AT+ROLE"));
    doCommandAndEchoResult(("AT+POWE"), F("0 = -23dbm, 1 = -6dbm, 2 = 0dbm, 3 = 6dbm"));
  }
}

void GenericBluetooth::notify(byte notify)
{
  String command(F("AT+NOTI"));
  command += notify;
  doCommandAndEchoResult(command.c_str());
}

void GenericBluetooth::setName(String val)
{
  String command(F("AT+NAME"));
  command += val;
  doCommandAndEchoResult(command.c_str());
}

void GenericBluetooth::setPin(String val)
{
  String command(F("AT+PASS"));
  command += val;
  doCommandAndEchoResult(command.c_str());
}

void GenericBluetooth::setPower(uint8_t dbm)
{
  //int dbm = readInt(F("Enter new module power (0 = -23dbm, 1 = -6dbm, 2 = 0dbm, 3 = 6dbm)"), 2); // 2 is the default
  String command(F("AT+POWE"));
  command += dbm;
  doCommandAndEchoResult(command.c_str());
}
void GenericBluetooth::doCommandAndEchoResult(const char * command, const __FlashStringHelper * meaning = NULL)
{
  if(moduleType!=Unknown){

  
    // send command
    if (moduleType == HM10)
      bleSerial->print(command);
    else
      bleSerial->println(command);
  
    // read and return response
    // don't use "readString", it can't handle long and slow responses (such as AT+HELP) well
    byte b;
    while (bleSerial->readBytes(&b, 1) == 1) // use "readBytes" and not "read" due to the timeout support
      Serial.write(b);
  
    // normalize line end
   if (moduleType == HM10)
      Serial.println();
  } else 
    moduleType=identifyDevice();

}

ModuleType GenericBluetooth::identifyDevice()
{
  //Serial.println(F("Detecting module type"));
  // check for HM-10
  bleSerial->print("AT");
  String s = bleSerial->readString();
  if (s.length() == 2 && s.compareTo("OK")==0)
  {
    Serial.println(F("HM-10 detected!"));
    return HM10;
  }
  else if (s.length() == 0)
  {
    // check for CC41
    bleSerial->println("");
    s = bleSerial->readString();
    if (s.length() == 4 && s.compareTo("OK\r\n") == 0)
    {
      Serial.println(F("CC41 detected!")); // An HM-10 clone
      return CC41;
    }
    else if (s.length() == 0)
    {
      // check for MLT_BT05
      bleSerial->println("AT");
      s = bleSerial->readString();
      if (s.length() == 4 && s.compareTo("OK\r\n") == 0)
      {
        Serial.println(F("MLT-BT05 detected!")); // An HM-10 clone
        return MLT_BT05;
      }
      else if (s.length() == 0)
      {


      }
      else
      {
        //Serial.print(F("Unexpected result of length="));
        //Serial.println(s.length());
        //Serial.println(s);
      }
    }
    else
    {
      //Serial.print(F("Unexpected result of length="));
      //Serial.println(s.length());
      //Serial.println(s);
    }
  }
  else
  {
    //Serial.print(F("Unexpected result of length="));
    //Serial.println(s.length());
    //Serial.println(s);
  }
  return Unknown;
}
