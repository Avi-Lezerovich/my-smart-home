#include "ac_controller.h"

#include <IRremote.hpp> 

#define IR_SEND_PIN 4

constexpr uint8_t MIN_TEMPERATURE = 16;
constexpr uint8_t MAX_TEMPERATURE = 30;

AController::AController() 
    : powerState(PowerState::Off),
      operationMode(OperationMode::Cooling),
      fanSpeed(FanSpeed::Auto),
      targetTemperature(22),
      transmitCommand(0x00000683),
{
    IrSender.begin(IR_SEND_PIN);      
}

void AController::transmitCurrentState()
{
    IrSender.sendPulseDistanceWidth(
        38, 
        9000, 
        4700, 
        500, 
        1800, 
        500, 
        700, 
        transmitCommand, 
        48, 
        PROTOCOL_IS_LSB_FIRST, 
        0, 0);
  
}

void AController::setPowerState(PowerState state)
{
    if (state == powerState)
        return;
    
    powerState = state;
    transmitCommand |= (static_cast<uint32_t>(state) << 18);
    transmitCurrentState();
}

void AController::setOperationMode(OperationMode mode)
{
   if (mode == operationMode)
        return;
    
    operationMode = mode;
    transmitCommand &= 0xF0FFFFFF; 
    transmitCommand |= (static_cast<uint32_t>(mode) << 24);
    transmitCurrentState();
   
}

void AController::setFanSpeed(FanSpeed speed)
{
   if (speed == fanSpeed)
        return;
    
    fanSpeed = speed;
    transmitCommand &= 0xFFF3FFFF;
    transmitCommand |= (static_cast<uint32_t>(speed) << 16);
    transmitCurrentState();
}

void AController::setTargetTemperature(uint32_t temp)
{
    if(temp == targetTemperature)
        return;

    // Ensure temperature is within valid range
    temp = temp >= MIN_TEMPERATURE && temp <= MAX_TEMPERATURE ? temp : targetTemperature; 
    targetTemperature = temp;
    temp -= MIN_TEMPERATURE; // Normalize temperature to 0-14

    temp = (temp << 28); // Shift temperature bits to correct position
    transmitCommand &= 0x0FFFFFFF; // Clear temperature bits  

    transmitCommand |= temp; // Set temperature bits
    transmitCurrentState();
}


PowerState AController::getPowerState() const
{
    return powerState;
}

OperationMode AController::getOperationMode() const
{
    return operationMode;
}

FanSpeed AController::getFanSpeed() const
{
    return fanSpeed;
}

uint8_t AController::getTargetTemperature() const
{
    return targetTemperature;
}

