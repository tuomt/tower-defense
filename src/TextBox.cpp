#include "TextBox.h"

TextBox::TextBox(sf::Sprite background)
	: Widget(background)
{
}

void TextBox::setPosition(sf::Vector2f pos)
{
	auto textOffset = m_text.getPosition() - m_background.getPosition();
	m_background.setPosition(pos);
	m_text.setPosition(pos + textOffset);
}

const sf::Vector2f& TextBox::getPosition() const
{
	return m_background.getPosition();
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_text);
}
