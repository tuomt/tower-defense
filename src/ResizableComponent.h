#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

class ResizableComponent : public Component
{
private:
	sf::Vector2f m_size;
	sf::Sprite m_sprite;
	sf::Sprite m_knob;
	sf::Vector2f getKnobOffset();
	void updateSize();
public:
	ResizableComponent(const sf::Texture& texture, const sf::Texture& knobTexture);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::FloatRect getGlobalBounds() const;
	const sf::Vector2f& getPosition() const override;
	const sf::Vector2f& getOrigin() const override;
	const sf::Vector2f& getSize() const;
	float getRotation() const override;

	const sf::Sprite& getKnob() const;

	void setPosition(const sf::Vector2f& position) override;
	void setOrigin(const sf::Vector2f& origin) override;
	void setSize(const sf::Vector2f& size);
	void rotate(float angle) override;
	void rotateToPoint(const sf::Vector2f& point);
	void resize(const sf::Vector2f& mousePosition);
};

