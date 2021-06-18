#pragma once
#include "Armor.h"
#include "Projectile.h"
#include "Tower.h"

class Collision
{
public:
	static bool collides(const Armor& armor, const Projectile& projectile);
	static bool collides(sf::Vector2f circle1, sf::Vector2f circle2, float r1, float r2);
	static bool isInRange(const Armor& armor, const Tower& tower);
};

