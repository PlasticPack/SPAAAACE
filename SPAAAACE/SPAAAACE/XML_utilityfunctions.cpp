/*****
Créé par Tristan Roy.
Permet l'écriture et la lecture de fichiers XML grâce à TinyXML1.
****/
#include "XML_utilities.hpp"

//Permet de charger les objets enregistrés dans dans la sauvegarde XML. Nécéssite qu'on lui passe les objets du script initialisés.
bool XML_u::loadObjects(Scene *s, std::vector<std::shared_ptr<GameObject> > &pureObjects, std::map<std::string, std::shared_ptr<GameObject>> roughtObjects, const std::string &filepath){
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
						if (object->hasComponent(std::type_index(idn(PhysicsComponent))))
							object->get<PhysicsComponent>()->setPositionComp(object->get<PositionComponent>());
						if (object->hasComponent(std::type_index(idn(GraphicsComponent))))
							object->get<GraphicsComponent>()->setPositionComponent(object->get<PositionComponent>());

						if (object->hasComponent(std::type_index(idn(AiComponent))))
							object->get<AiComponent>()->setPhysicsComp(object->get<PhysicsComponent>());

						for (element node = entity->FirstChildElement(); node;
							node = node->NextSiblingElement()){
							//std::cout << node->Value() << std::endl;
							//check if node has child(i.e. if node is a table or a table entry)
							//May change with a method with attributes
							//Don't fracking delete this!

							if (!node->FirstChildElement() == NULL){
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
								/*else if (strcmp("size", node->Value()) == 0){
									if (object->hasComponent(idn(GraphicsComponent))){
										for (element subnode = node->FirstChildElement(); subnode; subnode = subnode->NextSiblingElement()){
										if (strcmp("x", subnode->Value()) == 0){
										object->get<GraphicsComponent>()->setSize(Vec2(std::stoi(subnode->GetText()), object->get<GraphicsComponent>()->getSize().y()));
										}
										else if (strcmp("y", subnode->Value()) == 0){
										object->get<GraphicsComponent>()->setSize(Vec2(object->get<GraphicsComponent>()->getSize().x(), std::stoi(subnode->GetText())));
										}
										}
										}
									if (object->hasComponent(idn(PhysicsComponent))){
										phys(object)->setHitboxRadius(std::stoi(node->FirstChildElement()->GetText()));
									}
								}*/


								//else std::cout << "No position component!" << std::endl;
							}
						else{
								//std::cout << "Has attributes" << std::endl;

								if (strcmp("health", node->Value()) == 0){
									//For now we don't give a f*ck about health
									//std::cout << "Setting health" << std::endl;
								}
								else if (strcmp("sprite", node->Value()) == 0){
									//Sets the sprite of the object
									object->get<GraphicsComponent>()->setSprite(std::make_shared<Sprite>(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture(node->GetText(), 1, 1),node->GetText())));
								}
								else if (strcmp("mass", node->Value()) == 0){
									//Mass is supposed to be set by scripts
									if (object->hasComponent(idn(PhysicsComponent))){
										//std::cout << "Setting mass to : " << node->GetText() << std::endl;
										object->get<PhysicsComponent>()->setMass(std::stod(node->GetText()));
									}
								}
								else if (strcmp("id", node->Value()) == 0){
									object->setID(node->GetText());
									//object->setID(node->GetText());
									object->idSet(true);
								}
								else if (strcmp("trigger", node->Value()) == 0){
									if (object->hasComponent(idn(ActionComponent))){
										object->get<ActionComponent>()->setTrigger(stringToMessage(node->GetText()));
									}
								}
								else if (strcmp("answer", node->Value()) == 0){
									if (object->hasComponent(idn(ActionComponent))){
										object->get<ActionComponent>()->setAnswer(stringToMessage(node->GetText()));
									}
								}
								
								if (strcmp("done", node->Value()) == 0){
									//si c'est un objectif
									if (object->getID().find("objective") != std::string::npos){
										s->getMissionSystem()->activateObjective(object->getID(), true);
									}
								}
							}
							
						}
						
						if (!object->idSet()) {

							object->setID(entityTypes->Value());
							object->setType(entityTypes->Value());
						}
						else {
							std::cout << "<\n\n\n\n " << object->getID() << "  \n\n\n\n\n";
						}

						object->setType(entityTypes->Value());

						
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
/*
Permet de sauvegarded les objets transmis sous la forme d'un fichier XML du nom spécifié.
Le nom du script selon lequel a été initialisé sert normalement de balises dans le fichier. 
Toutes les entités d'objets de type emblables sont sauvegardés dans la même balise.
*/
bool XML_u::saveObjects(Scene *s, std::vector<std::shared_ptr<GameObject> > &objects, const std::string &filepath){
	std::map<std::string, std::vector<std::shared_ptr<GameObject>>> objects_byTypes;
	//Dont't touch this part, very important for class differ
	for (int i = 0; i < objects.size(); i++){
		if (objects_byTypes.find(objects[i]->getType()) != objects_byTypes.end())
			objects_byTypes[objects[i]->getType()].push_back(objects[i]);
		else objects_byTypes.insert(std::make_pair(objects[i]->getType(), std::vector<std::shared_ptr<GameObject>>(1, objects[i])));
	}
	TiXmlDocument doc;
	TiXmlDeclaration * declaration = new TiXmlDeclaration("1.0", "", "");

	for (std::map<std::string, std::vector<std::shared_ptr<GameObject>>>::iterator iterator = objects_byTypes.begin(); iterator != objects_byTypes.end(); iterator++){
		element script = new_element(iterator->first);
		int cpt = 0;
		for (std::shared_ptr<GameObject> object : iterator->second){

			element elements = new_element(iterator->first+ std::to_string(cpt));
			element id = new_element(STR(id));
			id->LinkEndChild(new_text(object->getID()));
			elements->LinkEndChild(id);
			if (object->getID().find("objective") != std::string::npos){
				if (s->getMissionSystem()->getObjective(object->getID()).done){
					element done = new_element(STR(done));
					done->LinkEndChild(new_text(""));
					elements->LinkEndChild(done);
				}
			}

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
				if (object->getType()=="planet"){
					element sprite = new_element(STR(sprite));
					sprite->LinkEndChild(new_text(object->get<GraphicsComponent>()->getFilename()));
					elements->LinkEndChild(sprite);
				}
				if (strcmp("planet", object->getType().c_str()) == 0 || strcmp("star", object->getType().c_str()) == 0){
					element size = new_element(STR(size));
					element x = new_element(STR(x));
					x->LinkEndChild(new_text(std::to_string(object->get<GraphicsComponent>()->getSize().x())));
					size->LinkEndChild(x);
					element y = new_element(STR(y));
					y->LinkEndChild(new_text(std::to_string(object->get<GraphicsComponent>()->getSize().y())));
					size->LinkEndChild(y);
					elements->LinkEndChild(size);
				}
			}
			script->LinkEndChild(elements);
			cpt++;
		}
		doc.LinkEndChild(script);
	}
	return doc.SaveFile(filepath);
	//declaration->
}


/****
Convertie la map en un vecteur tel que prévus par la focntion originale et emploi celle-ci sur le 
vecteur en question créé.
****/
bool XML_u::saveObjects(Scene *s,std::map<std::string, std::shared_ptr<GameObject>> &objects, const std::string &filepath){
	std::vector<std::shared_ptr<GameObject>> pure_objs;
	for (std::pair<std::string, std::shared_ptr<GameObject>> obj : objects)
		pure_objs.push_back(obj.second);
	return XML_u::saveObjects(s, pure_objs, filepath);
}

#undef idn
#undef element
#undef new_element(s)
#undef text
#undef new_text(s)
#undef STR(s)
#undef phys(o)
#undef pos(o)
