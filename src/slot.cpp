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

void Slot::draw(sf::RenderWindow& window) const
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
}
