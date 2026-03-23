#include <SFML/Graphics.hpp>
#pragma once

enum class DragMode { None, DraggingEnd, DraggingStart };

class Connection
{
public:
    Connection(sf::Vector2f start, sf::Vector2f end);


    sf::Vector2f getStart() const;


    sf::Vector2f getEnd() const;


    void setEnd(sf::Vector2f end);


    void setStart(sf::Vector2f start);


    bool isSelected = false;

    void draw(sf::RenderWindow& window, float thickness = 20.f);



private:

    sf::Vector2f m_start;
    sf::Vector2f m_end;

};
