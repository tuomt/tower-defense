#include <iostream>
#include "Tower.h"
#include "Helper.h"
#include "TextureManager.h"

using namespace Helper;

Tower::Tower(const json& attributes)
	: Placeable(attributes)
{
	setTexture(TextureManager::getInstance().getTexture(attributes["name"]));
	if (!attributes["textureRect"].is_null()) {
		sf::IntRect textureRect;
		textureRect.left = attributes["textureRect"][0];
		textureRect.top = attributes["textureRect"][1];
		textureRect.width = attributes["textureRect"][2];
		textureRect.height = attributes["textureRect"][3];
		setTextureRect(textureRect);
	}
	setMuzzlePosition(sf::Vector2f(attributes["muzzlePos"][0], attributes["muzzlePos"][1]));
	setRange(attributes["range"]);
	setTraverse(attributes["traverse"]);
	setFireRate(attributes["fireRate"]);
}

float Tower::getRange() const
{
	return m_range;
}

float Tower::getTraverse() const
{
	return m_traverse;
}

float Tower::getBaseDirection() const
{
	return m_baseDirection;
}

float Tower::getFireRate() const
{
	return m_fireRate;
}

float Tower::getReloadTime() const
{
	return m_reloadTime;
}

const sf::Vector2f Tower::getMuzzlePosition(bool transform) const
{
	if (transform) {
		return getTransform().transformPoint(m_muzzlePosition);
	}
	else return m_muzzlePosition;
}

bool Tower::isReloading() const
{
	return m_reloadTime > 0.f;
}

bool Tower::isInFOV(const Armor& armor) const
{
	float distance = getDistance(getPosition(), armor.getPosition());

	if (abs(distance) > getRange()) {
		return false;
	} else if (getTraverse() < 360.f) {
		sf::Vector2f aimVector;
		aimVector.x = armor.getPosition().x - getPosition().x;
		aimVector.y = armor.getPosition().y - getPosition().y;

		float angle = radToDeg(getAngle(aimVector));

		if (angle < 0.f) {
			angle += 360.f;
		}

		if (getBaseDirection() > angle) {
			angle = getBaseDirection() - angle;
		}
		else {
			angle = angle - getBaseDirection();
		}

		if (angle > 180.f) {
			angle = 360.f - angle;
		}

		float halfFOV = getTraverse() / 2.f;
		if (angle > halfFOV) {
			return false;
		}
	}

	return true;
}

void Tower::fire()
{
	// Set reload time
	m_reloadTime = 60.f / getFireRate();
	// Create a projectile
	Projectile p(m_attributes["projectiles"][0]);
	p.setTexture(TextureManager::getInstance().getTexture(m_attributes["projectiles"][0]["name"]));
	p.setSpeed(m_attributes["projectiles"][0]["speed"]);
	p.setOrigin(p.getGlobalBounds().width / 2.f, p.getGlobalBounds().height / 2.f);
	p.setRotation(getRotation());
	p.setPosition(getMuzzlePosition());
	p.setScale(getScale());
	auto targetVector = m_target - getPosition();
	targetVector = normalize(targetVector);
	p.setVelocity(sf::Vector2f(p.getSpeed() * targetVector.x, p.getSpeed() * targetVector.y));
	// Add the projectile into the projectiles vector
	m_projectiles.push_back(p);
}

void Tower::setRange(float range)
{
	m_range = range;
}

void Tower::setTraverse(float traverse)
{
	m_traverse = traverse;
}

void Tower::setBaseDirection(float baseDir)
{
	m_baseDirection = baseDir;
}

void Tower::setFireRate(float fireRate)
{
	m_fireRate = fireRate;
}

void Tower::aim(const sf::Vector2f& target)
{
	float angle = radToDeg(getAngle(target, getPosition()));
	setRotation(angle);
	m_target = target;
}

void Tower::setReloadTime(float reloadTime)
{
	m_reloadTime = reloadTime;
}

void Tower::setMuzzlePosition(const sf::Vector2f& position)
{
	m_muzzlePosition = position;
}

void Tower::reload(float dt) {
	m_reloadTime -= dt;
}
