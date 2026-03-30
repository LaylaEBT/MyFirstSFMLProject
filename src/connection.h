#include <SFML/Graphics.hpp>
#pragma once

enum class DragMode { None, DraggingEnd, DraggingStart };

class Connection
{
public:
    Connection(sf::Vector2f start, sf::Vector2f end, sf::Color color);


    sf::Vector2f getStart() const;


    sf::Vector2f getEnd() const;


    void setEnd(sf::Vector2f end);


    void setStart(sf::Vector2f start);


    bool isSelected = false;

    void draw(sf::RenderWindow& window, float thickness = 40.f);



private:

    sf::Vector2f m_start;
    sf::Vector2f m_end;
    sf::Color m_color;

};
