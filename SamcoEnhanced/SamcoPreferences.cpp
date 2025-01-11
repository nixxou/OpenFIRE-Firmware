/*!
 * @file SamcoPreferences.cpp
 * @brief Samco Prow Enhanced light gun preferences to save in non-volatile memory.
 *
 * @copyright Mike Lynch, 2021
 * @copyright GNU Lesser General Public License
 *
 * @author Mike Lynch
 * @author [That One Seong](SeongsSeongs@gmail.com)
 * @version V1.1
 * @date 2023
 */

#include "SamcoPreferences.h"
#include <Arduino.h>
#include <LittleFS.h>


// 4 byte header ID
/*
 * The latter two characters of the header correlate to the Save Table Version (see SamcoPreferences.h).
 * If the save table is ever changed (values added/removed or changed order/size),
 * the version number MUST be incremented in tandem so that the firmware will
 * appropriately update previously saved tables (TODO) or reset current NVRAM.
 * Failure to do this might cause save corruption or other undefined behavior.
 */
//const SamcoPreferences::HeaderId_t SamcoPreferences::HeaderId = {'N', 'I', 'X', '1'};

SamcoPreferences::TemporarySerialSettings_t SamcoPreferences::temporarySerialSettings;
String SamcoPreferences::baseJson = "";
bool SamcoPreferences::fsInitialized = false;


int SamcoPreferences::LoadProfiles()
{
  Serial.println("LoadProfiles()");
  
  if(!SamcoPreferences::fsInitialized){
     baseJson = SamcoPreferences::structuresToJson();
  // Initialiser LittleFS
      if (!LittleFS.begin()) {
        Serial.println("Erreur: Impossible de monter le système de fichiers.");
        return Error_NoStorage;
      }
      Serial.println("LittleFS monté avec succès.");  
      SamcoPreferences::fsInitialized = true;
  }

  // Lecture avec un buffer
  if (LittleFS.exists("/config.txt")) {
    File file = LittleFS.open("/config.txt", "r");
    if (file) {
      String fileContent = "";
      //Serial.println("Contenu de config.txt (lecture avec un buffer) :");
      char buffer[512 + 1];  // +1 pour le caractère nul (terminaison de chaîne)
      size_t bytesRead = 0;

      while (file.available()) {
        bytesRead = file.readBytes(buffer, 512);
        buffer[bytesRead] = '\0';  // Assurez-vous que la chaîne est terminée
        //Serial.print(buffer);
        fileContent += buffer;
      }
      file.close();
      //Serial.println();  // Pour finir la ligne après la lecture
      SamcoPreferences::JsonToStructures(fileContent);
      return Error_Success; 
    } else {
      return Error_Read;
      //Serial.println("Erreur: Impossible de lire config.txt.");
    }
  }
  
  return Error_Read;  
    
}

int SamcoPreferences::SaveProfiles()
{
    
  String JsonData = SamcoPreferences::structuresToJson();
  size_t taille = JsonData.length();

  Serial.print("La taille de la chaîne est : ");
  Serial.println(taille);  
  
  // Écriture avec un buffer
  File file = LittleFS.open("/config.txt", "w");
  if (file) {
    size_t bytesWritten = 0;
    while (bytesWritten < taille) {
      size_t chunkSize = min(512, taille - bytesWritten);
      file.write((const uint8_t*)JsonData.c_str() + bytesWritten, chunkSize);
      bytesWritten += chunkSize;
    }
    file.close();
    Serial.println("Écriture réussie avec un buffer dans config.txt.");
  } else {
    Serial.println("Erreur: Impossible d'écrire dans config.txt.");
  }    

    return Error_Success;
}


void SamcoPreferences::ResetPreferences()
{
    SamcoPreferences::JsonToStructures(baseJson);
    SamcoPreferences::SaveProfiles();
}

