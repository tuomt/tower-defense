#include "EditorStartGui.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "GuiStack.h"

EditorStartGui::EditorStartGui(sf::RenderWindow& window, sf::Font& font)
	: Gui(window, font)
{
	TextureManager& tm = TextureManager::getInstance();

	m_backButton = std::make_shared<Button>();
	m_backButton->m_background = sf::Sprite(tm.getTexture("button"));
	m_backButton->m_text = sf::Text("BACK", font, 20);
	m_backButton->m_text.setOutlineColor(sf::Color::Black);
	m_backButton->m_text.setOutlineThickness(1.f);
	m_backButton->m_text.setStyle(sf::Text::Bold);
	m_backButton->centerText();

	m_backButton->addListener(m_backButton->onClick, [](sf::Event event) {
		GuiStack::getInstance().pop();
	});

	m_textBox = std::make_shared<TextBox>();
	m_textBox->m_background = sf::Sprite(tm.getTexture("button"));
	m_textBox->m_text = sf::Text("", font, 20);
	m_textBox->m_text.setOutlineColor(sf::Color::Black);
	m_textBox->m_text.setOutlineThickness(1.f);
	m_textBox->m_text.setStyle(sf::Text::Italic);
	m_textBox->setPosition(sf::Vector2f(100.f, 100.f));

	m_textBox->addListener(m_textBox->onTextEntered);
	m_textBox->addListener(m_textBox->onClick);

	addWidget(m_backButton);
	addWidget(m_textBox);
}
