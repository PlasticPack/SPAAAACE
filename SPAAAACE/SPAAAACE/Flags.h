#ifndef FLAGS_H
#define FLAGS_H

enum message
{
	MS_COLLISION = 0x8500,
	MS_X_CHANGED,
	MS_Y_CHANGED,
	MS_LIFE_DOWN,
	MS_ENGINE_ACTIVE,
	MS_SHOOT,
	MS_DEAD,
	MS_SOUND_PLAYED,
	MS_PLAY,
	MS_MENU,
	MS_EXIT_REQUEST,
	MS_SELECT,
	MS_DIALOGUE_NEXT,
	MS_TRIGGER,
	MS_SWITCHED,
	MS_OBJECTIVE,
	MS_MISSION_OVER,
	MS_OBJ_DONE
};

/*
LISTE DES MS EN INT
MS_COLLISION 				34048
MS_X_CHANGED,				34049	
MS_Y_CHANGED,				34050	
MS_LIFE_DOWN,				34051
MS_ENGINE_ACTIVE,			34052
MS_SHOOT,					34053
MS_DEAD,					34054
MS_SOUND_PLAYED,			34055
MS_PLAY,					34056
MS_MENU,					34057
MS_EXIT_REQUEST				34058
MS_SELECT					34059
MS_DIALOGUE_NEXT			34060
MS_TRIGGER					34061
MS_SWITCHED					34062
MS_OBJECTIVE				34063
MS_MISSION_OVER				34064
*/

const int debutGamePadButtonList = 0x9900; //<-- !!!!NE PAS CHANGER!!!!!
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
	AC_SHOOT,
	AC_EXIT,
	AC_LEFT,
	AC_RIGHT,
	AC_NEXT,
	AC_NUMBER_OF_ACTION // Mettre tout autre action AVANT celle-ci
};

#endif /*FLAGS_H*/
