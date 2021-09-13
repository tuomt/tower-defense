#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Scene.h"

class TextureManager
{
public:
	static TextureManager& getInstance() {
		static TextureManager instance;
		return instance;
	}

private:
	TextureManager() {};

	const std::string m_path = "../img/";
	std::map<std::string, std::pair<sf::Texture, Scene*>> m_textures;
public:
	// Prevent making a copy of the singleton
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

	sf::Texture& getTexture(const std::string& name);
	void loadTexture(const std::string& name, const std::string& fileName, Scene* scene);
	void unloadTexture(const std::string& name);
	void unloadCommonTextures();
	void unloadSceneSpecificTextures(Scene* scene);
};
