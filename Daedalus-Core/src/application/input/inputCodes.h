#pragma once

// Input codes from glfw3.h

enum class InputCode
{
	/* Keyboard Keys */
	Key_Unkown          = -1,
	Key_Space           = 32,
	Key_Apostrophe      = 39,  /* ' */
	Key_Comma           = 44,  /* , */
	Key_Minus           = 45,  /* - */
	Key_Period          = 46,  /* . */
	Key_Slash           = 47,  /* / */
	Key_0               = 48,
	Key_1               = 49,
	Key_2               = 50,
	Key_3               = 51,
	Key_4               = 52,
	Key_5               = 53,
	Key_6               = 54,
	Key_7               = 55,
	Key_8               = 56,
	Key_9               = 57,
	Key_Semicolon       = 59,  /* ; */
	Key_Equal           = 61,  /* = */
	Key_A               = 65,
	Key_B               = 66,
	Key_C               = 67,
	Key_D               = 68,
	Key_E               = 69,
	Key_F               = 70,
	Key_G               = 71,
	Key_H               = 72,
	Key_I               = 73,
	Key_J               = 74,
	Key_K               = 75,
	Key_L               = 76,
	Key_M               = 77,
	Key_N               = 78,
	Key_O               = 79,
	Key_P               = 80,
	Key_Q               = 81,
	Key_R               = 82,
	Key_S               = 83,
	Key_T               = 84,
	Key_U               = 85,
	Key_V               = 86,
	Key_W               = 87,
	Key_X               = 88,
	Key_Y               = 89,
	Key_Z               = 90,
	Key_Left_Bracket    = 91,  /* [ */
	Key_Backslash       = 92,  /* \ */
	Key_Right_Bracket   = 93,  /* ] */
	Key_Grave_Accent    = 96,  /* ` */
	Key_World_1         = 161, /* non-US #1 */
	Key_World_2         = 162, /* non-US #2 */

	/* Function keys */
	Key_Escape          = 256,
	Key_Enter           = 257,
	Key_Tab             = 258,
	Key_Backspace       = 259,
	Key_Insert          = 260,
	Key_Delete          = 261,
	Key_Right           = 262,
	Key_Left            = 263,
	Key_Down            = 264,
	Key_Up              = 265,
	Key_Page_Up         = 266,
	Key_Page_Down       = 267,
	Key_Home            = 268,
	Key_End             = 269,
	Key_Caps_Lock       = 280,
	Key_Scroll_Lock     = 281,
	Key_Num_Lock        = 282,
	Key_Print_Screen    = 283,
	Key_Pause           = 284,
	Key_F1              = 290,
	Key_F2              = 291,
	Key_F3              = 292,
	Key_F4              = 293,
	Key_F5              = 294,
	Key_F6              = 295,
	Key_F7              = 296,
	Key_F8              = 297,
	Key_F9              = 298,
	Key_F10             = 299,
	Key_F11             = 300,
	Key_F12             = 301,
	Key_F13             = 302,
	Key_F14             = 303,
	Key_F15             = 304,
	Key_F16             = 305,
	Key_F17             = 306,
	Key_F18             = 307,
	Key_F19             = 308,
	Key_F20             = 309,
	Key_F21             = 310,
	Key_F22             = 311,
	Key_F23             = 312,
	Key_F24             = 313,
	Key_F25             = 314,
	Key_KP_0            = 320,
	Key_KP_1            = 321,
	Key_KP_2            = 322,
	Key_KP_3            = 323,
	Key_KP_4            = 324,
	Key_KP_5            = 325,
	Key_KP_6            = 326,
	Key_KP_7            = 327,
	Key_KP_8            = 328,
	Key_KP_9            = 329,
	Key_KP_Decimal      = 330,
	Key_KP_Divide       = 331,
	Key_KP_Multiply     = 332,
	Key_KP_Subtract     = 333,
	Key_KP_Add          = 334,
	Key_KP_Enter        = 335,
	Key_KP_Equal        = 336,
	Key_Left_Shift      = 340,
	Key_Left_Control    = 341,
	Key_Left_Alt        = 342,
	Key_Left_Super      = 343,
	Key_RIGHT_Shift     = 344,
	Key_RIGHT_Control   = 345,
	Key_RIGHT_Alt       = 346,
	Key_RIGHT_Super     = 347,
	Key_Menu            = 348,

