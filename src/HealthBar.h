#pragma once
#include <SFML/Graphics.hpp>

class HealthBar : public sf::Drawable
{
private:
	sf::Vector2f m_size;
	float m_remainingWidth;
	sf::VertexArray m_fullBar = sf::VertexArray(sf::Triangles, 6);
	sf::VertexArray m_remainingBar = sf::VertexArray(sf::Triangles, 6);
public:
	HealthBar(const sf::Vector2f& size);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setRemaining(float percentage);
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSize() const;
};

