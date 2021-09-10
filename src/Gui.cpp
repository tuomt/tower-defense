#include "Gui.h"

void Gui::addWidget(const std::shared_ptr<Widget>& widget)
{
	m_widgets.push_back(widget);
}

void Gui::removeWidget(const std::shared_ptr<Widget>& widget)
{
	m_widgets.remove(widget);
}

void Gui::handleInput(sf::Event& event, float dt)
{
	for (auto& widget : m_widgets) {
		widget->handleEvent(event);
	}
}

void Gui::update(float dt)
{
	for (auto& widget : m_widgets) {
		widget->update(dt);
	}
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& widget : m_widgets) {
		m_window.draw(*widget);
	}
}
