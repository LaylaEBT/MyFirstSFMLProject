#pragma once
#include <SFML/Graphics.hpp>

class Slot {
public:
    Slot(sf::Color color);

    bool isSelected = false;

    void setPosition(sf::Vector2f pos);
    

    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window);

    void setColor(sf::Color color);

private:
    sf::Vector2f m_position;
    sf::Color m_color;

};