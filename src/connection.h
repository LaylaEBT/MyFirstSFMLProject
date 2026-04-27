#pragma once
#include <SFML/Graphics.hpp>

class Connection
{
public:
    Connection(sf::Vector2f start, sf::Vector2f end, sf::Color color);

    // setters
    void setStart(sf::Vector2f start);
    void setEnd(sf::Vector2f end);
    void setSelected(bool selected);

    // getters
    sf::Vector2f getStart() const;
    sf::Vector2f getEnd()   const;
    sf::Color getColor() const;
    float getLength() const;
    bool isSelected() const;

    void draw(sf::RenderWindow& window, float thickness = 40.f) const;

private:
    sf::Vector2f m_start;
    sf::Vector2f m_end;
    sf::Color m_color;
    float m_length = 0.f;
    bool m_isSelected = false;
};