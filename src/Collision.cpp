#include "Collision.h"
#include "Helper.h"

bool Collision::collides(const Armor& armor, const Projectile& projectile)
{
	// Simple collision detection
	// Checks if the projectile's position is inside the bounding box of the armor
	return armor.getGlobalBounds().contains(projectile.getPosition());
}

bool Collision::collides(sf::Vector2f circle1, sf::Vector2f circle2, float r1, float r2) {
	return Helper::getDistance(circle2, circle1) < r1 + r2;
}