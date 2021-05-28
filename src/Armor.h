#pragma once
#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"
#include "TextureManager.h"
#include "HealthBar.h"

using json = nlohmann::json;

class Armor : public sf::Drawable
{
private:
	const json& m_attributes;
	TextureManager& m_textureManager;
	float m_health = 0.f;
	float m_speed = 0.f;
	unsigned int m_destroyReward = 0;
	sf::Vector2f m_velocity = sf::Vector2f(0.f, 0.f);
	std::vector<sf::Vector2f>& m_waypoints;
	std::size_t m_waypointIndex = 0;
	HealthBar m_healthBar = HealthBar(sf::Vector2f(50.f, 10.f));
	sf::Sprite m_sprite;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Armor(const json& attributes, TextureManager& textureManager, std::vector<sf::Vector2f>& waypoints);

	void setOrigin(float x, float y);
	void setOrigin(const sf::Vector2f& origin);
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	void setRotation(float angle);
	void move(float offsetX, float offsetY);
	void move(const sf::Vector2f& offset);

	void setHealth(float health);
	void setSpeed(float speed);
	void setDestroyReward(unsigned int reward);
	void setVelocity(const sf::Vector2f& velocity);
	void setWaypointIndex(std::size_t index);

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	const sf::Vector2f& getOrigin() const;
	const sf::Vector2f& getPosition() const;
	const float getRotation() const;

	float getHealth() const;
	float getSpeed() const;
	unsigned int getDestroyReward() const;
	const sf::Vector2f& getVelocity() const;
	const sf::Vector2f& getWaypoint() const;
	std::size_t getWaypointIndex() const;

	bool hasReachedWaypoint() const;
	bool selectNextWaypoint();
};

