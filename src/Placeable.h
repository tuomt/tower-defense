#pragma once
#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/* A base class for objects that can be placed on the field by player. */

class Placeable : public sf::Sprite
{
private:
	unsigned long m_cost = 0;
protected:
	const json& m_attributes;
public:
	Placeable(const json& attributes);
	unsigned long getCost() const;
	void setCost(unsigned long cost);
};

