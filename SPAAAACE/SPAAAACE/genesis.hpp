/*
Namespace qui permet de générer procéduralement des niveaux de jeux, des images ainsi que des surfaces de planètes.
*/

#pragma once

#include<iostream>
#include"gen_image.hpp"
#include<vector>
#include<random>
#include"Vec2.h"
#include"GameObject.h"
#include"Scene.h"

#define M_PI           3.14159265358979323846
#define CONTINENT_SIZE 257//must be (2^k)+1
#define PLANET_W 900//Must be bigger than CONTINENT_SIZE 
#define PLANET_H 300//^    ^  ^      ^    ^  
#define COLOR_BLUE 0
#define COLOR_RED 1
#define COLOR_WHITE 2
#define COMPRESSION_SETTING 3
#define CONTINENT_VARIATION 10
#define CONTINENT_BASENB 10
#define PC 30856775814913.672789139379577965
#define EARTH_MASS 
#define M_G 0.00000000006674
#define CONTINENT_GIANT_BASENB 4
#define CONTINENT_GIANT_VARIATION 3


namespace genesis{

	struct satelite{
		Vec2 position;
		double mass;//mesured in MA
	};

	struct planet{
		Vec2 position;
		double mass;//mesured in MA
		int type;
		std::vector<satelite> satelites;
	};

	struct star
	{
		int color;
		Vec2 position;
		double mass;//mesured in MA
		std::vector < planet > planets;
	};

	std::vector<std::vector<int>> generateContinent(const std::string& seed, int& mean, int& SD);
	std::vector<std::vector<int>> generatePlanet(const std::string& seed, int& type);
	std::vector<genesis::star> generateSystem(const std::string& filepath,const std::string& seed = "Supper's Ready");
	std::vector<std::shared_ptr<GameObject>> generateObjects(Scene *scene,const std::string& filepath, const std::string& seed = "Supper's Ready");
	std::string generateString();

}

