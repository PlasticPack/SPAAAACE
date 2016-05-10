/*	Créateur: Marc-Antoine Céré
Projet: SPAAAACE
Fichier: AiComponent.h
But: Prototype de la classe AiComponent
*/
#pragma once

#include "Component.h"
#include "PhysicsComponent.h"
#include "GameLogicComponent.h"

#include "Vec2.h"
#include <math.h>

class AiComponent : public Component{
protected:
	Vec2 m_vec;
	int m_typeAi;
	int m_acc;
	int m_pwr;
	std::shared_ptr<PhysicsComponent> m_target, m_phyComponent;
	std::shared_ptr<PhysicsComponent>  m_nearDanger;
	std::shared_ptr<GameLogicComponent> m_gameLogicComponent;


public:



	AiComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PhysicsComponent> comp = nullptr);

	//CODE DE CHARLES : contructeur de copie + operator=
	AiComponent(AiComponent const& ai);
	AiComponent& operator=(AiComponent const& ai);

	~AiComponent();


	int getType();
	Vec2 getVec();
	std::shared_ptr<PhysicsComponent> getPhysicsComponent();
	std::shared_ptr<GameLogicComponent> getGLComponent();
	std::shared_ptr<PhysicsComponent> getTarget();
	std::shared_ptr <PhysicsComponent> getNearDanger();
	int getPower();

	void setType(int);
	void setVec(Vec2);
	void setPhysicsComp(std::shared_ptr<PhysicsComponent> comp);
	void setGLComponent(std::shared_ptr<GameLogicComponent> comp);
	void setTarget(std::shared_ptr<PhysicsComponent> comp);
	void setNearDanger(std::shared_ptr <PhysicsComponent> comp);
	void setPower(int pwr);
	bool isCloseToDanger();
};
/*	Créateur: Marc-Antoine Céré
Projet: SPAAAACE
Fichier: AiComponent.h
But: Prototypes de la classe AiComponent

#pragma once

#include "Component.h"
#include "PhysicsComponent.h"

#include "Vec2.h"
#include <math.h>

class AiComponent : public Component{
protected:
	Vec2 m_vec;
	int m_typeAi;
	int m_acc;
	std::shared_ptr<PhysicsComponent> m_target, m_phyComponent;
	std::shared_ptr<PhysicsComponent>  m_nearDanger;
	

public:

	

	AiComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PhysicsComponent> comp = nullptr);

	//CODE DE CHARLES : contructeur de copie + operator=
	AiComponent(AiComponent const& ai);
	AiComponent& operator=(AiComponent const& ai);

	~AiComponent();
	

	int getType();
	Vec2 getVec();
	std::shared_ptr<PhysicsComponent> getPhysicsComponent();
	std::shared_ptr<PhysicsComponent> getTarget();
	std::shared_ptr <PhysicsComponent> getNearDanger();
	int getAcc();

	void setType(int);
	void setVec(Vec2);
	void setPhysicsComp(std::shared_ptr<PhysicsComponent> comp);
	void setTarget(std::shared_ptr<PhysicsComponent> comp);
	void setNearDanger(std::shared_ptr <PhysicsComponent> comp);
};
*/