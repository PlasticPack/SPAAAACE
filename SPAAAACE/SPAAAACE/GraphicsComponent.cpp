#include "GraphicsComponent.h"
#include <LuaBridge.h>

GraphicsComponent::GraphicsComponent()
{
	m_posComponent = nullptr;
	m_sprite = nullptr;
	m_center = Vec2(0, 0);
	m_size = Vec2(0, 0);
	m_camera = true;
}


GraphicsComponent::GraphicsComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PositionComponent> comp){
	GraphicsComponent();
	using namespace luabridge;
	auto spriteRef = componentTable["filename"];
	auto cols = componentTable["c"];
	auto rows = componentTable["r"];
	auto anim_speed = componentTable["anim_speed"];
	auto cam = componentTable["cam"];
	auto width = componentTable["width"];
	auto height = componentTable["height"];

	m_camera = true;
	if (spriteRef.isString()){

		int c = 1, r = 1;
		if (cols.isNumber())
			c = cols.cast<int>();

		if (rows.isNumber())
			r = rows.cast<int>();

		if (spriteRef.cast<std::string>().find("__") == std::string::npos){
			// si on a pas trouvé de __ ca veut dire qu'il y a pas de spécification de nom
			//donc on prend default 
			m_sprite = std::make_shared<Sprite>(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture(spriteRef), spriteRef, c, r));
			m_sprite->getCurrentSpriteSheet()->setFilename(spriteRef);
		}
		else {
			m_sprite = std::make_shared<Sprite>();
			//on récupère les différentes spritesheet
			std::string filepath = spriteRef.cast < std::string>();

			std::istringstream buf(filepath);
			std::istream_iterator<std::string> beg(buf), end;
			std::vector<std::string> sheets(beg, end);
			
			for (int s = 0; s < sheets.size(); s++){
				std::string part = sheets[s];

				if (isdigit(part[0]) && isdigit(part[1])){

					c = part[0] - '0';
					r = part[1] - '0';

					part.erase(0, 2);
				}

				std::string name = part.substr(0, part.find("__"));

				std::string file = part.substr(part.find("__") + 2, part.find("__") + 2 + part.length());

				std::cout << name << "__" << file << "\n";
				m_sprite->addSpriteSheet(name, std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture(file), file, c, r));
				m_sprite->getSpriteSheet(name)->setFilename(file);
			}
		}
		
		if (anim_speed.isNumber()){
			m_sprite->setAnimationSpeed(anim_speed);
		}

		if (cam.isString() && (cam == "true" || cam == "false")){
			if (cam == "true"){
				m_camera = true;
			}
			else {
				m_camera = false;
			}
		}

		if (width.isNumber() && height.isNumber()){
			m_size = Vec2(width, height);
		}
		else {
			m_size = Vec2(m_sprite->getCurrentSpriteSheet()->getCurrentRect().w, m_sprite->getCurrentSpriteSheet()->getCurrentRect().h);
		}

		m_center = Vec2(m_size.x() / 2, m_size.y() / 2);
		
	}
	else{

		setSprite(nullptr);
	}

	m_maxSize = m_size;
	this->m_posComponent = comp;
}

GraphicsComponent::GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr)
{
	m_camera = true;
	m_sprite = spr;
	m_center = Vec2(spr->getCurrentSpriteSheet()->getCurrentRect().w / 2, spr->getCurrentSpriteSheet()->getCurrentRect().h / 2);
	m_posComponent = comp;
	m_size = Vec2(spr->getCurrentSpriteSheet()->getCurrentRect().w, spr->getCurrentSpriteSheet()->getCurrentRect().h);
	m_maxSize = m_size;
}

void GraphicsComponent::setSprite(std::shared_ptr<Sprite> spr){
	m_sprite = spr;
}

GraphicsComponent::GraphicsComponent(GraphicsComponent const& g){
	if (this != &g){
		m_camera = g.m_camera;
		m_center = g.m_center;
		m_maxSize = g.m_maxSize;
		m_size = g.m_size;
		
		m_sprite.reset();
		m_sprite = std::make_shared<Sprite>(*g.m_sprite);

		m_posComponent.reset();
		m_posComponent = std::make_shared<PositionComponent>(*g.m_posComponent);
	}
}

GraphicsComponent& GraphicsComponent::operator=(GraphicsComponent const& g){
	if (this != &g){
		m_camera = g.m_camera;
		m_center = g.m_center;
		m_maxSize = g.m_maxSize;
		m_size = g.m_size;

		m_sprite.reset();
		m_sprite = std::make_shared<Sprite>(*g.m_sprite);

		m_posComponent.reset();
		m_posComponent = std::make_shared<PositionComponent>(*g.m_posComponent);
	}
	return *this;
}

GraphicsComponent::~GraphicsComponent()
{
}

void GraphicsComponent::setAnimationSpeed(double s){
	m_sprite->setAnimationSpeed(s);
}

Vec2 GraphicsComponent::getSize(){
	return m_size;
}

Vec2 GraphicsComponent::getMaxSize(){
	return m_maxSize;
}


void GraphicsComponent::setSize(Vec2 s){
	m_size = s;
}

Vec2 GraphicsComponent::getCenter(){
	return m_center;
}

std::shared_ptr<PositionComponent> GraphicsComponent::getPositionComponent(){
	return m_posComponent;
}

std::shared_ptr<Sprite> GraphicsComponent::getSprite() const{
	return m_sprite;
}

Vec2 GraphicsComponent::getPosition(){
	return m_posComponent->getPosition();
}

void GraphicsComponent::setPositionComponent(std::shared_ptr<PositionComponent> pcomp=nullptr){
	this->m_posComponent = pcomp;
}