/* Every menu must have a define associated with the number of elements (Start counting at 0, so ELEMENTS - 1) 
*/
/* General menu */
#define MENU_BASE_MAX     3 // This is the last element position from 0 to (X - 1) elements
const char *menu_base_msg[] =
{
  "Diesel engine",
  "Steam engine",
  "Electric engine",
  "Station messages",
};

/* First option of the base menu */
#define MENU_1_MAX     2
const char *menu_1_msg[] =
{
  "DE10",
  "DD51",
  "DF200",
};

/* Second option of the base menu */
#define MENU_2_MAX     0
const char *menu_2_msg[] =
{
  "D51",
};

/* Third option of the base menu */
#define MENU_3_MAX     2
const char *menu_3_msg[] =
{
  "E-235 Yamanote",
  "EF65-500",
  "EF210",
};

/* Fourth option of the base menu */
#define MENU_4_MAX     5
const char *menu_4_msg[] =
{
  "Announcement 01",
  "Announcement 02",
  "Announcement 03",
  "Announcement 04",
  "Shinkansen 01",
  "Sound 01",
};

/* Sound elements of the first element of the first option */
#define MENU_1_1_MAX     2
const char *menu_1_1_msg[] =
{
  "Passing by 01",
  "Passing by 02",
  "Passing by 03",
};

/* Sound elements of the second element of the first option */
#define MENU_1_2_MAX     5
const char *menu_1_2_msg[] =
{
  "Whistle 01",
  "Passing by 01",
  "Passing by 02",
  "Passing by 03",
  "Passing by 04",
  "Passing by 05",
};

/* Sound elements of the third element of the first option */
#define MENU_1_3_MAX     1
const char *menu_1_3_msg[] =
{
  "Whistle 01",
  "Passing by 01",
};

/* Sound elements of the first element of the second option */
#define MENU_2_1_MAX     7
const char *menu_2_1_msg[] =
{
  "Whistle 01",
  "Whistle 02",
  "Whistle 03",
  "Whistle 04",
  "Whistle 05",
  "Passing by 01",
  "Passing by 02",
  "Pass/Whist 01",
};

/* Sound elements of the first element of the third option */
#define MENU_3_1_MAX     0
const char *menu_3_1_msg[] =
{
  
};

/* Sound elements of the second element of the third option */
#define MENU_3_2_MAX     2
const char *menu_3_2_msg[] =
{
  "Passing by 01",
  "Passing by 02",
  "Passing by 03",
};

/* Sound elements of the thirdt element of the third option */
#define MENU_3_3_MAX     0
const char *menu_3_3_msg[] =
{
};
