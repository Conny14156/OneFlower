#ifndef RenderHandler_H
#define RenderHandler_H
#include<SFML\Graphics\Texture.hpp>
#include <map>
class Gfx
{
public:

	//Load a picture file into a Texture and not a sprite
	bool loadTexture(std::string locatonOfTheTexture);

	//Return a pointer towards
	const sf::Texture* requestTexture(std::string name) const;
	
	//request removal of texture if its not in use
	void requestRemovalOfTexture(std::string name);

private:
	//Map with all texture, key value is the name of the texture
	std::map<std::string, sf::Texture> loadedTextureMap;

};
//ReadOnly
const Gfx* RequestGfx();
//Returns a pointer value towards 
Gfx* SetGfx();

#endif