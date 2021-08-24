#include "Widget.h"

Widget::Widget()
{
	setNativeEventConditions();
}

Widget::Widget(sf::Sprite background)
	: m_background(background)
{
	setNativeEventConditions();
}

void Widget::setNativeEventConditions()
{
	// Set conditions for native widget events

	onClick.setCondition([&](sf::Event event) { 
			return contains(event.mouseButton.x, event.mouseButton.y);
		}
	);

	onHover.setCondition([&](sf::Event event) {
			if (contains(event.mouseMove.x, event.mouseMove.y)) {
				return true;
			}
			else {
				return false;
			}
		}
	);

	onHoverLeave.setCondition([&](sf::Event event) {
			if (contains(event.mouseMove.x, event.mouseMove.y)) {
				return false;
			}
			else {
				return true;
			}
		}
	);
}

void Widget::addListener(sf::Event::EventType eventType, std::function<void(sf::Event event)> action)
{
	WidgetEvent widgetEvent(eventType);
	widgetEvent.setAction(action);
	addListener(widgetEvent);
}

void Widget::addListener(WidgetEvent widgetEvent, std::function<void(sf::Event event)> action)
{	
	widgetEvent.setAction(action);
	addListener(widgetEvent);
}

void Widget::addListener(WidgetEvent widgetEvent)
{
	auto pair = std::make_pair(widgetEvent.getType(), widgetEvent);
	m_listeners.insert(pair);
}

void Widget::removeListeners(sf::Event::EventType eventType)
{
	m_listeners.erase(eventType);
}

void Widget::setVisible(bool visible)
{
	m_isVisible = visible;
}

bool Widget::getVisible() const
{
	return m_isVisible;
}

bool Widget::contains(const sf::Vector2f& point) const
{
	return m_background.getGlobalBounds().contains(point);
}

bool Widget::contains(float x, float y) const
{
	return contains(sf::Vector2f(x, y));
}

void Widget::handleEvent(sf::Event event)
{
	auto lowerBound = m_listeners.lower_bound(event.type);
	auto upperBound = m_listeners.upper_bound(event.type);
	auto& iterator = lowerBound;

	while (iterator != upperBound) {
		WidgetEvent we = iterator->second;
		if (we.hasAction()) {
			if (!we.hasCondition()) {
				we.getAction()(event);
			}
			else if (we.getCondition()(event)) {
				we.getAction()(event);
			}
		}
		++iterator;
	}
}

