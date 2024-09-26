#ifndef AC_CONTROLLER_H
#define AC_CONTROLLER_H

#include <stdint.h>

enum class PowerState
{
    On,
    Off
};

enum class OperationMode
{
    Heating = 0,
    Cooling = 2,
    Dry = 3,
    Fan = 4 
    
};

enum class FanSpeed
{
    Auto,
    High,
    Medium,
    Low
};

class AController
{
public:
    AController();

    void setFanSpeed(FanSpeed speed);
    void setTargetTemperature(uint8_t temp);
    void setPowerState(PowerState state);
    void setOperationMode(OperationMode mode);

    PowerState getPowerState() const { return powerState; }
    OperationMode getOperationMode() const { return operationMode; }
    FanSpeed getFanSpeed() const { return fanSpeed; }
    uint8_t getTargetTemperature() const { return targetTemperature; }

private:
    PowerState powerState;
    OperationMode operationMode;
    FanSpeed fanSpeed;
    uint8_t targetTemperature;

    uint32_t transmitCommand;

    void transmitCurrentState(); // Assumed to handle IR communication
};

#endif // HVAC_CONTROLLER_H