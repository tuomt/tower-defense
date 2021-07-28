#pragma once
#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Projectile : public sf::Sprite
{
private:
	float m_damage = 0;
	float m_speed = 0.f;
	sf::Vector2f m_velocity = sf::Vector2f(0.f, 0.f);
public:
	Projectile(const json& attributes);

	float getDamage() const;
	float getSpeed() const;
	const sf::Vector2f& getVelocity() const;

	void setDamage(float damage);
	void setSpeed(float speed);
	void setVelocity(const sf::Vector2f& velocity);

	bool isInWindow(const sf::Window& window);
};

