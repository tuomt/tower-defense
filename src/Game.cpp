#include <fstream>
#include <cstdio>
#include "Game.h"
#include "Helper.h"
#include "TextureManager.h"
#include <iostream>
#include "Collision.h"

using namespace Helper;

Game::Game(sf::RenderWindow& window)
	: Scene(window), m_shop((loadAttributes(), m_towerAttributes), m_font, m_money)
{
	// Load font
	if (!m_font.loadFromFile(DEFAULT_FONT_PATH))
		return;

	// Position healthbar
	m_healthBar.setPosition(m_window.getSize().x - 210.f, m_window.getSize().y - 110.f);

	// Configure texts
	m_roundText.setFont(m_font);
	m_roundText.setCharacterSize(20);
	m_roundText.setPosition(5.f, 5.f);
	m_roundText.setOutlineColor(sf::Color::Black);
	m_roundText.setOutlineThickness(1.f);
	m_roundText.setString("Round: ");

	m_moneyText.setFont(m_font);
	m_moneyText.setCharacterSize(20);
	m_moneyText.setPosition(m_healthBar.getPosition().x, m_healthBar.getPosition().y + m_healthBar.getSize().y + 10.f);
	m_moneyText.setOutlineColor(sf::Color::Black);
	m_moneyText.setOutlineThickness(1.f);

	// Load map and textures
	loadMap("level_1");
	//loadAttributes(); // Must be loaded before textures
	loadTextures();
	m_mapSprite.setTexture(m_textureManager.getTexture("background"));
	//m_shop.setAttributes(m_towerAttributes);
	m_shop.init(m_textureManager, m_window);

	setMoney(250);

	// Debug text
	debugText.setFont(m_font);
	debugText.setCharacterSize(20);
	debugText.setPosition(10.f, 50.f);
}

