#ifndef ZoneMap_HPP
#define ZoneMap_HPP
#include <string>
#include <vector>
#include "../Tile.h"
//#include "Component\BaseComponent.hpp"
#include "../Component/GameObject.h"
class ZoneMap
{
public:
	ZoneMap();

	std::string name;
	unsigned int ID;
	Tile background;

	std::vector<GameObject*> objects;
	std::vector<Tile> foregrounds;

	void setBackground(Tile &t);
	void addGameObject(GameObject *go);
	void readForegroundsVector(std::vector<Tile> tiles);
	void setID(unsigned int ID);
private:
	template<class Archive>
	friend void save(Archive& ar,const ZoneMap & zm);
	template<class Archive>
	friend void load(Archive& ar,ZoneMap & zm);
};

#endif