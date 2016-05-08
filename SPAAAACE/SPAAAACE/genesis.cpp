#include"genesis.hpp"
#include"LuaInit.hpp"
#include<boost\filesystem.hpp>


int factorial(int n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

std::vector<std::vector<int>> genesis::generateContinent(const std::string& seed, int& mean, int& SD){
	//std::cout << "Initialised hashs;" << std::endl;
	//Hash initialisers
	std::hash<int> hash_int;//Variable used to hash different integers
	std::hash<std::string> hash_str;//Variable used to hash different string
	//std::cout << "Initialised hashs;"<<std::endl;
	//To make sure it's not recalculated
	int seed_hashed = hash_str(seed);//Value of the hashed seed used by the function

	//Initial seed for corners
	//Setting a default value to the 4 corners of the square represented by the 2d vector
	double corners = hash_int(hash_str("Planet%dCorner") % hash_str(seed)) % 1000;
	//double corners = 500;

	std::vector<std::vector<int>>matrix(CONTINENT_SIZE, std::vector<int>(CONTINENT_SIZE, 0));

	//Set the value of the corners at origin
	matrix[0][0] =
		matrix[0][CONTINENT_SIZE - 1] =
		matrix[CONTINENT_SIZE - 1][0] =
		matrix[CONTINENT_SIZE - 1][CONTINENT_SIZE - 1] =
		corners;


	//Average range for each offset
	double range = 500;

	for (int sideLength = CONTINENT_SIZE - 1; sideLength >= 2; sideLength /= 2, range /= 2.0){
		//distance separating corners from middle of the diamond
		int halfSide = sideLength / 2;

		//square
		for (int x = 0; x < CONTINENT_SIZE-1; x += sideLength){
			for (int y = 0; y < CONTINENT_SIZE - 1; y += sideLength){
				//calculate the average value of the corners
				double average = matrix[x][y] +
					matrix[x + sideLength][y] +
					matrix[x][y + sideLength] +
					matrix[x + sideLength][y + sideLength];
				average /= 4;

				//create random value between 0 and 1000
				srand(hash_int(seed_hashed % ((hash_int(y) % (x + 1)) + 1)) % 1001);
				matrix[x + halfSide][y + halfSide] = average + (double(rand()) / 1000 * 2 * range) - range;
			}
		}

		//diamond
		for (int x = 0; x < CONTINENT_SIZE - 1; x += halfSide){
			for (int y = (x + halfSide) % sideLength; y < CONTINENT_SIZE - 1; y += sideLength){
				double avg = matrix[(x - halfSide + CONTINENT_SIZE) % CONTINENT_SIZE][y] +
					matrix[(x + halfSide) % CONTINENT_SIZE][y] +
					matrix[x][(y + halfSide) % CONTINENT_SIZE] +
					matrix[x][(y - halfSide + CONTINENT_SIZE) % CONTINENT_SIZE];
				avg /= 4;
				//generate new "random" value
				srand(hash_int(seed_hashed % (hash_int(x) % (y + 1) + 1)) % 1001);
				avg = avg + (double(rand()) / 1000 * 2 * range) - range;
				matrix[x][y] = avg;

				if (x == 0)  matrix[CONTINENT_SIZE - 1][y] = avg;
				if (y == 0)  matrix[x][CONTINENT_SIZE - 1] = avg;
			}
		}
	}

	//mean calculation
	int m = 0;
	for (int i = 0; i < ((CONTINENT_SIZE*CONTINENT_SIZE)); i++){
		m += abs(matrix[i%CONTINENT_SIZE][i / CONTINENT_SIZE]);
	}
	m /= CONTINENT_SIZE;
	m /= CONTINENT_SIZE;
	mean += m;

	//calculate SD
	int sd=0;
	for (int i = 0; i < (CONTINENT_SIZE*CONTINENT_SIZE); i++){
		sd += abs(matrix[i%CONTINENT_SIZE][i / CONTINENT_SIZE]-m);
	}
	sd /= (CONTINENT_SIZE*CONTINENT_SIZE);
	SD += sd;
	//std::cout << "Finished building the continent" << std::endl;
	return matrix;
}

std::vector<std::vector<int>> genesis::generatePlanet(const std::string& seed, int &type){
	//std::cout << "Initialised hashs;" << std::endl;
	std::hash<std::string> hash_str;
	std::hash<int> hash_int;
	int seed_hashed = hash_str(seed);
	std::vector<std::vector<int>> continent(CONTINENT_SIZE, std::vector<int>(CONTINENT_SIZE, 0)), 
		planet(PLANET_W,std::vector<int>(PLANET_H,0));

	int count_continents;
	if (type < TYPE_GAS_SATURN){
		count_continents = ((hash_int(hash_str("Nb%of_Continents!") % seed_hashed)) % CONTINENT_VARIATION) + CONTINENT_BASENB;
	}
	else{
		count_continents = ((hash_int(hash_str("Nb%of_Continents!") % seed_hashed)) % CONTINENT_GIANT_VARIATION); 
		count_continents += CONTINENT_GIANT_BASENB;
	}
	std::cout << count_continents << std::endl;
	//count_continents = 1;
	int SD = 0, mean = 0;
	for (int i = 0; i < count_continents; i++){
		//std::cout << "Creating continent " + i << std::endl;
		srand(factorial(i)*i);
		continent = generateContinent(std::string(seed+"continent"+std::to_string(rand())), mean, SD);
		int map[CONTINENT_SIZE][CONTINENT_SIZE];
		for (int i = 0; i < CONTINENT_SIZE*CONTINENT_SIZE; i++){
			map[i%CONTINENT_SIZE][i / CONTINENT_SIZE] = continent[i%CONTINENT_SIZE][i / CONTINENT_SIZE];
		}
		srand(i*i);
		//intarray2bmp::intarray2bmp("c" + std::to_string(rand())+".bmp", &map[0][0], CONTINENT_SIZE, CONTINENT_SIZE, 1, 9);
		
		//compress the continent and places it in the map
		srand(factorial(i));
		int compression = (hash_int(hash_str("compression" + 'c' + std::to_string(rand())) % seed_hashed) % COMPRESSION_SETTING) + 1;
		srand(rand());
		int compression2 = (hash_int(hash_str("wCompress" + std::to_string(rand())) % seed_hashed) % COMPRESSION_SETTING) + 1;
		srand(pow(2, i));
		int h = (hash_int(hash_str("Hsmall:)"+std::to_string(rand())) % (seed_hashed + 1)) % (PLANET_H - CONTINENT_SIZE
			+((CONTINENT_SIZE/compression)/4)
			));
		h -= ((CONTINENT_SIZE / compression)/4);
		//srand(rand());
		srand(i);
		srand(rand()*compression2);
		/*int w = (hash_int(hash_str("Wwew" + std::to_string(rand())) % (seed_hashed + 1)) % (PLANET_H - CONTINENT_SIZE
			+ ((CONTINENT_SIZE / compression2) / 4)
			));
		w -= ((CONTINENT_SIZE / compression2) / 4);
		std::cout << compression2;*/
		
		int w;
		//compression = 1;

		if (i % 2 == 0){

			w = hash_int(hash_str("decal%x" + std::to_string(rand())) % (seed_hashed + 1)) % (PLANET_W - CONTINENT_SIZE
				+((CONTINENT_SIZE/compression2)/4));
			
		}
		else{
			srand(i*i*tan(i));
			w = hash_int(hash_str(std::string(seed + std::to_string(rand())) + "decal_impair_x!") % (seed_hashed + 1)) % (PLANET_W - (CONTINENT_SIZE)+(CONTINENT_SIZE/compression2)/4);
			w -= (CONTINENT_SIZE / compression2);
		}
		w -= (CONTINENT_SIZE / compression2)/4;
		//compression = 1;
		compression2 = compression;
		for (int x = 0; x < CONTINENT_SIZE/compression2; x++){
			for (int y = 0; y < CONTINENT_SIZE/compression; y++){
				int c = 0;
				for (int a = 0;a < compression2; a++){
					for (int b = 0; b < compression; b++){
						c += continent[x*compression2 + a][y*compression + b];
					}
				}
				int pos_x = x + w;
				int pos_y = y + h;
				//x += w; y += h;
				c /= compression; c /= compression2;
				//x += w; y += h;
				//c /= compression; c /= compression2;
				//mean /= compression; mean /= compression2;
				if (pos_x < 0){
					pos_x += PLANET_W;
				}
				if (pos_y< 0){
					pos_y += PLANET_H;
				}

				if (c>planet[pos_x][pos_y]){
					planet[pos_x][pos_y] = c;
				}
				
				
				/*if (planet[x + w][y + h] == 0){
					planet[x + w][y + h] = continent[x][y];
				}
				else{
					planet[x + w][y + h] = (continent[x][y] + planet[x][y]) / 2;
				}*/
			}
		}
		//static int m[PLANET_H][PLANET_W];
		//for (int x = 0; x < PLANET_W; x++){
		//	for (int y = 0; y < PLANET_H; y++){
		//		m[y][x] = planet[x][y];
		//	}
		//}
		//intarray2bmp::intarray2bmp("planet" + std::to_string(i) + ".bmp", &m[0][0], PLANET_H, PLANET_W, 0, 9);
		//continent.~vector();
	}if (count_continents == 0){
		mean = 1;
		SD = 1;
	}
	else { mean /= count_continents; SD /= count_continents; }
	
	for (int x = 0; x < PLANET_W; x++){
		for (int y = 0; y < PLANET_H; y++){
			//planet[x][y] = (10*planet[x][y]) / (mean) - SD;
			
			if (planet[x][y] < mean - SD){
				planet[x][y] = 1;
			}
			else if (planet[x][y] < mean){
				planet[x][y] = 2;
			}
			else if (planet[x][y] < mean + (SD / 2)){
				planet[x][y] = 3;
			}
			else if (planet[x][y] < mean + SD+SD){
				planet[x][y] = 4;
			}
			else if (planet[x][y] < mean + 2*SD+SD/2){
				planet[x][y] = 5;
			}
			else if (planet[x][y] < mean + 3*SD){
				planet[x][y] = 6;
			}
			else if (planet[x][y]<4*SD)
			{
				planet[x][y] = 7;
			}
			else if (planet[x][y] < 4 * SD){
				planet[x][y] = 8;
			}
			else{
				planet[x][y] = planet[x][y] / SD;
			}
		}
	}
	
	return planet;
}

std::vector<std::vector<int>> circularMask(const std::vector<std::vector<int>> &rought, const int &radius){
	int min = !(rought.size() < rought[0].size()) ? rought[0].size() : rought.size();
	std::vector<std::vector<int>> pure(min, std::vector<int>(min, 255));
	for (int x = 0; x < min; x++){
		for (int y = 0; y < min; y++){
			if (min % 2 == 1){
				if (sqrt(pow(min/2+1 - (x+1), 2) + pow(min/2+1 - (y+1), 2)) <= radius){
					pure[x][y] = rought[x][y];
				}
				else pure[x][y] = 0;
			}
			else{
				if ((sqrt(pow(min / 2 + 1 - (x+1), 2) + pow(min / 2 + 1 - (y+1), 2)) < radius) || sqrt(pow(min / 2 - (x+1), 2) + pow(min / 2 - (y+1), 2)) < radius){
					pure[x][y] = rought[x][y];
				}
				else pure[x][y] = 0;
			}
		}
	}
	return pure;
}


std::vector<genesis::star> genesis::generateSystem(const std::string& filepath,const std::string& seed){
	std::vector<genesis::star> system(0);
	std::hash<std::string> hash_str;
	std::hash<int> hash_int;
	int seed_hashed = hash_str(seed);

	int count_stars = 5; //(hash_int(hash_str("Nb%dof_Stars") % (seed_hashed + 1)) % 5) + 5;
	for (int star = 0; star < count_stars; star++){
		genesis::star current_star;
		srand(factorial(star));
		current_star.color = (hash_int(hash_str("TisStar%col_or" + std::to_string(rand())) % (seed_hashed + 1)) % 3);
		current_star.mass = double((hash_int(hash_str("Star no" + std::to_string(star) + std::to_string(rand()) + "mass") % (seed_hashed + 1)) % 80 )+15) / 10;
		if (star == 0){
			current_star.position = Vec2(0, 0);
		}
		else{
			srand((sin(star) + cos(star))*factorial(star));
			double teta = double(hash_int(hash_str("Angle_star" + std::to_string(rand())) % (seed_hashed + 1)) % 40000) / 100 / 180 * M_PI;
			srand(rand() + star);
			double dis = double(hash_int(hash_str("Star_dis" + std::to_string(rand())) % (seed_hashed+1)) % 900) / 100;
			dis /= 50000000;
			current_star.position=(Vec2(cos(teta)*dis, sin(teta)*dis));
			
		}
		std::cout << "Star "<<std::to_string(star+1)<<std::endl
			<<"Position : " << std::to_string(current_star.position.x()) << ", " << std::to_string(current_star.position.y()) << std::endl;

		//Calculate habitable zone
		double luminosity;
		if (current_star.mass > 2){
			 luminosity= pow(current_star.mass / 1, 3.5) * 1;
		}
		else{
			luminosity = pow(current_star.mass / 1, 1.5) * 1;
		}
		double radius_i = sqrt(luminosity / 1.1);
		radius_i *= M_PI / 648000;
		double radius_s = sqrt(luminosity / 0.53);
		
		radius_s *= M_PI / 648000;
		
		int count_planet = (hash_int(hash_str("Nb_of_planets_for_star_no" + std::to_string(star)) % (seed_hashed + 1)) % 13) + 2;
		for (int planet = 0; planet < count_planet; planet++){
			genesis::planet current_planet;
			srand(factorial(star));
			srand(rand()*planet);
			current_planet.mass = ((hash_int(hash_str("PlAnEt" + std::to_string(rand()) + "mass") % seed_hashed) % 35000) + 50) / 100;
			srand(rand()*star*planet);
			std::cout << "Planet mass :" << current_planet.mass << std::endl;
			double orbital_period = current_planet.mass  * ((hash_int(hash_str("rad" + std::to_string(rand())) % seed_hashed) % 200)/10 + 1) 
				// 10
				+0.5;
			std::cout << "Orbital period :" << orbital_period << std::endl;
			std::cout << "Current star mass :" << current_star.mass << std::endl;
			//double radius = pow(((M_G*current_star.mass*(((orbital_period*365.2563 * 24 * 60 * 60/1000)))*(orbital_period*365.2563 * 24 * 60 * 60)) / (4 * M_PI*M_PI)), 1 / 3);
			//double radius = 1;
			double radius = M_G*(
			current_star.mass + (current_planet.mass / 332946)//converting both masses to stellar mass
				)*
				(24 * 60 * 60 *
				orbital_period)//converting orbital period form days to s
				/ (M_PI*M_PI * 4);
			std::cout << "Radius :" << radius << std::endl;
			radius = pow(radius, 1 / 3);
			radius /= pow((double(3.085677581 * double(10 ^ 16))), 2);
			radius /= 1.8;
			//radius /= 1000;
			srand(planet);
			srand(rand() + star);
			std::cout << "Normal radius :" << radius << std::endl << "Inferior limit :" << radius_i << std::endl<<"Superior limit :"<<radius_s<<std::endl;
			double teta = double(hash_int(hash_str("TetaPlan" + std::to_string(rand())) % (seed_hashed + 1)) % 36000) / 100 / 180 * M_PI;
			current_planet.position = Vec2(cos(teta)*radius + current_star.position.x(), sin(teta)*radius + current_star.position.y());
			std::cout << "Planet " << std::to_string(planet + 1) << std::endl
				<< "Position :" << current_planet.position.x() << ", " << current_planet.position.y() << std::endl;
			int type;
			//srand(planet);
			//srand(rand()*star);
			//srand(factorial(planet)+planet*star);
			//sets planets types
			std::cout << "Star luminosity : " << luminosity;
			std::cout << current_planet.mass << std::endl;
			srand(planet);
			srand(rand() + (star*star));
			if (radius < radius_i){
				type = hash_int(hash_str("You'reTheHotType!" + std::to_string(rand())) % seed_hashed) % (TYPE_DESERT_MARS);
				
			}
			else if (radius < radius_s){
				type = hash_int(hash_str("Earthlike!" + std::to_string(rand()*radius)) % seed_hashed) % (TYPE_EARTHLIKE_MAGMA - TYPE_DESERT_MARS) + TYPE_DESERT_MARS+1;
			}
			else{
				if (current_planet.mass > 80){
					type = hash_int(hash_str("ColdType_notMyTypeMhe..." + std::to_string(rand())) % seed_hashed) % (TYPE_GAS_PALESHELTER - TYPE_EARTHLIKE_MAGMA ) + TYPE_EARTHLIKE_MAGMA+1;
				}
				else{
					type = TYPE_ICE_ICE_BABY;
				}
			}
			current_planet.type = type;
			srand(type);
			srand(rand()*star);
			srand(rand()*planet);
			std::vector<std::vector<int>> wew =circularMask( genesis::generatePlanet(seed + std::to_string(rand()), type),((2*PLANET_H)+1)/4);
			static int image[PLANET_H][PLANET_H];
			for (int x = 0; x < wew.size(); x++){
				for (int y = 0; y < wew.size(); y++){
					//Converting to bmp is reading values like in a normal value, so we must reverse the axis to keep the right size
					image[y][x] = wew[x][y];
				}
			}
			intarray2bmp::intarray2bmp(std::string("saves"+filepath+"Star"+std::to_string(star)+"Planet" + std::to_string(planet) + ".bmp"), &image[0][0], wew.size(), wew.size(), 0, 8, type);
			boost::filesystem::path dir(std::string("saves/" + filepath).c_str());
			if (boost::filesystem::create_directory(dir)){
				std::cout << "Dir " << "saves/" + filepath + "was created" << std::endl;
			}

			//Magick::Image master("saves" + filepath + "Star" + std::to_string(star) + "Planet" + std::to_string(planet) + ".bmp");
			//master.write("saves/" + filepath + "/star" + std::to_string(star) + "planet" + std::to_string(planet) + ".png");
			
			if (1!=rename(std::string("saves" + filepath + "Star" + std::to_string(star) + "Planet" + std::to_string(planet) + ".bmp").c_str(), std::string("saves/" + filepath + "/star" + std::to_string(star) + "planet" + std::to_string(planet) + ".bmp").c_str())){
				std::cout << "File was renamed" << std::endl;
				std::cout << "saves/" + filepath + "/star" + std::to_string(star) + "planet" + std::to_string(planet) + ".bmp" << std::endl;
				boost::filesystem::remove(std::string(std::string("saves" + filepath + "Star" + std::to_string(star) + "Planet" + std::to_string(planet) + ".bmp").c_str()));
			}
			else std::cout << "ERROR" << std::endl;
			
			current_star.planets.push_back(current_planet);
		}
		std::cout << std::endl;
		
		system.push_back(current_star);
	}
	return system;
}


std::vector<std::shared_ptr<GameObject>> genesis::generateObjects(Scene *scene,const std::string& filepath, const std::string& seed){
	//GameObject pat_star;
	//pat_star.addComponent(idn(PositionComponent), std::make_shared<PositionComponent>());
	//pat_star.addComponent(idn(GraphicsComponent), std::make_shared<GraphicsComponent>());
	//pat_star.addComponent(idn(PhysicsComponent), std::make_shared<PhysicsComponent>(PhysicsComponent(pat_star.get<PositionComponent>())));
	std::map<std::string, std::shared_ptr<GameObject>> rought_objs;
	std::vector<std::shared_ptr<GameObject>> pure_objs;
	luain::loadFromRep(rought_objs, scene,"scripts");
	std::hash<std::string> hash_str;
	std::hash<int> hash_int;
	int seed_hashed = hash_str(seed);
	std::vector<std::shared_ptr<GameObject>> planets;

	//std::map<std::string, std::shared_ptr<GameObject>> map;
	if (rought_objs.find("planet") != rought_objs.end()  ){
#undef phys
#define phys(o) o.get<PhysicsComponent>()
		GameObject planet = *rought_objs.at("planet").get();
		GameObject st = planet;
		std::vector<GameObject> AIs;
		for (std::pair<std::string,std::shared_ptr<GameObject>> ai : rought_objs){
			if (ai.second->hasComponent(idn(AiComponent)) && ai.second->hasComponent(idn(PhysicsComponent)) && ai.second->hasComponent(idn(PositionComponent))&& ai.second->getType()!="yog_sogoth"){
				AIs.push_back(*ai.second);
			}
		}
		std::cout << AIs.size() << std::endl;
		planet.get<PositionComponent>()->fix(1);
		planet.setType("planet");
		phys(planet)->setVelocity(Vec2(0, 0));
		st.setType("star");
		st.get<PositionComponent>()->fix(1);
		phys(st)->setVelocity(Vec2(0, 0));
		int i = 0;
		std::vector<genesis::star> stars = genesis::generateSystem(filepath, seed);
		for (int current_star = 1; current_star < stars.size(); current_star++){
			std::cout << "*" << std::endl;
			GameObject obj_star = st;
			obj_star.setID("planet" + std::to_string(i));
			
			if (obj_star.hasComponent(typeid(PhysicsComponent)) && obj_star.hasComponent(typeid(PositionComponent))){
//#define PC2PX(p) Vec2(p.x()*(2/3)*pow(10,12), p.y()*(2/3)*pow(10,12))

				//phys(obj_star)->setPosition((current_star.position));
				obj_star.get<PositionComponent>()->setPosition(Vec2(stars[current_star].position.x()*2/3*pow(10,12),stars[current_star].position.y()*2/3*pow(10,12)));
				std::cout << phys(obj_star)->getPosition().x()<<std::endl;
				std::cout << phys(obj_star)->getPosition().y() << std::endl;
				std::cout << stars[current_star].position.x()<<std::endl;
				std::cout << stars[current_star].position.y() << std::endl;
				phys(obj_star)->setMass(stars[current_star].mass * 333000/500);
				phys(obj_star)->setHitboxRadius(pow(0.07957747154*phys(obj_star)->getMass(), 1 / 3) / 100);
				obj_star.get<GraphicsComponent>()->setSize(Vec2(phys(obj_star)->getHitboxRadius()*2, phys(obj_star)->getHitboxRadius()*2));
				obj_star.get<PositionComponent>()->fix(1);
				pure_objs.push_back(std::make_shared<GameObject>(obj_star));
				i++;
				int j = 0;
				for (genesis::planet current_planet : stars[current_star].planets){
					GameObject obj_planet = planet;
					obj_planet.setID("planet" + std::to_string((i+1)*stars.size()+j));
					
					if (obj_planet.hasComponent(typeid(PhysicsComponent)) && obj_planet.hasComponent(typeid(PositionComponent))){
						//Converts solar mass to earth mass
						phys(obj_planet)->setMass(current_planet.mass/500);
						//obj_planet.get<PositionComponent>()->getPosition.
						//phys(obj_planet)->setPosition((current_planet.position));
						std::cout << obj_star.get<PositionComponent>()->getPosition().x() << " , " << obj_star.get<PositionComponent>()->getPosition().y() << std::endl;
						std::cout << current_planet.position.x()*pow(10, 8) << ", " << current_planet.position.y()*pow(10, 8) << std::endl;
						obj_planet.get<PositionComponent>()->setPosition((Vec2(obj_star.get<PositionComponent>()->getPosition().x()
							+current_planet.position.x()*10e7
							//*2/3*1000000
							, obj_star.get<PositionComponent>()->getPosition().y()
							+current_planet.position.y()*10e7
							//*2/3*1000000
							)));
						obj_planet.get<PositionComponent>()->fix(1);
						//phys(obj_planet)->setPosition(Vec2(current_planet.position.x(), current_planet.position.y()));
						obj_planet.get<GraphicsComponent>()->setSprite(std::make_shared<Sprite>(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture(
							"saves/" + filepath + "/star" + std::to_string(i) + "planet" + std::to_string(j) + ".bmp", 
							//"star0planet0.png",
							1, 1), "saves/" + filepath + "/star" + std::to_string(i) + "planet" + std::to_string(j) + ".bmp")));
						
						phys(obj_planet)->setHitboxRadius(pow(0.04774648292*phys(obj_planet)->getMass(), 1 / 3)/1000);
						obj_planet.get<GraphicsComponent>()->setSize(Vec2(phys(obj_planet)->getHitboxRadius()*2, phys(obj_planet)->getHitboxRadius()*2));

						//"saves/" + filepath + "/star" + std::to_string(star) + "planet" + std::to_string(planet) + ".bmp"
						std::cout << obj_planet.get<GraphicsComponent>()->getFilename() << std::endl;
						//pure_objs.insert(std::make_pair("planet_" + std::to_string(i) + "_" + std::to_string(j), std::make_shared<GameObject>(obj_planet)));
						std::shared_ptr<GameObject> pRef = std::make_shared<GameObject>(obj_planet);
						pure_objs.push_back(pRef);
						planets.push_back(pRef);
						
						

						if (AIs.end() != AIs.begin()){
							//std::cout << "AI was loaded" << std::endl<<AIs.size();
							//check if planet is an habitable world
							if (
								(current_planet.type > TYPE_EARTHLIKE_FOREST)
								){
								std::cout << "Planet is habitable" << std::endl;
								srand(rand() + i);
								srand(rand() + j);
								int nem_count = (hash_str("Nb of enemys" + std::to_string(rand()) + std::to_string(i) + std::to_string(j))%seed_hashed)%15;
								for (int nem = 0; nem < nem_count; nem++){
									srand(nem);
									int nb = (hash_str("this nem is" + std::to_string(rand())) % seed_hashed) % AIs.size();
									//srand(rand() + current_planet.type);
									double teta = (hash_str("Teta" + std::to_string(rand())) % seed_hashed + 1) % 36000; teta /= 100;
									int dis = sqrt(M_G*current_planet.mass*5.972e24*phys(AIs[nb])->getMass() / 2500);
									dis /= 10000;
									std::cout << dis << std::endl;
									//dis *= (pow(3.24077929, -17));
									GameObject currentAI = AIs[nb];
									currentAI.get<PositionComponent>()->setPosition(Vec2(cos(teta)*dis/100 + obj_planet.get<PositionComponent>()->getPosition().x(), sin(teta)*dis/100 + obj_planet.get<PositionComponent>()->getPosition().y()));
									pure_objs.push_back(std::make_shared<GameObject>(currentAI));
									currentAI.setID("AIP" + std::to_string(j) + "N" + std::to_string(nem));
									std::cout << "Pushed " << STR(currentAI) << std::endl;
									int w;
									std::cout << "Ais sprite :"<<AIs[0].get<GraphicsComponent>()->getFilename();
									int p;
								}
							}
						}
					}
					j++;
				}
				

//#undef PC2PX
			}
			else std::cout << "Missing components!" << std::endl;
		}
		
	}

	//Sets id of a planet so that the objectives can point to it...
	
	if (planets.size() > 0){
		int pos = hash_int(hash_str("PositionOfPlanetOmega") % (seed_hashed + 1)) % planets.size();
		planets[pos]->setID("planet_Omega");
	}
	

	if (rought_objs.find("player") != rought_objs.end()){
		pure_objs.push_back(rought_objs["player"]);
	}
	
	for (std::string hc : {"base", "objective_1", "objective_2", "objective_3", "hud_base_pointer", "hud_fuel", "hud_fuel_container", "hud_life", "hud_life_container", "hud_obj_pointer","yog_sogoth" }){
		if (rought_objs.find(hc) != rought_objs.end()){
			rought_objs.at(hc)->setID(hc);
			pure_objs.push_back((rought_objs.at(hc)));
		}
	}

	
	XML_u::saveObjects(scene,pure_objs, "saves/"+filepath+".xml");
#undef phys
	return pure_objs;
}



