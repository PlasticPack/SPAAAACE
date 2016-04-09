#include "LuaInit.hpp"
#include <LuaBridge.h>
#include <iostream>
#include <memory>
#include"Scene.h"


//bool luain::loadScript(lua_State* L, const std::string& filename);


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
	//Violation of access happeniing right here...
	//like, just below this line, means the file wa not oppened !
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
/*
template <typename T>
void addComponent(Scene *s, std::shared_ptr<GameObject> e, luabridge::LuaRef& componentTable) {
	//auto n = T(componentTable);

	e->addComponent(std::type_index(typeid(T)), std::make_shared<T>(componentTable));
	s->addComponent<T>(e->get<T>());
}

std::shared_ptr<GameObject> luain::loadGameObjects(Scene *s, lua_State* L, const std::string& type){
	using namespace luabridge;
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	obj->setID(type);
	
	auto v = luain::getTableKeys(L, type);
	LuaRef entityTable = getGlobal(L, type.c_str());
	for (auto& componentName : v) {
		
		if (componentName == "Position") {
			LuaRef poscTable = entityTable[componentName];
			addComponent<PositionComponent>(s, obj, poscTable);
		}
		else if (componentName == "Physics"){
			LuaRef phycTable = entityTable[componentName];
			addComponent<PhysicsComponent>(s, obj, phycTable);
		}
		else if (componentName == "Graphics"){
			LuaRef graTable = entityTable[componentName];

			addComponent<GraphicsComponent>(s, obj, graTable);
		}
		else std::cout << "Unknown component: " << componentName;

		std::cout << "Added " << componentName  << " to " << type << std::endl;
	}
	if (obj->hasComponent(typeid(PositionComponent))){
		if (obj->hasComponent(typeid(GraphicsComponent))){
			obj->get<GraphicsComponent>()->setPositionComponent(obj->get<PositionComponent>());
		}
		if (obj->hasComponent(typeid(PhysicsComponent))){
			obj->get<PhysicsComponent>()->setPositionComp(obj->get<PositionComponent>());
		}
	}
	if (obj->hasComponent(typeid(PhysicsComponent))){
		
	}
	return obj;
}*/



std::vector<std::string> getFiles(const std::string& filepath, const std::string& ext){
	std::vector<std::string> files;
	//Check if 
	if (!filepath.empty()){

		boost::filesystem::path path(filepath);
		boost::filesystem::recursive_directory_iterator end;
		for (boost::filesystem::recursive_directory_iterator i(path); i != end; i++)
		{
			//wew, not required at all
			/*if (boost::filesystem::is_directory(*i)){
			std::vector<std::string> w = getFiles(boost::filesystem::path(*i).string(), ext);
			for (int i = 0; i < w.size(); i++)
			{
			files.push_back(w.at(i));
			}
			}*/
			if (boost::filesystem::is_regular_file(*i) && i->path().extension() == ext){
				files.push_back(boost::filesystem::path(*i).string());
			}
		}
	}
	return files;
}

void luain::loadFromRep(Scene *scene,std::vector<std::shared_ptr<GameObject>> &objects, const std::string& filepath, const std::string& ext){
	std::vector<std::string> files_dir = getFiles(filepath,ext);
	if (!files_dir.empty()){
		for (int i = 0; i < files_dir.size(); i++){
			std::cout << "Loading file :"<<files_dir[i]<<std::endl;
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);
			luaL_dofile(L, std::string(files_dir[i]).c_str());
			loadGetKeysFunction(L);
			boost::filesystem::path p(files_dir[i]);
			std::shared_ptr<GameObject> obj = loadGameObjects(scene,L, p.stem().string());
			//boost::filesystem::path p(files_dir[i]);
			obj->setID(p.stem().string());
			//std::cout << obj->getID() << std::endl;
			objects.push_back(obj);
		}
	}
	
}

void luain::loadFromRep(Scene* sc,std::map<std::string, std::shared_ptr<GameObject>> &objs, const std::string& filepath, const std::string& ext){
	std::vector<std::string> files_dir = getFiles(filepath, ext);
	if (!files_dir.empty()){
		for (int i = 0; i < files_dir.size(); i++){
			std::cout << "Loading file :" << files_dir[i] << std::endl;
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);
			luaL_dofile(L, std::string(files_dir[i]).c_str());
			loadGetKeysFunction(L);
			boost::filesystem::path p(files_dir[i]);
			std::shared_ptr<GameObject> obj = loadGameObjects(sc,L, p.stem().string());
			//boost::filesystem::path p(files_dir[i]);
			if (objs.find(p.stem().string()) == objs.end()){
				obj->setID(p.stem().string());
				//std::cout << obj->getID() << std::endl;
				objs.insert(std::make_pair(obj->getID(), obj));
			}
			else std::cout << "Collision at:" << p.stem().string() << std::endl;
			
		}
	}
}

template <typename T>
void addComponent(Scene *s, std::shared_ptr<GameObject> e, luabridge::LuaRef& componentTable) {
	//auto n = T(componentTable);

	e->addComponent(std::type_index(typeid(T)), std::make_shared<T>(componentTable));
	s->addComponent<T>(e->get<T>());
}

std::shared_ptr<GameObject> luain::loadGameObjects(Scene *s, lua_State* L, const std::string& type){
	using namespace luabridge;
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	obj->setID(type);

	auto v = luain::getTableKeys(L, type);
	LuaRef entityTable = getGlobal(L, type.c_str());
	for (auto& componentName : v) {
		if (componentName == "Position") {
			LuaRef poscTable = entityTable[componentName];
			addComponent<PositionComponent>(s, obj, poscTable);
		}
		else if (componentName == "Physics"){
			LuaRef phycTable = entityTable[componentName];
			addComponent<PhysicsComponent>(s, obj, phycTable);
		}
		else if (componentName == "Graphics"){
			LuaRef graTable = entityTable[componentName];
			addComponent<GraphicsComponent>(s, obj, graTable);
		}
		else if (componentName == "GameLogic"){
			LuaRef GLTable = entityTable[componentName];
			addComponent<GameLogicComponent>(s, obj, GLTable);
		}
		else if (componentName == "Action"){
			LuaRef ACTable = entityTable[componentName];
			addComponent<ActionComponent>(s, obj, ACTable);
		}
		else std::cout << "Unknown component: " << componentName;

		std::cout<< "Added " << componentName << " to " << type << std::endl;
	}

	if (obj->hasComponent(typeid(PositionComponent))){
		if (obj->hasComponent(typeid(GraphicsComponent))){
			obj->get<GraphicsComponent>()->setPositionComponent(obj->get<PositionComponent>());
		}
		if (obj->hasComponent(typeid(PhysicsComponent))){
			obj->get<PhysicsComponent>()->setPositionComp(obj->get<PositionComponent>());
		}
	}

	std::cout << "\n\n\n";
	return obj;
}