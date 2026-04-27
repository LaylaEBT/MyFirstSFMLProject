#pragma once
#include <SFML/Graphics.hpp>

class Slot {
public:
    Slot(sf::Color color, float radius);

    // setters
    void setPosition(sf::Vector2f pos);
    void setColor(sf::Color color);
    void setSelected(bool selected);
    void setHovered(bool hovered);
    void setEmpty(bool empty);

    // getters
    sf::Vector2f getPosition() const;
    sf::Vector2f getCenter() const;
    sf::Color getColor() const;
    bool isSelected() const;
    bool isHovered() const;
    bool isEmpty() const;

    void draw(sf::RenderWindow& window) const;

private:
    sf::Vector2f m_position;
    sf::Color m_color;
    float m_radius;

    bool m_isSelected = false;
    bool m_isEmpty = false;
    bool m_isHovered = false;
};