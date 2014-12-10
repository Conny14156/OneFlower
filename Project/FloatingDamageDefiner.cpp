#include "FloatingDamage.hpp"
#include <iostream>
FloatingDamage::FloatingDamage(std::string iconName, sf::Font& font, double duration, int len, int offsetX, int offsetY) : FloatingText(iconName, font, duration, len, offsetX, offsetY)
{

}
FloatingDamage::FloatingDamage(FloatingDamage& fdmg) : FloatingText(fdmg.iconName,fdmg.font,fdmg.duration,fdmg.maxLength,fdmg.offsetX,fdmg.offsetY)
{
	setColor(fdmg.color);
}
FloatingDamage::~FloatingDamage()
{

}

FloatingDamage& FloatingDamage::operator=(int damageDone)
{
	if (damageDone < 0)
		text.setString("Miss");
	else
		text.setString(std::to_string(damageDone));

	return *this;
}