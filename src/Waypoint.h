#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "PathLine.h"

class PathLine;

class Waypoint : public Component
{
private:
	sf::Sprite m_sprite;
public:
	std::shared_ptr<PathLine> m_headLine = nullptr;
	std::shared_ptr<PathLine> m_tailLine = nullptr;

	Waypoint(const sf::Texture& texture);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const sf::Vector2f& getPosition() const override;
	const sf::Vector2f& getOrigin() const override;
	const sf::Vector2f getSize() const;
	float getRotation() const override;

	sf::FloatRect getGlobalBounds() const;
	sf::FloatRect getLocalBounds() const;

	void setPosition(const sf::Vector2f& position) override;
	void setOrigin(const sf::Vector2f& origin) override;
	void rotate(float angle) override;
private:
};

