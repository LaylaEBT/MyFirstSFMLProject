#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <optional>
#include <algorithm>
#include <vector>
#include <cmath>

void drawcircle(sf::RenderWindow & window, float position_x, float position_y, sf::Color color)
{
 
    sf::CircleShape shape(10.f);
    shape.setFillColor(color);
    shape.setPosition({ position_x, position_y });
    window.draw(shape);

}


void selectCircle(const sf::Event& event, int& selectedRow, int& selectedCol)
{
    if (!event.is<sf::Event::KeyPressed>())
        return;



    switch (event.getIf<sf::Event::KeyPressed>()->scancode)
    {
    case sf::Keyboard::Scancode::Left:
    case sf::Keyboard::Scancode::Q: 
        selectedCol--;
        break;

    case sf::Keyboard::Scancode::Right:
    case sf::Keyboard::Scancode::D:
        selectedCol++;
        break;

    case sf::Keyboard::Scancode::Up:
    case sf::Keyboard::Scancode::Z:
        selectedRow--;// ajouter une fonction qui increase ou decrease le row ou la colonne si atteint la valeur max
        break;

    case sf::Keyboard::Scancode::Down:
    case sf::Keyboard::Scancode::S:
        selectedRow++;
        break;

    default:
        break;
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

    std::vector<sf::CircleShape> circles;


    const int rows = 16;
    const int cols = 16;
    const float spacing = 25.f;

    const float radius = 10.f;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            sf::CircleShape circle(radius);

            circle.setOrigin({ radius, radius }); 
            circle.setPosition({ col * spacing, row * spacing });
            circle.setFillColor(sf::Color::Green);

            circles.push_back(circle);
        }
    }


   

    while (window.isOpen()) // boucle moteur
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else
            {
                selectCircle(*event, selectedRow, selectedCol);
            }
        }

        window.clear();

        /*for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                bool isSelected = (row == selectedRow && col == selectedCol);

                sf::Color color = isSelected
                    ? sf::Color::Blue
                    : sf::Color::Green;

                drawcircle(
                    window,
                    col * spacing,
                    row * spacing,
                    color
                );
            }
        }*/

        for (auto& circle : circles)
        {
            window.draw(circle);
        }

        int selectedIndex = selectedRow * cols + selectedCol;

        drawConnection(window, circles[5], circles[15], 8.f);
        drawConnection(window, circles[75], circles[32], 8.f);
        
        window.display();
    }
    return 0;
}
