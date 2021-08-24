#include "TextBox.h"

TextBox::TextBox()
{
	setNativeEventConditions();
	setNativeEventActions();
}

TextBox::TextBox(sf::Sprite background)
	: Widget(background)
{
	setNativeEventConditions();
	setNativeEventActions();
}

void TextBox::setActive(bool active)
{
	m_active = active;
}

bool TextBox::getActive() const
{
	return m_active;
}

void TextBox::setNativeEventActions()
{
	onClick.setAction([&](sf::Event event) {
		setActive(true);
	});

	onClick.setAltAction([&](sf::Event event) {
		setActive(false);
	});

	onTextEntered.setAction([&](sf::Event event) {
		m_text.setString(m_text.getString() + event.text.unicode);
	});
}

void TextBox::setNativeEventConditions()
{
	onTextEntered.setCondition([&](sf::Event event) { return m_active; });
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
