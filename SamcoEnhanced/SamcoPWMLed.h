#ifdef USES_PWMLED
#define START_RECOIL() SamcoPWMLed::StartRecoilLed();digitalWrite(SamcoPreferences::pins.oSolenoid, HIGH)
#define STOP_RECOIL() SamcoPWMLed::StopRecoilLed();digitalWrite(SamcoPreferences::pins.oSolenoid, LOW)
#else
#define START_RECOIL() digitalWrite(SamcoPreferences::pins.oSolenoid, HIGH)
#define STOP_RECOIL() digitalWrite(SamcoPreferences::pins.oSolenoid, LOW)
#endif
#define FLIP_RECOIL() if(digitalRead(SamcoPreferences::pins.oSolenoid)){ START_RECOIL(); } else { STOP_RECOIL(); }

#ifdef USES_PWMLED
#ifndef _SAMCOPWMLED_H_
#define _SAMCOPWMLED_H_

#include <OpenFIREBoard.h>
#include <stdint.h>
#include "SamcoPreferences.h"



/// @brief Static instance of preferences to save in non-volatile memory
class SamcoPWMLed
{
public:

    enum LedPWMRecoilState {
        LedPWMRecoil_Inactif,
        LedPWMRecoil_Actif,
        LedPWMRecoil_Fade
    };
    
    static LedPWMRecoilState State_LedPWMRecoil;
    static uint8_t Level_LedPWMRecoil;
    static uint32_t lastUpdateTime;
    
    static void PwmLedInit();
    static void PwmLedSleep();
    static void SetLedPWM1Level(int level = -1);
    static void SetLedPWM2Level(int level = -1);
    static void StartRecoilLed();
    static void SetRecoilState(SamcoPWMLed::LedPWMRecoilState newState);
    static void StopRecoilLed();
    static void UpdateRecoilLed();


};

#endif
#endif