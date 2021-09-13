#pragma once
#include <SFML/Graphics.hpp>

class Scene : public sf::Drawable
{
protected:
	sf::RenderWindow& m_window;
public:
	Scene(sf::RenderWindow& window);
	virtual ~Scene() = 0;

	virtual void handleInput(sf::Event& event, float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void loadTextures() = 0;
	virtual void unloadTextures() = 0;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	const sf::Vector2f getMousePos();
};
