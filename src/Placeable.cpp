#pragma once
#include "Placeable.h"

Placeable::Placeable(const json& attributes, Placeable::Type type)
	: m_attributes(attributes), m_type(type)
{
	setCost(m_attributes["baseCost"]);
}

unsigned long Placeable::getCost() const 
{
	return m_cost;
}

void Placeable::setCost(unsigned long cost)
{
	m_cost = cost;
}

bool Placeable::isInBounds(const sf::FloatRect& bounds)
{
	sf::Vector2f pos = getPosition();
	return pos.x >= bounds.left &&
		pos.y >= bounds.top &&
		pos.x <= bounds.width &&
		pos.y <= bounds.height;
}

Placeable::Type Placeable::getType() const
{
	return m_type;
}