void SamcoPreferences::LoadPresets()
{
// For the Adafruit ItsyBitsy RP2040 - optimized for SAMCO boards
#ifdef ARDUINO_ADAFRUIT_ITSYBITSY_RP2040

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = -1;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 24;
    pins.oSolenoid = 25;
    pins.bTrigger = 6;
    pins.bGunA = 27;
    pins.bGunB = 26;
    pins.bGunC = 11;
    pins.bStart = 28;
    pins.bSelect = 29;
    pins.bGunUp = 9;
    pins.bGunDown = 7;
    pins.bGunLeft = 8;
    pins.bGunRight = 10;
    pins.bPedal = 4;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = -1;

// For the Adafruit KB2040 - optimized for GUN4IR boards
#elifdef ARDUINO_ADAFRUIT_KB2040_RP2040

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A0;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 5;
    pins.oSolenoid = 7;
    pins.bTrigger = A2;
    pins.bGunA = A3;
    pins.bGunB = 4;
    pins.bGunC = 6;
    pins.bStart = 9;
    pins.bSelect = 8;
    pins.bGunUp = 18;
    pins.bGunDown = 20;
    pins.bGunLeft = 19;
    pins.bGunRight = 10;
    pins.bPedal = -1;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = A1;

// For the Arduino Nano RP2040 Connect - because it was requested
#elifdef ARDUINO_NANO_RP2040_CONNECT

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A2;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

      // Button Pins setup
    pins.oRumble = 17;
    pins.oSolenoid = 16;
    pins.bTrigger = 15;
    pins.bGunA = 0;
    pins.bGunB = 1;
    pins.bGunC = 18;
    pins.bStart = 19;
    pins.bSelect = 20;
    pins.bGunUp = -1;
    pins.bGunDown = -1;
    pins.bGunLeft = -1;
    pins.bGunRight = -1;
    pins.bPedal = -1;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = -1;

// For the Waveshare RP2040 Zero - smallest/cheapest board
#elifdef ARDUINO_WAVESHARE_RP2040_ZERO

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A3;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = -1;
        pins.oLedG = -1;
        pins.oLedB = -1;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 17;
    pins.oSolenoid = 16;
    pins.bTrigger = 0;
    pins.bGunA = 1;
    pins.bGunB = 2;
    pins.bGunC = 3;
    pins.bStart = 4;
    pins.bSelect = 5;
    pins.bGunUp = -1;
    pins.bGunDown = -1;
    pins.bGunLeft = -1;
    pins.bGunRight = -1;
    pins.bPedal = -1;
    pins.bPedal2 = -1;
    pins.bPump = -1;
    pins.bHome = -1;

// For the Raspberry Pi Pico - first party baybeeee
#elif defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_W)

    #ifdef USES_SOLENOID
        #ifdef USES_TEMP    
            pins.aTMP36 = A2;
        #endif // USES_TEMP
    #endif // USES_SOLENOID

      // Remember: PWM PINS ONLY!
    #ifdef FOURPIN_LED
        #define LED_ENABLE
        pins.oLedR = 10;
        pins.oLedG = 11;
        pins.oLedB = 12;
    #endif // FOURPIN_LED

      // Any digital pin is fine for NeoPixels.
    #ifdef CUSTOM_NEOPIXEL
        #define LED_ENABLE
        pins.oPixel = -1;
    #endif // CUSTOM_NEOPIXEL

    pins.oRumble = 17;
    pins.oSolenoid = 16;
    pins.bTrigger = 15;
    pins.bGunA = 0;
    pins.bGunB = 1;
    pins.bGunC = 2;
    pins.bStart = 3;
    pins.bSelect = 4;
    pins.bGunUp = 6;
    pins.bGunDown = 7;
    pins.bGunLeft = 8;
    pins.bGunRight = 9;
    pins.bPedal = 14;
    pins.bPedal2 = -1;
    pins.bPump = 13;
    pins.bHome = 5;

    #endif // ARDUINO_BOARD

    PresetCam();
}

void SamcoPreferences::PresetCam()
{
#if defined(ARDUINO_ADAFRUIT_ITSYBITSY_RP2040) || defined(ARDUINO_ADAFRUIT_KB2040_RP2040)
    pins.pCamSCL = 3;
    pins.pCamSDA = 2;
#elifdef ARDUINO_NANO_RP2040_CONNECT
    pins.pCamSCL = 13;
    pins.pCamSDA = 12;
#elifdef ARDUINO_WAVESHARE_RP2040_ZERO
    pins.pCamSCL = 15;
    pins.pCamSDA = 14;
#else // RASPBERRY_PI_PICO et al
    pins.pCamSCL = 21;
    pins.pCamSDA = 20;
#endif // ARDUINO_BOARD
}

