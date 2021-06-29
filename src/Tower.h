#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "TextureManager.h"
#include "nlohmann/json.hpp"
#include <list>
#include "Armor.h"

using json = nlohmann::json;

class Tower : public sf::Sprite
{
private:
	const json& m_attributes;
	TextureManager& m_textureManager;
	float m_range = 100.f;
	float m_traverse = 360.f;
	float m_baseDirection = 0.f;
	float m_fireRate = 30.f;
	float m_reloadTime = 0.f;
	float m_secondsFromLastShot = 0.f;
	sf::Vector2f m_muzzlePosition = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_target = sf::Vector2f(0.f, 0.f);
	std::list<Projectile> m_projectiles;
public:
	Tower(const json& attributes, TextureManager& textureManager);

	float getRange() const;
	float getTraverse() const;
	float getBaseDirection() const;
	float getFireRate() const;
	float getReloadTime() const;
	const sf::Vector2f getMuzzlePosition(bool transform = true) const;
	std::list<Projectile>& getProjectiles();
	bool isReloading() const;
	bool isInFOV(const Armor& armor) const;

	void aim(const sf::Vector2f& target);
	void setRange(float range);
	void setTraverse(float traverse);
	void setBaseDirection(float baseDir);
	void setFireRate(float fireRate);
	void setReloadTime(float reloadTime);
	void setMuzzlePosition(const sf::Vector2f& position);
	void reload(float dt);
	void fire();
};
