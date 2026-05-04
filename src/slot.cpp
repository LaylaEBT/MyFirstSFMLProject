#include "slot.h"

Slot::Slot(sf::Color color, float radius)
    : m_color(color)
    , m_radius(radius)
{
}

void Slot::setPosition(sf::Vector2f pos)
{
    m_position = pos;
}

void Slot::setColor(sf::Color color)
{
    m_color = color;
}

void Slot::setSelected(bool selected)
{
    m_isSelected = selected;
}

void Slot::setHovered(bool hovered)
{
    m_isHovered = hovered;
}

void Slot::setEmpty(bool empty)
{
    m_isEmpty = empty;
}

void Slot::setValidTarget(bool valid)
{
    m_isValidTarget = valid;
}

sf::Vector2f Slot::getPosition() const
{
    return m_position;
}

sf::Vector2f Slot::getCenter() const
{
    return m_position + sf::Vector2f(m_radius, m_radius);
}

sf::Color Slot::getColor() const
{
    return m_color;
}

bool Slot::isSelected() const
{
    return m_isSelected;
}

bool Slot::isHovered() const
{
    return m_isHovered;
}

bool Slot::isEmpty() const
{
    return m_isEmpty;
}

bool Slot::isValidTarget() const
{
    return m_isValidTarget;
}

void Slot::draw(sf::RenderWindow& window, const sf::Clock& clock) const
{
    if (m_isEmpty) return;

    sf::Color displayColor;
    if (m_isSelected || m_isHovered)
        displayColor = sf::Color::White;
    else
        displayColor = m_color;

    sf::CircleShape shape(m_radius);
    shape.setFillColor(displayColor);
    shape.setPosition(m_position);
    window.draw(shape);

    if (m_isValidTarget)
    {
        float elapsed = clock.getElapsedTime().asSeconds();
        float pulse = (std::sin(elapsed * 6.f) + 1.f) / 2.f;

        float ringRadius = m_radius + 4.f + pulse * 8.f;

        sf::CircleShape ring(ringRadius);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(3.f);
        ring.setOutlineColor(sf::Color::White);

        ring.setPosition({
            m_position.x + m_radius - ringRadius,
            m_position.y + m_radius - ringRadius,
            });
        window.draw(ring);
    }
}
