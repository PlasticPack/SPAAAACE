#include "XML_utilities.hpp"
#include "tinystr.h"
#include "tinyxml.h"

bool XML_u::loadObjects(std::vector<GameObject> &pureObjects, std::map<std::string,GameObject> roughtObjects, const std::string &filepath){
	//Pure objects are the objects after having been initialised corrrectly, and so
	//should only be used as so and at the very end of that process. 
	//Rought objects are the molds that are used to make the pure objects. They are
	//the objects directly extracted from the script.
	TiXmlDocument doc(filepath.c_str());
	if (doc.LoadFile()){
		TiXmlElement *root = doc.RootElement();
		
		if (!root->ValueTStr().empty()){
			std::cout << "Root is not empty!" << std::endl;
			for (TiXmlElement *entityTypes = root; entityTypes;entityTypes= entityTypes->NextSiblingElement()){
				std::cout << entityTypes->Value() << std::endl;
				//Check if element category is actually an entry defined by scripted subclasses
				if (roughtObjects.find(entityTypes->Value()) != roughtObjects.end()){
					std::cout << "Found script for : " << entityTypes->Value() << std::endl;
					//Creates all the instances of the entitys
					for (TiXmlElement *entity = entityTypes->FirstChildElement(); entity; entity = entity->NextSiblingElement()){
						std::cout << "Initialising "<<entity->Value() <<" as "<<entityTypes->Value()<<" entity."<< std::endl;
						GameObject object = roughtObjects[entityTypes->Value()];
						for (TiXmlElement *node = entity->FirstChildElement(); node; 
							node = node->NextSiblingElement()){
							std::cout << node->Value() << std::endl;
							//check if node as child(i.e. if node is a table or a table entry)
							if (node->FirstChild()==NULL){
								std::cout << "Has attributes" << std::endl;
								
								if (strcmp("health", node->Value()) == 0){
									//For now we don't give a f*ck about health
									std::cout << "Setting health" << std::endl;
								}
								else if (strcmp("mass", node->Value()) == 0){
									//Mass is supposed to be set by scripts
									if (object.hasComponent(typeid(PhysicsComponent))){
										std::cout << "Setting mass to : " << node->GetText() << std::endl;
										object.get<PhysicsComponent>()->setMass(std::stod(node->GetText()));
									}
								}
							}
							else{
								
								std::cout << "Has non-attributes" << std::endl;
								if (strcmp("position", node->Value()) == 0){
									std::cout << "Setting position." << std::endl;
									if (object.hasComponent(typeid(PositionComponent))){
										
										for (TiXmlElement *subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
											if (strcmp("x", subnode->Value()) == 0){
												object.get<PositionComponent>()->setPosition(Vec2(std::stod(subnode->GetText()), object.get<PositionComponent>()->getPosition().y()));
											}
											else if (strcmp("y", subnode->Value()) == 0){
												object.get<PositionComponent>()->setPosition(Vec2(object.get<PositionComponent>()->getPosition().x(), std::stod(subnode->GetText())));
											}
										}
									}

								}
								else if (strcmp("velocity", node->Value()) == 0){
									std::cout << "Has physic component" << std::endl;
									if (object.hasComponent(typeid(PhysicsComponent))){
										std::cout << "Setting velocity." << std::endl;
										for (TiXmlElement *subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
											if (strcmp("x", subnode->Value()) == 0){
												object.get<PhysicsComponent>()->setVelocity(Vec2(std::stod(subnode->GetText()), object.get<PhysicsComponent>()->getVelocity().y()));

											}
											else if (strcmp("y", subnode->Value()) == 0){
												object.get<PhysicsComponent>()->setVelocity(Vec2(object.get<PhysicsComponent>()->getVelocity().x(), std::stod(subnode->GetText())));
											}
										}
									
										
									}
									
								}
								else if (strcmp("forces", node->Value()) == 0){
									if (object.hasComponent(typeid(PhysicsComponent))){
										std::cout << "Setting forces." << std::endl;
										for (TiXmlElement *subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
											if (strcmp("x", subnode->Value()) == 0){
												object.get<PhysicsComponent>()->setForces(Vec2(std::stod(subnode->GetText()), object.get<PhysicsComponent>()->getForces().y()));
											}
											else if (strcmp("y", subnode->Value()) == 0){
												object.get<PhysicsComponent>()->setForces(Vec2(object.get<PhysicsComponent>()->getForces().x(), std::stod(subnode->GetText())));
											}
										}
									}
								}
								else std::cout << "No position component!" << std::endl;
							}
							object.setID(entityTypes->Value());
							pureObjects.push_back(object);
						}
					}

				}
					
			}
		}
		else{ std::cout << "Root is empty" << std::endl; return false; }
	}
	else {
		std::cout << "File could not be opened" << std::endl; return false;
	}
	return true;
}