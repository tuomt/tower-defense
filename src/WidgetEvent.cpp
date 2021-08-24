#include "WidgetEvent.h"

WidgetEvent::WidgetEvent(sf::Event::EventType type)
	: m_type(type)
{ }

void WidgetEvent::setAction(std::function<void(sf::Event event)> action)
{
	m_action = action;
}

void WidgetEvent::setCondition(std::function<bool(sf::Event event)> condition)
{
	m_condition = condition;
}

const std::function<void(sf::Event event)>& WidgetEvent::getAction() const
{
	return m_action;
}

const std::function<bool(sf::Event event)>& WidgetEvent::getCondition() const
{
	return m_condition;
}

sf::Event::EventType WidgetEvent::getType() const
{
	return m_type;
}

bool WidgetEvent::hasAction() const
{
	if (m_action) return true;
	else return false;
}

bool WidgetEvent::hasCondition() const
{
	if (m_condition) return true;
	else return false;
}
