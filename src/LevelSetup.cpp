#include "LevelSetup.h"

// resets every slot to default
static void resetSlots(std::vector<Slot>& slots)
{
    for (auto& slot : slots)
    {
        slot.setColor(sf::Color::Cyan);
        slot.setEmpty(false);
    }
}

void setupLevel(int level,
    std::vector<Slot>& slots,
    std::vector<Connection>& connections,
    int k_cols, float k_spacing, float k_radius)
{
    resetSlots(slots);
    connections.clear();


    if (level == 1)

    {
        // solution slots
        slots[2].setColor(sf::Color::Blue);
        slots[32].setColor(sf::Color::Blue);
        slots[67].setColor(sf::Color::Green);
        slots[89].setColor(sf::Color::Green);
        slots[50].setColor(sf::Color(255, 165, 0));
        slots[54].setColor(sf::Color(255, 165, 0));
        slots[71].setColor(sf::Color::Magenta);
        slots[74].setColor(sf::Color::Magenta);

        // barriers
        for (int i = 23; i <= 26; ++i) slots[i].setEmpty(true);
        for (int i = 55; i <= 57; ++i) slots[i].setEmpty(true);
        slots[36].setEmpty(true);
        slots[46].setEmpty(true);

        // starting positions
        connections.emplace_back(slots[5].getCenter(), slots[35].getCenter(), sf::Color::Blue);
        connections.emplace_back(slots[7].getCenter(), slots[29].getCenter(), sf::Color::Green);
        connections.emplace_back(slots[59].getCenter(), slots[99].getCenter(), sf::Color(255, 165, 0));
        connections.emplace_back(slots[91].getCenter(), slots[94].getCenter(), sf::Color::Magenta);
    }


    else if (level == 2)

    {
        

        // solution slots
        slots[2].setColor(sf::Color::Blue);
        slots[72].setColor(sf::Color::Blue);
        slots[7].setColor(sf::Color::Green);
        slots[77].setColor(sf::Color::Green);
        slots[19].setColor(sf::Color(255, 165, 0));
        slots[59].setColor(sf::Color(255, 165, 0));
        slots[10].setColor(sf::Color::Magenta);
        slots[50].setColor(sf::Color::Magenta);
        slots[34].setColor(sf::Color::Red);
        slots[65].setColor(sf::Color::Red);

        // barriers
        for (int i = 22; i <= 27; ++i) slots[i].setEmpty(true); 
        for (int i = 41; i <= 48; ++i) slots[i].setEmpty(true);  
        slots[62].setEmpty(true);
        slots[63].setEmpty(true);
        slots[64].setEmpty(true);

        // starting positions
        connections.emplace_back(slots[0].getCenter(), slots[70].getCenter(), sf::Color::Blue);
        connections.emplace_back(slots[9].getCenter(), slots[79].getCenter(), sf::Color::Green);
        connections.emplace_back(slots[15].getCenter(), slots[55].getCenter(), sf::Color(255, 165, 0));
        connections.emplace_back(slots[18].getCenter(), slots[58].getCenter(), sf::Color::Magenta);
        connections.emplace_back(slots[30].getCenter(), slots[61].getCenter(), sf::Color::Red);
    }

    else if (level == 3)

    {
       

        // solution slots
        slots[0].setColor(sf::Color::Blue);
        slots[66].setColor(sf::Color::Blue);
        slots[9].setColor(sf::Color::Green);
        slots[63].setColor(sf::Color::Green);
        slots[90].setColor(sf::Color(255, 165, 0));
        slots[54].setColor(sf::Color(255, 165, 0));
        slots[99].setColor(sf::Color::Magenta);
        slots[55].setColor(sf::Color::Magenta);
        slots[4].setColor(sf::Color::Red);
        slots[94].setColor(sf::Color::Red);

        // barriers
        for (int i = 21; i <= 28; ++i) slots[i].setEmpty(true);  
        slots[52].setEmpty(true);                                
        slots[53].setEmpty(true);
        slots[56].setEmpty(true);
        for (int i = 72; i <= 77; ++i) slots[i].setEmpty(true);  

        // starting positions
        connections.emplace_back(slots[6].getCenter(), slots[60].getCenter(), sf::Color::Blue);
        connections.emplace_back(slots[3].getCenter(), slots[69].getCenter(), sf::Color::Green);
        connections.emplace_back(slots[95].getCenter(), slots[51].getCenter(), sf::Color(255, 165, 0));
        connections.emplace_back(slots[40].getCenter(), slots[84].getCenter(), sf::Color::Magenta);
        connections.emplace_back(slots[8].getCenter(), slots[98].getCenter(), sf::Color::Red);
    }
}