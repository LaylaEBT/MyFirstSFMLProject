#include "connection.h"	
#include <cmath>

Connection::Connection(sf::Vector2f start, sf::Vector2f end, sf::Color color)
    : m_start(start), m_end(end), m_color(color)
{
}

sf::Vector2f Connection::getStart() const
{
    return m_start;
}

sf::Vector2f Connection::getEnd() const
{
    return m_end;
}

void Connection::setEnd(sf::Vector2f end)
{
    m_end = end;
}

void Connection::setStart(sf::Vector2f start)
{
    m_start = start;
}

void Connection::draw(sf::RenderWindow& window, float thickness)
{
    sf::Color color = isSelected ? sf::Color::Red : m_color;


    sf::Vector2f delta = m_end - m_start;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);


    sf::Vector2f perp(-delta.y / length, delta.x / length);
    sf::Vector2f offset = perp * (thickness / 2.f);

    sf::VertexArray line(sf::PrimitiveType::TriangleStrip, 4);
    line[0].position = m_start + offset;
    line[1].position = m_start - offset;
    line[2].position = m_end + offset;
    line[3].position = m_end - offset;

    for (int i = 0; i < 4; ++i)
        line[i].color = color;

    window.draw(line);
}