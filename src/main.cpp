#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <optional>
#include <algorithm>
#include <vector>
#include <cmath>
#include <slot.h>
#include <connection.h>
#include <DragMode.h>


float distance(sf::Vector2f a, sf::Vector2f b);

void selectCircle(const sf::Event& event, int& selectedRow, int& selectedCol, std::vector<Slot>& slots,
    std::vector<Connection>& connections, int& selectedConnectionIndex,
    DragMode& dragMode, int rows, int cols, float spacing, float radius, float snapTolerance)
{
    if (!event.is<sf::Event::KeyPressed>())
        return;

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
        int currentIndex = selectedRow * cols + selectedCol;

        if (slots[currentIndex].isEmpty())
            break;

        sf::Vector2f currentSlotCenter = slots[currentIndex].getCenter();

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
            float originalLength = connections[selectedConnectionIndex].getLength();
            sf::Vector2f otherEnd;

            if (dragMode == DragMode::DraggingEnd)
                otherEnd = connections[selectedConnectionIndex].getStart();
            else
                otherEnd = connections[selectedConnectionIndex].getEnd();

            float newLength = distance(currentSlotCenter, otherEnd);

            if (std::abs(newLength - originalLength) > snapTolerance)
                break;

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
    int currentIndex = selectedRow * cols + selectedCol;
    if (slots[currentIndex].isEmpty())
    {

        for (int i = 0; i < (int)slots.size(); ++i)
        {
            if (!slots[i].isEmpty())
            {
                selectedRow = i / cols;
                selectedCol = i % cols;
                break;
            }
        }
    }

    if (dragMode != DragMode::None && selectedConnectionIndex != -1)
    {
        int idx = selectedRow * cols + selectedCol;
        sf::Vector2f newPos = slots[idx].getCenter();

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

int getNearestSlotIndex(sf::Vector2f mousePos, const std::vector<Slot>& slots, float radius)
{
    int nearest = -1;
    float bestDist = radius * 2.f;  

    for (int i = 0; i < (int)slots.size(); ++i)
    {
        if (slots[i].isEmpty())
            continue;

        float dist = distance(mousePos, slots[i].getCenter());;

        if (dist < bestDist)
        {
            bestDist = dist;
            nearest = i;
        }
    }
    return nearest; 
}

void handleMouse(const sf::Event& event, int& selectedRow, int& selectedCol, std::vector<Slot>& slots,
    std::vector<Connection>& connections, int& selectedConnectionIndex,
    DragMode& dragMode, int& hoveredSlotIndex,
    int rows, int cols, float spacing, float radius, float snapTolerance)
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
                if (distance(clickPos, connections[i].getEnd()) < radius)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingEnd;
                    return;
                }
                else if (distance(clickPos, connections[i].getStart()) < radius)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingStart;
                    return;
                }
            }

            int col = std::clamp((int)(clickPos.x / spacing), 0, cols - 1);
            int row = std::clamp((int)(clickPos.y / spacing), 0, rows - 1);
            selectedCol = col;
            selectedRow = row;
        }
        else
        {
            int col = std::clamp((int)(clickPos.x / spacing), 0, cols - 1);
            int row = std::clamp((int)(clickPos.y / spacing), 0, rows - 1);
            int targetIndex = row * cols + col;

            if (slots[targetIndex].isEmpty())              
                return;

            sf::Vector2f snappedPos = slots[targetIndex].getCenter();

            sf::Vector2f rawOther;
            if (dragMode == DragMode::DraggingEnd)
                rawOther = connections[selectedConnectionIndex].getStart();
            else
                rawOther = connections[selectedConnectionIndex].getEnd();

            int otherCol = std::clamp((int)std::round((rawOther.x - radius) / spacing), 0, cols - 1);
            int otherRow = std::clamp((int)std::round((rawOther.y - radius) / spacing), 0, rows - 1);
            sf::Vector2f snappedOther = slots[otherRow * cols + otherCol].getCenter();

            float originalLength = connections[selectedConnectionIndex].getLength();
            float newLength = distance(snappedPos, snappedOther);

            if (std::abs(newLength - originalLength) > snapTolerance)
                return;

            if (dragMode == DragMode::DraggingEnd)
                connections[selectedConnectionIndex].setEnd(snappedPos);
            else if (dragMode == DragMode::DraggingStart)
                connections[selectedConnectionIndex].setStart(snappedPos);


            selectedConnectionIndex = -1;
            dragMode = DragMode::None;
            hoveredSlotIndex = -1;
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto* mouse = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f mousePos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };

        hoveredSlotIndex = -1;

        if (dragMode != DragMode::None && selectedConnectionIndex != -1)
        {
            hoveredSlotIndex = getNearestSlotIndex(mousePos, slots, radius);

            if (dragMode == DragMode::DraggingEnd)
                connections[selectedConnectionIndex].setEnd(mousePos);
            else
                connections[selectedConnectionIndex].setStart(mousePos);
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Connecting dots");

    int selectedRow = 0;
    int selectedCol = 0;
    int selectedConnectionIndex = -1;
    int hoveredSlotIndex = -1;

    DragMode dragMode = DragMode::None;
    
    std::vector<Slot> slots;
    std::vector<Connection> connections;

    const int k_rows = 10;
    const int k_cols = 10;
    const float k_spacing = 50.f;
    const float k_radius = 20.f;
    const float k_snapTolerance = k_spacing * 0.75f;



    for (int row = 0; row < k_rows; ++row)
    {
        for (int col = 0; col < k_cols; ++col)
        {
            Slot slot(sf::Color::Cyan, k_radius);

            slot.setPosition({ col * k_spacing, row * k_spacing });

            slots.push_back(slot);         
        }     
    }

    //Solution Slots

    slots[2].setColor(sf::Color::Blue);
    slots[32].setColor(sf::Color::Blue);
    slots[67].setColor(sf::Color::Green);
    slots[89].setColor(sf::Color::Green);
    slots[50].setColor(sf::Color(255, 165, 0));  
    slots[54].setColor(sf::Color(255, 165, 0));
    slots[71].setColor(sf::Color::Magenta);
    slots[74].setColor(sf::Color::Magenta);


    //Empty Slots
    for (int i = 23; i <= 26; ++i)
        slots[i].setEmpty(true);

    
    for (int i = 55; i <= 57; ++i)
        slots[i].setEmpty(true);

   
    slots[35].setEmpty(true);
    slots[45].setEmpty(true);

    connections.emplace_back(slots[5].getCenter(), slots[35].getCenter(), sf::Color::Blue);
    connections.emplace_back(slots[79].getCenter(), slots[67].getCenter(), sf::Color::Green);
    connections.emplace_back(slots[93].getCenter(), slots[97].getCenter(), sf::Color(255, 165, 0));
    connections.emplace_back(slots[91].getCenter(), slots[94].getCenter(), sf::Color::Magenta);
  


   

    while (window.isOpen()) // boucle moteur
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else
            {
                selectCircle(*event, selectedRow, selectedCol, slots, connections,
                    selectedConnectionIndex, dragMode, k_rows, k_cols, k_spacing, k_radius, k_snapTolerance);
                handleMouse(*event, selectedRow, selectedCol, slots, connections,
                    selectedConnectionIndex, dragMode, hoveredSlotIndex, k_rows, k_cols, k_spacing, k_radius, k_snapTolerance);
            }
        }

        window.clear();


        int selectedIndex = selectedRow * k_cols + selectedCol;


        for (int i = 0; i < (int)slots.size(); ++i)
        {
            if (slots[i].isEmpty())
            {
                slots[i].setSelected(false);
                slots[i].setHovered(false);
                slots[i].draw(window); 
                continue;
            }
            slots[i].setSelected((dragMode == DragMode::None) && (i == selectedIndex));
            slots[i].setHovered((i == hoveredSlotIndex));
            slots[i].draw(window);
        }

        for (int i = 0; i < (int)connections.size(); ++i)
        {
            connections[i].setSelected(i == selectedConnectionIndex);
            connections[i].draw(window);
        }


        auto endpointMatchesSlot = [&](sf::Vector2f point, sf::Color targetColor) -> bool
            {
                for (int i = 0; i < (int)slots.size(); ++i)
                {
                    if (slots[i].isEmpty()) continue;
                    if (distance(point, slots[i].getCenter()) < k_snapTolerance &&
                        slots[i].getColor() == targetColor)
                        return true;
                }
                return false;
            };

        bool won = true;
        for (int i = 0; i < (int)connections.size(); ++i)
        {
            sf::Color c = connections[i].getColor();
            if (!endpointMatchesSlot(connections[i].getStart(), c) ||
                !endpointMatchesSlot(connections[i].getEnd(), c))
            {
                won = false;
                break;
            }
        }

        if (won)
        {
            window.clear(sf::Color::Black);

            std::vector<int> winDots = {
                // W
                30, 41, 52, 43, 54, 45, 36,
                // I
                46, 56, 
                // N
                37, 47, 57, 48, 39, 49, 59, 
            };

            for (int idx : winDots)
            {
                if (idx < (int)slots.size())
                {
                    sf::CircleShape dot(15.f);
                    dot.setFillColor(sf::Color::Green);
                    dot.setPosition(slots[idx].getPosition());
                    window.draw(dot);
                }
            }

            window.display();

            sf::sleep(sf::seconds(10.f));
            window.close();
        }
             
        window.display();
    }
    return 0;
}
