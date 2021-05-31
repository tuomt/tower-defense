#include "Shop.h"

Shop::Shop(const json& towerAttributes, sf::Font& font, unsigned long& money)
	: m_towerAttributes(towerAttributes), m_font(font), m_money(money)
{
	for (auto& text : m_description) {
		text.setFont(font);
		text.setCharacterSize(20);
	}
}

void Shop::init(TextureManager& texManager, const sf::RenderWindow& window)
{
	// Bar
	m_bar.setTexture(texManager.getTexture("shop_bar"));
	m_bar.setPosition(0, window.getSize().y - m_bar.getTextureRect().height);

	// Items
	int i = 0;
	const sf::Vector2f margin(20.f, 20.f);
	auto itemPos = m_bar.getPosition();

	for (auto& tower : m_towerAttributes) {
		Item item(i);
		std::string name = m_towerAttributes[i]["name"];

		// Set textures for the item
		item.getSprite().setTexture(texManager.getTexture(name));
		item.getBackground().setTexture(texManager.getTexture("shop_item_background"));
		
		// Scale the item
		float scale = m_towerAttributes[i]["shopScale"];
		item.getSprite().scale(scale, scale);

        // Set origin of the item's image sprite
        auto spriteRect = item.getSprite().getTextureRect();
        item.getSprite().setOrigin(sf::Vector2f(spriteRect.width / 2.f, spriteRect.height / 2.f));

		// Set position of the item
		auto bgRect = item.getBackground().getTextureRect();
		item.setPosition(itemPos + margin);
		itemPos.x += bgRect.width;

		// Rotate the tower image
		item.getSprite().rotate(-90.f);

		item.setCost(m_towerAttributes[i]["baseCost"]);
		m_items.push_back(item);
		i++;
	}
	updateBuyableItems();

	// Update position of the tower description
	updateDescPos();
}

Shop::Item* Shop::isMouseOnItem(sf::Vector2f mousePos)
{
	for (auto& item : m_items) {
		if (item.getBackground().getGlobalBounds().contains(mousePos)) {
			return &item;
		}
	}

	return nullptr;
}

void Shop::selectItem(Item* item)
{
	m_selectedItem = item;
	if (m_money >= item->getCost()) {
		item->getBackground().setColor(sf::Color::Green);
	}

	auto id = item->getId();
	// Update description
	float radius = m_towerAttributes[id]["radius"];
	unsigned int damage = m_towerAttributes[id]["projectiles"][0]["damage"];
	float fireRate = m_towerAttributes[id]["fireRate"];
	unsigned int cost = m_items[id].getCost();
	m_description[0].setString("Radius: " + std::to_string(radius));
	m_description[1].setString("Damage: " + std::to_string(damage));
	m_description[2].setString("Fire rate: " + std::to_string(fireRate));
	m_description[3].setString("Cost: " + std::to_string(cost));
}

void Shop::clearSelection()
{
	if (m_selectedItem) {
		if (m_money >= m_selectedItem->getCost()) {
			m_selectedItem->getBackground().setColor(sf::Color::White);
		}
		else {
			m_selectedItem->getBackground().setColor(sf::Color::Red);
		}
		clearDescription();
		m_selectedItem = nullptr;
	}
}

void Shop::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_bar, states);
	for (auto& item : m_items) {
		target.draw(item, states);
	}

	for (auto& text : m_description) {
		target.draw(text);
	}

	target.draw(m_buyButton);
}

void Shop::updateDescPos()
{
	auto barRect = m_bar.getGlobalBounds();

	int i = 0;
	for (auto& text : m_description) {
		sf::Vector2f margin(barRect.width - 500.f, 20.f);
		sf::Vector2f pos = m_bar.getPosition();
		pos.x += margin.x;
		pos.y += margin.y;
		pos.y += i * text.getCharacterSize();
		text.setPosition(pos);
		i++;
	}
}

void Shop::clearDescription()
{
	for (auto& text : m_description) {
		text.setString("");
	}
}

Shop::Item* Shop::getSelectedItem()
{
	return m_selectedItem;
}

const Shop::Item& Shop::getItem(std::size_t index) const
{
	return m_items[index];
}

void Shop::updateBuyableItems()
{
	for (auto& item : m_items) {
		if (m_money >= item.getCost()) {
			item.getBackground().setColor(sf::Color::White);
		}
		else {
			item.getBackground().setColor(sf::Color::Red);
		}
	}
}

// Shop::Item methods

Shop::Item::Item(std::size_t id)
	: m_id(id)
{ }

sf::Sprite& Shop::Item::getBackground()
{
	return m_background;
}

sf::Sprite& Shop::Item::getSprite()
{
	return m_sprite;
}

unsigned long Shop::Item::getCost() const
{
	return m_cost;
}

std::size_t Shop::Item::getId() const
{
	return m_id;
}

void Shop::Item::setCost(unsigned long cost)
{
	m_cost = cost;
}

void Shop::Item::setPosition(const sf::Vector2f& position)
{
	m_background.setPosition(position);
	auto rect = m_background.getTextureRect();
	auto bgCenter = sf::Vector2f(position.x + rect.width / 2.f, position.y + rect.height / 2.f);
	m_sprite.setPosition(bgCenter);
}

void Shop::Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_sprite);
}
