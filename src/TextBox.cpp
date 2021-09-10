#include "TextBox.h"

TextBox::TextBox(const sf::Texture& bgTexture, const sf::Font& font)
	: Widget(sf::Sprite(bgTexture)), m_text("", font, 12)
{
	initialize();
	setNativeEventConditions();
	setNativeEventActions();
}

void TextBox::centerText()
{
	auto bgRect = m_background.getGlobalBounds();
	auto textRect = m_textSprite.getLocalBounds();

	sf::Vector2f offset;
	offset.x = (bgRect.width - textRect.width) / 2.f;
	offset.y = (bgRect.height - textRect.height) / 2.f;

	sf::Vector2f pos = m_background.getPosition();
	pos.x += offset.x;
	pos.y += offset.y;
	m_textSprite.setPosition(pos.x, pos.y);

	m_text.setPosition(getTextPadding().x, (bgRect.height / 2.f) - (getCharacterSize() / 2.f));
}

void TextBox::initialize()
{
	auto& bounds = m_background.getGlobalBounds();
	m_textTexture.create(bounds.width, bounds.height);
}

void TextBox::update(float dt)
{
	m_textTexture.clear(sf::Color::Transparent);
	m_textTexture.draw(m_text);
	m_textTexture.display();
	m_textSprite.setTexture(m_textTexture.getTexture());
}

void TextBox::setActive(bool active)
{
	m_active = active;
}

bool TextBox::getActive() const
{
	return m_active;
}

void TextBox::setString(const sf::String& string)
{
	bool overflowBefore = m_text.getGlobalBounds().width > getTextBounds().width;
	auto boundsBefore = m_text.getGlobalBounds();
	m_text.setString(string);
	bool overflowAfter = m_text.getGlobalBounds().width > getTextBounds().width;
	auto boundsAfter = m_text.getGlobalBounds();

	float widthChange = boundsAfter.width - boundsBefore.width;

	if (overflowBefore || overflowAfter) {
		m_text.move(-widthChange, 0.f);
	}
}

void TextBox::setFont(const sf::Font& font)
{
	m_text.setFont(font);
	centerText();
}

void TextBox::setCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
	centerText();
}

void TextBox::setTextPadding(const sf::Vector2f& padding)
{
	m_textPadding = padding;

	sf::IntRect mask;
	mask.left = padding.x;
	mask.top = padding.y;
	mask.width = m_textTexture.getSize().x - (padding.x * 2);
	mask.height = m_textTexture.getSize().y - (padding.y * 2);
	m_textSprite.setTextureRect(mask);
}

void TextBox::setTextStyle(sf::Uint32 style)
{
	m_text.setStyle(style);
	centerText();
}

void TextBox::setTextOutlineColor(const sf::Color& color)
{
	m_text.setOutlineColor(color);
}

void TextBox::setTextOutlineThickness(float thickness)
{
	m_text.setOutlineThickness(thickness);
}

void TextBox::setTextFillColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}

void TextBox::setNativeEventActions()
{
	onClick.setAction([&](sf::Event event) {
		setActive(true);
		printf("CLICK!\n");
	});

	onClick.setAltAction([&](sf::Event event) {
		setActive(false);
	});

	onTextEntered.setAction([&](sf::Event event) {
		// 8 = backspace
		if (event.text.unicode == 8) {
			sf::String str = m_text.getString();
			if (str.getSize() > 0) {
				str.erase(str.getSize() - 1, 1);
				setString(str);
			}
		}
		else {
			setString(m_text.getString() + event.text.unicode);
		}
	});
}

void TextBox::setNativeEventConditions()
{
	onTextEntered.setCondition([&](sf::Event event) { return m_active; });
}

void TextBox::setPosition(sf::Vector2f pos)
{
	m_background.setPosition(pos);
	auto& padding = getTextPadding();
	m_textSprite.setPosition(pos.x + padding.x, pos.y + padding.y);
	centerText();
}

const sf::String& TextBox::getString() const
{
	return m_text.getString();
}

const sf::Font* TextBox::getFont() const
{
	return m_text.getFont();
}

unsigned int TextBox::getCharacterSize() const
{
	return m_text.getCharacterSize();
}

const sf::Vector2f& TextBox::getTextPadding() const
{
	return m_textPadding;
}

sf::FloatRect TextBox::getTextBounds() const
{
	auto& padding = getTextPadding();
	sf::FloatRect mask;
	mask.left = padding.x;
	mask.top = padding.y;
	mask.width = m_textTexture.getSize().x - (padding.x * 2);
	mask.height = m_textTexture.getSize().y - (padding.y * 2);
	return mask;
}

sf::Uint32 TextBox::getStyle() const
{
	return m_text.getStyle();
}

const sf::Color& TextBox::getTextFillColor() const
{
	return m_text.getFillColor();
}

const sf::Vector2f& TextBox::getPosition() const
{
	return m_textSprite.getPosition();
}

const sf::Color& TextBox::getTextOutlineColor() const
{
	return m_text.getOutlineColor();
}

float TextBox::getTextOutlineThickness() const
{
	return m_text.getOutlineThickness();
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background, states);
	target.draw(m_textSprite, states);
}
