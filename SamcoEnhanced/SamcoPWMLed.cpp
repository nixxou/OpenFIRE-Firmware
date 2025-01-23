#ifdef USES_PWMLED
#include "SamcoPWMLed.h"

SamcoPWMLed::LedPWMRecoilState SamcoPWMLed::State_LedPWMRecoil = SamcoPWMLed::LedPWMRecoil_Inactif;
uint32_t SamcoPWMLed::lastUpdateTime = 0;
uint8_t SamcoPWMLed::Level_LedPWMRecoil = 0;


void SamcoPWMLed::PwmLedInit()
{
    if (SamcoPreferences::pins.oLedPWMControl1 > 0) {
        pinMode(SamcoPreferences::pins.oLedPWMControl1, OUTPUT);
        //analogWriteFreq(1000); // Fréquence seulement
        analogWrite(SamcoPreferences::pins.oLedPWMControl1, SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM1Level);
    }
    if (SamcoPreferences::pins.oLedPWMControl2 > 0) {
        pinMode(SamcoPreferences::pins.oLedPWMControl2, OUTPUT);
        //analogWriteFreq(1000);
        analogWrite(SamcoPreferences::pins.oLedPWMControl2, SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM2Level);
    }
    if (SamcoPreferences::pins.oLedPWMControlRecoil > 0) {
        pinMode(SamcoPreferences::pins.oLedPWMControlRecoil, OUTPUT);
        //analogWriteFreq(1000);
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
    }
    
    SamcoPWMLed::State_LedPWMRecoil = SamcoPWMLed::LedPWMRecoil_Inactif;
    SamcoPWMLed::Level_LedPWMRecoil = SamcoPreferences::settings.ledPWMRecoil_max;
    SamcoPWMLed::lastUpdateTime = 0;
}

void SamcoPWMLed::PwmLedSleep()
{
    if (SamcoPreferences::pins.oLedPWMControl1 > 0) analogWrite(SamcoPreferences::pins.oLedPWMControl1, 0);
    if (SamcoPreferences::pins.oLedPWMControl2 > 0) analogWrite(SamcoPreferences::pins.oLedPWMControl2, 0);
    if (SamcoPreferences::pins.oLedPWMControlRecoil > 0) analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
    SamcoPWMLed::State_LedPWMRecoil = SamcoPWMLed::LedPWMRecoil_Inactif;
    SamcoPWMLed::Level_LedPWMRecoil = SamcoPreferences::settings.ledPWMRecoil_max;
    SamcoPWMLed::lastUpdateTime = 0;
}

void SamcoPWMLed::SetLedPWM1Level(int level){
    if(level >= 255) level = 255;
    if (SamcoPreferences::pins.oLedPWMControl1 > 0) {
        if(level >= 0) SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM1Level = level;
        analogWrite(SamcoPreferences::pins.oLedPWMControl1, SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM1Level);
    }
}

void SamcoPWMLed::SetLedPWM2Level(int level){
    if(level >= 255) level = 255;   
    if (SamcoPreferences::pins.oLedPWMControl2 > 0) {
        if(level >= 0) SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM2Level = level;     
        analogWrite(SamcoPreferences::pins.oLedPWMControl2, SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM2Level);
    }
}
    

void SamcoPWMLed::UpdateRecoilLed() {
    // Ne traiter que si l'état est LedPWMRecoil_Fade ou LedPWMRecoil_Actif
    if (SamcoPreferences::pins.oLedPWMControlRecoil < 0) {
        return; // Pas de réduction nécessaire si la LED est à pleine intensité
    }
    if(SamcoPWMLed::State_LedPWMRecoil == SamcoPWMLed::LedPWMRecoil_Fade) {
        uint32_t currentTime = millis(); // Temps actuel

        // Vérifie si le délai entre les mises à jour est écoulé
        if (currentTime - SamcoPWMLed::lastUpdateTime >= (SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWMRecoilFadeDuration / (((float)SamcoPreferences::settings.ledPWMRecoil_max) * 10.5f))) {
            SamcoPWMLed::lastUpdateTime = currentTime;
            float progress = ((float)SamcoPWMLed::Level_LedPWMRecoil / (float)SamcoPreferences::settings.ledPWMRecoil_max);
            float speedFactor = 20.0f * progress + 1.0f; // Entre 1.0 (début) et 6.0 (fin)
            float reductionStep = (((float)SamcoPreferences::settings.ledPWMRecoil_max) / SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWMRecoilFadeDuration) * speedFactor;
            uint8_t previousLevel = SamcoPWMLed::Level_LedPWMRecoil;

            SamcoPWMLed::Level_LedPWMRecoil -= reductionStep;
            if (SamcoPWMLed::Level_LedPWMRecoil < 0) SamcoPWMLed::Level_LedPWMRecoil = 0; // Éviter les valeurs négatives

            analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, SamcoPWMLed::Level_LedPWMRecoil);

            if (SamcoPWMLed::Level_LedPWMRecoil <= SamcoPreferences::settings.ledPWMRecoil_min) {
                analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
                SamcoPWMLed::Level_LedPWMRecoil = 0;
                SamcoPWMLed::State_LedPWMRecoil = SamcoPWMLed::LedPWMRecoil_Inactif; // Passer à l'état inactif
            }
        }


    }

}

void SamcoPWMLed::SetRecoilState(SamcoPWMLed::LedPWMRecoilState newState) {
    if (SamcoPWMLed::State_LedPWMRecoil == newState && newState != SamcoPWMLed::LedPWMRecoil_Inactif) {
        return; // Pas de changement d'état
    }
    SamcoPWMLed::State_LedPWMRecoil = newState;
    if (newState == SamcoPWMLed::LedPWMRecoil_Actif && SamcoPreferences::settings.ledPWMRecoil_max > 0) {
        // Si l'état devient actif, on initialise la LED à pleine intensité
        SamcoPWMLed::Level_LedPWMRecoil = SamcoPreferences::settings.ledPWMRecoil_max;
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, SamcoPreferences::settings.ledPWMRecoil_max);
    } else if (newState == SamcoPWMLed::LedPWMRecoil_Fade) {
        // Démarrer l'état Fade (réduction de la luminosité)
        SamcoPWMLed::Level_LedPWMRecoil = SamcoPreferences::settings.ledPWMRecoil_max;
    } else if (newState == SamcoPWMLed::LedPWMRecoil_Inactif) {
        // Éteindre la LED si elle est inactif
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
        SamcoPWMLed::Level_LedPWMRecoil = 0;
    }
}

void SamcoPWMLed::StartRecoilLed(){
    if (SamcoPreferences::pins.oLedPWMControlRecoil > 0) {
        SamcoPWMLed::SetRecoilState(SamcoPWMLed::LedPWMRecoil_Actif);
    }
}

void SamcoPWMLed::StopRecoilLed(){
    if (SamcoPreferences::pins.oLedPWMControlRecoil > 0) {
        if (SamcoPWMLed::State_LedPWMRecoil == SamcoPWMLed::LedPWMRecoil_Actif){
            if(SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWMRecoilFadeDuration > 0) SamcoPWMLed::SetRecoilState(SamcoPWMLed::LedPWMRecoil_Fade);
            else SamcoPWMLed::SetRecoilState(SamcoPWMLed::SamcoPWMLed::LedPWMRecoil_Inactif);
        }
    }
}
#endif