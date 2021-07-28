#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager
{
public:
	static TextureManager& getInstance() {
		static TextureManager instance;
		return instance;
	}

private:
	TextureManager() {}

	const std::string m_path = "../img/";
	std::map<std::string, sf::Texture> m_textures;
public:
	// Prevent making a copy of the singleton
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

	sf::Texture& getTexture(const std::string& name);
	void loadTexture(const std::string& name, const std::string& fileName);
};



