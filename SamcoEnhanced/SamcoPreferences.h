/*!
 * @file SamcoPreferences.h
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

#ifndef _SAMCOPREFERENCES_H_
#define _SAMCOPREFERENCES_H_

#include <OpenFIREBoard.h>
#include <stdint.h>


#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    #include <ArduinoJson.h>
#endif

/// @brief Static instance of preferences to save in non-volatile memory
class SamcoPreferences
{
public:
    /// @brief Error codes
    enum Errors_e {
        Error_Success = 0,
        Error_NoStorage = -1,
        Error_Read = -2,
        Error_NoData = -3,
        Error_Write = -4,
        Error_Erase = -5
    };
    
    typedef struct TemporarySerialSettings_s {
        int rumbleActive = -1;
        int solenoidActive = -1;
        int autofireActive = -1;    
        int rumbleFF = -1;
    } __attribute__ ((packed)) TemporarySerialSettings_t;
    
    static TemporarySerialSettings_t temporarySerialSettings;

    /*
    static bool GetRumbleActive(){ return SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleActive; };    
    static bool GetSolenoidActive(){ return SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].solenoidActive; };
    static bool GetAutofireActive(){ return SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].autofireActive; };
    static bool GetRumbleFF() {return SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleFF; };

    static void SetRumbleActive(bool value, bool temporary = false) {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleActive = value;
            SamcoPreferences::temporarySerialSettings.rumbleActive = -1;
    }
    static void SetSolenoidActive(bool value, bool temporary = false) {

            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].solenoidActive = value;
            SamcoPreferences::temporarySerialSettings.solenoidActive = -1;
    }
    static void SetAutofireActive(bool value, bool temporary = false) {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].autofireActive = value;
            SamcoPreferences::temporarySerialSettings.autofireActive = -1;
    }
    static void SetRumbleFF(bool value, bool temporary = false) {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleFF = value;
            SamcoPreferences::temporarySerialSettings.rumbleFF = -1;
    }        
    */
            
    
    static bool GetRumbleActive(){ return SamcoPreferences::temporarySerialSettings.rumbleActive != -1 ? SamcoPreferences::temporarySerialSettings.rumbleActive : SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleActive; };    
    static bool GetSolenoidActive(){ return SamcoPreferences::temporarySerialSettings.solenoidActive != -1 ? SamcoPreferences::temporarySerialSettings.solenoidActive : SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].solenoidActive; };
    static bool GetAutofireActive(){ return SamcoPreferences::temporarySerialSettings.autofireActive != -1 ? SamcoPreferences::temporarySerialSettings.autofireActive : SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].autofireActive; };
    static bool GetRumbleFF() {return SamcoPreferences::temporarySerialSettings.rumbleFF != -1 ? SamcoPreferences::temporarySerialSettings.rumbleFF : SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleFF; };
        
        
    static void SetRumbleActive(bool value, bool temporary = false) {
        if (temporary) {
            SamcoPreferences::temporarySerialSettings.rumbleActive = value;
        } else {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleActive = value;
            SamcoPreferences::temporarySerialSettings.rumbleActive = -1;
        }
        
    }
    static void SetSolenoidActive(bool value, bool temporary = false) {
        
        if (temporary) {
            SamcoPreferences::temporarySerialSettings.solenoidActive = value;
        } else {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].solenoidActive = value;
            SamcoPreferences::temporarySerialSettings.solenoidActive = -1;
        }
        
    }
    static void SetAutofireActive(bool value, bool temporary = false) {
        
        if (temporary) {
            SamcoPreferences::temporarySerialSettings.autofireActive = value;
        } else {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].autofireActive = value;
            SamcoPreferences::temporarySerialSettings.autofireActive = -1;
        }
        
    }
    static void SetRumbleFF(bool value, bool temporary = false) {
        
        if (temporary) {
            SamcoPreferences::temporarySerialSettings.rumbleFF = value;
        } else {
            SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].rumbleFF = value;
            SamcoPreferences::temporarySerialSettings.rumbleFF = -1;
        }
        
    }
      
    //Used when pins are not set
    static void DisableRumbleActive(){
        for(unsigned int i = 0; i < SamcoPreferences::profiles.profileCount; ++i) {SamcoPreferences::profiles.pProfileData[i].rumbleActive = false;}
        SamcoPreferences::temporarySerialSettings.rumbleActive = -1;
    }
    static void DisableSolenoidActive(){
        for(unsigned int i = 0; i < SamcoPreferences::profiles.profileCount; ++i) {SamcoPreferences::profiles.pProfileData[i].solenoidActive = false;}
        SamcoPreferences::temporarySerialSettings.solenoidActive = -1;
    }
    static void DisableAutofireActive(){
        for(unsigned int i = 0; i < SamcoPreferences::profiles.profileCount; ++i) {SamcoPreferences::profiles.pProfileData[i].autofireActive = false;}
        SamcoPreferences::temporarySerialSettings.autofireActive = -1;
    }    
    static void DisableRumbleFF(){
        for(unsigned int i = 0; i < SamcoPreferences::profiles.profileCount; ++i) {SamcoPreferences::profiles.pProfileData[i].rumbleFF = false;}
        SamcoPreferences::temporarySerialSettings.rumbleFF = -1;
    }        
    
    /// @brief Header ID
    typedef union HeaderId_u {
        uint8_t bytes[4];
        uint32_t u32;
    } __attribute__ ((packed)) HeaderId_t;

    /// @brief Profile data
    typedef struct ProfileData_s {
        int topOffset;              // Perspective: Offsets
        int bottomOffset;
        int leftOffset;
        int rightOffset;
        float TLled;                // Perspective: LED relative anchors
        float TRled;
        float adjX;                 // Perspective: adjusted axis
        float adjY;
        uint32_t irSensitivity : 3; // IR Sensitivity from 0-2
        uint32_t runMode : 5;       // Averaging mode
        uint32_t buttonMask : 16;   // Button mask assigned to this profile
        bool irLayout;              // square or diamond IR for this display?
        uint32_t color   : 24;      // packed color blob per profile

        
        uint8_t rumbleIntensity = 255;
        uint16_t rumbleInterval = 150;
        uint16_t solenoidNormalInterval = 45;
        uint16_t solenoidFastInterval = 30;
        uint16_t solenoidLongInterval = 500;
        uint8_t autofireWaitFactor = 3;
        uint32_t customLEDcolor1 = 0xFF0000;
        uint32_t customLEDcolor2 = 0x00FF00;
        uint32_t customLEDcolor3 = 0x0000FF;
        
        uint8_t ledPWM1Level = 7;
        uint8_t ledPWM2Level = 7;
        uint16_t ledPWMRecoilFadeDuration = 400;
        
        bool rumbleActive = true;       // Are we allowed to do rumble?
        bool solenoidActive = true;     // Are we allowed to use a solenoid?
        bool autofireActive = false;    // Is autofire enabled?
        bool rumbleFF = false;          // Rumble force-feedback, instead of Solenoid    


        char name[16];               // Profile display name        
        
    } __attribute__ ((packed)) ProfileData_t;

    /// @brief Preferences that can be stored in flash
    typedef struct Preferences_s {
        // pointer to ProfileData_t array
        SamcoPreferences::ProfileData_t* pProfileData;
        // number of ProfileData_t entries
        uint8_t profileCount;

        // default profile
        uint8_t selectedProfile;
    } __attribute__ ((packed)) Preferences_t;

    // single instance of the preference data
    static Preferences_t profiles;

    enum BoolTypes_e {
        Bool_CustomPins = 0,
        Bool_Rumble,
        Bool_Solenoid,
        Bool_Autofire,
        Bool_SimpleMenu,
        Bool_HoldToPause,
        Bool_CommonAnode,
        Bool_LowButtons,
        Bool_RumbleFF
    };

    typedef struct TogglesMap_s {
        bool customPinsInUse = false;   // Are we using custom pins mapping?
        bool rumbleActive = true;       // DEPRECIATED, Now on the ExtraProfile.
        bool solenoidActive = true;     // DEPRECIATED, Now on the ExtraProfile.
        bool autofireActive = false;    // DEPRECIATED, Now on the ExtraProfile.
        bool simpleMenu = false;        // Is simple pause menu active?
        bool holdToPause = false;       // Is holding A/B buttons to enter pause mode allowed?
        bool commonAnode = true;        // If LED is Common Anode (+, connects to 5V) rather than Common Cathode (-, connects to GND)
        bool lowButtonMode = false;     // Is low buttons mode active?
        bool rumbleFF = false;          // DEPRECIATED, Now on the ExtraProfile.
    } __attribute__ ((packed)) TogglesMap_t;

    static TogglesMap_t toggles;
    

    enum InputTypes_e {
        Pin_Trigger = 0,
        Pin_GunA,
        Pin_GunB,
        Pin_GunC,
        Pin_Start,
        Pin_Select,
        Pin_GunUp,
        Pin_GunDown,
        Pin_GunLeft,
        Pin_GunRight,
        Pin_Pedal,
        Pin_Pedal2,
        Pin_Home,
        Pin_Pump,
        Pin_RumbleSignal,
        Pin_SolenoidSignal,
        Pin_RumbleSwitch,
        Pin_SolenoidSwitch,
        Pin_AutofireSwitch,
        Pin_NeoPixel,
        Pin_LEDR,
        Pin_LEDG,
        Pin_LEDB,
        Pin_CameraSDA,
        Pin_CameraSCL,
        Pin_PeripheralSDA,
        Pin_PeripheralSCL,
        Pin_Battery,
        Pin_AnalogX,
        Pin_AnalogY,
        Pin_AnalogTMP,
        Pin_LedPWMControl1,
        Pin_LedPWMControl2,
        Pin_LedPWMControlRecoil
    };

    typedef struct PinsMap_s {
        int8_t bTrigger = -1;              // Trigger
        int8_t bGunA = -1;                 // Button A (GunCon 1/Stunner/Justifier)
        int8_t bGunB = -1;                 // Button B (GunCon 1)
        int8_t bStart = -1;                // Start Button (GCon-2)
        int8_t bSelect = -1;               // Select Button (GCon-2)
        int8_t bGunUp = -1;                // D-Pad Up (GCon-2)
        int8_t bGunDown = -1;              // D-Pad Down (GCon-2)
        int8_t bGunLeft = -1;              // D-Pad Left (GCon-2)
        int8_t bGunRight = -1;             // D-Pad Right (GCon-2)
        int8_t bGunC = -1;                 // Button C (GCon-2)
        int8_t bPedal = -1;                // External Pedal (DIY)
        int8_t bPedal2 = -1;               // External Pedal 2 (DIY)
        int8_t bHome = -1;                 // Home Button (Top Shot Elite)
        int8_t bPump = -1;                 // Pump Action Reload Button (Top Shot Elite)
        int8_t oRumble = -1;               // Rumble Signal Pin
        int8_t oSolenoid = -1;             // Solenoid Signal Pin
        int8_t sRumble = -1;               // Rumble Switch
        int8_t sSolenoid = -1;             // Solenoid Switch
        int8_t sAutofire = -1;             // Autofire Switch
        int8_t oPixel = -1;                // Custom NeoPixel Pin
        int8_t oLedR = -1;                 // 4-Pin RGB Red Pin
        int8_t oLedB = -1;                 // 4-Pin RGB Blue Pin
        int8_t oLedG = -1;                 // 4-Pin RGB Green Pin
        int8_t pCamSDA = -1;               // Camera I2C Data Pin
        int8_t pCamSCL = -1;               // Camera I2C Clock Pin
        int8_t pPeriphSDA = -1;            // Other I2C Peripherals Data Pin
        int8_t pPeriphSCL = -1;            // Other I2C Peripherals Clock Pin
        int8_t aBattRead = -1;             // Battery voltage circuit thingy?
        int8_t aStickX = -1;               // Analog Stick X-axis
        int8_t aStickY = -1;               // Analog Stick Y-axis
        int8_t aTMP36 = -1;                // Analog TMP36 Temperature Sensor Pin
        int8_t oLedPWMControl1 = -1;       // PWM Led driver control 1
        int8_t oLedPWMControl2 = -1;    // PWM Led driver control 2
        int8_t oLedPWMControlRecoil = -1;//PWM Led driver control on Recoil 
    } PinsMap_t;

    static PinsMap_t pins;

    enum SettingsTypes_e {
        Setting_RumbleIntensity = 0,
        Setting_RumbleInterval,
        Setting_SolenoidNormInt,
        Setting_SolenoidFastInt,
        Setting_SolenoidLongInt,
        Setting_AutofireFactor,
        Setting_PauseHoldLength,
        Setting_CustomLEDCount,
        Setting_CustomLEDStatic,
        Setting_Color1,
        Setting_Color2,
        Setting_Color3
    };

    typedef struct SettingsMap_s {
        //uint8_t rumbleIntensity = 255;  // DEPRECIATED, Now on the ExtraProfile.
        //uint16_t rumbleInterval = 150; // DEPRECIATED, Now on the ExtraProfile.
        //uint16_t solenoidNormalInterval = 45; // DEPRECIATED, Now on the ExtraProfile.
        //uint16_t solenoidFastInterval = 30; // DEPRECIATED, Now on the ExtraProfile.
        //uint16_t solenoidLongInterval = 500; // DEPRECIATED, Now on the ExtraProfile.
        //uint8_t autofireWaitFactor = 3; // DEPRECIATED, Now on the ExtraProfile.
        uint16_t pauseHoldLength = 2500;
        uint8_t customLEDcount = 1;
        uint8_t customLEDstatic = 0;
        //uint32_t customLEDcolor1 = 0xFF0000; // DEPRECIATED, Now on the ExtraProfile.
        //uint32_t customLEDcolor2 = 0x00FF00; // DEPRECIATED, Now on the ExtraProfile.
        //uint32_t customLEDcolor3 = 0x0000FF; // DEPRECIATED, Now on the ExtraProfile.
        
        uint8_t ledPWM1_min = 0;
        uint8_t ledPWM1_max = 0;
        uint8_t ledPWM2_min = 0;
        uint8_t ledPWM2_max = 0;
        uint8_t ledPWMRecoil_min = 0;
        uint8_t ledPWMRecoil_max = 0;
        
        int serverPort = 80;
        char apName[50];
        char apPassword[50];         
    } SettingsMap_t;

    static SettingsMap_t settings;

    enum USBTypes_e {
        USB_DeviceName = 0,
        USB_DevicePID
    };

    typedef struct USBMap_s {
        char deviceName[16];
        uint16_t devicePID;
    } USBMap_t;

    static USBMap_t usb;
    
    static String baseJson;
    
    static bool fsInitialized;

    /// @brief Required size for the preferences
    static unsigned int Size() { return sizeof(ProfileData_t) * profiles.profileCount + sizeof(HeaderId_u) + sizeof(profiles.selectedProfile); }
    
    /// @brief Load preferences
    /// @return An error code from Errors_e
    static int LoadProfiles();

    /// @brief Save current preferences
    /// @return An error code from Errors_e
    static int SaveProfiles();


    /// @brief Resets preferences with a zero-fill to the EEPROM.
    /// @return Nothing
    static void ResetPreferences();

    /// @brief Sets pre-set values according to the board
    /// @return Nothing
    static void LoadPresets();

    /// @brief Sets pre-set camera pins according to the board
    /// @return Nothing
    static void PresetCam();
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    
    static String structuresToJson();
    
    static bool JsonToStructures(const String& jsonString);
    
    #endif
};

#endif // _SAMCOPREFERENCES_H_
