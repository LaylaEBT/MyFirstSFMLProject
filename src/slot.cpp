#include "slot.h"

void Slot::setPosition(sf::Vector2f pos)
{
    m_position = pos;
}
 
sf::Vector2f Slot::getPosition() const
{
    return m_position;
}

void Slot::draw(sf::RenderWindow& window)
{

    sf::CircleShape shape(10.f);
    shape.setFillColor(isSelected ? sf::Color::Blue : sf::Color::Green);
    shape.setPosition(m_position);
    window.draw(shape);

}
