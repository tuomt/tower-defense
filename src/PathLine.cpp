#include "PathLine.h"
#include "Helper.h"
#include "CollisionHandler.h"

PathLine::PathLine(std::shared_ptr<Waypoint> start, std::shared_ptr<Waypoint> end) 
	: m_start(start), m_end(end)
{
	setFillColor(sf::Color::Black);
	updatePosition();
}

void PathLine::updatePosition()
{
	auto start = m_start.lock();
	auto end = m_end.lock();

	if (start && end) {
		auto startPoint = start->getPosition();
		auto endPoint = end->getPosition();
		// Define a radius for collision detection
		const float radius = start->getGlobalBounds().width / 2.f;

		// Check if the waypoints collide
		if (CollisionHandler::collides(startPoint, endPoint, radius, radius)) {
			// Set size to 0 so the line will be hidden
			setSize(sf::Vector2f(0.f, 0.f));
		} else {
			// Calculate angle from the first waypoint to the second
			float angle = Helper::getAngle(endPoint, startPoint);

			// Get points from the edges of the waypoints
			startPoint = Helper::getPerimeterPoint(startPoint, radius, angle);
			endPoint = Helper::getPerimeterPoint(endPoint, -radius, angle);

			// Calculate the distance between the edge points
			float dist = Helper::getDistance(endPoint, startPoint);

			setSize(sf::Vector2f(dist, m_thickness));
			setRotation(Helper::radToDeg(angle));
			setPosition(startPoint);
		}
	}
}

void PathLine::setStart(std::shared_ptr<Waypoint> start)
{
	m_start = start;
	updatePosition();
}

void PathLine::setEnd(std::shared_ptr<Waypoint> end)
{
	m_end = end;
	updatePosition();
}

