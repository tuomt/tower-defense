#include "Armor.h"
#include "Helper.h"

void Armor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_healthBar, states);
}

Armor::Armor(const json& attributes, TextureManager& textureManager, std::vector<sf::Vector2f>& waypoints)
	: m_attributes(attributes), m_textureManager(textureManager), m_waypoints(waypoints)
{
	m_sprite.setTexture(textureManager.getTexture(attributes["name"]));
	if (!attributes["textureRect"].is_null()) {
		sf::IntRect textureRect;
		textureRect.left = attributes["textureRect"][0];
		textureRect.top = attributes["textureRect"][1];
		textureRect.width = attributes["textureRect"][2];
		textureRect.height = attributes["textureRect"][3];
		m_sprite.setTextureRect(textureRect);
	}

	setHealth(attributes["health"]);
	setSpeed(attributes["speed"]);
	setDestroyReward(attributes["destroyReward"]);
}

void Armor::setOrigin(float x, float y)
{
	m_sprite.setOrigin(x, y);
}

void Armor::setOrigin(const sf::Vector2f& origin)
{
	setOrigin(origin.x, origin.y);
}

void Armor::setPosition(float x, float y)
{
	m_sprite.setPosition(x, y);
	m_healthBar.setPosition(x - 20.f, y - getGlobalBounds().height / 2.f - 20.f);
}

void Armor::setPosition(const sf::Vector2f& position)
{
	setPosition(position.x, position.y);
}

void Armor::setRotation(float angle)
{
	m_sprite.setRotation(angle);
}

void Armor::move(float offsetX, float offsetY)
{
	sf::Vector2f pos = getPosition();
	setPosition(pos.x + offsetX, pos.y + offsetY);
}

void Armor::move(const sf::Vector2f& offset)
{
	move(offset.x, offset.y);
}

void Armor::setHealth(float health)
{
	if (health < 0.f) {
		m_health = 0.f;
		m_healthBar.setRemaining(0.f);
	}
	else {
		m_health = health;
		m_healthBar.setRemaining(m_health / (float)m_attributes["health"]);
	}
}

void Armor::setSpeed(float speed)
{
	m_speed = speed;
}

void Armor::setDestroyReward(unsigned int reward)
{
	m_destroyReward = reward;
}

void Armor::setVelocity(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

void Armor::setWaypointIndex(std::size_t index) {
	m_waypointIndex = index;
}

sf::FloatRect Armor::getLocalBounds() const
{
	return m_sprite.getLocalBounds();
}

sf::FloatRect Armor::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

const sf::Vector2f& Armor::getOrigin() const
{
	return m_sprite.getOrigin();
}

const sf::Vector2f& Armor::getPosition() const
{
	return m_sprite.getPosition();
}

const float Armor::getRotation() const
{
	return m_sprite.getRotation();
}

float Armor::getHealth() const
{
	return m_health;
}

float Armor::getSpeed() const
{
	return m_speed;
}

unsigned int Armor::getDestroyReward() const
{
	return m_destroyReward;
}

const sf::Vector2f& Armor::getVelocity() const
{
	return m_velocity;
}

const sf::Vector2f& Armor::getWaypoint() const
{
	return m_waypoints.at(getWaypointIndex());
}

std::size_t Armor::getWaypointIndex() const
{
	return m_waypointIndex;
}

bool Armor::hasReachedWaypoint() const
{
	float distance = Helper::getDistance(getPosition(), getWaypoint());
	return distance < 1.f;
}

bool Armor::selectNextWaypoint()
{
	auto newIndex = getWaypointIndex() + 1;
	if (newIndex < m_waypoints.size()) {
		setWaypointIndex(newIndex);
		//setWaypoint(m_waypoints[newIndex]);
		auto waypointVector = getWaypoint() - getPosition();
		waypointVector = Helper::normalize(waypointVector);
		float rotation = Helper::radToDeg(Helper::getAngle(waypointVector));
		setRotation(rotation);
		sf::Vector2f velocity(getSpeed() * waypointVector.x, getSpeed() * waypointVector.y);
		setVelocity(velocity);
		return true;
	}
	else return false;
}
