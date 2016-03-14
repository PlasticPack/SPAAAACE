/*#include "LuaInit.hpp"
#undef main
int main(int argc, char** argv){
	
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	//luain::loadScript(L, "nem1.lua");
	luaL_dofile(L, "nem1.lua");
	luain::loadGetKeysFunction(L);

	std::shared_ptr<GameObject> obj = luain::loadGameObjects(L, "nem1");
	//auto wut = obj->get<PhysicsComponent>();
	//double mass = wut->getMass();
	std::cout <<obj->getID()<<" has a mass of: "<<obj->get<PhysicsComponent>()->getMass() << std::endl;
	system("PAUSE");

	return 1;
}*/