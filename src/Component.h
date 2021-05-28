#pragma once
#include <SFML/Graphics.hpp>

class Component : public sf::Drawable
{
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual const sf::Vector2f& getPosition() const = 0;
	virtual const sf::Vector2f& getOrigin() const = 0;
	virtual float getRotation() const = 0;

	virtual void setPosition(const sf::Vector2f& position) = 0;
	virtual void setOrigin(const sf::Vector2f& origin) = 0;
	virtual void rotate(float angle) = 0;
};

