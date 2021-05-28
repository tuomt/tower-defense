#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager
{
private:
	const std::string m_path = "../img/";
	std::map<std::string, sf::Texture> m_textures;
public:
	sf::Texture& getTexture(const std::string& name);
	void loadTexture(const std::string& name, const std::string& fileName);
};

