#include "ResizableComponent.h"
#include <math.h>

void ResizableComponent::updateSize()
{
	auto& r = m_sprite.getTextureRect();
	m_size.x = r.width;
	m_size.y = r.height;
}

ResizableComponent::ResizableComponent(const sf::Texture& texture, const sf::Texture& knobTexture)
{
	m_sprite.setTexture(texture);

	// Update m_size
	updateSize();

	// Knob initialization
	const sf::Vector2f knobSize(18.f, 18.f);
	m_knob.setTexture(knobTexture);
	auto size = knobTexture.getSize();
	sf::Vector2f scalingFactor;
	scalingFactor.x = knobSize.x / size.x;
	scalingFactor.y = knobSize.y / size.y;
	m_knob.setScale(scalingFactor);
	m_knob.setPosition(getKnobOffset());
}

sf::FloatRect ResizableComponent::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

const sf::Vector2f& ResizableComponent::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::Vector2f& ResizableComponent::getOrigin() const
{
	return m_sprite.getOrigin();
}

const sf::Vector2f& ResizableComponent::getSize() const
{
	return m_size;
}

float ResizableComponent::getRotation() const
{
	return m_sprite.getRotation();
}

const sf::Sprite& ResizableComponent::getKnob() const
{
	return m_knob;
}

void ResizableComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_knob, states);
}

void ResizableComponent::setPosition(const sf::Vector2f& position)
{
	m_sprite.setPosition(position);
	m_knob.setPosition(getKnobOffset());
}

void ResizableComponent::setOrigin(const sf::Vector2f& origin)
{
	m_sprite.setOrigin(origin);
	m_knob.setPosition(getKnobOffset());
}

void ResizableComponent::setSize(const sf::Vector2f& size)
{
	sf::IntRect textureRect = m_sprite.getTextureRect();
	sf::Vector2f diff(size.x - textureRect.width, size.y - textureRect.height);
	textureRect.width = size.x;
	textureRect.height = size.y;
	m_sprite.setTextureRect(textureRect);
	updateSize();
	m_knob.setPosition(getKnobOffset());
}

void ResizableComponent::rotate(float angle)
{
	m_sprite.rotate(angle);
	m_knob.rotate(angle);
	m_knob.setPosition(getKnobOffset());
}

void ResizableComponent::rotateToPoint(const sf::Vector2f& point)
{
	const float pi = 3.1415926535;

	// Get position of the top left corner
	auto& topLeftCorner = getPosition();
	//auto topLeftCorner = m_sprite.getTransform().transformPoint(sf::Vector2f(0.f, 0.f));

	// Get vector from the corner to mouse position
	auto offset = point - topLeftCorner;

	// Calculate angle of the sprite rectangle's diagonal
	float diagonalAngle = atan2(getSize().y, getSize().x) * 180.f / pi;

	// Calculate angle of the offset vector
	float offsetAngle = atan2(offset.y, offset.x) * 180.f / pi;

	// Set rotation
	m_sprite.setRotation(offsetAngle - diagonalAngle);
	m_knob.setRotation(offsetAngle - diagonalAngle);

	m_knob.setPosition(getKnobOffset());
}

void ResizableComponent::resize(const sf::Vector2f& mousePosition)
{
	// Get position of the corner
	auto corner = m_sprite.getTransform().transformPoint(getSize());
	// Get offset vector (axis aligned) from the corner to mouse position
	auto offset = mousePosition - corner;
	// Get distance between mouse position and the corner
	float distance = sqrt(offset.x * offset.x + offset.y * offset.y);

	// If rotation has been applied, we have to calculate a new vector that is not axis aligned
	if (getRotation() != 0) {
		const float pi = 3.1415926535;
		float rotation = getRotation();
		// Calculate angle of the offset vector
		float offsetAngle = atan2(offset.y, offset.x);
		// Calculate angle of a non-axis-aligned offset vector
		float angle = offsetAngle - (rotation * pi / 180.f);
		// Set the offset vector to a non-axis-aligned vector
		offset.x = distance * cos(angle);
		offset.y = distance * sin(angle);
	}

	// Calculate new size
	auto newSize = getSize() + offset;

	// Check if the new size is smaller than the minimum size
	if (newSize.x < 10) newSize.x = 10.f;
	if (newSize.y < 10) newSize.y = 10.f;

	// Set a new texture rectangle
	sf::IntRect textureRect = m_sprite.getTextureRect();
	textureRect.width = newSize.x;
	textureRect.height = newSize.y;
	m_sprite.setTextureRect(textureRect);

	// Update size
	updateSize();

	// Update knob position
	m_knob.setPosition(getKnobOffset());
}

sf::Vector2f ResizableComponent::getKnobOffset()
{
	return m_sprite.getTransform().transformPoint(getSize());
}
