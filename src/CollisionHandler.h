#pragma once
#include <SFML/Graphics.hpp>
#include "Armor.h"
#include "Projectile.h"
#include "Tower.h"

class CollisionHandler
{
public:
	static bool collides(const Armor& armor, const Projectile& projectile);
	static bool collides(sf::Vector2f circle1, sf::Vector2f circle2, float r1, float r2);
	static bool collides(const Tower& tower, const sf::Sprite& rect);
	static bool collides(const sf::Sprite& s1, const sf::Sprite& s2);
};
