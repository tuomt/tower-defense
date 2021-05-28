#include "Waypoint.h"

Waypoint::Waypoint(const sf::Texture& texture)
{
	m_sprite.setTexture(texture);
	m_sprite.setScale(0.5f, 0.5f);
}

void Waypoint::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

const sf::Vector2f& Waypoint::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::Vector2f& Waypoint::getOrigin() const
{
	return m_sprite.getOrigin();
}

const sf::Vector2f Waypoint::getSize() const
{
	auto& rect = m_sprite.getTextureRect();
	return sf::Vector2f(rect.width, rect.height);
}

float Waypoint::getRotation() const
{
	return m_sprite.getRotation();
}

void Waypoint::setPosition(const sf::Vector2f& position)
{
	m_sprite.setPosition(position);
}

void Waypoint::setOrigin(const sf::Vector2f& origin)
{
	m_sprite.setOrigin(origin);
}

void Waypoint::rotate(float angle)
{
}
