#include "Button.h"
#include <cstdio>

Button::Button()
{
}

Button::Button(sf::Sprite background, sf::Text text) 
	: Widget(background),
	m_text(text)
{
}

void Button::centerText()
{
	auto bgRect = m_background.getLocalBounds();
	auto textRect = m_text.getLocalBounds();

	float leftoverWidth = bgRect.width - textRect.width;
	float leftoverHeight = bgRect.height - textRect.height;

	sf::Vector2f pos = m_text.getPosition();
	pos.x += leftoverWidth / 2.f;
	pos.y += leftoverHeight / 2.f;
	m_text.move(leftoverWidth / 2.f - textRect.left, leftoverHeight / 2.f - textRect.top);
}

void Button::setPosition(sf::Vector2f pos)
{
	auto textOffset = m_text.getPosition() - m_background.getPosition();
	m_background.setPosition(pos);
	m_text.setPosition(pos + textOffset);
}

const sf::Vector2f& Button::getPosition() const
{
	return m_background.getPosition();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_text);
}
