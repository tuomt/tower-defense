#include "Scene.h"

Scene::Scene(sf::RenderWindow& window)
	: m_window(window)
{
}

const sf::Vector2f Scene::getMousePos()
{
	return (sf::Vector2f)sf::Mouse::getPosition(m_window);
}
