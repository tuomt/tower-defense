#include "Helper.h"

void Helper::centerSprite(sf::Sprite& marker, float x, float y) {
	auto rect = marker.getGlobalBounds();
	float offsetX = rect.width / 2.f;
	float offsetY = rect.height / 2.f;
	marker.setPosition(x - offsetX, y - offsetY);
}

sf::Vector2f Helper::getSpriteCenter(sf::Sprite& sprite) {
	auto& pos = sprite.getPosition();
	auto rect = sprite.getGlobalBounds();
	float offsetX = rect.width / 2.f;
	float offsetY = rect.height / 2.f;
	return sf::Vector2f(pos.x + offsetX, pos.y + offsetY);
}

float Helper::getDistance(sf::Vector2f a, sf::Vector2f b) {
	sf::Vector2f diff = b - a;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

float Helper::getAngle(sf::Vector2f vector)
{
	return atan2(vector.y, vector.x);
}

float Helper::getAngle(sf::Vector2f p1, sf::Vector2f p2)
{
	float dy = p1.y - p2.y;
	float dx = p1.x - p2.x;
	return atan2(dy, dx);
}

float Helper::radToDeg(float radians) {
	const float pi = 3.14159265f;
	return radians * 180 / pi;
}

sf::Vector2f Helper::getPerimeterPoint(sf::Vector2f center, float radius, float angle) {
	float y = sin(angle) * radius;
	float x = cos(angle) * radius;
	return sf::Vector2f(center.x + x, center.y + y);
}

sf::Vector2f Helper::normalize(const sf::Vector2f& vector)
{
	float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
	return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
}

