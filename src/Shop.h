#pragma once
#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Shop : public sf::Drawable
{
public:
	class Item : public sf::Drawable
	{
	private:
		unsigned long m_cost = 0;
		std::size_t m_id;
		sf::Sprite m_background;
		sf::Sprite m_sprite;
	public:
		Item(std::size_t id);
		sf::Sprite& getBackground();
		sf::Sprite& getSprite();
		unsigned long getCost() const;
		std::size_t getId() const;
		void setCost(unsigned long cost);
		void setPosition(const sf::Vector2f& position);
		// Inherited via Drawable
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
private:
	unsigned long& m_money;
	std::vector<Item> m_items;

	sf::Text m_description[4];
	sf::Font& m_font;
	sf::Sprite m_bar;
	sf::Sprite m_buyButton;

	Item* m_selectedItem = nullptr;
	const json& m_towerAttributes;

	void updateDescPos();
public:

	Shop(const json& towerAttributes, sf::Font& font, unsigned long& money);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void init(const sf::RenderWindow& window);
	Item* isMouseOnItem(sf::Vector2f mousePos);
	void selectItem(Item* item);
	void clearSelection();
	void clearDescription();
	Item* getSelectedItem();
	void updateBuyableItems();
	const Item& getItem(std::size_t index) const;
};
