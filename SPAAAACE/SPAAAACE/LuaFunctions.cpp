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
	
	if (level == 0) { return; }

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
	
	lua_getglobal(L, "getKeys"); //Appelle la fonction qui permet d'obtenir les éléments 
	if (lua_isnil(L, -1)) {
		std::cout << "Get keys function is not loaded. Loading..." << std::endl;
		loadGetKeysFunction(L);
		lua_getglobal(L, "getKeys");
	}
	
	lua_getToStack(L, name.c_str());
	lua_pcall(L, 1, 1, 0);//La fonction précise qu'une seule valeur de retour est attendue

	lua_pushnil(L);
	std::vector<std::string> keys;
	//Violation of access happeniing right here...
	//like, just below this line, means the file was not oppened !
	while (lua_next(L, -2)) { // get values one by one
		//std::cout << "*";
		if (lua_type(L, -1) == LUA_TSTRING) { // Vérifie si la valeur est une chaine de caractères
			keys.push_back(lua_tostring(L, -1));
		}
		lua_pop(L, 1);
	}
	
	lua_settop(L, 0); // Retire la fonction getkeys du stack
	return keys;
}

/*****
Fonction qui permet d'obtenir l'adresse de tous les fichiers situés dans le répertoire indiqué
*****/
std::vector<std::string> getFiles(const std::string& filepath, const std::string& ext){
	std::vector<std::string> files;
	//Check if 
	if (!filepath.empty()){

		boost::filesystem::path path(filepath);
		boost::filesystem::recursive_directory_iterator end;
		for (boost::filesystem::recursive_directory_iterator i(path); i != end; i++)
		{
			if (boost::filesystem::is_regular_file(*i) && i->path().extension() == ext) 
				files.push_back(boost::filesystem::path(*i).string());
		}
	}
	return files;
}

/****
Permet d'initialiser les scripts et de sauvegader ceux-ci dans le vecteur fournis
****/
void luain::loadFromRep(std::vector<std::shared_ptr<GameObject>> &objects, Scene *s, const std::string& filepath, const std::string& ext){
	//obtient l'emplaement de tous les fichiers
	std::vector<std::string> files_dir = getFiles(filepath,ext);
	if (!files_dir.empty()){
		for (int i = 0; i < files_dir.size(); i++){
			std::cout << "Loading file :"<<files_dir[i]<<std::endl;
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);
			luaL_dofile(L, std::string(files_dir[i]).c_str());
			loadGetKeysFunction(L);
			boost::filesystem::path p(files_dir[i]);
			std::shared_ptr<GameObject> obj = loadGameObjects(L, s, p.stem().string());
			obj->setID(p.stem().string());
			obj->setType(p.stem().string());
			objects.push_back(obj);
		}
	}
}

void luain::loadFromRep(std::map<std::string, std::shared_ptr<GameObject>> &objs, Scene *s, const std::string& filepath, const std::string& ext){
	std::vector<std::string> files_dir = getFiles(filepath, ext);
	if (!files_dir.empty())
		for (int i = 0; i < files_dir.size(); i++){
			std::cout << "Loading file :" << files_dir[i] << std::endl;
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);
			luaL_dofile(L, std::string(files_dir[i]).c_str());
			loadGetKeysFunction(L);
			boost::filesystem::path p(files_dir[i]);
			std::shared_ptr<GameObject> obj = loadGameObjects(L, s, p.stem().string());
			//boost::filesystem::path p(files_dir[i]);
			if (objs.find(p.stem().string()) == objs.end()){
				obj->setID(p.stem().string());
				obj->setType(p.stem().string());
				std::cout << p.stem().string()<<std::endl;
				//std::cout << obj->getID() << std::endl;
				objs.insert(std::make_pair(obj->getID(), obj));
			}
			else std::cout << "Collision at:" << p.stem().string() << std::endl;
			
		}
}

//template qui permet d'ajouter les différentes composantes aux vecteurs des objets
template <typename T>
void addComponent(std::shared_ptr<GameObject> e, luabridge::LuaRef& componentTable) {
	e->addComponent(std::type_index(typeid(T)), std::make_shared<T>(componentTable));
}

//
std::shared_ptr<GameObject> luain::loadGameObjects(lua_State* L, Scene* s, const std::string& type){
	using namespace luabridge;
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	obj->setID(type);
	obj->setType(type);

	auto v = luain::getTableKeys(L, type);
	LuaRef entityTable = getGlobal(L, type.c_str());
	for (auto& componentName : v) {
		if (componentName == "Position") {
			LuaRef poscTable = entityTable[componentName];
			addComponent<PositionComponent>( obj, poscTable);
		}
		else if (componentName == "Physics"){
			LuaRef phycTable = entityTable[componentName];
			addComponent<PhysicsComponent>( obj, phycTable);
		}
		else if (componentName == "Graphics"){
			LuaRef graTable = entityTable[componentName];
			addComponent<GraphicsComponent>( obj, graTable);
		}
		else if (componentName == "GameLogic"){
			LuaRef GLTable = entityTable[componentName];
			addComponent<GameLogicComponent>( obj, GLTable);
		}
		else if (componentName == "Action"){
			LuaRef ACTable = entityTable[componentName];
			addComponent<ActionComponent>( obj, ACTable);
		} 
		else if (componentName == "Objective"){
			LuaRef ObjTable = entityTable[componentName];
			s->getMissionSystem()->addObjective(ObjTable);
		}
		else if (componentName == "AI"){
			LuaRef ACTable = entityTable[componentName];
			addComponent<AiComponent>( obj, ACTable);
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

			if (obj->hasComponent(typeid(AiComponent))){
				obj->get<AiComponent>()->setPhysicsComp(obj->get<PhysicsComponent>());
			}
		}
	}

	std::cout << "\n\n\n";
	return obj;
}
