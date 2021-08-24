#include <assert.h>
#include "MainMenu.h"
#include "MainGui.h"
#include "Helper.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "GuiStack.h"

MainMenu::MainMenu(sf::RenderWindow& window)
	: Scene(window)
{
	m_font.loadFromFile(DEFAULT_FONT_PATH);
	TextureManager::getInstance().loadTexture("button", "button.png");
	GuiStack::getInstance().push(std::make_shared<MainGui>(m_window, m_font));
}

void MainMenu::handleInput(sf::Event& event, float dt)
{
	GuiStack::getInstance().get().top()->handleInput(event, dt);
}

void MainMenu::update(float dt)
{
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto& guiStack = GuiStack::getInstance().get();
	assert(guiStack.empty() == false);
	m_window.draw(*guiStack.top());
}