	/* Mouse buttons */
	Mouse_Button_1      = 0,
	Mouse_Button_2      = 1,
	Mouse_Button_3      = 2,
	Mouse_Button_4      = 3,
	Mouse_Button_5      = 4,
	Mouse_Button_6      = 5,
	Mouse_Button_7      = 6,
	Mouse_Button_8      = 7,
	Mouse_Button_Left   = Mouse_Button_1,
	Mouse_Button_Right  = Mouse_Button_2,
	Mouse_Button_Middle = Mouse_Button_3
};


//#define DD_INPUT_KEY_UNKNOWN            -1
//
//#define DD_INPUT_KEY_SPACE              32
//#define DD_INPUT_KEY_APOSTROPHE         39  /* ' */
//#define DD_INPUT_KEY_COMMA              44  /* , */
//#define DD_INPUT_KEY_MINUS              45  /* - */
//#define DD_INPUT_KEY_PERIOD             46  /* . */
//#define DD_INPUT_KEY_SLASH              47  /* / */
//#define DD_INPUT_KEY_0                  48
//#define DD_INPUT_KEY_1                  49
//#define DD_INPUT_KEY_2                  50
//#define DD_INPUT_KEY_3                  51
//#define DD_INPUT_KEY_4                  52
//#define DD_INPUT_KEY_5                  53
//#define DD_INPUT_KEY_6                  54
//#define DD_INPUT_KEY_7                  55
//#define DD_INPUT_KEY_8                  56
//#define DD_INPUT_KEY_9                  57
//#define DD_INPUT_KEY_SEMICOLON          59  /* ; */
//#define DD_INPUT_KEY_EQUAL              61  /* = */
//#define DD_INPUT_KEY_A                  65
//#define DD_INPUT_KEY_B                  66
//#define DD_INPUT_KEY_C                  67
//#define DD_INPUT_KEY_D                  68
//#define DD_INPUT_KEY_E                  69
//#define DD_INPUT_KEY_F                  70
//#define DD_INPUT_KEY_G                  71
//#define DD_INPUT_KEY_H                  72
//#define DD_INPUT_KEY_I                  73
//#define DD_INPUT_KEY_J                  74
//#define DD_INPUT_KEY_K                  75
//#define DD_INPUT_KEY_L                  76
//#define DD_INPUT_KEY_M                  77
//#define DD_INPUT_KEY_N                  78
//#define DD_INPUT_KEY_O                  79
//#define DD_INPUT_KEY_P                  80
//#define DD_INPUT_KEY_Q                  81
//#define DD_INPUT_KEY_R                  82
//#define DD_INPUT_KEY_S                  83
//#define DD_INPUT_KEY_T                  84
//#define DD_INPUT_KEY_U                  85
//#define DD_INPUT_KEY_V                  86
//#define DD_INPUT_KEY_W                  87
//#define DD_INPUT_KEY_X                  88
//#define DD_INPUT_KEY_Y                  89
//#define DD_INPUT_KEY_Z                  90
//#define DD_INPUT_KEY_LEFT_BRACKET       91  /* [ */
//#define DD_INPUT_KEY_BACKSLASH          92  /* \ */
//#define DD_INPUT_KEY_RIGHT_BRACKET      93  /* ] */
//#define DD_INPUT_KEY_GRAVE_ACCENT       96  /* ` */
//#define DD_INPUT_KEY_WORLD_1            161 /* non-US #1 */
//#define DD_INPUT_KEY_WORLD_2            162 /* non-US #2 */
//
///* Function keys */
//#define DD_INPUT_KEY_ESCAPE             256
//#define DD_INPUT_KEY_ENTER              257
//#define DD_INPUT_KEY_TAB                258
//#define DD_INPUT_KEY_BACKSPACE          259
//#define DD_INPUT_KEY_INSERT             260
//#define DD_INPUT_KEY_DELETE             261
//#define DD_INPUT_KEY_RIGHT              262
//#define DD_INPUT_KEY_LEFT               263
//#define DD_INPUT_KEY_DOWN               264
//#define DD_INPUT_KEY_UP                 265
//#define DD_INPUT_KEY_PAGE_UP            266
//#define DD_INPUT_KEY_PAGE_DOWN          267
//#define DD_INPUT_KEY_HOME               268
//#define DD_INPUT_KEY_END                269
//#define DD_INPUT_KEY_CAPS_LOCK          280
//#define DD_INPUT_KEY_SCROLL_LOCK        281
//#define DD_INPUT_KEY_NUM_LOCK           282
//#define DD_INPUT_KEY_PRINT_SCREEN       283
//#define DD_INPUT_KEY_PAUSE              284
//#define DD_INPUT_KEY_F1                 290
//#define DD_INPUT_KEY_F2                 291
//#define DD_INPUT_KEY_F3                 292
//#define DD_INPUT_KEY_F4                 293
//#define DD_INPUT_KEY_F5                 294
//#define DD_INPUT_KEY_F6                 295
//#define DD_INPUT_KEY_F7                 296
//#define DD_INPUT_KEY_F8                 297
//#define DD_INPUT_KEY_F9                 298
//#define DD_INPUT_KEY_F10                299
//#define DD_INPUT_KEY_F11                300
//#define DD_INPUT_KEY_F12                301
//#define DD_INPUT_KEY_F13                302
//#define DD_INPUT_KEY_F14                303
//#define DD_INPUT_KEY_F15                304
//#define DD_INPUT_KEY_F16                305
//#define DD_INPUT_KEY_F17                306
//#define DD_INPUT_KEY_F18                307
//#define DD_INPUT_KEY_F19                308
//#define DD_INPUT_KEY_F20                309
//#define DD_INPUT_KEY_F21                310
//#define DD_INPUT_KEY_F22                311
//#define DD_INPUT_KEY_F23                312
//#define DD_INPUT_KEY_F24                313
//#define DD_INPUT_KEY_F25                314
//#define DD_INPUT_KEY_KP_0               320
//#define DD_INPUT_KEY_KP_1               321
//#define DD_INPUT_KEY_KP_2               322
//#define DD_INPUT_KEY_KP_3               323
//#define DD_INPUT_KEY_KP_4               324
//#define DD_INPUT_KEY_KP_5               325
//#define DD_INPUT_KEY_KP_6               326
//#define DD_INPUT_KEY_KP_7               327
//#define DD_INPUT_KEY_KP_8               328
//#define DD_INPUT_KEY_KP_9               329
//#define DD_INPUT_KEY_KP_DECIMAL         330
//#define DD_INPUT_KEY_KP_DIVIDE          331
//#define DD_INPUT_KEY_KP_MULTIPLY        332
//#define DD_INPUT_KEY_KP_SUBTRACT        333
//#define DD_INPUT_KEY_KP_ADD             334
//#define DD_INPUT_KEY_KP_ENTER           335
//#define DD_INPUT_KEY_KP_EQUAL           336
//#define DD_INPUT_KEY_LEFT_SHIFT         340
//#define DD_INPUT_KEY_LEFT_CONTROL       341
//#define DD_INPUT_KEY_LEFT_ALT           342
//#define DD_INPUT_KEY_LEFT_SUPER         343 /* left windows / cmd */
//#define DD_INPUT_KEY_RIGHT_SHIFT        344
//#define DD_INPUT_KEY_RIGHT_CONTROL      345
//#define DD_INPUT_KEY_RIGHT_ALT          346
//#define DD_INPUT_KEY_RIGHT_SUPER        347 /* right windows / cmd */
//#define DD_INPUT_KEY_MENU               348
//
//#define DD_INPUT_KEY_LAST               DD_INPUT_KEY_MENU
//
////From glfw3.h
//#define DD_INPUT_MOUSE_BUTTON_1         0
//#define DD_INPUT_MOUSE_BUTTON_2         1
//#define DD_INPUT_MOUSE_BUTTON_3         2
//#define DD_INPUT_MOUSE_BUTTON_4         3
//#define DD_INPUT_MOUSE_BUTTON_5         4
//#define DD_INPUT_MOUSE_BUTTON_6         5
//#define DD_INPUT_MOUSE_BUTTON_7         6
//#define DD_INPUT_MOUSE_BUTTON_8         7
//#define DD_INPUT_MOUSE_BUTTON_LAST      DD_INPUT_MOUSE_BUTTON_8
//#define DD_INPUT_MOUSE_BUTTON_LEFT      DD_INPUT_MOUSE_BUTTON_1
//#define DD_INPUT_MOUSE_BUTTON_RIGHT     DD_INPUT_MOUSE_BUTTON_2
//#define DD_INPUT_MOUSE_BUTTON_MIDDLE    DD_INPUT_MOUSE_BUTTON_3