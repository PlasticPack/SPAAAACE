#include "XML_utilities.hpp"

bool XML_u::loadObjects(std::vector<std::shared_ptr<GameObject> > &pureObjects, std::map<std::string, std::shared_ptr<GameObject>> roughtObjects, const std::string &filepath){
	//Pure objects are the objects after having been initialised corrrectly, and so
	//should only be used as so and at the very end of that process. 
	//Rought objects are the molds that are used to make the pure objects. They are
	//the objects directly extracted from the script.
	TiXmlDocument doc(filepath.c_str());
	if (doc.LoadFile()){
		element root = doc.RootElement();
		std::cout << "Start of XML parsing" << std::endl;
		if (!root->ValueTStr().empty()){
			std::cout << "Root is not empty!" << std::endl;
			for (element entityTypes = root; entityTypes; entityTypes = entityTypes->NextSiblingElement()){
				std::cout << "Checking concordance for "<<entityTypes->Value() << std::endl;
				//Check if element category is actually an entry defined by scripted subclasses
				if (roughtObjects.find(entityTypes->Value()) != roughtObjects.end()){
					std::cout << "Found script for " << entityTypes->Value() << std::endl;
					//Creates all the instances of the entitys
					for (element entity = entityTypes->FirstChildElement(); entity; entity = entity->NextSiblingElement()){
						std::cout << "Initialising " << entity->Value() << " as " << entityTypes->Value() << " entity." << std::endl;
						std::shared_ptr<GameObject> object = std::make_shared<GameObject>(*roughtObjects[entityTypes->Value()]);

						//on arrange les components de l'objet pour qu'ils pointent tous sur un seul 
						//position component
						if (object->hasComponent(std::type_index(idn(PhysicsComponent)))){
							object->get<PhysicsComponent>()->setPositionComp(object->get<PositionComponent>());
						}

						if (object->hasComponent(std::type_index(idn(GraphicsComponent)))){
							object->get<GraphicsComponent>()->setPositionComponent(object->get<PositionComponent>());
						}

						if (object->hasComponent(std::type_index(idn(AiComponent)))){
							object->get<AiComponent>()->setPhysicsComp(object->get<PhysicsComponent>());
						}

						for (element node = entity->FirstChildElement(); node;
							node = node->NextSiblingElement()){
							//std::cout << node->Value() << std::endl;
							//check if node has child(i.e. if node is a table or a table entry)
							//May change with a method with attributes
							//Don't fracking delete this!

							if (node->FirstChildElement() == NULL){
								//std::cout << "Has attributes" << std::endl;

								if (strcmp("health", node->Value()) == 0){
									//For now we don't give a f*ck about health
									//std::cout << "Setting health" << std::endl;
								}
								else if (strcmp("mass", node->Value()) == 0){
									//Mass is supposed to be set by scripts
									if (object->hasComponent(idn(PhysicsComponent))){
										//std::cout << "Setting mass to : " << node->GetText() << std::endl;
										object->get<PhysicsComponent>()->setMass(std::stod(node->GetText()));
									}
								}
							}
							else{

								//std::cout << "Has non-attributes" << std::endl;
								if (strcmp("position", node->Value()) == 0){
									//std::cout << "Setting position." << std::endl;
									if (object->hasComponent(idn(PositionComponent))){

										for (element subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
											if (strcmp("x", subnode->Value()) == 0){
												pos(object)->setPosition(Vec2(std::stod(subnode->GetText()), pos(object)->getPosition().y()));
											}
											else if (strcmp("y", subnode->Value()) == 0){
												pos(object)->setPosition(Vec2(pos(object)->getPosition().x(), std::stod(subnode->GetText())));
											}
										}
									}

								}
								else if (strcmp("velocity", node->Value()) == 0){
									//std::cout << "Has physic component" << std::endl;
									if (object->hasComponent(idn(PhysicsComponent))){
										//std::cout << "Setting velocity." << std::endl;
										for (element subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
											if (strcmp("x", subnode->Value()) == 0){
												phys(object)->setVelocity(Vec2(std::stod(subnode->GetText()), phys(object)->getVelocity().y()));

											}
											else if (strcmp("y", subnode->Value()) == 0){
												phys(object)->setVelocity(Vec2(phys(object)->getVelocity().x(), std::stod(subnode->GetText())));
											}
										}


									}

								}
								else if (strcmp("forces", node->Value()) == 0){
									if (object->hasComponent(idn(PhysicsComponent))){
										//std::cout << "Setting forces." << std::endl;
										for (TiXmlElement *subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
											if (strcmp("x", subnode->Value()) == 0){
												phys(object)->setForces(Vec2(std::stod(subnode->GetText()), phys(object)->getForces().y()));
											}
											else if (strcmp("y", subnode->Value()) == 0){
												object->get<PhysicsComponent>()->setForces(Vec2(phys(object)->getForces().x(), std::stod(subnode->GetText())));
											}
										}
									}
								}
								//else std::cout << "No position component!" << std::endl;
							}
							object->setID(entityTypes->Value());

						}
						
						pureObjects.push_back(object);
					}

				}
				else{
					std::cout << "Script not found for" << entityTypes->Value() << std::endl;
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

bool XML_u::saveObjects(std::vector<std::shared_ptr<GameObject> > &objects, const std::string &filepath){
	std::map<std::string, std::vector<std::shared_ptr<GameObject>>> objects_byTypes;
	//Dont't touch this part, very important for class differ
	for (int i = 0; i < objects.size(); i++){
		//std::cout << objects[i]->getID() << std::endl;
		if (objects_byTypes.find(objects[i]->getID()) != objects_byTypes.end()){
			objects_byTypes[objects[i]->getID()].push_back(objects[i]);
		}
		else objects_byTypes.insert(std::make_pair(objects[i]->getID(), std::vector<std::shared_ptr<GameObject>>(1, objects[i])));
	}


	TiXmlDocument doc;
	TiXmlDeclaration * declaration = new TiXmlDeclaration("1.0", "", "");
	for (std::map<std::string, std::vector<std::shared_ptr<GameObject>>>::iterator iterator = objects_byTypes.begin(); iterator != objects_byTypes.end(); iterator++){
		element script = new_element(iterator->first);
		int cpt = 0;
		for (std::shared_ptr<GameObject> object : iterator->second){
			element elements = new_element(object->getID()
				+ std::to_string(cpt)
				);
			if (object->hasComponent(idn(PositionComponent)))
			{
				element position = new_element(STR(position));
				position->SetAttribute(STR(x), object->get<PositionComponent>()->getPosition().x());
				position->SetAttribute(STR(y), object->get<PositionComponent>()->getPosition().y());
			}
			if (object->hasComponent(idn(PhysicsComponent)))
			{
				//Set mass
				element mass = new_element(STR(mass));
				mass->LinkEndChild(new TiXmlText(std::to_string(phys(object)->getMass())));
				elements->LinkEndChild(mass);

				//Set velocity
				element velocity = new_element(STR(velocity));
				element v_x = new_element(STR(x));
				velocity->LinkEndChild(v_x);
				element v_y = new_element(STR(y));
				velocity->LinkEndChild(v_y);
				v_x->LinkEndChild(new_text(std::to_string(phys(object)->getVelocity().x())));
				v_y->LinkEndChild(new_text(std::to_string(phys(object)->getVelocity().y())));

				//velocity->LinkEndChild((new_element(STR(x)))->LinkEndChild(new_text(std::to_string(object.get<PhysicsComponent>()->getVelocity().x()))));
				//velocity->LinkEndChild((new_element(STR(y)))->LinkEndChild(new_text(std::to_string(object.get<PhysicsComponent>()->getVelocity().y()))));
				//velocity->SetAttribute(STR(x), object.get<PhysicsComponent>()->getVelocity().x());
				//velocity->SetAttribute(STR(Y), object.get<PhysicsComponent>()->getVelocity().y());
				elements->LinkEndChild(velocity);

				element forces = new_element(STR(forces));
				element f_x = new_element(STR(x));
				element f_y = new_element(STR(y));
				forces->LinkEndChild(f_x);
				forces->LinkEndChild(f_y);

				f_x->LinkEndChild(new_text(std::to_string(phys(object)->getForces().x())));
				f_y->LinkEndChild(new_text(std::to_string(phys(object)->getForces().y())));
				//forces->SetAttribute(STR(x), object.get<PhysicsComponent>()->getForces().x());
				//forces->SetAttribute(STR(y), object.get<PhysicsComponent>()->getForces().y());
				elements->LinkEndChild(forces);

				//element x = new_element(STR(x));
				//velocity->LinkEndChild(x);
				//element y = new_element(STR(y));
				//velocity->LinkEndChild(y);
				element position = new_element(STR(position));
				element p_x = new_element(STR(x));
				element p_y = new_element(STR(y));
				position->LinkEndChild(p_x);
				position->LinkEndChild(p_y);

				p_x->LinkEndChild(new_text(std::to_string(pos(object)->getPosition().x())));
				p_y->LinkEndChild(new_text(std::to_string(pos(object)->getPosition().y())));


				elements->LinkEndChild(position);

			}
			if (object->hasComponent(idn(GraphicsComponent)))
			{

			}
			script->LinkEndChild(elements);
			cpt++;
		}
		doc.LinkEndChild(script);
	}
	return doc.SaveFile(filepath);
	//declaration->
}