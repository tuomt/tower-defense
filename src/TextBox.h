#pragma once
#include "Widget.h"
#include "WidgetEvent.h"
#include "Gui.h"

class TextBox : public Widget
{
private:
	bool m_active = false;
	sf::Sprite m_textSprite;
	sf::Text m_text;
	sf::RenderTexture m_textTexture;
	sf::Vector2f m_textPadding;

	void centerText();
	void initialize();
public:
	TextBox(const sf::Texture& bgTexture, const sf::Font& font);
	void update(float dt) override;

	void setString(const sf::String& string);
	void setFont(const sf::Font& font);
	void setCharacterSize(unsigned int size);
	void setTextStyle(sf::Uint32 style);
	void setTextFillColor(const sf::Color& color);
	void setTextOutlineColor(const sf::Color& color);
	void setTextOutlineThickness(float thickness);

	const sf::String& getString() const;
	const sf::Font* getFont() const;
	unsigned int getCharacterSize() const;
	sf::Uint32 getStyle() const;
	const sf::Color& getTextFillColor() const;
	const sf::Color& getTextOutlineColor() const;
	float getTextOutlineThickness() const;

	void setActive(bool active);
	bool getActive() const;
	void setTextPadding(const sf::Vector2f& padding);
	const sf::Vector2f& getTextPadding() const;
	sf::FloatRect getTextBounds() const;

	WidgetEvent onTextEntered = WidgetEvent(sf::Event::EventType::TextEntered);
	virtual void setNativeEventActions();

	// Inherited via Widget
	virtual void setNativeEventConditions() override;
	virtual void setPosition(sf::Vector2f pos) override;
	virtual const sf::Vector2f& getPosition() const override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};