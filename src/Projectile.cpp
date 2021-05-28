#include "Projectile.h"

Projectile::Projectile(const json& attributes, TextureManager& textureManager)
{
	setTexture(textureManager.getTexture(attributes["name"]));
	setDamage(attributes["damage"]);
	setSpeed(attributes["speed"]);
}

float Projectile::getDamage() const
{
	return m_damage;
}

float Projectile::getSpeed() const
{
	return m_speed;
}

const sf::Vector2f& Projectile::getVelocity() const
{
	return m_velocity;
}

void Projectile::setDamage(float damage)
{
	m_damage = damage;
}

void Projectile::setSpeed(float speed)
{
	m_speed = speed;
}

void Projectile::setVelocity(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

bool Projectile::isInWindow(const sf::Window& window)
{
	auto& pos = getPosition();
	auto size = window.getSize();
	return pos.x >= 0 && pos.y >= 0 && pos.x <= size.x && pos.y <= size.y;
}

