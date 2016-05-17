/*
Auteur: Tristan Roy
Namespace qui permet de sauvegarder les informations contenues dans les objets propres au jeu
sous la forme de fichiers structurés XML. Permet aussi de charger ces informations.
*/
#pragma once

#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include"PhysicsComponent.h"
#include"GraphicsComponent.h"
#include"PositionComponent.h"
#include"tinystr.h"
#include"tinyxml.h"

#define idn typeid
#define element TiXmlElement*
#define new_element(s) new TiXmlElement(s)
#define text TiXmlText*
#define new_text(s) new TiXmlText(s)
#define STR(s) #s
#define phys(o) o->get<PhysicsComponent>() 
#define pos(o) o->get<PositionComponent>() 



namespace XML_u{
	bool loadObjects(Scene *s, std::vector<std::shared_ptr<GameObject> > &pureObjects, std::map<std::string, std::shared_ptr<GameObject> > roughtObjects, const std::string &filepath);
	bool saveObjects(Scene *s, std::vector<std::shared_ptr<GameObject> > &objects, const std::string &filepath);
	bool saveObjects(Scene *s, std::map<std::string, std::shared_ptr<GameObject>> &objects, const std::string &filepath);
}
