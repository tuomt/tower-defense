#pragma once
#include "Widget.h"

class Button : public Widget
{
public:
	Button();
	Button(sf::Sprite background, sf::Text text);

	sf::Text m_text;

	void centerText();

	// Inherited via Widget
	virtual void setPosition(sf::Vector2f pos) override;
	virtual const sf::Vector2f& getPosition() const override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};