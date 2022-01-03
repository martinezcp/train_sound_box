/*
train_sound_box.cpp - Main file for the train sound box application.
Version: 1.0.0
(c) 2021 Pablo Martínez

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "menu_defs.h"
#include "menu.h"

/* Resistors boundaries for the button configuration 
*  In case you use another resistor values, please, update these parameters to match those selected.
*/
#define RESISTOR_UP_LINE_MIN      480
#define RESISTOR_UP_LINE_MAX      550

#define RESISTOR_DOWN_LINE_MIN    995
#define RESISTOR_DOWN_LINE_MAX    1005

#define RESISTOR_INSIDE_MIN      1005
#define RESISTOR_INSIDE_MAX      1015

#define RESISTOR_OUTSIDE_MIN     969
#define RESISTOR_OUTSIDE_MAX     975

#define RESISTOR_UP_VOL_MIN      950
#define RESISTOR_UP_VOL_MAX      960

#define RESISTOR_DOWN_VOL_MIN    925
#define RESISTOR_DOWN_VOL_MAX    932

/* Screen configuration
* I2C address, column number and line number.
*/
#define SCREEN_I2C_ADDR         0x27
#define SCREEN_MAX_COLUMNS      20
#define SCREEN_MAX_LINES        4
/* DFPlayer configuration */
#define DFPLAYER_RX             10
#define DFPLAYER_TX             11

/* Global variables */
SoftwareSerial        dfplayer_serial(DFPLAYER_RX, DFPLAYER_TX);
LiquidCrystal_I2C     lcd(SCREEN_I2C_ADDR, SCREEN_MAX_COLUMNS, SCREEN_MAX_LINES);
DFRobotDFPlayerMini   dfplayer;

/* General variables */
uint8_t volume;
uint8_t option_max;
/* Button pressed variables */
int     sensorPin   = A0;
int     sensorValue = 0;
/* Variables for the base menu */
uint8_t option_menu;
Menu    *menu_selected;
uint8_t option;
uint8_t block_flag;


/** Move an asterisk a number of lines.
 * @param current_position  Current position of the asterisk in the screen.
 * @param movement          Number of lines in one direction or another (Negativa: Upwards, position: downwards).
 */
void move_asterisk(uint8_t current_position, int8_t movement)
{
  lcd.setCursor(1, current_position);
  lcd.print(" ");
  current_position += movement;
  lcd.setCursor(1, current_position);
  lcd.print("*");
}


/** Mark the selected line as the selected option.
 * @param selected_position Line to the used as selected option.
 */
void select_option(int8_t selected_position)
{
  lcd.setCursor(1, selected_position);
  lcd.print(">");
  delay(600);
}

/** Updates the asterisk place to the desired position.
 * @param pos New position for the asterisk.
 */
void place_asterisk(uint8_t pos)
{
  uint8_t i = 0;

  for(i = 0; i < SCREEN_MAX_LINES; i++)
  {
    lcd.setCursor(1, i);
    lcd.print(" ");
  }
  lcd.setCursor(1, pos);
  lcd.print("*");
}

/** Loads the menu on the screen.
 * @param menu        Menu to the shown.
 * @param from_value  Init line to be shown.
 * @param max_value   Max line to be shown.
 */
void load_menu(const char **menu, uint8_t from_value, uint8_t max_value) {
  uint8_t i = 0;
  uint8_t c = 0;

  lcd.clear();
  for (i = from_value; ((i <= max_value) && (c < SCREEN_MAX_LINES)); i++) { // Only print the first X lines to fill the screen
    lcd.setCursor(3, c);
    lcd.print(menu[i]);
    c++;
  }
}

