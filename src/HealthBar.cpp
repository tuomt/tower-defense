#include "HealthBar.h"

HealthBar::HealthBar(const sf::Vector2f& size)
	: m_width(size.x), m_height(size.y), m_remainingWidth(size.x)
{
	m_fullBar[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Red);
	m_fullBar[1] = sf::Vertex(sf::Vector2f(0.f, m_height), sf::Color::Red);
	m_fullBar[2] = sf::Vertex(sf::Vector2f(m_width, 0.f), sf::Color::Red);
	m_fullBar[3] = sf::Vertex(sf::Vector2f(0.f, m_height), sf::Color::Red);
	m_fullBar[4] = sf::Vertex(sf::Vector2f(m_width, 0.f), sf::Color::Red);
	m_fullBar[5] = sf::Vertex(sf::Vector2f(m_width, m_height), sf::Color::Red);

	m_remainingBar[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::Green);
	m_remainingBar[1] = sf::Vertex(sf::Vector2f(0.f, m_height), sf::Color::Green);
	m_remainingBar[2] = sf::Vertex(sf::Vector2f(m_width, 0.f), sf::Color::Green);
	m_remainingBar[3] = sf::Vertex(sf::Vector2f(0.f, m_height), sf::Color::Green);
	m_remainingBar[4] = sf::Vertex(sf::Vector2f(m_width, 0.f), sf::Color::Green);
	m_remainingBar[5] = sf::Vertex(sf::Vector2f(m_width, m_height), sf::Color::Green);
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_fullBar, states);
	target.draw(m_remainingBar, states);
}

void HealthBar::setRemaining(float percentage)
{
	float width = percentage * m_width;
	m_remainingWidth = width;
	if (m_remainingWidth <= 0.f) return;

	m_remainingBar[2].position.x = getPosition().x + width;
	m_remainingBar[4].position.x = getPosition().x + width;
	m_remainingBar[5].position.x = getPosition().x + width;
}

void HealthBar::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

void HealthBar::setPosition(const sf::Vector2f& position)
{
	m_fullBar[0].position = position;
	m_fullBar[1].position.x = position.x;
	m_fullBar[1].position.y = position.y + m_height;
	m_fullBar[2].position.x = position.x + m_width;
	m_fullBar[2].position.y = position.y;
	m_fullBar[3].position.x = position.x;
	m_fullBar[3].position.y = position.y + m_height;
	m_fullBar[4].position.x = position.x + m_width;
	m_fullBar[4].position.y = position.y;
	m_fullBar[5].position.x = position.x + m_width;
	m_fullBar[5].position.y = position.y + m_height;

	if (m_remainingWidth > 0.f) {
		m_remainingBar[0].position = position;
		m_remainingBar[1].position.x = position.x;
		m_remainingBar[1].position.y = position.y + m_height;
		m_remainingBar[2].position.x = position.x + m_remainingWidth;
		m_remainingBar[2].position.y = position.y;
		m_remainingBar[3].position.x = position.x;
		m_remainingBar[3].position.y = position.y + m_height;
		m_remainingBar[4].position.x = position.x + m_remainingWidth;
		m_remainingBar[4].position.y = position.y;
		m_remainingBar[5].position.x = position.x + m_remainingWidth;
		m_remainingBar[5].position.y = position.y + m_height;
	}
}

const sf::Vector2f& HealthBar::getPosition() const
{
	return m_remainingBar[0].position;
}
