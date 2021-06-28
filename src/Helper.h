#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>

// Define default font path
#ifdef __unix__
#define DEFAULT_FONT_PATH "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf"
#else
#define DEFAULT_FONT_PATH "C:\\windows\\fonts\\arial.ttf"
#endif

namespace Helper {
	static const std::string defaultFontPath = DEFAULT_FONT_PATH;
	void centerSprite(sf::Sprite& sprite, float x, float y);
	sf::Vector2f getSpriteCenter(sf::Sprite& sprite);
	float getDistance(sf::Vector2f a, sf::Vector2f b);
	float getAngle(sf::Vector2f vector);
	float getAngle(sf::Vector2f p1, sf::Vector2f p2);
	float radToDeg(float radians);
	float degToRad(float degrees);
	sf::Vector2f getPerimeterPoint(sf::Vector2f center, float radius, float angle);
	sf::Vector2f normalize(const sf::Vector2f& vector);
}

