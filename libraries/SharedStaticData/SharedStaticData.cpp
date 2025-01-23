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

#include "SharedStaticData.h"

uint8_t SharedStaticData::controlMode = SharedStaticData::ControlMode_e::ControlMode_Mouse;
bool SharedStaticData::loop1Started = false;

// Définition des variables statiques
unsigned long SharedStaticData::reservedI2CUntil = 0;
volatile bool SharedStaticData::i2cLock = false;

uint8_t SharedStaticData::analogDpadStartPin = -1;
uint8_t SharedStaticData::analogDpadSelectPin = -1;

uint8_t SharedStaticData::analogDpadUpState = 0;
uint8_t SharedStaticData::analogDpadDownState = 0;
uint8_t SharedStaticData::analogDpadToggleState = 0;
uint8_t SharedStaticData::analogDpadLeftState = 0;
uint8_t SharedStaticData::analogDpadRightState = 0;
uint8_t SharedStaticData::analogDpadMidState = 0;