/* Create instances for BASE MENU and FIRST LEVEL MENUS */
Menu menu_base     (menu_base_msg,                (byte)MENU_BASE_MAX);
Menu menu_diesel   (menu_1_msg, &menu_base,       (byte)MENU_1_MAX);
Menu menu_steam    (menu_2_msg, &menu_base,       (byte)MENU_2_MAX);
Menu menu_electric (menu_3_msg, &menu_base,       (byte)MENU_3_MAX);
Menu menu_station  (menu_4_msg, &menu_base,       (byte)MENU_4_MAX,   8);
/* Create instances for DIESEL first level menu */
Menu menu_de10     (menu_1_1_msg, &menu_diesel,   (byte)MENU_1_1_MAX, 1);
Menu menu_dd51     (menu_1_2_msg, &menu_diesel,   (byte)MENU_1_2_MAX, 2);
Menu menu_df200    (menu_1_3_msg, &menu_diesel,   (byte)MENU_1_3_MAX, 3);
/* Create instances for STEAM first level menu */
Menu menu_d51      (menu_2_1_msg, &menu_steam,    (byte)MENU_2_1_MAX, 4);
/* Create instances for ELECTRIC first level menu */
Menu menu_e235     (menu_3_1_msg, &menu_electric, (byte)MENU_3_1_MAX, 5);
Menu menu_ef65     (menu_3_2_msg, &menu_electric, (byte)MENU_3_2_MAX, 6);
Menu menu_ef210    (menu_3_3_msg, &menu_electric, (byte)MENU_3_3_MAX, 7);
/* Create instances for STATION first level menu */

/***************************************************************/

