#pragma once
#include<vector>
#include <iostream>
#include <memory>
//#include<LuaBridge.h>
//Linking it will cause multiple issues

#include<iostream>
#include"GraphicsComponent.h"
#include"PhysicsComponent.h"
#include"PositionComponent.h"
#include"GameObject.h"
#include"InputsComponent.h"
#include <boost\filesystem.hpp>
#include "InputsComponent.h"
#include "Scene.h"

extern"C"{
# include"lua.h"
# include"lauxlib.h"
# include"lualib.h"
}

struct lua_State;
struct Scene;

class Scene;

namespace luain{
	//bool loadScript(lua_State* L, const std::string& filename);
	void lua_getToStack(lua_State* L, const std::string& variableName);
	void loadGetKeysFunction(lua_State* L);
	std::vector<std::string> getTableKeys(lua_State* L, const std::string& name);
	//std::shared_ptr<GameObject> loadGameObjects(lua_State* L, const std::string& type);
	void loadFromRep(Scene*,std::vector<std::shared_ptr<GameObject>> &objects, const std::string& filepath, const std::string& ext=".lua");
	void loadFromRep(Scene*,std::map<std::string, std::shared_ptr<GameObject>> &objs, const std::string& filepath, const std::string& ext=".lua");
	std::shared_ptr<GameObject> loadGameObjects(Scene* s, lua_State* L, const std::string& type);
}
