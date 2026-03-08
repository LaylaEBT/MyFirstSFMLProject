#include <SFML/Graphics.hpp>

class Slot {
public:

    bool isSelected = false;

    void setPosition(sf::Vector2f pos);
    

    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window);

private:
    sf::Vector2f m_position;

};