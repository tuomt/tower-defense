#pragma once
#include "Widget.h"
#include "WidgetEvent.h"
#include "Gui.h"

class TextBox : public Widget
{
private:
	bool m_active = false;
public:
	TextBox();
	TextBox(sf::Sprite background);
	sf::Text m_text;

	void setActive(bool active);
	bool getActive() const;

	WidgetEvent onTextEntered = WidgetEvent(sf::Event::EventType::TextEntered);

	virtual void setNativeEventActions();

	// Inherited via Widget
	virtual void setNativeEventConditions() override;
	virtual void setPosition(sf::Vector2f pos) override;
	virtual const sf::Vector2f& getPosition() const override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};