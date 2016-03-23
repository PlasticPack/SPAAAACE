#pragma once

#include <vector>
#include "GameObject.h"
#include"PhysicsComponent.h"
#include"GraphicsComponent.h"
#include"PositionComponent.h"

namespace XML_u{
	bool loadObjects(std::vector<GameObject> &pureObjects, std::map<std::string,GameObject> roughtObjects, const std::string &filepath);
	bool saveObjects(std::vector<GameObject> objects, const std::string &filepath);
}