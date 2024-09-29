#include "ac_controller.h"
#include <IRremote.h>

#define IR_SEND_PIN 4



constexpr uint8_t MIN_TEMPERATURE = 16;
constexpr uint8_t MAX_TEMPERATURE = 30;



void AcController::AcControllerBegin() 
{
    IrSender.begin(IR_SEND_PIN);  
    powerState = PowerState::Off;
    operationMode = OperationMode::Cooling;
    fanSpeed = FanSpeed::Auto;
    targetTemperature = 22;
    transmitData = 0x02000683;    
}

void AcController::transmitCurrentState()
{   
      uint64_t c = transmitData;
      IrSender.sendPulseDistanceWidth(38, 
        9050, 
        4650, 
        550, 
        1750, 
        550, 
        600, 
        c, 
        48, 
        PROTOCOL_IS_LSB_FIRST, 
        0, 
        0);

}

void AcController::setPowerState(PowerState state)
{
    powerState = state;
    transmitData &= 0xFFFBFFFF;
    transmitData |= 1 << 18;
    transmitCurrentState();
    transmitData &= 0xFFFBFFFF;
}

void AcController::setOperationMode(OperationMode mode)
{
    operationMode = mode; 
    transmitData &= 0xF0FFFFFF; 
    transmitData |= (static_cast<uint64_t>(mode) << 24);
    transmitCurrentState();
}

void AcController::setFanSpeed(FanSpeed speed)
{
    fanSpeed = speed;
    transmitData &= 0xFFFCFFFF;
    transmitData |= (static_cast<uint64_t>(speed) << 16);
    transmitCurrentState();
}

void AcController::setTargetTemperature(uint64_t temp)
{
    // Ensure temperature is within valid range
    temp = temp >= MIN_TEMPERATURE && temp <= MAX_TEMPERATURE ? temp : targetTemperature; 
    targetTemperature = temp;
    
    temp = (temp - MIN_TEMPERATURE) << 28; // Shift temperature bits to correct position
    transmitData &= 0x0FFFFFFF; // Clear temperature bits  

    transmitData |= temp; // Set temperature bits
    transmitCurrentState();
}

