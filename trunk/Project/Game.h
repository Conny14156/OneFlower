#ifndef Game_H
#define Game_H
#include <vector>
#include <string>
#include <map>
class GameObject;
class Tile;
class Message;
class Game
{

public:
	//Get a pointer towards a GameObject from the first match via its name
	GameObject* requestGameObject(std::string);
	//Get a pointer towards a GameObject via a its IdentityCode
	GameObject* requestGameObject(unsigned int ID);
	//Get a pointer towards a GameObject via a copy pointer 
	GameObject* requestGameObject(const GameObject* gameObjectPointer );

	//This one work
	void addGameObject(GameObject* entity);

	void addGameObject(GameObject entity);
	//This one doesnt work
	void addGameObject(std::string);

	//Return a const pointer of all the gameobject vector
	const std::vector<GameObject*>* requestAllGameObjecVector() const;

	void requestRemoveal(GameObject* entity);
	void Update();

	void addBackground(Tile* t);
	void addForeground(Tile* t);
	void requestRemovealForeground(Tile* t);

//private:
	std::vector<GameObject*> allGameObjectPointers;
	std::map<GameObject*,int> mapOfGameObjects;

};
//ReadOnly, Returns a const pointer towards game
const Game* RequestGame();

//Returns a pointer value towards game
Game* SetGame();

#endif