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
<<<<<<< HEAD
#include"InputsComponent.h"
#include <boost\filesystem.hpp>
=======
#include "InputsComponent.h"
#include "Scene.h"

>>>>>>> origin
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
<<<<<<< HEAD
	std::shared_ptr<GameObject> loadGameObjects(lua_State* L, const std::string& type);
	void loadFromRep(std::vector<GameObject> &objects, const std::string& filepath, const std::string& ext=".lua");
	void loadFromRep(std::map<std::string, GameObject> &objs, const std::string& filepath, const std::string& ext=".lua");
	std::shared_ptr<GameObject> loadGameObjects(Scene* s, lua_State* L, const std::string& type);
}

std::vector<std::string> getFiles(const std::string& filepath, const std::string& ext);
=======
	std::shared_ptr<GameObject> loadGameObjects(Scene* s, lua_State* L, const std::string& type);
}
>>>>>>> origin
