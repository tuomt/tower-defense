#include <SFML/Graphics.hpp>
#include <time.h>
#include "Helper.h"
#include "SceneManager.h"
#include "GuiStack.h"
#include "MapEditor.h"
#include "Game.h"
#include "TestScene.h"
#include "MainMenu.h"

using namespace Helper;

int main()
{
	srand(time(nullptr));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1600, 900), "Tower Defense", sf::Style::Default, settings);
	window.setFramerateLimit(120);

	SceneManager& sceneManager = SceneManager::getInstance();
	sceneManager.enqueueScene(std::make_unique<MainMenu>(window));

	sf::Clock clock;

	while (window.isOpen())
	{
		GuiStack::getInstance().update();

		if (sceneManager.nextScene) {
			sceneManager.currentScene = std::move(sceneManager.nextScene);
		}

		sf::Event event;

		// Get delta time
		float dt = clock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else sceneManager.currentScene->handleInput(event, dt);
		}
		sceneManager.currentScene->update(dt);

		window.clear();
		window.draw(*sceneManager.currentScene);
		window.display();
	}

	return 0;
}
