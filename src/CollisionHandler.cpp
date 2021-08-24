#include "CollisionHandler.h"
#include "Helper.h"
#include "Collision.h"

using namespace Collision;

bool CollisionHandler::collides(const Armor& armor, const Projectile& projectile)
{
	// Simple collision detection
	// Checks if the projectile's position is inside the bounding box of the armor
	return armor.getGlobalBounds().contains(projectile.getPosition());
}

bool CollisionHandler::collides(sf::Vector2f circle1, sf::Vector2f circle2, float r1, float r2) {
	return Helper::getDistance(circle2, circle1) < r1 + r2;
}


bool CollisionHandler::collides(const Tower& tower, const sf::Sprite& rect)
{
	return BoundingBoxTest(tower, rect);
}

bool CollisionHandler::collides(const sf::Sprite& s1, const sf::Sprite& s2)
{
	return BoundingBoxTest(s1, s2);
}
