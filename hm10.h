#include <SoftwareSerial.h>

enum ModuleType { HM10, CC41, MLT_BT05, Unknown };

class HM10
{
  public:
    ModuleType identifyDevice();
    void doCommandAndEchoResult(const char * command, const __FlashStringHelper * meaning);
	ModuleType moduleType;
  private:
	
};