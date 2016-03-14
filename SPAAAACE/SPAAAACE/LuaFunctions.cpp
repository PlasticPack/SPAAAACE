#include "LuaInit.hpp"
#include <LuaBridge.h>
#include <iostream>
#include <memory>

bool luain::loadScript(lua_State* L, const std::string& filename);


void luain::lua_getToStack(lua_State* L, const std::string& variableName){
	int level = 0;
	std::string var = "";
	for (unsigned int i = 0; i < variableName.size(); i++) {
		if (variableName.at(i) == '.') {
			if (level == 0) {
				lua_getglobal(L, var.c_str());
			}
			else {
				lua_getfield(L, -1, var.c_str());
			}

			if (lua_isnil(L, -1)) {
				std::cout << "Error, can't get " << variableName << std::endl;
				return;
			}
			else {
				var = "";
				level++;
			}
		}
		else {
			var += variableName.at(i);
		}
	}
	
	if (level == 0) {
		lua_getglobal(L, var.c_str());
	}
	else {
		lua_getfield(L, -1, var.c_str());
	}
	
	if (level == 0) { return; } // no need to remove anything

	int tableIndex = lua_gettop(L) - level;
	lua_replace(L, tableIndex);
	lua_settop(L, tableIndex);
}

void luain::loadGetKeysFunction(lua_State* L){
	std::string code =
		R"(function getKeys(t)
        s = {}
        for k, v in pairs(t) do
            table.insert(s, k)
            end 
        return s 
        end)";
	
	luaL_dostring(L, code.c_str());
}

std::vector<std::string> luain::getTableKeys(lua_State* L, const std::string& name){
	
	lua_getglobal(L, "getKeys"); // get function
	if (lua_isnil(L, -1)) {
		std::cout << "Get keys function is not loaded. Loading..." << std::endl;
		loadGetKeysFunction(L);
		lua_getglobal(L, "getKeys");
	}
	
	lua_getToStack(L, name.c_str());
	lua_pcall(L, 1, 1, 0);//One paramer & one return

	lua_pushnil(L);
	std::vector<std::string> keys;
	while (lua_next(L, -2)) { // get values one by one
		//std::cout << "*";
		if (lua_type(L, -1) == LUA_TSTRING) { // check if key is a string
			keys.push_back(lua_tostring(L, -1));
		}
		lua_pop(L, 1);
	}
	
	lua_settop(L, 0); // remove the getkey function from the lua stack*/
	return keys;
}

template <typename T>
void addComponent(std::shared_ptr<GameObject> e, luabridge::LuaRef& componentTable) {
	//auto n = T(componentTable);
	e->addComponent(std::type_index(typeid(T)), std::make_shared<T>(componentTable));
}

std::shared_ptr<GameObject> luain::loadGameObjects(lua_State* L, const std::string& type){
	using namespace luabridge;
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	obj->setID(type);
	
	auto v = luain::getTableKeys(L, type);
	LuaRef entityTable = getGlobal(L, type.c_str());
	for (auto& componentName : v) {
		
		if (componentName == "Position") {
			LuaRef poscTable = entityTable[componentName];
			addComponent<PositionComponent>(obj, poscTable);
		}
		else if (componentName == "Physics"){
			LuaRef phycTable = entityTable["Physics"];
			addComponent<PhysicsComponent>(obj, phycTable);

		}
		else if (componentName == "Graphics"){
			LuaRef graTable = entityTable[componentName];
			addComponent<GraphicsComponent>(obj, graTable);
		}
		else if (componentName == "Input"){
			LuaRef inTable = entityTable[componentName];
			addComponent<GraphicsComponent>(obj, inTable);
		}
		else std::cout << "Unknown component: " << componentName;
		std::cout << "Added " << componentName << " to " << type << std::endl;
	}
	if (obj->hasComponent(typeid(PositionComponent))){
		if (obj->hasComponent(typeid(GraphicsComponent))){
			obj->get<GraphicsComponent>()->setPositionComponent(obj->get<PositionComponent>());
		}
		if (obj->hasComponent(typeid(PhysicsComponent))){
			obj->get<PhysicsComponent>()->setPositionComp(obj->get<PositionComponent>());
		}
	}
	/*if (obj->hasComponent(typeid(PhysicsComponent))){
		if (obj->hasComponent(typeid(InputsComponent))){
			obj->get<InputsComponent>()->setPhysComp(obj->get<PhysicsComponent>());
		}
	}*/
	
	
	return obj;
}



