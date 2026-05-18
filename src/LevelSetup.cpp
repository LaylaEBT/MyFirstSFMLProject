#include "LevelSetup.h"

//Wong colour-blind safe palette 
static const sf::Color k_blue = sf::Color(0, 114, 178);
static const sf::Color k_vermillion = sf::Color(213, 94, 0);
static const sf::Color k_bluishGreen = sf::Color(0, 158, 115);
static const sf::Color k_yellow = sf::Color(240, 228, 66);
static const sf::Color k_skyBlue = sf::Color(86, 180, 233);
static const sf::Color k_orange = sf::Color(230, 159, 0);
static const sf::Color k_reddishPurple = sf::Color(204, 121, 167);

static void resetSlots(std::vector<Slot>& slots)
{
    for (auto& slot : slots)
    {
        slot.setColor(sf::Color(80, 80, 80));  // neutral dark grey — clearly distinct from all wire colours
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
        slots[2].setColor(k_blue);
        slots[32].setColor(k_blue);
        slots[67].setColor(k_bluishGreen);
        slots[89].setColor(k_bluishGreen);
        slots[50].setColor(k_orange);
        slots[54].setColor(k_orange);
        slots[71].setColor(k_reddishPurple);
        slots[74].setColor(k_reddishPurple);

        for (int i = 23; i <= 26; ++i) slots[i].setEmpty(true);
        for (int i = 55; i <= 57; ++i) slots[i].setEmpty(true);
        slots[36].setEmpty(true);
        slots[46].setEmpty(true);

        connections.emplace_back(slots[5].getCenter(), slots[35].getCenter(), k_blue);
        connections.emplace_back(slots[7].getCenter(), slots[29].getCenter(), k_bluishGreen);
        connections.emplace_back(slots[59].getCenter(), slots[99].getCenter(), k_orange);
        connections.emplace_back(slots[91].getCenter(), slots[94].getCenter(), k_reddishPurple);
    }
    else if (level == 2)
    {
        slots[2].setColor(k_blue);
        slots[72].setColor(k_blue);
        slots[7].setColor(k_bluishGreen);
        slots[77].setColor(k_bluishGreen);
        slots[19].setColor(k_orange);
        slots[59].setColor(k_orange);
        slots[10].setColor(k_reddishPurple);
        slots[50].setColor(k_reddishPurple);
        slots[34].setColor(k_vermillion);
        slots[65].setColor(k_vermillion);

        for (int i = 22; i <= 27; ++i) slots[i].setEmpty(true);
        for (int i = 41; i <= 48; ++i) slots[i].setEmpty(true);
        slots[62].setEmpty(true);
        slots[63].setEmpty(true);
        slots[64].setEmpty(true);

        connections.emplace_back(slots[0].getCenter(), slots[70].getCenter(), k_blue);
        connections.emplace_back(slots[9].getCenter(), slots[79].getCenter(), k_bluishGreen);
        connections.emplace_back(slots[15].getCenter(), slots[55].getCenter(), k_orange);
        connections.emplace_back(slots[18].getCenter(), slots[58].getCenter(), k_reddishPurple);
        connections.emplace_back(slots[30].getCenter(), slots[61].getCenter(), k_vermillion);
    }
    else if (level == 3)
    {
        slots[0].setColor(k_blue);
        slots[66].setColor(k_blue);
        slots[9].setColor(k_bluishGreen);
        slots[63].setColor(k_bluishGreen);
        slots[90].setColor(k_orange);
        slots[54].setColor(k_orange);
        slots[99].setColor(k_reddishPurple);
        slots[55].setColor(k_reddishPurple);
        slots[4].setColor(k_vermillion);
        slots[94].setColor(k_vermillion);

        for (int i = 21; i <= 28; ++i) slots[i].setEmpty(true);
        slots[52].setEmpty(true);
        slots[53].setEmpty(true);
        slots[56].setEmpty(true);
        for (int i = 72; i <= 77; ++i) slots[i].setEmpty(true);

        connections.emplace_back(slots[6].getCenter(), slots[60].getCenter(), k_blue);
        connections.emplace_back(slots[3].getCenter(), slots[69].getCenter(), k_bluishGreen);
        connections.emplace_back(slots[95].getCenter(), slots[51].getCenter(), k_orange);
        connections.emplace_back(slots[40].getCenter(), slots[84].getCenter(), k_reddishPurple);
        connections.emplace_back(slots[8].getCenter(), slots[98].getCenter(), k_vermillion);
    }
}