/*************************
Cr�� par Charles Murphy
Syst�me g�rant les ActionComponent
**************************/
#include "ActionComponent.h"


class ActionSystem{

public :

	static void update(Message &postman, ActionComponent& ac);

protected:
	ActionSystem();
	~ActionSystem();
};