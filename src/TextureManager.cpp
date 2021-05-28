#include "TextureManager.h"

sf::Texture& TextureManager::getTexture(const std::string& name)
{
	return m_textures.at(name);
}

void TextureManager::loadTexture(const std::string& name, const std::string& fileName)
{
	// Load a texture from a file
	sf::Texture texture;
	if (!texture.loadFromFile(m_path + fileName))
		exit(-1);
	// Insert the texture into the map
	m_textures[name] = texture;
}
