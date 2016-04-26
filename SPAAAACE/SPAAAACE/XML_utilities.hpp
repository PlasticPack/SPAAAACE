#pragma once

#include <vector>
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
	bool loadObjects(std::vector<std::shared_ptr<GameObject> > &pureObjects, std::map<std::string, std::shared_ptr<GameObject> > roughtObjects, const std::string &filepath);
	bool saveObjects(std::vector<std::shared_ptr<GameObject> > &objects, const std::string &filepath);
	bool saveObjects(std::map<std::string, std::shared_ptr<GameObject>> &objects, const std::string &filepath);
}