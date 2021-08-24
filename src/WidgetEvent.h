#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class WidgetEvent
{
private:
	// This action will be executed when the event is fired and conditions are fulfilled
	std::function<void(sf::Event event)> m_action = nullptr;
	// This condition has to be passed in order to execute the action
	std::function<bool(sf::Event event)> m_condition = nullptr;
	// The event type that an event listener listens to in order to fire this event
	sf::Event::EventType m_type;
public:
	WidgetEvent(sf::Event::EventType type);
	void setAction(std::function<void(sf::Event event)> action);
	void setCondition(std::function<bool(sf::Event event)> condition);

	const std::function<void(sf::Event event)>& getAction() const;
	const std::function<bool(sf::Event event)>& getCondition() const;
	sf::Event::EventType getType() const;

	bool hasAction() const;
	bool hasCondition() const;
};