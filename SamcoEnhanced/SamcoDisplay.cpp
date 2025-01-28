/*!
 * @file SamcoDisplay.cpp
 * @brief Macros for lightgun HUD display.
 *
 * @copyright That One Seong, 2024
 *
 *  SamcoDisplay is free software: you can redistribute it and/or modify
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

// we're using our own splash screen kthx ada
#define SSD1306_NO_SPLASH

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "SamcoDisplay.h"
#include "SamcoPreferences.h"
#include <TinyUSB_Devices.h>

// include heuristics for determining Wire or Wire1 SDA/SCL pins, ref'd from SamcoPreferences::pins

Adafruit_SSD1306 *display;

ExtDisplay::ExtDisplay() {}

  const uint8_t* ExtDisplay::number_bitmaps[] = {
    number_0, number_1, number_2, number_3, number_4,
    number_5, number_6, number_7, number_8, number_9
};

	unsigned long ExtDisplay::UseSecondBuffer = 0;

	int ExtDisplay::rumbleActive = -1;
	bool ExtDisplay::rumbleActive_change = false;
	int ExtDisplay::solenoidActive = -1;
	bool ExtDisplay::solenoidActive_change = false;
	int ExtDisplay::autofireActive = -1;    
	bool ExtDisplay::autofireActive_change = false;
	int ExtDisplay::rumbleFF = -1;
	bool ExtDisplay::rumbleFF_change = false;
	int ExtDisplay::controlMode = -1;
	bool ExtDisplay::controlMode_change = false;
	int ExtDisplay::wifiStatus = -1;
	bool ExtDisplay::wifiStatus_change = false;
	int ExtDisplay::profileIndex = -1;
	bool ExtDisplay::profileIndex_change = false;	
	int ExtDisplay::mamehookerActive = -1;
	bool ExtDisplay::mamehookerActive_change = false;	
	int ExtDisplay::wideScreenMode = -1;
	bool ExtDisplay::wideScreenMode_change = false;

  int ExtDisplay::serialDisplayValue = (int) ExtDisplay::ScreenSerialInit_e::ScreenSerial_None;
  bool ExtDisplay::serialDisplayValue_change = false;
  bool ExtDisplay::useLifeBar = false;
  bool ExtDisplay::useLifeBar_change = false;
  bool ExtDisplay::useAmmoBullets = false;
  bool ExtDisplay::useAmmoBullets_change = false;
  int ExtDisplay::lifeValue = 0;
  bool ExtDisplay::life_change = false;
  int ExtDisplay::lifeMax = 0;
  int ExtDisplay::ammoValue = 0;
  bool ExtDisplay::ammo_change = false;	
  int ExtDisplay::ammoMax = 0;
	
  unsigned long ExtDisplay::previousMillisReload = 0;
  bool ExtDisplay::showReload = false;
  bool ExtDisplay::reloadIsVisible = false;   

  int ExtDisplay::screenState = ExtDisplay::ScreenMode_e::Screen_None;
  bool ExtDisplay::screenStateChange = false;


	bool ExtDisplay::topBar_change = false;
	bool ExtDisplay::content_change = false;
	bool ExtDisplay::display_change = false;
	bool ExtDisplay::showTopBar = false;

	void ExtDisplay::setRumbleActive(bool value, bool temporary){
		int newValue = (int)value;
		if(temporary) newValue += 10;
		if(ExtDisplay::rumbleActive != newValue){
			ExtDisplay::rumbleActive = newValue;
			ExtDisplay::rumbleActive_change = true;
			ExtDisplay::topBar_change = true;
		}
	}
	void ExtDisplay::setSolenoidActive(bool value, bool temporary){
		int newValue = (int)value;
		if(temporary) newValue += 10;
		if(ExtDisplay::solenoidActive != newValue){
			ExtDisplay::solenoidActive = newValue;
			ExtDisplay::solenoidActive_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setAutofireActive(bool value, bool temporary){
		int newValue = (int)value;
		if(temporary) newValue += 10;
		if(ExtDisplay::autofireActive != newValue){
			ExtDisplay::autofireActive = newValue;
			ExtDisplay::autofireActive_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setRumbleFF(bool value, bool temporary){
		int newValue = (int)value;
		if(temporary) newValue += 10;
		if(ExtDisplay::rumbleFF != newValue){
			ExtDisplay::rumbleFF = newValue;
			ExtDisplay::rumbleFF_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setControlMode(int value, bool temporary){
		int newValue = (int)value;
		if(temporary) newValue += 10;
		if(ExtDisplay::controlMode != newValue){
			ExtDisplay::controlMode = newValue;
			ExtDisplay::controlMode_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setWifiStatus(int value){
		if(ExtDisplay::wifiStatus != value){
			ExtDisplay::wifiStatus = value;
			ExtDisplay::wifiStatus_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setProfileIndex(int value){
		if(ExtDisplay::profileIndex != value){
			ExtDisplay::profileIndex = value;
			ExtDisplay::profileIndex_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setMamehookerActive(bool value){
		if(ExtDisplay::mamehookerActive != value){
			ExtDisplay::mamehookerActive = value;
			ExtDisplay::mamehookerActive_change = true;
			ExtDisplay::topBar_change = true;
		}		
	}
	void ExtDisplay::setWideScreenMode(bool value, bool temporary){
		int newValue = (int)value;
		if(temporary) newValue += 10;
		if(ExtDisplay::wideScreenMode != newValue){
			ExtDisplay::wideScreenMode = newValue;
			ExtDisplay::wideScreenMode_change = true;
		}		
	}

  void ExtDisplay::setSerialDisplay(int value){
    if(ExtDisplay::serialDisplayValue != value){
      ExtDisplay::serialDisplayValue = value;
      ExtDisplay::serialDisplayValue_change = true;
    }
  }

  void ExtDisplay::setUseLifeBar(bool value){
    if(ExtDisplay::useLifeBar != value){
      ExtDisplay::useLifeBar = value;
      ExtDisplay::useLifeBar_change = true;
    }
  }

  void ExtDisplay::setUseAmmoBullets(bool value){
    if(ExtDisplay::useAmmoBullets != value){
      ExtDisplay::useAmmoBullets = value;
      ExtDisplay::useAmmoBullets_change = true;
    }
  }

  void ExtDisplay::setLife(int value){
    if(ExtDisplay::lifeValue != value){
      if(value > ExtDisplay::lifeMax) ExtDisplay::lifeMax = value;
      ExtDisplay::lifeValue = value;
      ExtDisplay::life_change = true;
    }
  }

  void ExtDisplay::setAmmo(int value){
    if(ExtDisplay::ammoValue != value){
      if(value > ExtDisplay::ammoValue) ExtDisplay::ammoMax = value;
      if(ammoValue > 0 && value == 0){
        ExtDisplay::previousMillisReload = 0;
        ExtDisplay::reloadIsVisible = false;
        ExtDisplay::showReload = true;
      }
      else ExtDisplay::showReload = false;
      ExtDisplay::ammoValue = value;
      ExtDisplay::ammo_change = true;
    }
  }

  void ExtDisplay::setScreenState(int value){
    if(ExtDisplay::screenState != value){
      ExtDisplay::screenState = value;
      ExtDisplay::screenStateChange = true;
    }
  }

	
void ExtDisplay::Update(){
  bool forceUpdate = false;
	if(ExtDisplay::UseSecondBuffer >0){
    if(millis() > ExtDisplay::UseSecondBuffer){
      ExtDisplay::UseSecondBuffer = 0;
      ExtDisplay::content_change = true;
      display->fillRect(0, 0, 128, 64, BLACK);
      forceUpdate = true;
    }
  }

  if(screenStateChange){
    UpdateScreenStateChange();
    forceUpdate = true;
  }
	if(ExtDisplay::showTopBar) UpdateTopBar(forceUpdate);
  if(screenState == Screen_Mamehook_Single) UpdateMameHookerSingle(forceUpdate);
  if(screenState == Screen_Mamehook_Dual) UpdateMameHookerDual(forceUpdate);
	if(screenState == Screen_Pause) UpdatePauseMenu(forceUpdate);
	if(ExtDisplay::content_change){
    ExtDisplay::rumbleActive_change = false;
    ExtDisplay::solenoidActive_change = false;
    ExtDisplay::autofireActive_change = false;
    ExtDisplay::rumbleFF_change = false;
    ExtDisplay::controlMode_change = false;
    ExtDisplay::wifiStatus_change = false;
    ExtDisplay::profileIndex_change = false;
    ExtDisplay::mamehookerActive_change = false;
    ExtDisplay::wideScreenMode_change = false;

    ExtDisplay::life_change = false;
    ExtDisplay::ammo_change = false;
    ExtDisplay::serialDisplayValue_change = false;
    ExtDisplay::useLifeBar_change = false;
    ExtDisplay::useAmmoBullets_change = false;


	  ExtDisplay::content_change = false;
	  if(ExtDisplay::UseSecondBuffer==0) Display();
	}
    
}

void ExtDisplay::UpdateScreenStateChange(){
		display->fillRect(0, 0, 128, 64, BLACK);
    switch(screenState) {
			case Screen_None:
				ExtDisplay::showTopBar = false;
				break;
			case Screen_Init:
				ExtDisplay::showTopBar = false;
				display->setTextColor(WHITE, BLACK);
        display->setTextSize(2);
        display->setCursor(20, 18);
        display->println("Welcome!");
        display->setTextSize(1);
        display->setCursor(12, 40);
        display->println(" Pull trigger to");
        display->setCursor(12, 52);
        display->println("start calibration!");
				break;
			case Screen_Normal:
				ExtDisplay::showTopBar = true;
        break;
			case Screen_Pause:
				ExtDisplay::showTopBar = false;
				selectedPauseItem = ExtDisplay::PauseItem_e::PauseItem_Profile;
				ExtDisplay::UpdatePauseMenu(true);
				SelectPauseItem(selectedPauseItem,true);
				break;
			case Screen_Profile:
				ExtDisplay::showTopBar = false;
				display->setTextColor(WHITE, BLACK);
				display->setTextSize(1);
				display->setCursor(0, 30);	
				display->println("Screen_Profile!");
				break;
			case Screen_Saving:
				ExtDisplay::showTopBar = false;
				display->setTextColor(WHITE, BLACK);
				display->setTextSize(1);
				display->setCursor(0, 30);	
				display->println("Screen_Saving!");
				break;
			case Screen_SaveSuccess:
				ExtDisplay::showTopBar = false;
				display->setTextColor(WHITE, BLACK);
				display->setTextSize(1);
				display->setCursor(0, 30);	
				display->println("Screen_SaveSuccess!");
				break;
			case Screen_SaveError:
				ExtDisplay::showTopBar = false;
				display->setTextColor(WHITE, BLACK);
				display->setTextSize(1);
				display->setCursor(0, 30);	
				display->println("Screen_SaveError!");
				break;
			case Screen_Calibrating:
				ExtDisplay::showTopBar = false;
				break;
			case Screen_IRTest:
				ExtDisplay::showTopBar = false;
				break;
			case Screen_Docked:
				ExtDisplay::showTopBar = false;
				break;
			case Screen_Mamehook_Single:
				ExtDisplay::showTopBar = true;
				break;
			case Screen_Mamehook_Dual:
				ExtDisplay::showTopBar = false;
				break;				
		}
    ExtDisplay::screenStateChange = false;
		ExtDisplay::content_change = true;  
}

void ExtDisplay::UpdateMameHookerSingle(bool forceFullUpdate){
  bool doChange = false;
  if(forceFullUpdate){
    ammo_change = false;
    life_change = false;
    serialDisplayValue_change = true;
    if(serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Ammo){
      ammo_change = true;
    }
    if(serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Life){
      life_change = true;
    }
  }

  if(showReload && serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Ammo){
    if(showReload){
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisReload >= 500 || previousMillisReload == 0) {
            previousMillisReload = currentMillis; // Mise à jour du dernier moment
            reloadIsVisible = !reloadIsVisible; // Basculer la visibilité
            doChange = true;
            display->fillRect(0, 12, SCREEN_WIDTH, SCREEN_HEIGHT-12, BLACK);
            display->drawBitmap(SCREEN_WIDTH-LIFEBAR_BANNERNEW_WIDTH, 13, ammoBannerNew, LIFEBAR_BANNERNEW_WIDTH, LIFEBAR_BANNERNEW_HEIGHT, WHITE);            
        }
        if (reloadIsVisible){
          display->setTextColor(WHITE, BLACK);
          display->setTextSize(3);
          display->setCursor(10, 25);				
          display->println("RELOAD");          
        }
    }
  }  

  if((ammo_change || useAmmoBullets_change || serialDisplayValue_change) && serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Ammo && !showReload){
    doChange = true;
    display->fillRect(0, 12, SCREEN_WIDTH, SCREEN_HEIGHT-12, BLACK);
    display->drawBitmap(SCREEN_WIDTH-LIFEBAR_BANNERNEW_WIDTH, 13, ammoBannerNew, LIFEBAR_BANNERNEW_WIDTH, LIFEBAR_BANNERNEW_HEIGHT, WHITE);

    if(ammoMax <=32 && useAmmoBullets){
      const uint8_t* iconToUse = bulletLarge;
      uint8_t spacingX = 0;
      uint8_t spacingY = 5;
      int maxIcoPerLine = 16;
      int bulletSizeWidth = BULLET_LARGE_WIDTH;
      int bulletSizeHeight = BULLET_LARGE_HEIGHT;

      if(ammoMax <=8){
        iconToUse = bulletExtraLarge;
        spacingX = 0;
        if(ammoMax <=7) spacingX = 2;
        spacingY = 5;
        maxIcoPerLine = 8;
        bulletSizeWidth = BULLET_EXTRALARGE_WIDTH;
        bulletSizeHeight = BULLET_EXTRALARGE_HEIGHT;   
      }  
      else if(ammoMax <=11){
      iconToUse = bulletLarge;
      spacingX = 0;
      if(ammoMax ==10) spacingX = 1;
      if(ammoMax <=9) spacingX = 2;
      spacingY = 5;
      maxIcoPerLine = 11;
      bulletSizeWidth = BULLET_LARGE_WIDTH;
      bulletSizeHeight = BULLET_LARGE_HEIGHT;   
      }
      else if(ammoMax <=32){
      iconToUse = bulletSmall;
      spacingX = 0;
      spacingY = 5;
      maxIcoPerLine = 16;
      bulletSizeWidth = BULLET_SMALL_WIDTH;
      bulletSizeHeight = BULLET_SMALL_HEIGHT;   

      }

      if (ammoMax < maxIcoPerLine && ammoMax > 0) maxIcoPerLine = ammoMax;

      int nbLines = (ammoValue + maxIcoPerLine - 1) / maxIcoPerLine;
      int nbOnFirstLine = ammoValue % maxIcoPerLine;
      if (nbOnFirstLine == 0) nbOnFirstLine = maxIcoPerLine;

      int posX = (SCREEN_WIDTH - (maxIcoPerLine * (bulletSizeWidth + spacingX) - spacingX)) / 2;
      int posY = (((SCREEN_HEIGHT - 22) - (nbLines * (bulletSizeHeight + spacingY) - spacingY)) / 2) + 22;

      int drawn = 0;

      for (int line = 0; line < nbLines; line++) {
          int heartsOnThisLine = (line == 0) ? nbOnFirstLine : maxIcoPerLine;
          int emptySpaces = maxIcoPerLine - heartsOnThisLine;
          int adjustedPosX = posX + emptySpaces * (bulletSizeWidth + spacingX);

          for (int i = 0; i < heartsOnThisLine; i++) {
              display->drawBitmap(
                  adjustedPosX + i * (bulletSizeWidth + spacingX),
                  posY,
                  iconToUse,
                  bulletSizeWidth,
                  bulletSizeHeight,
                  WHITE
              );
              drawn++;
              if (drawn >= ammoValue) break;
          }
          posY += bulletSizeHeight + spacingY;
      }
      
    }
    else{
      uint8_t hundred = ammoValue / 100;
      uint8_t ten = (ammoValue / 10) % 10;
      uint8_t unit = ammoValue % 10;
      uint8_t spacing = 4;
      uint8_t glyphWidthWithSpacing = NUMBER_GLYPH_WIDTH + spacing;
      uint8_t posX = 0;
      uint8_t visibleDigits = 0;
      //display->fillRect((SCREEN_WIDTH - ((NUMBER_GLYPH_WIDTH + spacing) * 3 - spacing)) / 2, 22, (NUMBER_GLYPH_WIDTH + spacing) * 3 - spacing, NUMBER_GLYPH_HEIGHT, BLACK);
      if (hundred > 0) visibleDigits++;
      if (hundred > 0 || ten > 0) visibleDigits++; // Affiche les dizaines si les centaines sont visibles
      visibleDigits++; // Toujours afficher les unités
      int totalSize = (NUMBER_GLYPH_WIDTH + spacing) * visibleDigits - spacing;
      posX = (SCREEN_WIDTH - totalSize) / 2;
      if (hundred > 0) {
          display->drawBitmap(posX, 22, number_bitmaps[hundred], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
          posX += glyphWidthWithSpacing;
      }
      if (hundred > 0 || ten > 0) { // Afficher les dizaines si les centaines sont affichées ou > 0
          display->drawBitmap(posX, 22, number_bitmaps[ten], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
          posX += glyphWidthWithSpacing;
      }
      display->drawBitmap(posX, 22, number_bitmaps[unit], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);

    }
  }

  
  if((life_change || useLifeBar_change || serialDisplayValue_change) && serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Life){
    doChange = true;
    display->fillRect(0, 12, SCREEN_WIDTH, SCREEN_HEIGHT-12, BLACK);
    display->drawBitmap(SCREEN_WIDTH-LIFEBAR_BANNERNEW_WIDTH, 13, lifeBarBannerNew, LIFEBAR_BANNERNEW_WIDTH, LIFEBAR_BANNERNEW_HEIGHT, WHITE);
    if(useLifeBar && lifeMax>0){
      int dispLifePercentage = (100 * lifeValue) / lifeMax; // Calculate the Life % to show 
      display->drawBitmap(52, 23, lifeBarBanner, LIFEBAR_BANNER_WIDTH, LIFEBAR_BANNER_HEIGHT, WHITE);
      display->drawBitmap(11, 35, lifeBarLarge, LIFEBAR_LARGE_WIDTH, LIFEBAR_LARGE_HEIGHT, WHITE);
      display->fillRect(14, 37, 100, 9, BLACK);
      display->fillRect(52, 51, 30, 8, BLACK);
      display->fillRect(14, 37, dispLifePercentage, 9, WHITE);
      display->setTextSize(1);
      display->setCursor(52, 51);
      display->setTextColor(WHITE, BLACK);
      display->print(dispLifePercentage);
      display->println(" %");


    }
    else{
      if(lifeMax >10){
        uint8_t hundred = lifeValue / 100;
        uint8_t ten = (lifeValue / 10) % 10;
        uint8_t unit = lifeValue % 10;
        uint8_t spacing = 4;
        uint8_t glyphWidthWithSpacing = NUMBER_GLYPH_WIDTH + spacing;
        uint8_t posX = 0;
        uint8_t visibleDigits = 0;
        if (hundred > 0) visibleDigits++;
        if (hundred > 0 || ten > 0) visibleDigits++;
        visibleDigits++;
        int totalSize = (NUMBER_GLYPH_WIDTH + spacing) * visibleDigits - spacing;
        posX = (SCREEN_WIDTH - totalSize) / 2;
        if (hundred > 0) {
            display->drawBitmap(posX, 22, number_bitmaps[hundred], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
            posX += glyphWidthWithSpacing;
        }
        if (hundred > 0 || ten > 0) {
            display->drawBitmap(posX, 22, number_bitmaps[ten], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
            posX += glyphWidthWithSpacing;
        }
        display->drawBitmap(posX, 22, number_bitmaps[unit], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
      }
      else{
        uint8_t spacing = 4;
        int maxIcoPerLine = 5;
        if (lifeMax < maxIcoPerLine && lifeMax > 0) maxIcoPerLine = lifeMax;

        int nbLines = (lifeValue + maxIcoPerLine - 1) / maxIcoPerLine;
        int nbOnFirstLine = lifeValue % maxIcoPerLine;
        if (nbOnFirstLine == 0) nbOnFirstLine = maxIcoPerLine;

        int posX = (SCREEN_WIDTH - (maxIcoPerLine * (HEART_NEW_WIDTH + spacing) - spacing)) / 2;
        int posY = (((SCREEN_HEIGHT - 22) - (nbLines * (HEART_NEW_HEIGHT + spacing) - spacing)) / 2) + 22;

        int drawn = 0;

        for (int line = 0; line < nbLines; line++) {
            int heartsOnThisLine = (line == 0) ? nbOnFirstLine : maxIcoPerLine;
            int emptySpaces = maxIcoPerLine - heartsOnThisLine;
            int adjustedPosX = posX + emptySpaces * (HEART_NEW_WIDTH + spacing);

            for (int i = 0; i < heartsOnThisLine; i++) {
                display->drawBitmap(
                    adjustedPosX + i * (HEART_NEW_WIDTH + spacing),
                    posY,
                    lifeIcoNew,
                    HEART_NEW_WIDTH,
                    HEART_NEW_HEIGHT,
                    WHITE
                );
                drawn++;
                if (drawn >= lifeValue) break;
            }
            posY += HEART_NEW_HEIGHT + spacing;
        }
      }

    }
  }

  if(doChange && ExtDisplay::screenState == Screen_Mamehook_Single){
    ammo_change = false;
    life_change = false;
    content_change = true;
  }

}

void ExtDisplay::UpdateMameHookerDual(bool forceFullUpdate){
  bool doChange = false;
  if(forceFullUpdate){
    ammo_change = true;
    life_change = true;
  }

  if((life_change || useLifeBar_change) && serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Both){
    doChange = true;
    display->fillRect(0, 0, SCREEN_WIDTH, 13, BLACK);
    //display->drawLine(0,14,SCREEN_WIDTH,14, WHITE);
    if(useLifeBar && lifeMax>0){
      
      int dispLifePercentage = (100 * lifeValue) / lifeMax; // Calculate the Life % to show 
      display->drawBitmap(11, 0, lifeBarLarge, LIFEBAR_LARGE_WIDTH, LIFEBAR_LARGE_HEIGHT, WHITE);
      display->fillRect(14, 2, 100, 9, BLACK);
      display->fillRect(14, 2, dispLifePercentage, 9, WHITE);
      display->setTextSize(1);
      if(dispLifePercentage > 40 && dispLifePercentage < 60) display->setCursor(35,2); 
      else display->setCursor(55, 2);
      display->setTextColor(WHITE, dispLifePercentage > 40 ? BLACK : WHITE);
      display->println(String(dispLifePercentage) + "%");
      

    }
    else{
      if(lifeMax >8){
        display->setTextSize(1);
        display->setCursor(58, 0);
        display->setTextColor(WHITE, BLACK);
        display->println("Life : " + String(lifeValue));
      }
      else{
        uint8_t spacing = 1;
        if(lifeValue == 7) spacing = 3;
        if(lifeValue < 7 ) spacing = 4;
        
        int maxIcoPerLine = 8;
        if (lifeMax < maxIcoPerLine && lifeMax > 0) maxIcoPerLine = lifeMax;

        int nbLines = (lifeValue + maxIcoPerLine - 1) / maxIcoPerLine;
        int nbOnFirstLine = lifeValue % maxIcoPerLine;
        if (nbOnFirstLine == 0) nbOnFirstLine = maxIcoPerLine;

        int posX = (SCREEN_WIDTH - (maxIcoPerLine * (HEART_NEW_WIDTH + spacing) - spacing)) / 2;
        int posY = 0;

        int drawn = 0;

        for (int line = 0; line < nbLines; line++) {
            int heartsOnThisLine = (line == 0) ? nbOnFirstLine : maxIcoPerLine;
            int emptySpaces = maxIcoPerLine - heartsOnThisLine;
            int adjustedPosX = posX + emptySpaces * (HEART_NEW_WIDTH + spacing);

            for (int i = 0; i < heartsOnThisLine; i++) {
                display->drawBitmap(
                    adjustedPosX + i * (HEART_NEW_WIDTH + spacing),
                    posY,
                    lifeIcoNew,
                    HEART_NEW_WIDTH,
                    HEART_NEW_HEIGHT,
                    WHITE
                );
                drawn++;
                if (drawn >= lifeValue) break;
            }
            posY += HEART_NEW_HEIGHT + spacing;
        }
      }

    }
  }

  if(showReload && serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Both){
    if(showReload){
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisReload >= 500 || previousMillisReload == 0) {
            previousMillisReload = currentMillis; // Mise à jour du dernier moment
            reloadIsVisible = !reloadIsVisible; // Basculer la visibilité
            doChange = true;
            display->fillRect(0, 14, SCREEN_WIDTH, SCREEN_HEIGHT-14, BLACK);         
        }
        if (reloadIsVisible){
          display->setTextColor(WHITE, BLACK);
          display->setTextSize(3);
          display->setCursor(10, 25);				
          display->println("RELOAD");          
        }
    }
  }  

  if((ammo_change || useAmmoBullets_change || serialDisplayValue_change) && serialDisplayValue == ScreenSerialInit_e::ScreenSerial_Both && !showReload){
    doChange = true;
    display->fillRect(0, 14, SCREEN_WIDTH, SCREEN_HEIGHT-14, BLACK);

    if(ammoMax <=32 && useAmmoBullets){
      const uint8_t* iconToUse = bulletLarge;
      uint8_t spacingX = 0;
      uint8_t spacingY = 5;
      int maxIcoPerLine = 16;
      int bulletSizeWidth = BULLET_LARGE_WIDTH;
      int bulletSizeHeight = BULLET_LARGE_HEIGHT;

      if(ammoMax <=8){
        iconToUse = bulletExtraLarge;
        spacingX = 0;
        if(ammoMax <=7) spacingX = 2;
        spacingY = 5;
        maxIcoPerLine = 8;
        bulletSizeWidth = BULLET_EXTRALARGE_WIDTH;
        bulletSizeHeight = BULLET_EXTRALARGE_HEIGHT;   
      }  
      else if(ammoMax <=11){
      iconToUse = bulletLarge;
      spacingX = 0;
      if(ammoMax ==10) spacingX = 1;
      if(ammoMax <=9) spacingX = 2;
      spacingY = 5;
      maxIcoPerLine = 11;
      bulletSizeWidth = BULLET_LARGE_WIDTH;
      bulletSizeHeight = BULLET_LARGE_HEIGHT;   
      }
      else if(ammoMax <=32){
      iconToUse = bulletSmall;
      spacingX = 0;
      spacingY = 5;
      maxIcoPerLine = 16;
      bulletSizeWidth = BULLET_SMALL_WIDTH;
      bulletSizeHeight = BULLET_SMALL_HEIGHT;   

      }

      if (ammoMax < maxIcoPerLine && ammoMax > 0) maxIcoPerLine = ammoMax;

      int nbLines = (ammoValue + maxIcoPerLine - 1) / maxIcoPerLine;
      int nbOnFirstLine = ammoValue % maxIcoPerLine;
      if (nbOnFirstLine == 0) nbOnFirstLine = maxIcoPerLine;

      int posX = (SCREEN_WIDTH - (maxIcoPerLine * (bulletSizeWidth + spacingX) - spacingX)) / 2;
      int posY = (((SCREEN_HEIGHT - 22) - (nbLines * (bulletSizeHeight + spacingY) - spacingY)) / 2) + 22;

      int drawn = 0;

      for (int line = 0; line < nbLines; line++) {
          int heartsOnThisLine = (line == 0) ? nbOnFirstLine : maxIcoPerLine;
          int emptySpaces = maxIcoPerLine - heartsOnThisLine;
          int adjustedPosX = posX + emptySpaces * (bulletSizeWidth + spacingX);

          for (int i = 0; i < heartsOnThisLine; i++) {
              display->drawBitmap(
                  adjustedPosX + i * (bulletSizeWidth + spacingX),
                  posY,
                  iconToUse,
                  bulletSizeWidth,
                  bulletSizeHeight,
                  WHITE
              );
              drawn++;
              if (drawn >= ammoValue) break;
          }
          posY += bulletSizeHeight + spacingY;
      }
      
    }
    else{
      uint8_t hundred = ammoValue / 100;
      uint8_t ten = (ammoValue / 10) % 10;
      uint8_t unit = ammoValue % 10;
      uint8_t spacing = 4;
      uint8_t glyphWidthWithSpacing = NUMBER_GLYPH_WIDTH + spacing;
      uint8_t posX = 0;
      uint8_t visibleDigits = 0;
      //display->fillRect((SCREEN_WIDTH - ((NUMBER_GLYPH_WIDTH + spacing) * 3 - spacing)) / 2, 22, (NUMBER_GLYPH_WIDTH + spacing) * 3 - spacing, NUMBER_GLYPH_HEIGHT, BLACK);
      if (hundred > 0) visibleDigits++;
      if (hundred > 0 || ten > 0) visibleDigits++; // Affiche les dizaines si les centaines sont visibles
      visibleDigits++; // Toujours afficher les unités
      int totalSize = (NUMBER_GLYPH_WIDTH + spacing) * visibleDigits - spacing;
      posX = (SCREEN_WIDTH - totalSize) / 2;
      if (hundred > 0) {
          display->drawBitmap(posX, 22, number_bitmaps[hundred], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
          posX += glyphWidthWithSpacing;
      }
      if (hundred > 0 || ten > 0) { // Afficher les dizaines si les centaines sont affichées ou > 0
          display->drawBitmap(posX, 22, number_bitmaps[ten], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);
          posX += glyphWidthWithSpacing;
      }
      display->drawBitmap(posX, 22, number_bitmaps[unit], NUMBER_GLYPH_WIDTH, NUMBER_GLYPH_HEIGHT, WHITE);

    }
  }

  if(doChange && ExtDisplay::screenState == Screen_Mamehook_Dual){
    ammo_change = false;
    life_change = false;
    content_change = true;
  }  

}

void ExtDisplay::PauseMenuDrawAroundItem(int itemNum,uint16_t color) {
	
	itemNum--;
    // Calcul des coordonnées de la grille
    int row = itemNum / 5; // Ligne de l'icône
    int col = itemNum % 5; // Colonne de l'icône

    // Calcul des positions de l'icône
    int x = 5 + col * (ICON_WIDTH + 7); // Position X
    int y = 14 + row * (ICON_HEIGHT + 6); // Position Y

    // Cadre de sélection (2 pixels autour de l'icône : 1 noir + 1 blanc pour le cadre)
    int x1 = x - 2; // Bord gauche
    int y1 = y - 2; // Bord haut
    int x2 = x + ICON_WIDTH + 1; // Bord droit
    int y2 = y + ICON_HEIGHT + 1; // Bord bas

    // Dessin du cadre
    display->drawFastHLine(x1, y1, ICON_WIDTH + 4, color);        // Ligne supérieure
    display->drawFastHLine(x1, y2, ICON_WIDTH + 4, color);        // Ligne inférieure
    display->drawFastVLine(x1, y1, ICON_HEIGHT + 4, color);       // Ligne gauche
    display->drawFastVLine(x2, y1, ICON_HEIGHT + 4, color);       // Ligne droite
}

void ExtDisplay::PauseMenuIconPosition(int itemNum, int &x, int &y) {
	itemNum--;
    // Calcul des coordonnées
    int row = itemNum / 5; // Ligne de l'icône
    int col = itemNum % 5; // Colonne de l'icône

    x = 5 + col * (ICON_WIDTH + 7); // x
    y = 14 + row * (ICON_HEIGHT + 6); // y
}

void ExtDisplay::SelectPauseItemNext(){
  int nextItem = (int)selectedPauseItem+1;
  if(nextItem>11 & nextItem<15) nextItem = 15;
	ExtDisplay::SelectPauseItem((ExtDisplay::PauseItem_e)(nextItem));
}

void ExtDisplay::SelectPauseItemPrevious(){
  int nextItem = (int)selectedPauseItem-1;
  if(nextItem<15 && nextItem>11) nextItem = 11;

	ExtDisplay::SelectPauseItem((ExtDisplay::PauseItem_e)(nextItem));
}

void ExtDisplay::SelectPauseItemUp(){
  int nextItem = (int)selectedPauseItem-5;
  if(nextItem<PauseItem_Profile) nextItem = PauseItem_Save + nextItem;
  if(nextItem>11 & nextItem<15){
    nextItem -= 5;
    if(nextItem<PauseItem_Profile) nextItem = PauseItem_Save + nextItem;
  }

	ExtDisplay::SelectPauseItem((ExtDisplay::PauseItem_e)(nextItem));
}

void ExtDisplay::SelectPauseItemDown(){
  int nextItem = (int)selectedPauseItem+5;
  if(nextItem>PauseItem_Save) nextItem = nextItem - PauseItem_Save;
  if(nextItem>11 & nextItem<15){
    nextItem += 5;
    if(nextItem>PauseItem_Save) nextItem = nextItem - PauseItem_Save;
  }
	ExtDisplay::SelectPauseItem((ExtDisplay::PauseItem_e)(nextItem));
}

void ExtDisplay::SelectPauseItem(ExtDisplay::PauseItem_e pauseItem, bool force){
  if(pauseItem<PauseItem_Profile) pauseItem = PauseItem_Save;
  if(pauseItem>PauseItem_Save) pauseItem = PauseItem_Profile;

	if(pauseItem == selectedPauseItem && !force) return;
	selectedPauseItem = pauseItem;
	for(int i=0;i<15;i++) ExtDisplay::PauseMenuDrawAroundItem(i+1,BLACK);
	ExtDisplay::PauseMenuDrawAroundItem(pauseItem,WHITE);

  labelPauseItem = "";
  ExtDisplay::UpdatePauseLabel();


	ExtDisplay::content_change = true;
}

void ExtDisplay::UpdatePauseLabel(){
    String previousLabel = labelPauseItem;
    if(selectedPauseItem == PauseItem_Profile) labelPauseItem = SamcoPreferences::profiles.pProfileData[SamcoPreferences::profiles.selectedProfile].name;
    if(selectedPauseItem == PauseItem_Rumble){ 
      labelPauseItem = SamcoPreferences::GetRumbleActive() ? "Rumble On" : "Rumble Off";
      if(ExtDisplay::rumbleActive >= 10) labelPauseItem += " (Tmp)";
    }
    if(selectedPauseItem == PauseItem_Solenoid){
      labelPauseItem = SamcoPreferences::GetSolenoidActive() ? "Solenoid On" : "Solenoid Off";
      if(ExtDisplay::solenoidActive >= 10) labelPauseItem += " (Tmp)";
    }
    if(selectedPauseItem == PauseItem_RumbleFF){
      labelPauseItem = SamcoPreferences::GetRumbleFF() ? "Rumble FF On" : "Rumble FF Off";
      if(ExtDisplay::rumbleFF >= 10) labelPauseItem += " (Tmp)";
    }

    if(selectedPauseItem == PauseItem_Autofire){
      labelPauseItem = SamcoPreferences::GetAutofireActive() ? "Autofire On" : "Autofire Off";
      if(ExtDisplay::autofireActive >= 10) labelPauseItem += " (Tmp)";
    }
    if(selectedPauseItem == PauseItem_ControlMode){
      labelPauseItem = SamcoPreferences::GetControlMode() == 0 ? "Mouse" : SamcoPreferences::GetControlMode() == 1 ? "Gamepad" : "Gamepad Alt";
      if(ExtDisplay::controlMode >= 10) labelPauseItem += " (Tmp)";
    }

    if(selectedPauseItem == PauseItem_WideScreenMode){
      labelPauseItem = SamcoPreferences::GetWideScreenMode() ? "16/9" : "4/3";
      if(ExtDisplay::wideScreenMode >= 10) labelPauseItem += " (Tmp)";
    }

    if(selectedPauseItem == PauseItem_Led) labelPauseItem = "LED Settings";
    if(selectedPauseItem == PauseItem_Wifi) labelPauseItem = "Wifi Settings";

    if(selectedPauseItem == PauseItem_Shortcuts) labelPauseItem = "Edit Shortcuts";
    if(selectedPauseItem == PauseItem_Calibrate) labelPauseItem = "Calibrate";
    if(selectedPauseItem == PauseItem_Save) labelPauseItem = "Save Settings";
    if(previousLabel != labelPauseItem){
      display->fillRect(0, 0, 110, 12, BLACK);
      display->setTextColor(WHITE, BLACK);
      display->setTextSize(1);
      display->setCursor(0, 3);				
      display->println(labelPauseItem);
    }
}

void ExtDisplay::UpdatePauseMenu(bool forceFullUpdate){
	if(forceFullUpdate){
		display->fillRect(0, 0, 128, 64, BLACK);
		ExtDisplay::rumbleActive_change = true;
		ExtDisplay::solenoidActive_change = true;
		ExtDisplay::autofireActive_change = true;
		ExtDisplay::rumbleFF_change = true;
		ExtDisplay::controlMode_change = true;
		ExtDisplay::wifiStatus_change = true;
		ExtDisplay::profileIndex_change = true;
		ExtDisplay::mamehookerActive_change = true;	
    ExtDisplay::wideScreenMode_change = true;
    labelPauseItem = "";
	}
	
	if((ExtDisplay::rumbleActive_change + ExtDisplay::solenoidActive_change + ExtDisplay::autofireActive_change + ExtDisplay::rumbleFF_change + ExtDisplay::controlMode_change + ExtDisplay::wifiStatus_change + ExtDisplay::profileIndex_change + ExtDisplay::mamehookerActive_change + ExtDisplay::wideScreenMode_change)>0){


    ExtDisplay::UpdatePauseLabel();

    int posx, posy;
  	if(ExtDisplay::profileIndex_change){
      display->fillRect(110, 0, 18, 12, BLACK);
			if(ExtDisplay::profileIndex == 0) display->drawBitmap(110, 0, profileAIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::profileIndex == 1) display->drawBitmap(110, 0, profileBIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::profileIndex == 2) display->drawBitmap(110, 0, profileCIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::profileIndex == 3) display->drawBitmap(110, 0, profileSIco, ICON_WIDTH, ICON_HEIGHT, WHITE);

      PauseMenuIconPosition(PauseItem_e::PauseItem_Profile,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::profileIndex == 0) display->drawBitmap(posx, posy, profileAIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::profileIndex == 1) display->drawBitmap(posx, posy, profileBIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::profileIndex == 2) display->drawBitmap(posx, posy, profileCIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::profileIndex == 3) display->drawBitmap(posx, posy, profileSIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}
		
		if(ExtDisplay::controlMode_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_ControlMode,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::controlMode == 0) display->drawBitmap(posx, posy, mouseNewIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::controlMode == 1) display->drawBitmap(posx, posy, gamepadIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::controlMode == 2) display->drawBitmap(posx, posy, gamepadAltIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::controlMode == 10) display->drawBitmap(posx, posy, mouseNewIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::controlMode == 11) display->drawBitmap(posx, posy, gamepadIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::controlMode == 12) display->drawBitmap(posx, posy, gamepadAltIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}

		if(ExtDisplay::solenoidActive_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_Solenoid,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::solenoidActive == 0) display->drawBitmap(posx, posy, selenoidOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::solenoidActive == 1) display->drawBitmap(posx, posy, selenoidONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::solenoidActive == 10) display->drawBitmap(posx, posy, selenoidOFFIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::solenoidActive == 11) display->drawBitmap(posx, posy, selenoidONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}

		if(ExtDisplay::rumbleActive_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_Rumble,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::rumbleActive == 0) display->drawBitmap(posx, posy, rumbleOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::rumbleActive == 1) display->drawBitmap(posx, posy, rumbleONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::rumbleActive == 10) display->drawBitmap(posx, posy, rumbleOFFIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::rumbleActive == 11) display->drawBitmap(posx, posy, rumbleONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}        

		if(ExtDisplay::rumbleFF_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_RumbleFF,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::rumbleFF == 0) display->drawBitmap(posx, posy, rumbleFFOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::rumbleFF == 1) display->drawBitmap(posx, posy, rumbleFFONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::rumbleFF == 10) display->drawBitmap(posx, posy, rumbleFFOFFIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::rumbleFF == 11) display->drawBitmap(posx, posy, rumbleFFONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}

		if(ExtDisplay::autofireActive_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_Autofire,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::autofireActive == 0) display->drawBitmap(posx, posy, autofireOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::autofireActive == 1) display->drawBitmap(posx, posy, autofireONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::autofireActive == 10) display->drawBitmap(posx, posy, autofireOFFIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::autofireActive == 11) display->drawBitmap(posx, posy, autofireONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}

		if(ExtDisplay::wideScreenMode_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_WideScreenMode,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
			if(ExtDisplay::wideScreenMode == 0) display->drawBitmap(posx, posy, widescreenOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::wideScreenMode == 1) display->drawBitmap(posx, posy, widescreenONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::wideScreenMode == 10) display->drawBitmap(posx, posy, widescreenOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::wideScreenMode == 11) display->drawBitmap(posx, posy, widescreenONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
		}

    PauseMenuIconPosition(PauseItem_e::PauseItem_Led,posx,posy);
    display->drawBitmap(posx, posy, diodeIco, ICON_WIDTH, ICON_HEIGHT, WHITE);

    PauseMenuIconPosition(PauseItem_e::PauseItem_Wifi,posx,posy);
    display->drawBitmap(posx,posy, wifiONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);

    PauseMenuIconPosition(PauseItem_e::PauseItem_Shortcuts,posx,posy);
    display->drawBitmap(posx,posy, shortcutsIco, ICON_WIDTH, ICON_HEIGHT, WHITE);

    PauseMenuIconPosition(PauseItem_e::PauseItem_Calibrate,posx,posy);
    display->drawBitmap(posx,posy, calibrateIco, ICON_WIDTH, ICON_HEIGHT, WHITE);      

    PauseMenuIconPosition(PauseItem_e::PauseItem_Save,posx,posy);
    display->drawBitmap(posx,posy, saveIco, ICON_WIDTH, ICON_HEIGHT, WHITE);   


/*
		if(ExtDisplay::wifiStatus_change){
      PauseMenuIconPosition(PauseItem_e::PauseItem_Wifi,posx,posy);
      display->fillRect(posx, posy, 18, 12, BLACK);
      if(ExtDisplay::wifiStatus == 0) display->drawBitmap(posx,posy, wifiOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::wifiStatus == 1) display->drawBitmap(posx,posy, wifiONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
			if(ExtDisplay::wifiStatus == 2) display->drawBitmap(posx,posy, wifiAPIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
		} 		     		
*/

		ExtDisplay::SelectPauseItem(selectedPauseItem);

		ExtDisplay::content_change = true;		
		
	}
	
}

