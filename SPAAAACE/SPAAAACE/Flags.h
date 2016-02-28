#pragma once

enum GamePadButton
{
	GP_DPAD_UP,
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
	GP_BUTTON_Y
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
	AC_LEFT,
	AC_RIGHT,

	NUMBER_OF_ACTION // Mettre tout autre action avant celle-ci
};
