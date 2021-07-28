#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include "Shop.h"
#include "Round.h"
#include "Scene.h"
#include "Tower.h"
#include "Armor.h"
#include "Projectile.h"
#include "TextureManager.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Game : public Scene
{
private:
	sf::Font m_font;
	sf::Text m_roundText;
	sf::Text m_moneyText;

	const float m_maxHealth = 2.f;
	float m_health = m_maxHealth;
	HealthBar m_healthBar = sf::Vector2f(200.f, 30.f);

	enum class MouseState { RELEASED, PRESSED };
	MouseState m_leftMouse = MouseState::RELEASED;
	MouseState m_rightMouse = MouseState::RELEASED;
	TextureManager m_textureManager;
	Round m_round;
	float m_timeSinceSpawn = 0.f;
	float m_spawnDelay = 3.f;
	unsigned long m_money = 0;

	// Attributes for towers, armors, etc.
	json m_armorAttributes;
	json m_towerAttributes;

	Shop m_shop;
	Shop::Item* m_selectedItem = nullptr;

	std::queue<std::size_t> m_armorQueue;
	std::list<Armor> m_armors;
	std::list<Tower> m_towers;
	std::vector<sf::Vector2f> m_waypoints;
	std::vector<sf::RectangleShape> m_restrictedAreas;
	sf::Sprite m_mapSprite;

	void loadAttributes();
	void loadMap(std::string mapName);
	void loadTextures();

	bool m_pause = false;
	bool m_rotating = false;

	// DEBUG STUFF
	sf::RectangleShape debugRect;
	sf::RectangleShape debugTraverseCenter;
	sf::RectangleShape debugTraverseLeft;
	sf::RectangleShape debugTraverseRight;
	sf::RectangleShape debugAim;
	sf::CircleShape debugCircle;
	sf::CircleShape debugMuzzle; // debug muzzle position
	sf::Text debugText;
	void setDebug(Tower& tower);
	void updateDebug(Tower& tower);
public:
	Game(sf::RenderWindow& window);
	virtual void handleInput(sf::Event& event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void fillArmorQueue();
	void spawnNextArmor();
	bool isReadyToSpawn();

	void startPlacingItem();
	void placeSelectedItem();

	unsigned long getMoney();
	void setMoney(unsigned long money);
};