void ExtDisplay::SleepMode(){
  //display->clearDisplay();
  display->fillRect(0, 0, 128, 64, BLACK);
  display->setTextColor(WHITE, BLACK);
  display->setTextSize(1);
  display->setCursor(0, 30);				
  display->println("Enter Sleep Mode");
  Display();
  delay(1000);
  //display->clearDisplay();
  display->fillRect(0, 0, 128, 64, BLACK);
  Display();  
}

void ExtDisplay::UpdateTopBar(bool forceFullUpdate){
  if(!topBar_change && !forceFullUpdate){
    return;
  } 
	if(!ExtDisplay::showTopBar){
    return;
  }
	
	if(forceFullUpdate){
		//display->fillRect(0, 0, 128, 12, BLACK);
		ExtDisplay::rumbleActive_change = true;
		ExtDisplay::solenoidActive_change = true;
		ExtDisplay::autofireActive_change = true;
		ExtDisplay::rumbleFF_change = true;
		ExtDisplay::controlMode_change = true;
		ExtDisplay::wifiStatus_change = true;
		ExtDisplay::profileIndex_change = true;
		ExtDisplay::mamehookerActive_change = true;	
	}


if((ExtDisplay::rumbleActive_change + ExtDisplay::solenoidActive_change + ExtDisplay::autofireActive_change + ExtDisplay::rumbleFF_change + ExtDisplay::controlMode_change + ExtDisplay::wifiStatus_change + ExtDisplay::profileIndex_change + ExtDisplay::mamehookerActive_change)>0){
		if(ExtDisplay::controlMode_change) display->fillRect(0, 0, 18, 12, BLACK);
		if(ExtDisplay::solenoidActive_change) display->fillRect(20, 0, 18, 12, BLACK);
		if(ExtDisplay::rumbleActive_change) display->fillRect(40, 0, 18, 12, BLACK);
		if(ExtDisplay::rumbleFF_change || ExtDisplay::autofireActive_change) display->fillRect(60, 0, 18, 12, BLACK);
		if(ExtDisplay::wifiStatus_change) display->fillRect(80, 0, 10, 12, BLACK);
		if(ExtDisplay::mamehookerActive_change) display->fillRect(90, 0, 18, 12, BLACK);
		if(ExtDisplay::profileIndex_change) display->fillRect(110, 0, 18, 12, BLACK);
	
	
    if(forceFullUpdate || ExtDisplay::controlMode_change){
      if(ExtDisplay::controlMode == 0) display->drawBitmap(0, 0, mouseNewIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::controlMode == 1) display->drawBitmap(0, 0, gamepadIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::controlMode == 2) display->drawBitmap(0, 0, gamepadAltIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::controlMode == 10) display->drawBitmap(0, 0, mouseNewIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::controlMode == 11) display->drawBitmap(0, 0, gamepadIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::controlMode == 12) display->drawBitmap(0, 0, gamepadAltIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
    }
    
    if(forceFullUpdate || ExtDisplay::solenoidActive_change){
      if(ExtDisplay::solenoidActive == 0) display->drawBitmap(20, 0, selenoidOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::solenoidActive == 1) display->drawBitmap(20, 0, selenoidONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::solenoidActive == 10) display->drawBitmap(20, 0, selenoidOFFIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::solenoidActive == 11) display->drawBitmap(20, 0, selenoidONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
    }
    
    if(forceFullUpdate || ExtDisplay::rumbleActive_change){
      if(ExtDisplay::rumbleActive == 0) display->drawBitmap(40, 0, rumbleOFFIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::rumbleActive == 1) display->drawBitmap(40, 0, rumbleONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::rumbleActive == 10) display->drawBitmap(40, 0, rumbleOFFIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::rumbleActive == 11) display->drawBitmap(40, 0, rumbleONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);
    }
    
    if(forceFullUpdate || ExtDisplay::rumbleFF_change || ExtDisplay::autofireActive_change){
      if(ExtDisplay::rumbleFF == 1 || ExtDisplay::rumbleFF == 11){
        if(ExtDisplay::rumbleFF == 1) display->drawBitmap(60, 0, rumbleFFONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
        if(ExtDisplay::rumbleFF == 11) display->drawBitmap(60, 0, rumbleFFONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);	
      }
      else{
        if(ExtDisplay::autofireActive == 1 || ExtDisplay::autofireActive == 11){
          if(ExtDisplay::autofireActive == 1) display->drawBitmap(60, 0, autofireONIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
          if(ExtDisplay::autofireActive == 11) display->drawBitmap(60, 0, autofireONIcoTemp, ICON_WIDTH, ICON_HEIGHT, WHITE);				
        }
      }
    }


    if(forceFullUpdate || ExtDisplay::wifiStatus_change){
      if(ExtDisplay::wifiStatus == 1) display->drawBitmap(80, 0, wifiONIcoMini, ICONMINI_WIDTH, ICONMINI_HEIGHT, WHITE);
      if(ExtDisplay::wifiStatus == 2) display->drawBitmap(80, 0, wifiAPIcoMini, ICONMINI_WIDTH, ICONMINI_HEIGHT, WHITE);
    }

    if(forceFullUpdate || ExtDisplay::mamehookerActive_change){
      if(ExtDisplay::mamehookerActive == 1) display->drawBitmap(90, 0, mamehookerNewIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
    }	
    
    if(forceFullUpdate || ExtDisplay::profileIndex_change){
      if(ExtDisplay::profileIndex == 0) display->drawBitmap(110, 0, profileAIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::profileIndex == 1) display->drawBitmap(110, 0, profileBIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::profileIndex == 2) display->drawBitmap(110, 0, profileCIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
      if(ExtDisplay::profileIndex == 3) display->drawBitmap(110, 0, profileSIco, ICON_WIDTH, ICON_HEIGHT, WHITE);
    }
    
    ExtDisplay::content_change = true;
  }
	//ExtDisplay::topBar_change = false;
}
	
void ExtDisplay::Display()
{
    #ifdef USES_NUNCHUCK
    if(SamcoPreferences::pins.pNunchuckSDA >= 0 && SamcoPreferences::pins.pNunchuckSCL >= 0){
      Wire1.end();
      Wire1.setSDA(SamcoPreferences::pins.pPeriphSDA);
      Wire1.setSCL(SamcoPreferences::pins.pPeriphSCL);
      Wire1.begin();
      nunchuckActif = false; 
      display->display();
      unsigned long now = millis();
      if(nextStepNunchuck < now + 10) nextStepNunchuck = now + 10;
      nunchuckStepWrite = true;
      nunchuckActif = true;
      Wire1.end();
      Wire1.setSDA(SamcoPreferences::pins.pNunchuckSDA);
      Wire1.setSCL(SamcoPreferences::pins.pNunchuckSCL);
      Wire1.begin(); 
    }
    else{
      display->display();
    }
    #else
      display->display();
    #endif
}

bool ExtDisplay::Begin()
{
    if(display != nullptr) { display->clearDisplay(); delete display, displayValid = false; }

    if(SamcoPreferences::pins.pPeriphSCL >= 0 && SamcoPreferences::pins.pPeriphSDA >= 0) {
        if(bitRead(SamcoPreferences::pins.pPeriphSCL, 1) && bitRead(SamcoPreferences::pins.pPeriphSDA, 1)) {
            // I2C1
            if(bitRead(SamcoPreferences::pins.pPeriphSCL, 0) && !bitRead(SamcoPreferences::pins.pPeriphSDA, 0)) {
                // SDA/SCL are indeed on verified correct pins
                Wire1.setSDA(SamcoPreferences::pins.pPeriphSDA);
                Wire1.setSCL(SamcoPreferences::pins.pPeriphSCL);
                display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);
                displayValid = true;
            } else {
                displayValid = false;
                return false;
            }
        } else if(!bitRead(SamcoPreferences::pins.pPeriphSCL, 1) && !bitRead(SamcoPreferences::pins.pPeriphSDA, 1)) {
            // I2C0
            if(bitRead(SamcoPreferences::pins.pPeriphSCL, 0) && !bitRead(SamcoPreferences::pins.pPeriphSDA, 0)) {
                // SDA/SCL are indeed on verified correct pins
                Wire.setSDA(SamcoPreferences::pins.pPeriphSDA);
                Wire.setSCL(SamcoPreferences::pins.pPeriphSCL);
                display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
                displayValid = true;
            } else {
                displayValid = false;
                return false;
            }
        } else {
            displayValid = false;
            return false;
        }
    } else {
        displayValid = false;
        return false;
    }

    if(display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        display->clearDisplay();
		display->fillRect(0, 0, 128, 64, BLACK);
		Display();
		//DISPLAYTODOREWRITE
        ScreenModeChange(Screen_None);
        return true;
    } else {
      displayValid = false;
      return false;
    }
}


void ExtDisplay::ScreenModeChange(int8_t screenMode)
{
  ExtDisplay::setScreenState(screenMode);
}

// Warning: SLOOOOW, should only be used in cali/where the mouse isn't being updated.
// Use at your own discression.
void ExtDisplay::DrawVisibleIR(int pointX[4], int pointY[4], unsigned int see[4])
{
    if(displayValid) {
      bool shouldUpdateDisplay = false;
      for (uint8_t i = 0; i < 4; i++) {
          if (pointX[i] != previousPointX[i] || pointY[i] != previousPointY[i] || see[i] != previousSee[i]) {
              shouldUpdateDisplay = true; // Un changement a été détecté
              break; // Sortir de la boucle si un changement est trouvé
          }
      }
      if(shouldUpdateDisplay){
        for (uint8_t i = 0; i < 4; i++) {
          previousPointX[i] = pointX[i];
          previousPointY[i] = pointY[i];
          previousSee[i] = see[i];
        }
        display->fillRect(0, 0, 128, 64, BLACK);
        for(uint8_t i = 0; i < 4; i++) {
          if(see[i] != 0x0){
            pointX[i] = map(pointX[i], 0, 1920, 0, 128);
            pointY[i] = map(pointY[i], 0, 1080, 16, 64);
            pointY[i] = constrain(pointY[i], 16, 64);
            display->fillCircle(pointX[i], pointY[i], 1, WHITE);
          }
        }
        ExtDisplay::content_change = true;
        //Display();
      }
      Update();
    }
}


void ExtDisplay::SendSplashMessage(String message, int duration){
  UseSecondBuffer = millis() + duration;
  display->fillRect(0, 0, 128, 64, BLACK);
  display->setTextColor(WHITE, BLACK);
  display->setTextSize(1);
  display->setCursor(5, 10);
  display->println(message);
  Display();
}

void ExtDisplay::SendNixxLogo(int duration){
  UseSecondBuffer = millis() + duration;
  display->fillRect(0, 0, 128, 64, BLACK);
  display->drawBitmap(0, 0, splashNixxLogo, 128, 64, WHITE);
  Display();
}

void ExtDisplay::SaveScreen(uint8_t status)
{
    if(displayValid) {
        display->fillRect(0, 16, 128, 48, BLACK);
        display->setTextColor(WHITE, BLACK);
        display->setTextSize(2);
        display->setCursor(24, 24);
        display->println("Saving...");
        Display();
    }
}