/*
int main(int argc, char* argv[]){
	
	auto objects = genesis::generateObjects("save1","Test");
	std::cout << "saves/Test/";
	
	/*
	std::cout << "Main" << std::endl;
	std::string seed= "Marc-Antoine";
	for (int type = 0; type < TYPE_GAS_PALESHELTER; type++)
	{
		std::vector<std::vector<int>> wew = genesis::generatePlanet(seed+std::to_string(type),type);
		static int planet[PLANET_H][PLANET_W];
		for (int x = 0; x < PLANET_W; x++){
			for (int y = 0; y < PLANET_H; y++){
				planet[y][x] = wew[x][y];
			}
		}
		intarray2bmp::intarray2bmp(seed +std::to_string(type) +".bmp", &planet[0][0], PLANET_H, PLANET_W, 0, 9, type);

	}
	
	genesis::generateSystem();
	system("pause");
	return 1;
}*/

#include<ctime>
//generates a random string
std::string genesis::generateString(){
	std::string s = "";
	char a[] = "1!2@3#4$5%6^7&8*9(0)-_=+qQwWeErRtTyYuUiIoOpP[{]}aAsSdDfFgGhHjJkKlL;:'\"zZxXcCvVbBnNmM,<.>/?";
	srand(time(0));
	int r = rand() % (20) + 2;
	for (int i = 0; i < r - 1; i++){
		s = s + a[rand() % (sizeof(a) - 1)];
	}
	return s;
}