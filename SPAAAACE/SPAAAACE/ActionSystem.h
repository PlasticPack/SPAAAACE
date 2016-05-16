
/*************************
Créé par Charles Murphy
Système gérant les ActionComponent
**************************/
#include "ActionComponent.h"


class ActionSystem{

public :

	static void update(Message &postman, ActionComponent& ac);

protected:
	ActionSystem();
	~ActionSystem();
};
