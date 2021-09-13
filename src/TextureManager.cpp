#include <assert.h>
#include "TextureManager.h"
#include "SceneManager.h"

sf::Texture& TextureManager::getTexture(const std::string& name)
{
	auto it = m_textures.find(name);
	assert(it != m_textures.end());
	return it->second.first;
}

void TextureManager::loadTexture(const std::string& name, const std::string& fileName, Scene* scene)
{
	// Load a texture from a file
	sf::Texture texture;
	assert(texture.loadFromFile(m_path + fileName));

	// Insert the texture into the map
	printf("Loading texture: %s\n", name.c_str());
	m_textures[name] = std::make_pair(texture, scene);
}

void TextureManager::unloadTexture(const std::string& name)
{
	// Unload texture by it's name
	auto it = m_textures.find(name);
	assert(it != m_textures.end());
	printf("Unloading texture: %s\n", it->first.c_str());
	m_textures.erase(it);
}

void TextureManager::unloadCommonTextures()
{
	if (m_textures.empty()) return;

	// Unload textures that are used in multiple scenes
	for (auto it = m_textures.cbegin(); it != m_textures.cend();) {
		Scene* relatedScene = it->second.second;
		if (relatedScene == nullptr) {
			printf("Unloading texture: %s\n", it->first.c_str());
			m_textures.erase(it++);
		}
		else {
			++it;
		}
	}
}

void TextureManager::unloadSceneSpecificTextures(Scene* scene)
{
	if (m_textures.empty()) return;

	// Unload textures that are used in the scene
	for (auto it = m_textures.cbegin(); it != m_textures.cend();) {
		Scene* relatedScene = it->second.second;
		if (relatedScene == scene) {
			printf("Unloading texture: %s\n", it->first.c_str());
			m_textures.erase(it++);
		}
		else {
			++it;
		}
	}
}