void Game::handleInput(sf::Event& event, float dt)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			m_window.close();
		}
		else if (event.key.code == sf::Keyboard::Space) {
			if (!m_round.isInProgress()) {
				std::cout << "Starting new round..\n";
				// Start round
				m_round.start();
				m_roundText.setString("Round: " + std::to_string(m_round.getNumber()));
				fillArmorQueue();
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		auto& mousePos = getMousePos();
		if (m_selectedItem) {
			m_towers.back().setPosition(mousePos);
		}
		auto item = m_shop.isMouseOnItem(mousePos);

		if (item) {
			m_shop.clearSelection();
			m_shop.selectItem(item);
		}
		else {
			m_shop.clearSelection();
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (m_leftMouse == MouseState::RELEASED) {
				if (m_selectedItem) {
					placeSelectedItem();
				}
				else {
					Shop::Item* selectedItem = m_shop.getSelectedItem();

					if (selectedItem && getMoney() >= selectedItem->getCost()) {
						m_selectedItem = selectedItem;
						startPlacingItem();
					}
				}
				m_leftMouse = MouseState::PRESSED;
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right) {
			if (m_rightMouse == MouseState::RELEASED) {
				if (m_selectedItem) {
					// Unselect item
					m_towers.pop_back();
					m_selectedItem = nullptr;
				}
			}
			m_rightMouse = MouseState::PRESSED;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (m_leftMouse == MouseState::PRESSED) {
				m_leftMouse = MouseState::RELEASED;
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right) {
			m_rightMouse = MouseState::RELEASED;
		}
	}
}

void Game::update(float dt)
{
	if (m_round.isInProgress()) {
		m_timeSinceSpawn += dt;

		if (m_armorQueue.empty() && m_armors.empty()) {
			m_round.end();
			m_timeSinceSpawn = 0.f;
		}
		else if (!m_armorQueue.empty() && m_timeSinceSpawn >= m_spawnDelay) {
			spawnNextArmor();
			m_timeSinceSpawn = 0.f;
		}
	}

	bool oncePerArmor = true;

	for (auto& tower : m_towers) {
		if (&tower == &m_towers.back() && m_selectedItem) {
			break;
		}

		bool hasTarget = false;

		auto armor = m_armors.begin();
		while (armor != m_armors.end()) {
			if (oncePerArmor) {
				if (armor->hasReachedWaypoint()) {
					if (armor->selectNextWaypoint() == false) {
						// All waypoints have been reached
						// Remove armor
						armor = m_armors.erase(armor);
						m_health -= 1.f;

						if (m_health <= 0) {
							// Game over
							m_healthBar.setRemaining(0.f);
						}
						else {
							m_healthBar.setRemaining(m_health / m_maxHealth);
						}

						break;
					}
				}
				armor->move(armor->getVelocity().x * dt, armor->getVelocity().y * dt);
			}

			// Check if there is an armor inside the tower's radius
			if (!hasTarget && Collision::isInRadius(*armor, tower)) {
				hasTarget = true;
				// Aim the tower at the armor
				tower.aim(armor->getPosition());

				if (tower.isReloading()) {
					tower.reload(dt);
				}
				else {
					tower.fire();
				}
			}

			// Check if any of the tower's projectiles collide with the armor
			auto proj = tower.getProjectiles().begin();
			while (proj != tower.getProjectiles().end()) {

				bool projectileCollides = Collision::collides(*armor, *proj);

				if (projectileCollides) {
					armor->setHealth(armor->getHealth() - proj->getDamage());
					proj = tower.getProjectiles().erase(proj);
				}
				else {
					++proj;
				}
			}

			// Check health of the armor
			if (armor->getHealth() <= 0.f) {
				// Give reward for destroying the armor
				setMoney(getMoney() + armor->getDestroyReward());
				// Remove the armor
				armor = m_armors.erase(armor);
			}
			else ++armor;
		}
		oncePerArmor = false;

		auto proj = tower.getProjectiles().begin();
		while (proj != tower.getProjectiles().end()) {
			// Check if the projectile is out of bounds
			if (!proj->isInWindow(m_window)) {
				proj = tower.getProjectiles().erase(proj);
			}
			else {
				proj->move(proj->getVelocity().x * dt, proj->getVelocity().y * dt);
				++proj;
			}
		}
	}

	if (m_towers.size() > 0)
		updateDebug(m_towers.back());

	std::string s = "(" + std::to_string(getMousePos().x);
	s += ", " + std::to_string(getMousePos().y) + ")\n";
	debugText.setString(s);
}

void Game::draw()
{
	m_window.draw(m_mapSprite);
	m_window.draw(m_shop);

	for (auto& armor : m_armors) {
		m_window.draw(armor);
	}

	m_window.draw(debugRect);
	for (auto& tower : m_towers) {
		m_window.draw(tower);
		for (auto& p : tower.getProjectiles()) {
			m_window.draw(p);
		}
	}

	m_window.draw(m_roundText);
	m_window.draw(m_moneyText);
	m_window.draw(m_healthBar);

	m_window.draw(debugCircle);
	m_window.draw(debugMuzzle);
	m_window.draw(debugText);
}

bool Game::isReadyToSpawn() {
	return m_timeSinceSpawn >= m_spawnDelay;
}

unsigned long Game::getMoney()
{
	return m_money;
}

void Game::setMoney(unsigned long money)
{
	m_money = money;
	// Update money text
	m_moneyText.setString("Money: " + std::to_string(m_money));
	// Update shop
	m_shop.updateBuyableItems();
}

void Game::fillArmorQueue() {
	std::size_t minIndex = m_round.getMinArmorIndex();
	std::size_t maxIndex = m_round.getMaxArmorIndex();

	std::vector<size_t> attributes;
	for (std::size_t i = minIndex; i <= maxIndex; i++) {
		attributes.push_back(i);
	}

	std::list<size_t> armorIndices;
	float strength = 0.f;
	while (attributes.size() > 0 && strength < m_round.getAttackerStrength()) {
		std::size_t randIndex = rand() % attributes.size();
		float addedStrength = m_armorAttributes[randIndex]["strength"];
		if (strength + addedStrength <= m_round.getAttackerStrength()) {
			strength += addedStrength;

			// Insert armors in order by their speed
			float speed = m_armorAttributes[randIndex]["speed"];
			auto it = armorIndices.begin();
			while (it != armorIndices.end()) {
				if (speed >= m_armorAttributes[*it]["speed"]) {
					break;
				}
				it++;
			}

			armorIndices.insert(it, randIndex);
		}
		else {
			attributes.erase(attributes.begin() + randIndex);
		}
	}

	std::cout << "Queue: ";
	for (auto& armor : armorIndices) {
		m_armorQueue.push(armor);
		std::cout << armor;
	}
	std::cout << "\n";
}

void Game::spawnNextArmor() {
	std::size_t index = m_armorQueue.front();
	m_armorQueue.pop();

	Armor armor(m_armorAttributes[index], m_textureManager, m_waypoints);
	armor.setOrigin(armor.getGlobalBounds().width / 2.f, armor.getGlobalBounds().height / 2.f);
	armor.setPosition(m_waypoints[0]);
	armor.selectNextWaypoint();
	auto diff = armor.getWaypoint() - armor.getPosition();
	auto normalized = normalize(diff);
	armor.setVelocity(sf::Vector2f(armor.getSpeed() * normalized.x, armor.getSpeed() * normalized.y));
	m_armors.push_back(armor);
}

void Game::loadAttributes()
{
	std::cout << "loadAttributes()\n";
	std::ifstream ifStream;
	ifStream.open("../cfg/towers.json");
	ifStream >> m_towerAttributes;
	ifStream.close();

	ifStream.open("../cfg/armors.json");
	ifStream >> m_armorAttributes;
	ifStream.close();
}

void Game::loadMap(std::string mapName)
{
	std::ifstream mapFile("../cfg/maps/" + mapName + ".json");
	json map;
	mapFile >> map;
	for (auto& wp : map["waypoints"]) {
		m_waypoints.push_back(sf::Vector2f(wp["x"], wp["y"]));
	}

	for (auto& area : map["restrictedAreas"]) {
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(-2.f);
		rect.setSize(sf::Vector2f(area["size"]["x"], area["size"]["y"]));
		rect.setOrigin(sf::Vector2f(area["origin"]["x"], area["origin"]["y"]));
		rect.setPosition(sf::Vector2f(area["position"]["x"], area["position"]["y"]));
		rect.setRotation(area["rotation"]);
		m_restrictedAreas.push_back(rect);
	}
}

void Game::loadTextures()
{
	std::cout << "loadTextures()\n";
	m_textureManager.loadTexture("background", "level_1.png");
	m_textureManager.loadTexture("shop_bar", "shop_bar.png");
	m_textureManager.loadTexture("shop_item_background", "shop_item_background.png");
	m_textureManager.loadTexture("buy_button", "buy_button.png");

	for (auto& tower : m_towerAttributes) {
		std::cout << "Loading texture: " << tower["texture"] << std::endl;
		m_textureManager.loadTexture(tower["name"], tower["texture"]);
		for (auto& projectile : tower["projectiles"]) {
			m_textureManager.loadTexture(projectile["name"], projectile["texture"]);
			m_textureManager.getTexture(projectile["name"]).setSmooth(true);
		}
		m_textureManager.getTexture(tower["name"]).setSmooth(true);
	}

	for (auto& armor : m_armorAttributes) {
		m_textureManager.loadTexture(armor["name"], armor["texture"]);
		m_textureManager.getTexture(armor["name"]).setSmooth(true);
	}
}

void Game::startPlacingItem()
{
	Tower tower(m_towerAttributes[m_selectedItem->getId()], m_textureManager);
	tower.setOrigin(tower.getGlobalBounds().width / 2.f, tower.getGlobalBounds().height / 2.f);
	tower.setPosition(getMousePos());
	tower.setScale(0.5f, 0.5f);
	m_towers.push_back(tower);
	setDebug(tower);
}

void Game::placeSelectedItem()
{
	setMoney(getMoney() - m_selectedItem->getCost());
	debugRect.setFillColor(sf::Color::Transparent);
	m_selectedItem = nullptr;
}

void Game::setDebug(Tower& tower)
{
	sf::Vector2f size(tower.getGlobalBounds().width, tower.getGlobalBounds().height);

	// DEBUG RECT
	debugRect.setSize(size);
	debugRect.setOrigin(size.x / 2.f, size.y / 2.f);
	debugRect.setOutlineThickness(1.f);
	debugRect.setOutlineColor(sf::Color::Green);
	debugRect.setFillColor(sf::Color(0, 255, 0, 100));
	debugRect.setPosition(tower.getPosition());

	// DEBUG CIRCLE
	debugCircle.setRadius(tower.getRadius());
	debugCircle.setOrigin(tower.getRadius(), tower.getRadius());
	debugCircle.setOutlineThickness(1.f);
	debugCircle.setOutlineColor(sf::Color::White);
	debugCircle.setFillColor(sf::Color::Transparent);
	debugCircle.setPosition(tower.getPosition());

	// DEBUG FIRING OFFSET
	debugMuzzle.setRadius(2.f);
	debugMuzzle.setOrigin(debugMuzzle.getRadius(), debugMuzzle.getRadius());
	debugMuzzle.setPosition(tower.getMuzzlePosition());
}

void Game::updateDebug(Tower& tower)
{
	debugRect.setPosition(tower.getPosition());
	debugCircle.setPosition(tower.getPosition());
	debugMuzzle.setPosition(tower.getMuzzlePosition());
}

