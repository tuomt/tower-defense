#include <fstream>
#include "Game.h"
#include "Helper.h"
#include "TextureManager.h"
#include "CollisionHandler.h"
#include "SceneManager.h"
#include "Debug.h"

using namespace Helper;

Game::Game(sf::RenderWindow& window)
	: Scene(window), m_shop((loadAttributes(), m_towerAttributes), m_font, m_money)
{
	m_window.setKeyRepeatEnabled(false);
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
	auto& textureManager = TextureManager::getInstance();
	loadTextures();
	loadMap("level_1");
	//loadAttributes(); // Must be loaded before textures
	m_mapSprite.setTexture(textureManager.getTexture("background"));
	//m_shop.setAttributes(m_towerAttributes);
	m_shop.init(m_window);

	setMoney(250);

	// Debug text
	debugText.setFont(m_font);
	debugText.setCharacterSize(20);
	debugText.setPosition(10.f, 50.f);
}

Game::~Game()
{
	unloadTextures();
}

void Game::handleInput(sf::Event& event, float dt)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			SceneManager::getInstance().quit();
		}
		else if (event.key.code == sf::Keyboard::D) {
			m_showDebug = !m_showDebug;
		}
		else if (event.key.code == sf::Keyboard::Space) {
			if (!m_round.isInProgress()) {
				DEBUG_COUT("Starting new round..\n");
				// Start round
				m_round.start();
				m_roundText.setString("Round: " + std::to_string(m_round.getNumber()));
				fillArmorQueue();
			}
		}
		else if (event.key.code == sf::Keyboard::P) {
			m_pause = !m_pause;
		}
		else if (event.key.code == sf::Keyboard::R) {
			if (m_selection.isActive() &&
					m_selection.object->getType() == Placeable::Type::Tower) {
				m_selection.state = Selection::State::Rotating;
				m_window.setMouseCursorVisible(false);
			}
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::R) {
			if (m_selection.isActive()) {
				sf::Mouse::setPosition((sf::Vector2i)m_selection.object->getPosition(), m_window);
				m_selection.state = Selection::State::Moving;
			}
			m_window.setMouseCursorVisible(true);
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		auto& mousePos = getMousePos();
		if (m_selection.isActive()) {
			switch (m_selection.state) 
			{
			case Selection::State::Moving:
				m_selection.object->setPosition(mousePos);
				break;
			case Selection::State::Rotating:
				float angle = radToDeg(getAngle(mousePos, m_towers.back().getPosition()));
				m_towers.back().setRotation(angle);
				m_towers.back().setBaseDirection(m_towers.back().getRotation());
				updateDebug(m_towers.back());
				break;
			}
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
				if (m_selection.isActive() && m_selection.canBePlaced) {
					placeSelectedItem();
					setDebug(m_towers.back());
				}
				else if (!m_selection.isActive()) {
					Shop::Item* selectedItem = m_shop.getSelectedItem();

					if (selectedItem && getMoney() >= selectedItem->getCost()) {
						startPlacingItem();
						setDebug(m_towers.back());
					}
				}
				m_leftMouse = MouseState::PRESSED;
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right) {
			if (m_rightMouse == MouseState::RELEASED) {
				if (m_selection.isActive()) {
					// Unselect item
					m_towers.pop_back();
					m_selection.deactivate();
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
	if (m_pause) return;

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

	if (m_selection.isActive()) {
		for (auto& area : m_restrictedAreas) {
			if (CollisionHandler::collides(*m_selection.object, area) ||
				!m_selection.object->isInBounds(m_mapSprite.getGlobalBounds())) {
				debugRect.setFillColor(sf::Color::Red);
				debugText.setString("TRUE");
				debugText.setFillColor(sf::Color::Green);
				m_selection.canBePlaced = false;
				break;
			}
			else {
				debugRect.setFillColor(sf::Color::Green);
				debugText.setString("FALSE");
				debugText.setFillColor(sf::Color::Red);
				m_selection.canBePlaced = true;
			}
		}
	}

	auto armor = m_armors.begin();
	bool oncePerTower = true;

	std::vector<bool> towerTargets(m_towers.size(), false);

	while (armor != m_armors.end()) {
		bool armorErased = false;
		armor->move(armor->getVelocity().x * dt, armor->getVelocity().y * dt);

		int i = 0;

		for (auto& tower : m_towers) {
			// Skip if the tower is being placed
			if (&tower == &m_towers.back() && m_selection.isActive()) {
				break;
			}

			if (tower.isReloading()) {
				tower.reload(dt);
			}

			// Check if the armor is inside the tower's FOV
			if (!towerTargets[i] && tower.isInFOV(*armor)) {

				auto dist = getDistance(tower.getPosition(), armor->getPosition());
				debugAim.setSize(sf::Vector2f(dist, debugAim.getSize().y));
				debugAim.setPosition(tower.getPosition());
				debugAim.setRotation(tower.getRotation());

				// Aim the tower at the armor
				tower.aim(armor->getPosition());
				towerTargets[i] = true;

				if (!tower.isReloading()) {
					tower.fire();
				}
			}

			// Check if any of the tower's projectiles collide with the armor
			auto proj = tower.m_projectiles.begin();
			while (proj != tower.m_projectiles.end()) {

				if (oncePerTower) {
					proj->move(proj->getVelocity().x * dt, proj->getVelocity().y * dt);
					if (!proj->isInWindow(m_window)) {
						proj = tower.m_projectiles.erase(proj);
						continue;
					}
				}

				if (CollisionHandler::collides(*armor, *proj)) {
					armor->setHealth(armor->getHealth() - proj->getDamage());
					proj = tower.m_projectiles.erase(proj);
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
				armorErased = true;
				break;
			}
			i++;
		}

		oncePerTower = false;

		if (armorErased) {
			if (m_armors.empty() && m_armorQueue.empty()) {
				// Remove all projectiles when there's no armors left
				for (auto& tower : m_towers) {
					tower.m_projectiles.clear();
				}
				break;
			}
			else {
				continue;
			}
		}

		if (armor->hasReachedWaypoint() && armor->selectNextWaypoint() == false) {
			// All waypoints have been reached
			// Remove the armor
			armor = m_armors.erase(armor);
			m_health -= 1.f;

			if (m_health <= 0) {
				// Game over
				m_healthBar.setRemaining(0.f);
			}
			else {
				m_healthBar.setRemaining(m_health / m_maxHealth);
			}
		}
		else ++armor;
	}

	if (m_towers.size() > 0)
		updateDebug(m_towers.back());

}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_window.draw(m_mapSprite);
	m_window.draw(m_shop);

	for (auto& armor : m_armors) {
		m_window.draw(armor);
	}

	if (m_showDebug) m_window.draw(debugRect);

	for (auto& tower : m_towers) {
		m_window.draw(tower);
		for (auto& p : tower.m_projectiles) {
			m_window.draw(p);
		}
	}

	m_window.draw(m_roundText);
	m_window.draw(m_moneyText);
	m_window.draw(m_healthBar);


	if (m_showDebug) {
		for (auto& area : m_restrictedAreas) {
			m_window.draw(area);
		}
		m_window.draw(debugCircle);
		m_window.draw(debugMuzzle);
		m_window.draw(debugText);
		m_window.draw(debugTraverseCenter);
		m_window.draw(debugTraverseLeft);
		m_window.draw(debugTraverseRight);
		m_window.draw(debugAim);
	}

}

void Game::loadTextures()
{
	auto& textureManager = TextureManager::getInstance();
	textureManager.loadTexture("background", "level_1.png", this);
	textureManager.loadTexture("restricted_area", "restricted_area.png", this);
	textureManager.getTexture("restricted_area").setRepeated(true);
	textureManager.getTexture("restricted_area").setSmooth(true);
	textureManager.loadTexture("shop_bar", "shop_bar.png", this);
	textureManager.loadTexture("shop_item_background", "shop_item_background.png", this);
	textureManager.loadTexture("buy_button", "buy_button.png", this);

	for (auto& tower : m_towerAttributes) {
		textureManager.loadTexture(tower["name"], tower["texture"], this);
		for (auto& projectile : tower["projectiles"]) {
			textureManager.loadTexture(projectile["name"], projectile["texture"], this);
			textureManager.getTexture(projectile["name"]).setSmooth(true);
		}
		textureManager.getTexture(tower["name"]).setSmooth(true);
	}

	for (auto& armor : m_armorAttributes) {
		textureManager.loadTexture(armor["name"], armor["texture"], this);
		textureManager.getTexture(armor["name"]).setSmooth(true);
	}
}

void Game::unloadTextures()
{
	TextureManager::getInstance().unloadSceneSpecificTextures(this);
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

	DEBUG_PRINTF("Queue: ");
	for (auto& armor : armorIndices) {
		m_armorQueue.push(armor);
		DEBUG_COUT(armor);
	}
	DEBUG_COUT("\n");
}

void Game::spawnNextArmor() {
	std::size_t index = m_armorQueue.front();
	m_armorQueue.pop();

	Armor armor(m_armorAttributes[index], m_waypoints);
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
	auto& textureManager = TextureManager::getInstance();

	std::ifstream mapFile("../cfg/maps/" + mapName + ".json");
	json map;
	mapFile >> map;
	for (auto& wp : map["waypoints"]) {
		m_waypoints.push_back(sf::Vector2f(wp["x"], wp["y"]));
	}

	for (auto& area : map["restrictedAreas"]) {
		sf::Sprite ra;
		ra.setTexture(textureManager.getTexture("restricted_area"));
		sf::Vector2f size(area["size"]["x"], area["size"]["y"]);
		sf::IntRect textureRect;
		textureRect.width = size.x;
		textureRect.height = size.y;
		ra.setTextureRect(textureRect);
		ra.setOrigin(sf::Vector2f(area["origin"]["x"], area["origin"]["y"]));
		ra.setPosition(sf::Vector2f(area["position"]["x"], area["position"]["y"]));
		ra.setRotation(area["rotation"]);
		m_restrictedAreas.push_back(ra);

		/*
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(-2.f);
		rect.setSize(sf::Vector2f(area["size"]["x"], area["size"]["y"]));
		rect.setOrigin(sf::Vector2f(area["origin"]["x"], area["origin"]["y"]));
		rect.setPosition(sf::Vector2f(area["position"]["x"], area["position"]["y"]));
		rect.setRotation(area["rotation"]);
		m_restrictedAreas.push_back(rect);
		*/
	}
}

void Game::startPlacingItem()
{
	auto item = m_shop.getSelectedItem();

	switch (item->getType()) 
	{
		default:
			// TODO: add new types
			break;
		case Placeable::Type::Tower:
			Tower tower(m_towerAttributes[m_shop.getSelectedItem()->getId()]);
			tower.setOrigin(tower.getGlobalBounds().width / 2.f, tower.getGlobalBounds().height / 2.f);
			tower.setPosition(getMousePos());
			tower.setScale(0.5f, 0.5f);
			m_towers.push_back(tower);
			m_selection.activate(&m_towers.back());
			break;
	}
}

void Game::placeSelectedItem()
{
	setMoney(getMoney() - m_selection.object->getCost());
	debugRect.setFillColor(sf::Color::Transparent);
	m_selection.deactivate();
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
	debugCircle.setPointCount(250);
	debugCircle.setRadius(tower.getRange());
	debugCircle.setOrigin(tower.getRange(), tower.getRange());
	debugCircle.setOutlineThickness(1.f);
	debugCircle.setOutlineColor(sf::Color::White);
	debugCircle.setFillColor(sf::Color::Transparent);
	debugCircle.setPosition(tower.getPosition());

	// DEBUG FIRING OFFSET
	debugMuzzle.setRadius(2.f);
	debugMuzzle.setOrigin(debugMuzzle.getRadius(), debugMuzzle.getRadius());
	debugMuzzle.setPosition(tower.getMuzzlePosition());

	// DEBUG FIELD OF VIEW
	debugTraverseLeft.setFillColor(sf::Color::Red);
	debugTraverseLeft.setPosition(tower.getPosition());
	debugTraverseLeft.setSize(sf::Vector2f(tower.getRange(), 1.f));
	debugTraverseLeft.setOrigin(sf::Vector2f(0.f, 0.f));
	debugTraverseLeft.setRotation(tower.getBaseDirection() - tower.getTraverse() / 2.f);

	debugTraverseRight.setFillColor(sf::Color::Red);
	debugTraverseRight.setPosition(tower.getPosition());
	debugTraverseRight.setSize(sf::Vector2f(tower.getRange(), 1.f));
	debugTraverseRight.setOrigin(sf::Vector2f(0.f, 0.f));
	debugTraverseRight.setRotation(tower.getBaseDirection() + tower.getTraverse() / 2.f);

	debugAim.setFillColor(sf::Color::Cyan);
	debugAim.setSize(sf::Vector2f(1.f, 1.f));
}

void Game::updateDebug(Tower& tower)
{
	debugRect.setPosition(tower.getPosition());
	debugCircle.setPosition(tower.getPosition());
	debugMuzzle.setPosition(tower.getMuzzlePosition());
	debugTraverseCenter.setPosition(tower.getPosition());
	debugTraverseLeft.setPosition(tower.getPosition());
	debugTraverseLeft.setRotation(tower.getBaseDirection() - tower.getTraverse() / 2.f);
	debugTraverseRight.setPosition(tower.getPosition());
	debugTraverseRight.setRotation(tower.getBaseDirection() + tower.getTraverse() / 2.f);
}

