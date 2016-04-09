/*#include "LuaInit.hpp"
#include"XML_utilities.hpp"

int main(int argc, char *argv[]){
	std::vector<std::shared_ptr<GameObject>> pure_objs;
	std::map<std::string, std::shared_ptr<GameObject>> rought_objs;
	Scene *scene=new Scene("","");
	luain::loadFromRep(scene, rought_objs, "scripts");
	//std::cout << "Has loaded scripts?" << std::endl;
	XML_u::loadObjects(pure_objs, rought_objs, "Saves/save.xml");
	//std::cout << pure_objs[0].get<PhysicsComponent>()->getMass() << std::endl;
	element test = new_element(STR(test));
	XML_u::saveObjects(pure_objs, "Saves/save2.xml");
	system("PAUSE");
	return 1;
}*/