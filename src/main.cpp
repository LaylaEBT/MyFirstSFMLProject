#include <SFML/Graphics.hpp>

void drawcircle(sf::RenderWindow & window, float position_x, float position_y)
{
 
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({ position_x, position_y });
    window.draw(shape);

}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        
        for (float position_x = 0.f; position_x <= 380.f; position_x += 25.f)
        {
            for (float position_y = 0.f; position_y <= 380.f; position_y += 25.f)
            {
                drawcircle(window, position_x, position_y);
            }
        }
        window.display();
    }
    return 0;
}