#if defined(ARDUINO_RASPBERRY_PI_PICO_W) && defined(USES_WEBSERVER)
/*
String SamcoPreferences::exportToJson() {
  DynamicJsonDocument doc(1024);
  
  doc["apName"] = SamcoPreferences::settings.apName;
  doc["apPassword"] = SamcoPreferences::settings.apPassword;
  doc["serverPort"] = SamcoPreferences::settings.serverPort;

  String jsonString;
  serializeJsonPretty(doc, jsonString);
  
  return jsonString;
}

bool SamcoPreferences::saveJsonToStruct(const String& jsonPayload) {
  DynamicJsonDocument doc(1024);
  
  DeserializationError error = deserializeJson(doc, jsonPayload);
  if (error) {
    return false;
  }

  strlcpy(SamcoPreferences::settings.apName, doc["apName"] | "", sizeof(SamcoPreferences::settings.apName));
  strlcpy(SamcoPreferences::settings.apPassword, doc["apPassword"] | "", sizeof(SamcoPreferences::settings.apPassword));
  SamcoPreferences::settings.serverPort = doc["serverPort"] | 80;
  
  return true;
}
*/
#endif


String  SamcoPreferences::structuresToJson() {
    // Créer un document JSON dynamique
    DynamicJsonDocument doc(2048);
    
    JsonArray profilesJson = doc.createNestedArray("profiles");

    for (uint8_t i = 0; i < SamcoPreferences::profiles.profileCount; ++i) {
        const SamcoPreferences::ProfileData_t& profile = SamcoPreferences::profiles.pProfileData[i];
        JsonObject profileJson = profilesJson.createNestedObject();

        profileJson["topOffset"] = profile.topOffset;
        profileJson["bottomOffset"] = profile.bottomOffset;
        profileJson["leftOffset"] = profile.leftOffset;
        profileJson["rightOffset"] = profile.rightOffset;
        profileJson["TLled"] = profile.TLled;
        profileJson["TRled"] = profile.TRled;
        profileJson["adjX"] = profile.adjX;
        profileJson["adjY"] = profile.adjY;
        profileJson["irSensitivity"] = profile.irSensitivity;
        profileJson["runMode"] = profile.runMode;
        profileJson["buttonMask"] = profile.buttonMask;
        profileJson["irLayout"] = profile.irLayout;
        
        String hexColor = String(profile.color, HEX);
        while (hexColor.length() < 6) {
            hexColor = "0" + hexColor;
        }
        profileJson["color"] = "#" + hexColor;

        profileJson["name"] = profile.name;
        
        profileJson["rumbleIntensity"] = profile.rumbleIntensity;
        profileJson["rumbleInterval"] = profile.rumbleInterval;
        profileJson["solenoidNormalInterval"] = profile.solenoidNormalInterval;
        profileJson["solenoidFastInterval"] = profile.solenoidFastInterval;
        profileJson["solenoidLongInterval"] = profile.solenoidLongInterval;
        profileJson["autofireWaitFactor"] = profile.autofireWaitFactor;
        
        hexColor = String(profile.customLEDcolor1, HEX);
        while (hexColor.length() < 6) {
            hexColor = "0" + hexColor;
        }
        profileJson["customLEDcolor1"] = "#" + hexColor;
        hexColor = String(profile.customLEDcolor2, HEX);
        while (hexColor.length() < 6) {
            hexColor = "0" + hexColor;
        }
        profileJson["customLEDcolor2"] = "#" + hexColor;
        hexColor = String(profile.customLEDcolor3, HEX);
        while (hexColor.length() < 6) {
            hexColor = "0" + hexColor;
        }
        profileJson["customLEDcolor3"] = "#" + hexColor;

        profileJson["ledPWM1Level"] = profile.ledPWM1Level;
        profileJson["ledPWM2Level"] = profile.ledPWM2Level;
        profileJson["ledPWMRecoilFadeDuration"] = profile.ledPWMRecoilFadeDuration;
        
        profileJson["rumbleActive"] = profile.rumbleActive;
        profileJson["solenoidActive"] = profile.solenoidActive;
        profileJson["autofireActive"] = profile.autofireActive;
        profileJson["rumbleFF"] = profile.rumbleFF;
    }

    // Ajouter le profil sélectionné
    doc["selectedProfile"] = SamcoPreferences::profiles.selectedProfile;    

    // Ajouter les valeurs de la structure TogglesMap_t
    JsonObject togglesJson = doc.createNestedObject("toggles");
    togglesJson["customPinsInUse"] = SamcoPreferences::toggles.customPinsInUse;
    togglesJson["simpleMenu"] = SamcoPreferences::toggles.simpleMenu;
    togglesJson["holdToPause"] = SamcoPreferences::toggles.holdToPause;
    togglesJson["commonAnode"] = SamcoPreferences::toggles.commonAnode;
    togglesJson["lowButtonMode"] = SamcoPreferences::toggles.lowButtonMode;

    // Ajouter les valeurs de la structure PinsMap_t
    JsonObject pinsJson = doc.createNestedObject("pins");
    pinsJson["bTrigger"] = SamcoPreferences::pins.bTrigger;
    pinsJson["bGunA"] = SamcoPreferences::pins.bGunA;
    pinsJson["bGunB"] = SamcoPreferences::pins.bGunB;
    pinsJson["bStart"] = SamcoPreferences::pins.bStart;
    pinsJson["bSelect"] = SamcoPreferences::pins.bSelect;
    pinsJson["bGunUp"] = SamcoPreferences::pins.bGunUp;
    pinsJson["bGunDown"] = SamcoPreferences::pins.bGunDown;
    pinsJson["bGunLeft"] = SamcoPreferences::pins.bGunLeft;
    pinsJson["bGunRight"] = SamcoPreferences::pins.bGunRight;
    pinsJson["bGunC"] = SamcoPreferences::pins.bGunC;
    pinsJson["bPedal"] = SamcoPreferences::pins.bPedal;
    pinsJson["bPedal2"] = SamcoPreferences::pins.bPedal2;
    pinsJson["bHome"] = SamcoPreferences::pins.bHome;
    pinsJson["bPump"] = SamcoPreferences::pins.bPump;
    pinsJson["oRumble"] = SamcoPreferences::pins.oRumble;
    pinsJson["oSolenoid"] = SamcoPreferences::pins.oSolenoid;
    pinsJson["sRumble"] = SamcoPreferences::pins.sRumble;
    pinsJson["sSolenoid"] = SamcoPreferences::pins.sSolenoid;
    pinsJson["sAutofire"] = SamcoPreferences::pins.sAutofire;
    pinsJson["oPixel"] = SamcoPreferences::pins.oPixel;
    pinsJson["oLedR"] = SamcoPreferences::pins.oLedR;
    pinsJson["oLedB"] = SamcoPreferences::pins.oLedB;
    pinsJson["oLedG"] = SamcoPreferences::pins.oLedG;
    pinsJson["pCamSDA"] = SamcoPreferences::pins.pCamSDA;
    pinsJson["pCamSCL"] = SamcoPreferences::pins.pCamSCL;
    pinsJson["pPeriphSDA"] = SamcoPreferences::pins.pPeriphSDA;
    pinsJson["pPeriphSCL"] = SamcoPreferences::pins.pPeriphSCL;
    pinsJson["aBattRead"] = SamcoPreferences::pins.aBattRead;
    pinsJson["aStickX"] = SamcoPreferences::pins.aStickX;
    pinsJson["aStickY"] = SamcoPreferences::pins.aStickY;
    pinsJson["aTMP36"] = SamcoPreferences::pins.aTMP36;
    pinsJson["oLedPWMControl1"] = SamcoPreferences::pins.oLedPWMControl1;
    pinsJson["oLedPWMControl2"] = SamcoPreferences::pins.oLedPWMControl2;
    pinsJson["oLedPWMControlRecoil"] = SamcoPreferences::pins.oLedPWMControlRecoil;


    // Ajouter les valeurs de la structure SettingsMap_t
    JsonObject settingsJson = doc.createNestedObject("settings");
    settingsJson["pauseHoldLength"] = SamcoPreferences::settings.pauseHoldLength;
    settingsJson["customLEDcount"] = SamcoPreferences::settings.customLEDcount;
    settingsJson["customLEDstatic"] = SamcoPreferences::settings.customLEDstatic;
    
    settingsJson["ledPWM1_min"] = SamcoPreferences::settings.ledPWM1_min;
    settingsJson["ledPWM1_max"] = SamcoPreferences::settings.ledPWM1_max;
    settingsJson["ledPWM2_min"] = SamcoPreferences::settings.ledPWM2_min;
    settingsJson["ledPWM2_max"] = SamcoPreferences::settings.ledPWM2_max;
    settingsJson["ledPWMRecoil_min"] = SamcoPreferences::settings.ledPWMRecoil_min;
    settingsJson["ledPWMRecoil_max"] = SamcoPreferences::settings.ledPWMRecoil_max;

    settingsJson["apName"] = SamcoPreferences::settings.apName;
    settingsJson["apPassword"] = SamcoPreferences::settings.apPassword;
    settingsJson["serverPort"] = SamcoPreferences::settings.serverPort;    
    

    // Ajouter les valeurs de la structure USBMap_t
    JsonObject usbJson = doc.createNestedObject("usb");
    usbJson["deviceName"] = SamcoPreferences::usb.deviceName;
    usbJson["devicePID"] = SamcoPreferences::usb.devicePID;

    // Convertir le document JSON en chaîne
    String jsonString;
    serializeJsonPretty(doc, jsonString);
    return jsonString;
}

