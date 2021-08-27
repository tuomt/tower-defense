#include "MainGui.h"
#include "SceneManager.h"
#include "GuiStack.h"
#include "TextureManager.h"
#include "Game.h"
#include "MapEditor.h"
#include "EditorStartGui.h"
#include "WidgetEvent.h"

MainGui::MainGui(sf::RenderWindow& window, sf::Font& font)
	: Gui(window, font)
{
	auto windowCenter = sf::Vector2f(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
	auto& textureManager = TextureManager::getInstance();

	m_playButton = std::make_shared<Button>();
	m_playButton->m_background = sf::Sprite(textureManager.getTexture("menu_button"));
	m_playButton->m_text = sf::Text("Play", font, 40);
	m_playButton->m_text.setStyle(sf::Text::Bold);
	m_playButton->centerText();

	auto m_playButtonRect = m_playButton->m_background.getGlobalBounds();
	m_playButton->setPosition(sf::Vector2f(windowCenter.x - m_playButtonRect.width / 2.f, 300.f));

	m_editorButton = std::make_shared<Button>();
	m_editorButton->m_background = sf::Sprite(textureManager.getTexture("menu_button"));
	m_editorButton->m_text = sf::Text("Map Editor", font, 40);
	m_editorButton->m_text.setStyle(sf::Text::Bold);
	m_editorButton->centerText();

	auto m_editorButtonRect = m_editorButton->m_background.getGlobalBounds();
	m_editorButton->setPosition(sf::Vector2f(windowCenter.x - m_editorButtonRect.width / 2.f, 450.f));

	m_quitButton = std::make_shared<Button>();
	m_quitButton->m_background = sf::Sprite(textureManager.getTexture("menu_button"));
	m_quitButton->m_text = sf::Text("Quit", font, 40);
	m_quitButton->m_text.setStyle(sf::Text::Bold);
	m_quitButton->centerText();

	auto m_quitButtonRect = m_quitButton->m_background.getGlobalBounds();
	m_quitButton->setPosition(sf::Vector2f(windowCenter.x - m_quitButtonRect.width / 2.f, 600.f));

	// Add listeners

	m_playButton->addListener(m_playButton->onClick, [&](auto event) { 
			SceneManager::getInstance().enqueueScene(std::make_unique<Game>(m_window));
		}
	);
	m_playButton->addListener(m_playButton->onHover, 
		[&](auto event) {
			m_playButton->m_text.setFillColor(sf::Color::Green); 
		}
	);
	m_playButton->addListener(m_playButton->onHoverLeave, 
		[&](auto event) {
			m_playButton->m_text.setFillColor(sf::Color::White); 
		}
	);

	m_editorButton->addListener(m_editorButton->onClick, [&](auto event) { 
			GuiStack::getInstance().push(std::make_shared<EditorStartGui>(m_window, m_font));
			//SceneManager::getInstance().enqueueScene(std::make_unique<MapEditor>(m_window));
		}
	);
	m_editorButton->addListener(m_editorButton->onHover, 
		[&](auto event) {
			m_editorButton->m_text.setFillColor(sf::Color::Green); 
		}
	);
	m_editorButton->addListener(m_editorButton->onHoverLeave, 
		[&](auto event) {
			m_editorButton->m_text.setFillColor(sf::Color::White); 
		}
	);

	m_quitButton->addListener(m_quitButton->onClick, [&](auto event) { m_window.close(); });

	m_quitButton->addListener(m_quitButton->onHover, 
		[&](auto event) {
			m_quitButton->m_text.setFillColor(sf::Color::Green); 
		}
	);
	m_quitButton->addListener(m_quitButton->onHoverLeave, 
		[&](auto event) {
			m_quitButton->m_text.setFillColor(sf::Color::White); 
		}
	);

	// Add widgets
	addWidget(m_playButton);
	addWidget(m_editorButton);
	addWidget(m_quitButton);
}
