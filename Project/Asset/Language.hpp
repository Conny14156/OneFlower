#ifndef Language_HPP
#define Language_HPP
#include <Asset/IRequestable.hpp>
#include <Asset/Requestor.hpp>
#include <Asset/PrimitiveSaveable.hpp>

#include <Core/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class Language : public IRequestable
{
	Core::String fallbackLanguage;

	Core::String languageFile;
	sf::Font m_font;
	void loadFont(const Core::String& name);
	Requestor<PrimitiveSaveable<Core::String>> stringList;

public:

	Language(Core::String fontName);
	sf::Font& getFont();

	Core::String getString(size_t id);
	sf::Text getText(size_t id, size_t charSize);

};

#endif