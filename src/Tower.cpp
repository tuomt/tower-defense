#include "Tower.h"
#include "Helper.h"
#include <iostream>

using namespace Helper;

Tower::Tower(const json& attributes, TextureManager& textureManager)
	: m_attributes(attributes), m_textureManager(textureManager)
{
	setTexture(textureManager.getTexture(attributes["name"]));
	if (!attributes["textureRect"].is_null()) {
		sf::IntRect textureRect;
		textureRect.left = attributes["textureRect"][0];
		textureRect.top = attributes["textureRect"][1];
		textureRect.width = attributes["textureRect"][2];
		textureRect.height = attributes["textureRect"][3];
		setTextureRect(textureRect);
	}
	setMuzzlePosition(sf::Vector2f(attributes["muzzlePos"][0], attributes["muzzlePos"][1]));
	setRadius(attributes["radius"]);
	setFireRate(attributes["fireRate"]);
}

float Tower::getRadius() const
{
	return m_radius;
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

std::list<Projectile>& Tower::getProjectiles()
{
	return m_projectiles;
}

bool Tower::isReloading() const
{
	return m_reloadTime > 0.f;
}

void Tower::fire()
{
	// Set reload time
	m_reloadTime = 60.f / getFireRate();
	// Create a projectile
	Projectile p(m_attributes["projectiles"][0], m_textureManager);
	p.setTexture(m_textureManager.getTexture(m_attributes["projectiles"][0]["name"]));
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

void Tower::setRadius(float radius)
{
	m_radius = radius;
}

void Tower::setFireRate(float fireRate)
{
	m_fireRate = fireRate;
}

void Tower::aim(const sf::Vector2f& target)
{
	float angle = radToDeg(getAngle(target, getPosition()));
	setRotation(angle + 90.f);
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
