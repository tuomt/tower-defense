#pragma once
#include "Placeable.h"

Placeable::Placeable(const json& attributes)
	: m_attributes(attributes) 
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

