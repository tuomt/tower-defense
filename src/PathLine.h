#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Waypoint.h"

class Waypoint;

class PathLine : public sf::RectangleShape
{
private:
	std::weak_ptr<Waypoint> m_start;
	std::weak_ptr<Waypoint> m_end;

	float m_thickness = 2.f;
public:
	PathLine(std::shared_ptr<Waypoint> start, std::shared_ptr<Waypoint> end);

	void updatePosition();

	void setStart(std::shared_ptr<Waypoint> start);
	void setEnd(std::shared_ptr<Waypoint> end);
};
