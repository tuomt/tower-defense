#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

class Waypoint : public Component
{
private:
	sf::Sprite m_sprite;
public:
	Waypoint(const sf::Texture& texture);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const sf::Vector2f& getPosition() const override;
	const sf::Vector2f& getOrigin() const override;
	const sf::Vector2f getSize() const;
	float getRotation() const override;

	void setPosition(const sf::Vector2f& position) override;
	void setOrigin(const sf::Vector2f& origin) override;
	void rotate(float angle) override;
private:
};

