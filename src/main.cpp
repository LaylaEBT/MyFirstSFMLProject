#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <optional>
#include <algorithm>
#include <vector>
#include <cmath>
#include <slot.h>
#include <connection.h>

void drawcircle(sf::RenderWindow & window, float position_x, float position_y, sf::Color color)
{
 
    sf::CircleShape shape(10.f);
    shape.setFillColor(color);
    shape.setPosition({ position_x, position_y });
    window.draw(shape);

}



/*class Connection
{
public:
    Connection(sf::Vector2f start, sf::Vector2f end)
        : m_start(start), m_end(end)
    {}

    sf::Vector2f getStart() const
    {
        return m_start;
    }

    sf::Vector2f getEnd() const
    {
        return m_end;
    }

    void setEnd(sf::Vector2f end) 
    { 
        m_end = end; 
    }

    void setStart(sf::Vector2f start) 
    { 
        m_start = start; 
    }

    bool isSelected = false;

    void draw(sf::RenderWindow& window, float thickness = 20.f)
    {
        sf::Color color = isSelected ? sf::Color::Red : sf::Color::Yellow;


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


private:

    sf::Vector2f m_start;
    sf::Vector2f m_end;

};*/

void selectCircle(const sf::Event& event, int& selectedRow, int& selectedCol,
    std::vector<Connection>& connections, int& selectedConnectionIndex,
    bool& movingEnd, int rows, int cols)
{
    if (!event.is<sf::Event::KeyPressed>())
        return;

    int prevRow = selectedRow;
    int prevCol = selectedCol;

    switch (event.getIf<sf::Event::KeyPressed>()->scancode)
    {
    case sf::Keyboard::Scancode::Left:
    case sf::Keyboard::Scancode::Q:
        selectedCol = std::max(0, selectedCol - 1);
        break;

    case sf::Keyboard::Scancode::Right:
    case sf::Keyboard::Scancode::D:
        selectedCol = std::min(cols - 1, selectedCol + 1);
        break;

    case sf::Keyboard::Scancode::Up:
    case sf::Keyboard::Scancode::Z:
        selectedRow = std::max(0, selectedRow - 1);
        break;

    case sf::Keyboard::Scancode::Down:
    case sf::Keyboard::Scancode::S:
        selectedRow = std::min(rows - 1, selectedRow + 1);
        break;

    case sf::Keyboard::Scancode::Enter:
    {
        sf::Vector2f currentSlotCenter = {
            selectedCol * 25.f + 10.f,
            selectedRow * 25.f + 10.f
        };

        if (!movingEnd)
        {
            for (int i = 0; i < connections.size(); ++i)
            {
                if (connections[i].getEnd() == currentSlotCenter ||
                    connections[i].getStart() == currentSlotCenter)
                {
                    selectedConnectionIndex = i;
                    movingEnd = true;
                    break;
                }
            }
        }
        else
        {
            connections[selectedConnectionIndex].setEnd(currentSlotCenter);
            selectedConnectionIndex = -1;
            movingEnd = false;

        }
        break;
    }

    default:
        break;
    }

    if (movingEnd && selectedConnectionIndex != -1)
    {
        sf::Vector2f newEnd = {
            selectedCol * 25.f + 10.f,
            selectedRow * 25.f + 10.f
        };
        connections[selectedConnectionIndex].setEnd(newEnd);
    }
}


void drawConnection(sf::RenderWindow& window, sf::CircleShape& circleA, sf::CircleShape& circleB, float thickness)
{
    sf::Vector2f start = circleA.getPosition();
    sf::Vector2f end = circleB.getPosition();

    sf::Vector2f delta = end - start;

    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    float angle = std::atan2(delta.y, delta.x) * 180.f / 3.14159265f;

    sf::RectangleShape line({ length, thickness });
    line.setPosition(start);
    line.setRotation(sf::degrees(angle));
    line.setFillColor(sf::Color::Yellow);

    window.draw(line);
}



int main()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");

    int selectedRow = 0;
    int selectedCol = 0;
    int selectedConnectionIndex = -1;
    bool movingEnd = false; 

    std::vector<Slot> slots;
    std::vector<Connection> connections;



    const int rows = 16;
    const int cols = 16;
    const float spacing = 25.f;

    const float radius = 10.f;
    sf::Vector2f center_offset = { radius, radius };

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            Slot slot;

             
            slot.setPosition({ col * spacing, row * spacing });
    

            slots.push_back(slot);

           
        }
    }
     

    connections.emplace_back(slots[5].getPosition() + center_offset, slots[15].getPosition() + center_offset);
    connections.emplace_back(slots[75].getPosition() + center_offset, slots[32].getPosition() + center_offset);

  


   

    while (window.isOpen()) // boucle moteur
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else
            {
                selectCircle(*event, selectedRow, selectedCol, connections,
                    selectedConnectionIndex, movingEnd, rows, cols);
            }
        }

        window.clear();


        int selectedIndex = selectedRow * cols + selectedCol;

        for (int i = 0; i < (int)slots.size(); ++i)
        {
            slots[i].isSelected = (i == selectedIndex);
            slots[i].draw(window);
        }

        for (int i = 0; i < (int)connections.size(); ++i)
        {
            connections[i].isSelected = (i == selectedConnectionIndex);
            connections[i].draw(window);
        }

        


      
        
        window.display();
    }
    return 0;
}
