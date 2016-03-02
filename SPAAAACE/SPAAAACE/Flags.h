#pragma once
const int debutGamePadButtonList = 0x9900; // !!!!NE PAS CHANGER!!!!!
enum GamePadButton
{
	GP_DPAD_UP = debutGamePadButtonList,
	GP_DPAD_DOWN,
	GP_DPAD_LEFT,
	GP_DPAD_RIGHT,
	GP_SELECT,
	GP_START,
	GP_LEFT_STICK_PUSH,
	GP_RIGHT_STICK_PUSH,
	GP_LB,
	GP_RB,
	GP_BUTTON_A,
	GP_BUTTON_B,
	GP_BUTTON_X,
	GP_BUTTON_Y,

	GP_NUMBER_OF_GAMEPAD_BUTTON	
};

const int debutGamePadAxisList = GP_NUMBER_OF_GAMEPAD_BUTTON + 1;
enum GamePadAxis
{
	GP_AXIS_LEFT_JOY_X = debutGamePadAxisList,
	GP_AXIS_LEFT_JOY_Y,
	GP_AXIS_RIGHT_JOY_X,
	GP_AXIS_RIGHT_JOY_Y,
	GP_AXIS_LT,
	GP_AXIS_RT,

	GP_AXIS_NUMBER_OF_GAMEPAD_AXIS
};

const int debutActionList = 0x9000; // !!!!NE PAS CHANGER!!!!!
enum action // METTRE NOUVELLEs ACTIONS ICI
{
	AC_START = debutActionList,
	AC_SELECT,
	AC_PRINCIPAL_KEY,
	AC_SECONDARY_KEY,
	AC_UP,
	AC_DOWN,
	AC_HORIZONTAL_PUSH,
	AC_VERTICAL_PUSH,
	AC_EXIT,

	AC_NUMBER_OF_ACTION // Mettre tout autre action AVANT celle-ci
};
