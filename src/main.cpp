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



void selectCircle(const sf::Event& event, int& selectedRow, int& selectedCol,
    std::vector<Connection>& connections, int& selectedConnectionIndex,
    DragMode& dragMode, int rows, int cols)
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

        if (dragMode == DragMode::None)
        {
            for (int i = 0; i < (int)connections.size(); ++i)
            {
                if (connections[i].getEnd() == currentSlotCenter)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingEnd;
                    break;
                }
                else if (connections[i].getStart() == currentSlotCenter)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingStart;
                    break;
                }
            }
        }
        else
        {
            if (dragMode == DragMode::DraggingEnd)
                connections[selectedConnectionIndex].setEnd(currentSlotCenter);
            else if (dragMode == DragMode::DraggingStart)
                connections[selectedConnectionIndex].setStart(currentSlotCenter);

            selectedConnectionIndex = -1;
            dragMode = DragMode::None;
        }
        break;
    }

    default:
        break;
    }

    if (dragMode != DragMode::None && selectedConnectionIndex != -1)
    {
        sf::Vector2f newPos = {
            selectedCol * 25.f + 10.f,
            selectedRow * 25.f + 10.f
        };

        if (dragMode == DragMode::DraggingEnd)
            connections[selectedConnectionIndex].setEnd(newPos);
        else if (dragMode == DragMode::DraggingStart)
            connections[selectedConnectionIndex].setStart(newPos);
    }
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
    sf::Vector2f delta = b - a;
    return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}

void handleMouse(const sf::Event& event, int& selectedRow, int& selectedCol,
    std::vector<Connection>& connections, int& selectedConnectionIndex,
    DragMode& dragMode, int rows, int cols, float spacing)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouse->button != sf::Mouse::Button::Left)
            return;

        sf::Vector2f clickPos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };

        if (dragMode == DragMode::None)
        {

            for (int i = 0; i < (int)connections.size(); ++i)
            {
                if (distance(clickPos, connections[i].getEnd()) < 15.f)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingEnd;
                    return;
                }
                else if (distance(clickPos, connections[i].getStart()) < 15.f)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingStart;
                    return;
                }
            }

            int col = (int)(clickPos.x / spacing);
            int row = (int)(clickPos.y / spacing);
            selectedCol = std::clamp(col, 0, cols - 1);
            selectedRow = std::clamp(row, 0, rows - 1);
        }
        else
        {
            int col = (int)(clickPos.x / spacing);
            int row = (int)(clickPos.y / spacing);
            col = std::clamp(col, 0, cols - 1);
            row = std::clamp(row, 0, rows - 1);

            sf::Vector2f snappedPos = {
                col * spacing + 10.f,
                row * spacing + 10.f
            };

            if (dragMode == DragMode::DraggingEnd)
                connections[selectedConnectionIndex].setEnd(snappedPos);
            else if (dragMode == DragMode::DraggingStart)
                connections[selectedConnectionIndex].setStart(snappedPos);

            selectedConnectionIndex = -1;
            dragMode = DragMode::None;
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        if (dragMode == DragMode::None || selectedConnectionIndex == -1)
            return;

        auto* mouse = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f mousePos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };

        if (dragMode == DragMode::DraggingEnd)
            connections[selectedConnectionIndex].setEnd(mousePos);
        else if (dragMode == DragMode::DraggingStart)
            connections[selectedConnectionIndex].setStart(mousePos);
    }
}



int main()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");

    int selectedRow = 0;
    int selectedCol = 0;
    int selectedConnectionIndex = -1;

    DragMode dragMode = DragMode::None;
    
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
                    selectedConnectionIndex, dragMode, rows, cols);
                handleMouse(*event, selectedRow, selectedCol, connections,
                    selectedConnectionIndex, dragMode, rows, cols, spacing);
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
