#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <optional>
#include <algorithm>

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
        selectedRow--;
        break;

    case sf::Keyboard::Scancode::Down:
    case sf::Keyboard::Scancode::S:
        selectedRow++;
        break;

    default:
        break;
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");

    int selectedRow = 0;
    int selectedCol = 0;

    const int rows = 16;
    const int cols = 16;
    const float spacing = 25.f;

   

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

        for (int row = 0; row < rows; ++row)
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
        }

        window.display();
    }
    return 0;
}
