/*
Menu.h - Class file for the Menu I2C display Arduino Library.
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

#ifndef _MENU_H_
#define _MENU_H_

class Menu {
    private:
        byte        _current_pos;
        byte        _max_pos;
        const char  **_messages;
        byte        _first_shown;
        byte        _last_shown;
        Menu        *_father;
        Menu        *_sons[10];

        byte        _folder;
        
    public:
        Menu                         (const char **menu, byte max_pos);
        Menu                         (const char **menu, Menu *father, byte max_pos);
        Menu                         (const char **menu, Menu *father, byte max_pos, byte folder);
        byte GetCurrentPos           ();
        byte GetMaxPos               ();

        void SetCurrentPos           (byte current_pos);
        const char* GetTextMessage   (byte position);
        const char** GetTextMessages ();

        Menu* GetFather              ();
        void AddSon                  (Menu *son, int position);
        Menu* GetSon                 (byte position);
        void CleanSons               ();

        byte GetFirstShown          ();
        byte GetLastShown           ();
        void SetFirstShown          (byte first);
        void SetLastShown           (byte last);
        
        byte GetFolder              ();
};

#endif
