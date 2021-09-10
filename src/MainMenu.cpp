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
	loadTextures();
	m_font.loadFromFile(DEFAULT_FONT_PATH);
	GuiStack::getInstance().push(std::make_shared<MainGui>(m_window, m_font));
	m_window.setKeyRepeatEnabled(true);
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
	m_window.draw(m_background);

	auto& guiStack = GuiStack::getInstance().get();
	assert(guiStack.empty() == false);
	m_window.draw(*guiStack.top());
}

void MainMenu::loadTextures()
{
	auto& tm = TextureManager::getInstance();
	tm.loadTexture("button", "button.png");
	tm.loadTexture("menu_button", "main_menu_button.png");
	tm.loadTexture("menu_background", "main_menu_bg.png");
	m_background.setTexture(tm.getTexture("menu_background"));
}
