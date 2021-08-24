#pragma once
#include <SFML/Graphics.hpp>
#include "WidgetEvent.h"
#include <functional>
#include <map>

class Widget : public sf::Drawable
{
protected:
	bool m_isVisible = true;
	std::multimap<sf::Event::EventType, WidgetEvent> m_listeners;
	virtual void setNativeEventConditions();
public:	
	Widget();
	Widget(sf::Sprite background);

	sf::Sprite m_background;

	// Native events
	WidgetEvent onClick = WidgetEvent(sf::Event::EventType::MouseButtonPressed);
	WidgetEvent onHover = WidgetEvent(sf::Event::EventType::MouseMoved);
	WidgetEvent onHoverLeave = WidgetEvent(sf::Event::EventType::MouseMoved);

	void addListener(sf::Event::EventType eventType, std::function<void(sf::Event event)> action);
	void addListener(WidgetEvent widgetEvent, std::function<void(sf::Event event)> action);
	void addListener(WidgetEvent widgetEvent);
	void removeListeners(sf::Event::EventType eventType);

	void handleEvent(sf::Event event);

	void setVisible(bool visible);
	bool getVisible() const;

	virtual bool contains(const sf::Vector2f& point) const;
	virtual bool contains(float x, float y) const;

	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual const sf::Vector2f& getPosition() const = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};