#include "WorldManagement.hpp"
#include "Zone.hpp"
#include <string>
#include <fstream>
#include "LoadAndSave.hpp"
#include "Engine.hpp"
#include "Component\RenderComponent.h"
#include "Component\GameObject.h"
#include "Gfx.h"


// load zone with ID
void WorldManagement::loadZone(unsigned int zoneID)
{
	if(worldmap.find(zoneID) != worldmap.end())
	{
		std::cout << std::endl << "zone with ID: [" << zoneID << "] is already loaded" << std::endl << "continues to load zone..." << std::endl;
		// load the Zone with the zone id
		// loadZoneFromMap(zoneID);
		//worldFromZone(zoneID);
		
	}
	else
	{
		zone = new Zone();
		loadZoneFile(zoneInfo.find(zoneID)->second,*zone);
		worldmap.insert(std::pair<unsigned int, Zone*>(zone->getID(),zone));
		worldFromZone(zoneID);
	}
}
// default constructor
WorldManagement::WorldManagement() {
	
	loadZoneInfo(zoneInfo);
	lastLoadedZone = 0;
	
}
// deconstructor
WorldManagement::~WorldManagement() {
	/*
	for (int i = 0; i < zone->objects.size(); i++) {
		delete zone->objects[i];
		zone->objects[i] = nullptr;
	}
	//*/
	//rmeove the last loaded zone
	for (int i = 0; i < worldmap[lastLoadedZone]->objects.size(); i++) {
		// request removal of GameObjects /to fix 
		Engine::game.requestRemoveal(worldmap[lastLoadedZone]->objects[i]);
		worldmap[lastLoadedZone]->objects[i] = nullptr;
	}
	delete zone;
	zone = nullptr;
}
// load a zone from the world, removes the old one
void WorldManagement::worldFromZone(unsigned int zoneID) 
{
	SetGfx()->camera.reset(sf::FloatRect(0, 0, 800, 600));
	for (int i = 0; i < worldmap[zoneID]->objects.size(); i++)
	{
		Engine::game.addGameObject(worldmap[zoneID]->objects[i]);
	}
	//add background;
	Engine::game.addBackground(&worldmap[zoneID]->getBackground());
	//add foregrounds;
	for (int i = 0; i < worldmap[zoneID]->foregrounds.size(); i++)
	{
		Engine::game.addForeground(&worldmap[zoneID]->foregrounds[i]);
	}
	// when loading first zone
	if (lastLoadedZone == 0)
		lastLoadedZone = zoneID;
	else 
	{
		for (int i = 0; i < worldmap[lastLoadedZone]->foregrounds.size(); i++) {
			Engine::game.requestRemovealForeground(&worldmap[lastLoadedZone]->foregrounds[i]);
		}
		for (int i = 0; i < worldmap[lastLoadedZone]->objects.size(); i++) {
			// request removal of GameObjects /to fix 
			Engine::game.requestRemoveal(worldmap[lastLoadedZone]->objects[i]);
			worldmap[lastLoadedZone]->objects[i] = nullptr;
		}
		delete worldmap[lastLoadedZone];
		worldmap[lastLoadedZone] = nullptr;
		worldmap.erase(lastLoadedZone);
		lastLoadedZone = zoneID;
	}
}