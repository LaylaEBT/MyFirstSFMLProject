#include "slot.h"

Slot::Slot(sf::Color color)
    : m_color(color)
{
}

void Slot::setPosition(sf::Vector2f pos)
{
    m_position = pos;
}
 
sf::Vector2f Slot::getPosition() const
{
    return m_position;
}

void Slot::setColor(sf::Color color)
{
    m_color = color;
}

void Slot::draw(sf::RenderWindow& window)
{

    sf::CircleShape shape(20.f);
    shape.setFillColor(isSelected ? sf::Color::White : m_color);
    shape.setPosition(m_position);
    window.draw(shape);

}
