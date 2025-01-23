/*!
 * @file LightgunButtons.h
 * @brief HID buttons originally intended for use with a light gun.
 *
 * @copyright Mike Lynch, 2021
 *
 *  LightgunButtons is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _SHAREDSTATICDATA_H_
#define _SHAREDSTATICDATA_H_

#include <Arduino.h>

class SharedStaticData {
public:
    enum ControlMode_e {
        ControlMode_Mouse,
        ControlMode_Gamepad,
        ControlMode_GamepadCamOnRightStick,
    };    
    
    static uint8_t controlMode;
    static bool loop1Started;

	static uint8_t analogDpadStartPin;
	static uint8_t analogDpadSelectPin;
	static uint8_t analogDpadUpState;
	static uint8_t analogDpadDownState;
	static uint8_t analogDpadToggleState;
	static uint8_t analogDpadLeftState;
	static uint8_t analogDpadRightState;
	static uint8_t analogDpadMidState;


	
    static unsigned long reservedI2CUntil;  // Temps jusqu'à lequel l'I2C est réservé
    static volatile bool i2cLock;          // Mutex simulé (drapeau)

    // Méthode pour essayer de réserver l'I2C
	static bool tryReserveI2C(unsigned long durationMs) {
		if (!SharedStaticData::i2cLock) {  // Essaye de verrouiller
		
			unsigned long currentMillis = millis();
			if (currentMillis > SharedStaticData::reservedI2CUntil) {
				SharedStaticData::i2cLock=true;
				SharedStaticData::reservedI2CUntil = currentMillis + durationMs;  // Réserve l'I2C
				return true;
			}
		
		}
		return false;  // Mutex non dispo
	}


	static void releaseI2C() {
		SharedStaticData::i2cLock = false;
	}

};


#endif // _LIGHTGUNBUTTONS_H_
