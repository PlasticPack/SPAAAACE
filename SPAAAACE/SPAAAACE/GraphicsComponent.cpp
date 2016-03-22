#include "GraphicsComponent.h"
#include <LuaBridge.h>

GraphicsComponent::GraphicsComponent()
{
	m_posComponent = nullptr;
	m_sprite = nullptr;
	m_halo = { 255, 25, 25, 255};
	m_haloOn = false;
	m_center = Vec2(0, 0);
	m_size = Vec2(0, 0);
}


GraphicsComponent::GraphicsComponent(luabridge::LuaRef& componentTable){
	GraphicsComponent();
	using namespace luabridge;
	auto spriteRef = componentTable["filename"];
	if (spriteRef.isString()){
		auto heightRef = componentTable["h"];
		auto wideRef = componentTable["w"];
		auto texture = IMG_LoadTexture(NULL, spriteRef.cast<std::string>().c_str());
		if (wideRef.isNumber() && heightRef.isNumber()){
			
			auto rect = SDL_Rect();
			rect.h = heightRef.cast<int>();
			rect.w = wideRef.cast<int>();
			auto size = SDL_Rect();
			setSprite(std::make_shared<Sprite>(std::make_shared<SpriteSheet>(texture, rect,size)));
			
		}
		else{
			setSprite(std::make_shared<Sprite>(std::make_shared<SpriteSheet>(texture, SDL_Rect(), SDL_Rect())));
		}
		
	}
	else{
		setSprite(nullptr);
	}


	this->m_posComponent = nullptr;
}

GraphicsComponent::GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr)
{
	m_halo = { 255, 25, 25, 255 };
	m_sprite = spr;
	m_center = Vec2(spr->getCurrentSpriteSheet()->getCurrentRect().w / 2, spr->getCurrentSpriteSheet()->getCurrentRect().h / 2);
	//std::cout << m_center.x() << "  TWAS THE CENTER :D \n";
	m_posComponent = comp;
	m_size = Vec2(spr->getCurrentSpriteSheet()->getCurrentRect().w, spr->getCurrentSpriteSheet()->getCurrentRect().h);
}

void GraphicsComponent::setSprite(std::shared_ptr<Sprite> spr){
	m_sprite = spr;
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

void GraphicsComponent::setHaloColor(C_Color c){
	m_halo = c;
}

void GraphicsComponent::activateHalo(bool h){
	m_haloOn = h;
}

C_Color GraphicsComponent::getHaloColor(){
	return m_halo;
}

bool GraphicsComponent::isHaloOn(){
	return m_haloOn;
}

void GraphicsComponent::setPositionComponent(std::shared_ptr<PositionComponent> pcomp=nullptr){
	this->m_posComponent = pcomp;
}