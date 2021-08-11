#include "CollisionHandler.h"
#include "Helper.h"

bool CollisionHandler::collides(const Armor& armor, const Projectile& projectile)
{
	// Simple collision detection
	// Checks if the projectile's position is inside the bounding box of the armor
	return armor.getGlobalBounds().contains(projectile.getPosition());
}

bool CollisionHandler::collides(sf::Vector2f circle1, sf::Vector2f circle2, float r1, float r2) {
	return Helper::getDistance(circle2, circle1) < r1 + r2;
}


bool CollisionHandler::collides(const Tower& tower, const sf::RectangleShape& rect)
{
	auto towerBounds = tower.getGlobalBounds();
	auto rectBounds = rect.getGlobalBounds();
	return towerBounds.intersects(rectBounds);
}
