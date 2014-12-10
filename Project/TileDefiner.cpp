#include "Tile.h"
#include "Gfx.h"
#include <string>

/*
Tile::Tile()
{
	name = "test.png";
	sprite.setTexture(*SetGfx()->requestTexture("test.png"));
	//sprite.setOrigin(sprite.getTextureRect().width / 2,sprite.getTextureRect().height / 2);
	sizeX = 0;
	sizeY = 0;
}
//*/
Tile::Tile(std::string n, Vector2<int> pos)
{
	name = n;
	sprite.setTexture(*SetGfx()->requestTexture(n));
	//sprite.setOrigin(sprite.getTextureRect().width / 2,sprite.getTextureRect().height / 2);
	position = pos;
	sprite.setPosition(position.x,position.y);
	sizeX = 0;
	sizeY = 0;
}
Tile::Tile(std::string n, int x, int y)
{
	name = n;
	sprite.setTexture(*SetGfx()->requestTexture(n));
	//sprite.setOrigin(sprite.getTextureRect().width / 2,sprite.getTextureRect().height / 2);
	position.x = x;
	position.y = y;
	sprite.setPosition(position.x, position.y);

	sizeX = 0;
	sizeY = 0;
}
Tile::Tile(const Tile &tile)
{
	name = tile.name;
	sprite.setTexture(*SetGfx()->requestTexture(name));
	position.x = tile.position.x;
	position.y = tile.position.y;
	sprite.setPosition(position.x, position.y);

	sizeX = tile.sizeX;
	sizeY = tile.sizeY;
}
Tile& Tile::operator=(const Tile &tile)
{
	name = tile.name;
	sprite.setTexture(*SetGfx()->requestTexture(name));
	//sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	position.x = tile.position.x;
	position.y = tile.position.y;
	sprite.setPosition(position.x, position.y);

	sizeX = tile.sizeX;
	sizeY = tile.sizeY;
	return *this;
}
void Tile::setRepeated(bool b)
{
	sf::Texture &tmp = *SetGfx()->requestTexture(name);
	tmp.setRepeated(true);
}