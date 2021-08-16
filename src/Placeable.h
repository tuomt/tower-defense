#pragma once
#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/* A base class for objects that can be placed on the field by player. */

class Placeable : public sf::Sprite
{
public:
	enum class Type { Tower, Mine };
private:
	unsigned long m_cost = 0;
	Type m_type;
protected:
	const json& m_attributes;
public:
	Placeable(const json& attributes, Type type);
	unsigned long getCost() const;
	void setCost(unsigned long cost);
	bool isInBounds(const sf::FloatRect& bounds);

	Type getType() const;
};