bool SamcoPreferences::JsonToStructures(const String& jsonString) {
    // Créer un document JSON dynamique
    DynamicJsonDocument doc(2048);

    // Désérialiser la chaîne JSON
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
        Serial.println("error");
        return false; // Erreur de désérialisation
    }
    Serial.println("no error");
    
    // Récupérer les profils
    JsonArray profilesJson = doc["profiles"];
    if (!profilesJson.isNull()) {
        for (uint8_t i = 0; i < profilesJson.size() && i < SamcoPreferences::profiles.profileCount; ++i) {
            const JsonObject& profileJson = profilesJson[i];
            SamcoPreferences::ProfileData_t& profile = SamcoPreferences::profiles.pProfileData[i];
            char hexColor[8]; // La chaîne hexadécimale pour la couleur (#RRGGBB)
            
            if (profileJson.containsKey("topOffset")) profile.topOffset = profileJson["topOffset"];
            if (profileJson.containsKey("bottomOffset")) profile.bottomOffset = profileJson["bottomOffset"];
            if (profileJson.containsKey("leftOffset")) profile.leftOffset = profileJson["leftOffset"];
            if (profileJson.containsKey("rightOffset")) profile.rightOffset = profileJson["rightOffset"];
            if (profileJson.containsKey("TLled")) profile.TLled = profileJson["TLled"];
            if (profileJson.containsKey("TRled")) profile.TRled = profileJson["TRled"];
            if (profileJson.containsKey("adjX")) profile.adjX = profileJson["adjX"];
            if (profileJson.containsKey("adjY")) profile.adjY = profileJson["adjY"];
            if (profileJson.containsKey("irSensitivity")) profile.irSensitivity = profileJson["irSensitivity"];
            if (profileJson.containsKey("runMode")) profile.runMode = profileJson["runMode"];
            if (profileJson.containsKey("buttonMask")) profile.buttonMask = profileJson["buttonMask"];
            if (profileJson.containsKey("irLayout")) profile.irLayout = profileJson["irLayout"];
            //if (profileJson.containsKey("color")) profile.color = profileJson["color"].as<const char*>();
            if (profileJson.containsKey("color")){
                const char* colorStr = profileJson["color"].as<const char*>();
                // Assurez-vous que la chaîne est bien formatée en hex (par exemple #RRGGBB)
                if (colorStr[0] == '#') {
                    uint32_t color;
                    sscanf(colorStr + 1, "%6x", &color); // Ignorez le caractère '#' et lisez un hex
                    profile.color = color;
                }
            }
            
            if (profileJson.containsKey("name")) {
                strncpy(profile.name, profileJson["name"].as<const char*>(), sizeof(profile.name) - 1);
                profile.name[sizeof(profile.name) - 1] = '\0';
            }
            if (profileJson.containsKey("rumbleIntensity")) profile.rumbleIntensity = profileJson["rumbleIntensity"];
            if (profileJson.containsKey("rumbleInterval")) profile.rumbleInterval = profileJson["rumbleInterval"];
            if (profileJson.containsKey("solenoidNormalInterval")) profile.solenoidNormalInterval = profileJson["solenoidNormalInterval"];
            if (profileJson.containsKey("solenoidFastInterval")) profile.solenoidFastInterval = profileJson["solenoidFastInterval"];
            if (profileJson.containsKey("solenoidLongInterval")) profile.solenoidLongInterval = profileJson["solenoidLongInterval"];
            if (profileJson.containsKey("autofireWaitFactor")) profile.autofireWaitFactor = profileJson["autofireWaitFactor"];
            //if (profileJson.containsKey("customLEDcolor1")) profile.customLEDcolor1 = profileJson["customLEDcolor1"];
            //if (profileJson.containsKey("customLEDcolor2")) profile.customLEDcolor2 = profileJson["customLEDcolor2"];
            //if (profileJson.containsKey("customLEDcolor3")) profile.customLEDcolor3 = profileJson["customLEDcolor3"];
            
            if (profileJson.containsKey("customLEDcolor1")){
                const char* colorStr = profileJson["customLEDcolor1"].as<const char*>();
                // Assurez-vous que la chaîne est bien formatée en hex (par exemple #RRGGBB)
                if (colorStr[0] == '#') {
                    uint32_t color;
                    sscanf(colorStr + 1, "%6x", &color); // Ignorez le caractère '#' et lisez un hex
                    profile.customLEDcolor1 = color;
                }
            }
            if (profileJson.containsKey("customLEDcolor2")){
                const char* colorStr = profileJson["customLEDcolor2"].as<const char*>();
                // Assurez-vous que la chaîne est bien formatée en hex (par exemple #RRGGBB)
                if (colorStr[0] == '#') {
                    uint32_t color;
                    sscanf(colorStr + 1, "%6x", &color); // Ignorez le caractère '#' et lisez un hex
                    profile.customLEDcolor2 = color;
                }
            }
            if (profileJson.containsKey("customLEDcolor3")){
                const char* colorStr = profileJson["customLEDcolor3"].as<const char*>();
                // Assurez-vous que la chaîne est bien formatée en hex (par exemple #RRGGBB)
                if (colorStr[0] == '#') {
                    uint32_t color;
                    sscanf(colorStr + 1, "%6x", &color); // Ignorez le caractère '#' et lisez un hex
                    profile.customLEDcolor3 = color;
                }
            }            
            
            if (profileJson.containsKey("ledPWM1Level")) profile.ledPWM1Level = profileJson["ledPWM1Level"];
            if (profileJson.containsKey("ledPWM2Level")) profile.ledPWM2Level = profileJson["ledPWM2Level"];
            if (profileJson.containsKey("ledPWMRecoilFadeDuration")) profile.ledPWMRecoilFadeDuration = profileJson["ledPWMRecoilFadeDuration"];
            
            if (profileJson.containsKey("rumbleActive")) profile.rumbleActive = profileJson["rumbleActive"];
            if (profileJson.containsKey("solenoidActive")) profile.solenoidActive = profileJson["solenoidActive"];
            if (profileJson.containsKey("autofireActive")) profile.autofireActive = profileJson["autofireActive"];
            if (profileJson.containsKey("rumbleFF")) profile.rumbleFF = profileJson["rumbleFF"];
        }
    }

    // Profil sélectionné
    if (doc.containsKey("selectedProfile")) {
        SamcoPreferences::profiles.selectedProfile = doc["selectedProfile"];
    }

    // Structure TogglesMap_t
    JsonObject togglesJson = doc["toggles"];
    if (!togglesJson.isNull()) {
        if (togglesJson.containsKey("customPinsInUse")) SamcoPreferences::toggles.customPinsInUse = togglesJson["customPinsInUse"];
        if (togglesJson.containsKey("rumbleActive")) SamcoPreferences::toggles.rumbleActive = togglesJson["rumbleActive"];
        if (togglesJson.containsKey("solenoidActive")) SamcoPreferences::toggles.solenoidActive = togglesJson["solenoidActive"];
        if (togglesJson.containsKey("autofireActive")) SamcoPreferences::toggles.autofireActive = togglesJson["autofireActive"];
        if (togglesJson.containsKey("simpleMenu")) SamcoPreferences::toggles.simpleMenu = togglesJson["simpleMenu"];
        if (togglesJson.containsKey("holdToPause")) SamcoPreferences::toggles.holdToPause = togglesJson["holdToPause"];
        if (togglesJson.containsKey("commonAnode")) SamcoPreferences::toggles.commonAnode = togglesJson["commonAnode"];
        if (togglesJson.containsKey("lowButtonMode")) SamcoPreferences::toggles.lowButtonMode = togglesJson["lowButtonMode"];
        if (togglesJson.containsKey("rumbleFF")) SamcoPreferences::toggles.rumbleFF = togglesJson["rumbleFF"];
    }

    // Structure PinsMap_t
    JsonObject pinsJson = doc["pins"];
    if (!pinsJson.isNull()) {
        if (pinsJson.containsKey("bTrigger")) SamcoPreferences::pins.bTrigger = pinsJson["bTrigger"];
        if (pinsJson.containsKey("bGunA")) SamcoPreferences::pins.bGunA = pinsJson["bGunA"];
        if (pinsJson.containsKey("bGunB")) SamcoPreferences::pins.bGunB = pinsJson["bGunB"];
        if (pinsJson.containsKey("bStart")) SamcoPreferences::pins.bStart = pinsJson["bStart"];
        if (pinsJson.containsKey("bSelect")) SamcoPreferences::pins.bSelect = pinsJson["bSelect"];
        if (pinsJson.containsKey("bGunUp")) SamcoPreferences::pins.bGunUp = pinsJson["bGunUp"];
        if (pinsJson.containsKey("bGunDown")) SamcoPreferences::pins.bGunDown = pinsJson["bGunDown"];
        if (pinsJson.containsKey("bGunLeft")) SamcoPreferences::pins.bGunLeft = pinsJson["bGunLeft"];
        if (pinsJson.containsKey("bGunRight")) SamcoPreferences::pins.bGunRight = pinsJson["bGunRight"];
        if (pinsJson.containsKey("bGunC")) SamcoPreferences::pins.bGunC = pinsJson["bGunC"];
        if (pinsJson.containsKey("bPedal")) SamcoPreferences::pins.bPedal = pinsJson["bPedal"];
        if (pinsJson.containsKey("bPedal2")) SamcoPreferences::pins.bPedal2 = pinsJson["bPedal2"];
        if (pinsJson.containsKey("bHome")) SamcoPreferences::pins.bHome = pinsJson["bHome"];
        if (pinsJson.containsKey("bPump")) SamcoPreferences::pins.bPump = pinsJson["bPump"];
        if (pinsJson.containsKey("oRumble")) SamcoPreferences::pins.oRumble = pinsJson["oRumble"];
        if (pinsJson.containsKey("oSolenoid")) SamcoPreferences::pins.oSolenoid = pinsJson["oSolenoid"];
        if (pinsJson.containsKey("sRumble")) SamcoPreferences::pins.sRumble = pinsJson["sRumble"];
        if (pinsJson.containsKey("sSolenoid")) SamcoPreferences::pins.sSolenoid = pinsJson["sSolenoid"];
        if (pinsJson.containsKey("sAutofire")) SamcoPreferences::pins.sAutofire = pinsJson["sAutofire"];
        if (pinsJson.containsKey("oPixel")) SamcoPreferences::pins.oPixel = pinsJson["oPixel"];
        if (pinsJson.containsKey("oLedR")) SamcoPreferences::pins.oLedR = pinsJson["oLedR"];
        if (pinsJson.containsKey("oLedB")) SamcoPreferences::pins.oLedB = pinsJson["oLedB"];
        if (pinsJson.containsKey("oLedG")) SamcoPreferences::pins.oLedG = pinsJson["oLedG"];
        if (pinsJson.containsKey("pCamSDA")) SamcoPreferences::pins.pCamSDA = pinsJson["pCamSDA"];
        if (pinsJson.containsKey("pCamSCL")) SamcoPreferences::pins.pCamSCL = pinsJson["pCamSCL"];
        if (pinsJson.containsKey("pPeriphSDA")) SamcoPreferences::pins.pPeriphSDA = pinsJson["pPeriphSDA"];
        if (pinsJson.containsKey("pPeriphSCL")) SamcoPreferences::pins.pPeriphSCL = pinsJson["pPeriphSCL"];
        if (pinsJson.containsKey("aBattRead")) SamcoPreferences::pins.aBattRead = pinsJson["aBattRead"];
        if (pinsJson.containsKey("aStickX")) SamcoPreferences::pins.aStickX = pinsJson["aStickX"];
        if (pinsJson.containsKey("aStickY")) SamcoPreferences::pins.aStickY = pinsJson["aStickY"];
        if (pinsJson.containsKey("aTMP36")) SamcoPreferences::pins.aTMP36 = pinsJson["aTMP36"];
        if (pinsJson.containsKey("oLedPWMControl1")) SamcoPreferences::pins.oLedPWMControl1 = pinsJson["oLedPWMControl1"];
        if (pinsJson.containsKey("oLedPWMControl2")) SamcoPreferences::pins.oLedPWMControl2 = pinsJson["oLedPWMControl2"];
        if (pinsJson.containsKey("oLedPWMControlRecoil")) SamcoPreferences::pins.oLedPWMControlRecoil = pinsJson["oLedPWMControlRecoil"];        
        
    }

    // Structure SettingsMap_t
    JsonObject settingsJson = doc["settings"];
    if (!settingsJson.isNull()) {
        if (settingsJson.containsKey("pauseHoldLength")) SamcoPreferences::settings.pauseHoldLength = settingsJson["pauseHoldLength"];
        if (settingsJson.containsKey("customLEDcount")) SamcoPreferences::settings.customLEDcount = settingsJson["customLEDcount"];
        if (settingsJson.containsKey("customLEDstatic")) SamcoPreferences::settings.customLEDstatic = settingsJson["customLEDstatic"];
        
        if (settingsJson.containsKey("ledPWM1_min")) SamcoPreferences::settings.ledPWM1_min = settingsJson["ledPWM1_min"];
        if (settingsJson.containsKey("ledPWM1_max")) SamcoPreferences::settings.ledPWM1_max = settingsJson["ledPWM1_max"];
        if (settingsJson.containsKey("ledPWM2_min")) SamcoPreferences::settings.ledPWM2_min = settingsJson["ledPWM2_min"];
        if (settingsJson.containsKey("ledPWM2_max")) SamcoPreferences::settings.ledPWM2_max = settingsJson["ledPWM2_max"];
        if (settingsJson.containsKey("ledPWMRecoil_min")) SamcoPreferences::settings.ledPWM1_min = settingsJson["ledPWMRecoil_min"];
        if (settingsJson.containsKey("ledPWMRecoil_max")) SamcoPreferences::settings.ledPWM1_max = settingsJson["ledPWMRecoil_max"];
        
        if (settingsJson.containsKey("apName")) {
            strncpy(SamcoPreferences::settings.apName, settingsJson["apName"].as<const char*>(), sizeof(SamcoPreferences::settings.apName) - 1);
            SamcoPreferences::settings.apName[sizeof(SamcoPreferences::settings.apName) - 1] = '\0';
        }
        if (settingsJson.containsKey("apPassword")) {
            strncpy(SamcoPreferences::settings.apPassword, settingsJson["apPassword"].as<const char*>(), sizeof(SamcoPreferences::settings.apPassword) - 1);
            SamcoPreferences::settings.apPassword[sizeof(SamcoPreferences::settings.apPassword) - 1] = '\0';
        }
        if (settingsJson.containsKey("serverPort")) SamcoPreferences::settings.serverPort = settingsJson["serverPort"];
    }

    // Structure USBMap_t
    JsonObject usbJson = doc["usb"];
    if (!usbJson.isNull()) {
        if (usbJson.containsKey("deviceName")) {
            strncpy(SamcoPreferences::usb.deviceName, usbJson["deviceName"].as<const char*>(), sizeof(SamcoPreferences::usb.deviceName) - 1);
            SamcoPreferences::usb.deviceName[sizeof(SamcoPreferences::usb.deviceName) - 1] = '\0';
        }
        if (usbJson.containsKey("devicePID")) SamcoPreferences::usb.devicePID = usbJson["devicePID"];
    }

    return true; // Succès
}
