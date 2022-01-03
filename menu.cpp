/*
Menu.cpp - Class file for the Menu I2C display Arduino Library.
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

#include "Arduino.h"
#include "menu.h"

/** Base menu constructor.
 * @param Menu messages displayed on the base level
 * @param Max position in the array (Elements - 1)
 */
Menu::Menu(const char **menu, byte max_pos)
{
    _messages      = menu;
    _max_pos       = max_pos;
    _current_pos   = 0;
    memset(_sons, 0x00, sizeof(_sons));
}

/** Menu constructor for all the elements excep the base in the menu.
 * @param Menu messages displayed on the base level
 * @param Father menu. Previous level.
 * @param Max position in the array (Elements - 1)
 */
Menu::Menu(const char **menu, Menu *father, byte max_pos)
{
    _messages      = menu;
    _max_pos       = max_pos;
    _current_pos   = 0;
    _father        = father;
    _first_shown   = 0;
    _last_shown    = 0;
    memset(_sons, 0x00, sizeof(_sons));
}

/** Menu constructor for the last element of a menu.
 * @param Menu messages displayed on the base level
 * @param Father menu. Previous level.
 * @param Max position in the array (Elements - 1)
 * @param Folder number 
 */
Menu::Menu(const char **menu, Menu *father, byte max_pos, byte folder)
{
    _messages      = menu;
    _max_pos       = max_pos;
    _current_pos   = 0;
    _father        = father;
    _first_shown   = 0;
    _last_shown    = 0;
    _folder        = folder;
    
    memset(_sons, 0x00, sizeof(_sons));
}

/** Get the current position where the asterisk is placed
 * @return Current Position
 */
byte Menu::GetCurrentPos()
{
    return _current_pos;
}

/** Get the max position available for this level
 * @return MAx Position
 */
byte Menu::GetMaxPos()
{
    return _max_pos;
}

/** Set the current position where the asterisk is placed
 * @param Current Position
 */
void Menu::SetCurrentPos(byte current_pos)
{
    _current_pos = current_pos;
}

/** Get the text message for the selected position
 * @return Text message
 */
const char* Menu::GetTextMessage(byte position)
{
    return _messages[position];
}

/** Get all the messsages for the current level
 * @return Text messages
 */
const char** Menu::GetTextMessages()
{
    return _messages;
}

/** Get the father menu level for the current menu shown
 * @return Father menu
 */
Menu* Menu::GetFather()
{
    return _father;
}

/** Add a new menu son for the current element
 * @param Son
 * @param Position
 */
void Menu::AddSon(Menu *son, int position)
{
  _sons[position] = son;
}

/** Get the selected son for the current menu level
 * @param Son position
 * @return Selected son
 */
Menu* Menu::GetSon(byte position)
{
    return _sons[position];
}

/** Clean the list of sons
 */
void Menu::CleanSons()
{
  memset(_sons, 0x00, sizeof(_sons));
}

/** Get the number of the first text message shown on the screen
 * @return First position shown
 */
byte Menu::GetFirstShown()
{
  return _first_shown;
}

/** Get the number of the last text message shown on the screen
 * @return Last position shown
 */
byte Menu::GetLastShown()
{
  return _last_shown;
}

/** Set the number of the first text message shown on the screen
 * @param First position shown
 */
void Menu::SetFirstShown(byte first)
{
  _first_shown = first;
}

/** Set the number of the last text message shown on the screen
 * @param Last position shown
 */
void Menu::SetLastShown(byte last)
{
  _last_shown = last;
}

/** Get the folder number for the selected option. This is useful in case of the  using the SDCard.
 * @return Folder number
 */
byte Menu::GetFolder()
{
  return _folder;
}
