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

#include <stdint.h>


class SharedStaticData {
public:
    enum ControlMode_e {
        ControlMode_Mouse,
        ControlMode_Gamepad,
        ControlMode_GamepadCamOnRightStick,
    };    
    
    static uint8_t controlMode;
    static bool loop1Started;

};


#endif // _LIGHTGUNBUTTONS_H_
