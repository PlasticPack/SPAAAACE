#pragma once
#include "Message.h"
#include "GameObject.h"
#include "cinematique.h"
#include <vector>
#include "Command.h"
class CinemaSystem
{
private:
	bool m_isPlaying;
	std::map<std::string,Cinematique> m_cinematique;
	
public:
	CinemaSystem(); // TODO: prend les cinématique d'un fichier texte
	~CinemaSystem();

	void updateObject(GameObject* objet, Message &postman);
	void update(Message &postman);

	bool openCin(std::string filePath);


	bool isPlaying(void) { return m_isPlaying; }
	//fonction de cinématique
	
};
