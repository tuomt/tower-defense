#pragma once
#include "Widget.h"
#include "Gui.h"

class TextBox : public Widget
{
public:
	TextBox() {};
	TextBox(sf::Sprite background);
	sf::Text m_text;

	// Inherited via Widget
	virtual void setPosition(sf::Vector2f pos) override;
	virtual const sf::Vector2f& getPosition() const override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};