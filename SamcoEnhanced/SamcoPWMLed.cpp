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
        analogWrite(SamcoPreferences::pins.oLedPWMControl1, 
            SamcoPWMLed::PwmValueFromLevel(
                SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM1Level,
                SamcoPreferences::settings.ledPWM1_min,
                SamcoPreferences::settings.ledPWM1_max
            )
        );
    }
    if (SamcoPreferences::pins.oLedPWMControl2 > 0) {
        pinMode(SamcoPreferences::pins.oLedPWMControl2, OUTPUT);
        //analogWriteFreq(1000);
        analogWrite(SamcoPreferences::pins.oLedPWMControl2, 
            SamcoPWMLed::PwmValueFromLevel(
                SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM2Level,
                SamcoPreferences::settings.ledPWM2_min,
                SamcoPreferences::settings.ledPWM2_max
            )
        );
    }
    if (SamcoPreferences::pins.oLedPWMControlRecoil > 0) {
        pinMode(SamcoPreferences::pins.oLedPWMControlRecoil, OUTPUT);
        //analogWriteFreq(1000);
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
    }
    
    SamcoPWMLed::State_LedPWMRecoil = SamcoPWMLed::LedPWMRecoil_Inactif;
    SamcoPWMLed::Level_LedPWMRecoil = 10;
    SamcoPWMLed::lastUpdateTime = 0;
}

void SamcoPWMLed::SetLedPWM1Level(uint8_t level){
    if (SamcoPreferences::pins.oLedPWMControl1 > 0) {
        if(level >= 0) SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM1Level = level;
        analogWrite(SamcoPreferences::pins.oLedPWMControl1, 
            SamcoPWMLed::PwmValueFromLevel(
                SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM1Level,
                SamcoPreferences::settings.ledPWM1_min,
                SamcoPreferences::settings.ledPWM1_max
            )
        );
    }
}

void SamcoPWMLed::SetLedPWM2Level(uint8_t level){
    if (SamcoPreferences::pins.oLedPWMControl2 > 0) {
        if(level >= 0) SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM2Level = level;
        analogWrite(SamcoPreferences::pins.oLedPWMControl2, 
            SamcoPWMLed::PwmValueFromLevel(
                SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWM2Level,
                SamcoPreferences::settings.ledPWM2_min,
                SamcoPreferences::settings.ledPWM2_max
            )
        );
    }
}
    

uint8_t SamcoPWMLed::PwmValueFromLevel(uint8_t level, uint8_t min, uint8_t max)
{
    // Gérer les cas limites
    if (level == 0) return 0;           // Retourne 0 si le niveau est 0
    if (max > 255) max = 255;           // Clamp max à 255
    if (level > 10) level = 10;         // Clamp level à 10

    if(max < min) min = max;
    // Si level == 1, retourne min, si level == 10, retourne max
    if (level == 1) return min;
    if (level == 10) return max;

    // Calculer la valeur interpolée pour les niveaux intermédiaires
    uint8_t pwmValue = min + (((max - min) * (level - 1) + 4) / 9);

    return pwmValue;
}

void SamcoPWMLed::UpdateRecoilLed() {
    // Ne traiter que si l'état est LedPWMRecoil_Fade ou LedPWMRecoil_Actif
    if (SamcoPreferences::pins.oLedPWMControlRecoil < 0 || SamcoPWMLed::State_LedPWMRecoil == SamcoPWMLed::LedPWMRecoil_Actif) {
        return; // Pas de réduction nécessaire si la LED est à pleine intensité
    }

    uint32_t currentTime = millis(); // Temps actuel

    // Vérifie si le délai entre les mises à jour est écoulé
    if (currentTime - SamcoPWMLed::lastUpdateTime >= (SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWMRecoilFadeDuration / 10)) {
        SamcoPWMLed::lastUpdateTime = currentTime;

        // Calculer la vitesse de réduction proportionnelle
        float speedFactor = 1.0f + 2.0f * (float(SamcoPWMLed::Level_LedPWMRecoil) / 10.0f); // Entre 1.0 et 3.0
        float reductionStep = (10.0f / SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWMRecoilFadeDuration) * speedFactor;

        // Réduire le niveau
        SamcoPWMLed::Level_LedPWMRecoil -= reductionStep;
        if (SamcoPWMLed::Level_LedPWMRecoil < 0) SamcoPWMLed::Level_LedPWMRecoil = 0; // Éviter les valeurs négatives

        // Appliquer la nouvelle valeur de PWM
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 
            SamcoPWMLed::PwmValueFromLevel(
                SamcoPWMLed::Level_LedPWMRecoil,
                SamcoPreferences::settings.ledPWMRecoil_min,
                SamcoPreferences::settings.ledPWMRecoil_max
            )
        );

        // Vérifier si la luminosité est tombée au minimum
        if (SamcoPWMLed::Level_LedPWMRecoil <= SamcoPreferences::settings.ledPWMRecoil_min) {
            analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
            SamcoPWMLed::Level_LedPWMRecoil = 0;
            SamcoPWMLed::State_LedPWMRecoil = SamcoPWMLed::LedPWMRecoil_Inactif; // Passer à l'état inactif
        }
    }
}

void SamcoPWMLed::SetRecoilState(SamcoPWMLed::LedPWMRecoilState newState) {
    if (SamcoPWMLed::State_LedPWMRecoil == newState) {
        return; // Pas de changement d'état
    }

    SamcoPWMLed::State_LedPWMRecoil = newState;
    if (newState == SamcoPWMLed::LedPWMRecoil_Actif) {
        // Si l'état devient actif, on initialise la LED à pleine intensité
        SamcoPWMLed::Level_LedPWMRecoil = 10;
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 
            SamcoPWMLed::PwmValueFromLevel(SamcoPWMLed::Level_LedPWMRecoil, 
                SamcoPreferences::settings.ledPWMRecoil_min, 
                SamcoPreferences::settings.ledPWMRecoil_max)
        );
    } else if (newState == SamcoPWMLed::LedPWMRecoil_Fade) {
        // Démarrer l'état Fade (réduction de la luminosité)
        SamcoPWMLed::Level_LedPWMRecoil = 10;
    } else if (newState == SamcoPWMLed::LedPWMRecoil_Inactif) {
        // Éteindre la LED si elle est inactif
        analogWrite(SamcoPreferences::pins.oLedPWMControlRecoil, 0);
        SamcoPWMLed::Level_LedPWMRecoil = 0;
    }
}

void SamcoPWMLed::StartRecoilLed(){
    if (SamcoPreferences::pins.oLedPWMControl1 > 0) {
        SamcoPWMLed::SetRecoilState(SamcoPWMLed::LedPWMRecoil_Actif);
    }
}

void SamcoPWMLed::StopRecoilLed(){
    if (SamcoPreferences::pins.oLedPWMControl1 > 0) {
        if(SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].ledPWMRecoilFadeDuration > 0) SamcoPWMLed::SetRecoilState(SamcoPWMLed::LedPWMRecoil_Fade);
        else SamcoPWMLed::SetRecoilState(SamcoPWMLed::SamcoPWMLed::LedPWMRecoil_Inactif);
    }
}
#endif