void setup()
{
  volume      = 20;
  block_flag  = 0;  /* It is used to avoid multiple key pressing while keep pressing a button */

  lcd.init();
  lcd.backlight();
  dfplayer_serial.begin(9600);

  if (!dfplayer.begin(dfplayer_serial))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Unable to begin:"));
    lcd.setCursor(1, 1);
    lcd.print(F("1. Recheck conn"));
    lcd.setCursor(1, 2);
    lcd.print(F("2. Insert SD card"));
    lcd.setCursor(1, 3);
    lcd.print(F("3. Please, reset"));
    while(true)
    {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  dfplayer.volume(volume);

  /* Add all the elements to the BASE menu      */
  menu_base.AddSon(&menu_diesel,     0);
  menu_base.AddSon(&menu_steam,      1);
  menu_base.AddSon(&menu_electric,   2);
  menu_base.AddSon(&menu_station,    3);
  /* Add all the elements for the DIESEL menu   */
  menu_diesel.AddSon(&menu_de10,     0);
  menu_diesel.AddSon(&menu_dd51,     1);
  menu_diesel.AddSon(&menu_df200,    2);
  /* Add all the elements for the STEAM menu    */
  menu_steam.AddSon(&menu_d51,       0);
  /* Add all the elements for the ELECTRIC menu */
  menu_electric.AddSon(&menu_e235,   0);
  menu_electric.AddSon(&menu_ef65,   0);
  menu_electric.AddSon(&menu_ef210,  0);
  /* Add all the elements for the STATION menu  */

  /**********************************************/
  menu_selected = &menu_base;
  /* Load all the options for the base menu on the screen */
  menu_selected->SetFirstShown(0);
  menu_selected->SetLastShown((menu_selected->GetMaxPos() >= SCREEN_MAX_LINES ? (SCREEN_MAX_LINES - 1) : menu_selected->GetMaxPos()));
  load_menu(menu_selected->GetTextMessages(), menu_selected->GetFirstShown(), menu_selected->GetMaxPos());
  option_max  = menu_selected->GetMaxPos();

  option_menu = 0;
  place_asterisk(0);
}

void loop()
{
  sensorValue = analogRead(sensorPin);          // Reads the button value.
  if ((sensorValue > 20) && (block_flag == 0))  // Remove false readings. Sensor is reading all the time values between 0 and 20
  {
    block_flag = 1;                             // Block the flag to avoid multiple pressing from one press
    if ((sensorValue > RESISTOR_DOWN_LINE_MIN) && (sensorValue < RESISTOR_DOWN_LINE_MAX))
    {
      /*************************************************/
      /*                MOVE LINE DOWN                 */
      /*************************************************/
      if (option_menu < (menu_selected->GetMaxPos() < (SCREEN_MAX_LINES - 1) ? menu_selected->GetMaxPos() : (SCREEN_MAX_LINES - 1)))
      {
        move_asterisk(option_menu, 1);
        option_menu += 1;
      } else if (menu_selected->GetLastShown() < menu_selected->GetMaxPos())
      {
          menu_selected->SetFirstShown(menu_selected->GetFirstShown() + 1);
          menu_selected->SetLastShown(menu_selected->GetLastShown()   + 1);
          load_menu(menu_selected->GetTextMessages(), menu_selected->GetFirstShown(), menu_selected->GetLastShown());
          place_asterisk(3);
        }
    } else if ((sensorValue > RESISTOR_UP_LINE_MIN) && (sensorValue < RESISTOR_UP_LINE_MAX))
    {
     /*************************************************/
     /*                MOVE LINE UP                   */
     /*************************************************/
     if (option_menu > 0) // If already on the first line, dont go up.
     {
       move_asterisk(option_menu, -1);
       option_menu -= 1;
     } else if (menu_selected->GetFirstShown() > 0)
     {
      menu_selected->SetFirstShown(menu_selected->GetFirstShown() - 1);
      menu_selected->SetLastShown(menu_selected->GetLastShown()   - 1);
      load_menu(menu_selected->GetTextMessages(), menu_selected->GetFirstShown(), menu_selected->GetLastShown());
      place_asterisk(0);
     }
    } else if ((sensorValue > RESISTOR_INSIDE_MIN) && (sensorValue < RESISTOR_INSIDE_MAX))
    {
      /*************************************************/
      /*              MOVE LEVEL INSIDE                */
      /*************************************************/
      if (menu_selected->GetSon(option_menu) != NULL) // Go down a level
      {
        menu_selected = menu_selected->GetSon(option_menu);
        select_option(option_menu);
        option_menu = 0;
        load_menu(menu_selected->GetTextMessages(), 0, (menu_selected->GetMaxPos() > SCREEN_MAX_LINES ? SCREEN_MAX_LINES : menu_selected->GetMaxPos()));
        menu_selected->SetFirstShown(0);
        menu_selected->SetLastShown((menu_selected->GetMaxPos() >= SCREEN_MAX_LINES ? (SCREEN_MAX_LINES - 1) : menu_selected->GetMaxPos()));
        place_asterisk(0);
    } else // It has to play a sound
    {
      dfplayer.stop();
      delay(100);
      dfplayer.playFolder(menu_selected->GetFolder(), (menu_selected->GetFirstShown() + option_menu + 1));
    }
  } else if ((sensorValue > RESISTOR_OUTSIDE_MIN) && (sensorValue < RESISTOR_OUTSIDE_MAX))
    {
      /*************************************************/
      /*                MOVE LEVEL OUTSIDE             */
      /*************************************************/
      if (menu_selected->GetFather() != NULL) // Go up a level
      {
        option_menu = 0;
        menu_selected = menu_selected->GetFather();
        load_menu(menu_selected->GetTextMessages(), 0, (menu_selected->GetMaxPos() > SCREEN_MAX_LINES ? (SCREEN_MAX_LINES - 1) : menu_selected->GetMaxPos()));
        menu_selected->SetFirstShown(0);
        menu_selected->SetLastShown((menu_selected->GetMaxPos() >= (SCREEN_MAX_LINES - 1) ? (SCREEN_MAX_LINES - 1) : menu_selected->GetMaxPos()));
        place_asterisk(0);
      }
    } else if ((sensorValue > RESISTOR_UP_VOL_MIN) && (sensorValue < RESISTOR_UP_VOL_MAX))
    {
      /*************************************************/
      /*                INCREASE VOLUME                */
      /*************************************************/
      if (volume < 25)
      {
        volume += 1;
        dfplayer.volume(volume);
      }
    } else if ((sensorValue > RESISTOR_DOWN_VOL_MIN) && (sensorValue < RESISTOR_DOWN_VOL_MAX))
    {
      /*************************************************/
      /*                DECREASE VOLUME                */
      /*************************************************/
      if (volume > 0)
      {
        volume -= 1;
        dfplayer.volume(volume);
      }     
    } 
  } else if (sensorValue <= 5) // Unlock block flag ir button unpressed
  {
    block_flag = 0;
  }
}
