#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Main.hpp>
#include <optional>
#include <algorithm>
#include <vector>
#include <cmath>
#include <slot.h>
#include <connection.h>
#include <DragMode.h>
#include <GameState.h>
#include <LevelSetup.h>


float distance(sf::Vector2f a, sf::Vector2f b);

//  selectCircle — handles keyboard input for moving the cursor and
//  picking up / dropping connection endpoints.
//  Returns true only when a connection endpoint is successfully dropped.

bool selectCircle(const sf::Event& event, int& selectedRow, int& selectedCol, std::vector<Slot>& slots, std::vector<Connection>& connections,
    int& selectedConnectionIndex, DragMode& dragMode, int rows, int cols, float spacing, float radius, float snapTolerance)
{
    if (!event.is<sf::Event::KeyPressed>())
        return false;

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
            // try to pick up a connection whose endpoint is on this slot
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
            // try to drop the connection on this slot
            float originalLength = connections[selectedConnectionIndex].getLength();

            sf::Vector2f otherEnd = (dragMode == DragMode::DraggingEnd)
                ? connections[selectedConnectionIndex].getStart()
                : connections[selectedConnectionIndex].getEnd();

            float newLength = distance(currentSlotCenter, otherEnd);

            if (std::abs(newLength - originalLength) > snapTolerance)
                break;  // wrong length — reject drop

            if (dragMode == DragMode::DraggingEnd)
                connections[selectedConnectionIndex].setEnd(currentSlotCenter);
            else
                connections[selectedConnectionIndex].setStart(currentSlotCenter);

            selectedConnectionIndex = -1;
            dragMode = DragMode::None;
            return true;  // successful drop
        }
        break;
    }

    default:
        break;
    }

    // if the cursor landed on an empty slot, skip to the first non-empty one
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

    // update live preview while dragging with keyboard
    if (dragMode != DragMode::None && selectedConnectionIndex != -1)
    {
        int idx = selectedRow * cols + selectedCol;
        sf::Vector2f newPos = slots[idx].getCenter();

        if (dragMode == DragMode::DraggingEnd)
            connections[selectedConnectionIndex].setEnd(newPos);
        else
            connections[selectedConnectionIndex].setStart(newPos);
    }

    return false;
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
    sf::Vector2f delta = b - a;
    return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}

//  getNearestSlotIndex — returns index of the nearest non-empty slot to mousePos within (radius * 2) pixels, or -1 if none close enough


int getNearestSlotIndex(sf::Vector2f mousePos,
    const std::vector<Slot>& slots, float radius)
{
    int   nearest = -1;
    float bestDist = radius * 2.f;

    for (int i = 0; i < (int)slots.size(); ++i)
    {
        if (slots[i].isEmpty()) continue;

        float dist = distance(mousePos, slots[i].getCenter());
        if (dist < bestDist)
        {
            bestDist = dist;
            nearest = i;
        }
    }
    return nearest;
}

//  handleMouse — handles mouse click and move events.
//  Returns true only when a connection endpoint is successfully dropped.

bool handleMouse(const sf::Event& event, sf::RenderWindow& window, int& selectedRow, int& selectedCol, std::vector<Slot>& slots, std::vector<Connection>& connections, 
    int& selectedConnectionIndex, DragMode& dragMode, int& hoveredSlotIndex, int rows, int cols, float spacing, float radius, float snapTolerance)
{

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouse->button != sf::Mouse::Button::Left)
            return false;

        sf::Vector2f clickPos = window.mapPixelToCoords(
            sf::Mouse::getPosition(window));

        if (dragMode == DragMode::None)
        {
            // try to pick up a connection endpoint close to the click
            for (int i = 0; i < (int)connections.size(); ++i)
            {
                if (distance(clickPos, connections[i].getEnd()) < radius)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingEnd;
                    return false;
                }
                if (distance(clickPos, connections[i].getStart()) < radius)
                {
                    selectedConnectionIndex = i;
                    dragMode = DragMode::DraggingStart;
                    return false;
                }
            }

            // click didn't hit an endpoint — just move the keyboard cursor
            selectedCol = static_cast<int>(
                std::clamp(clickPos.x / spacing, 0.f, static_cast<float>(cols - 1)));
            selectedRow = static_cast<int>(
                std::clamp(clickPos.y / spacing, 0.f, static_cast<float>(rows - 1)));

            return false;
        }
        else
        {
            // currently dragging — try to drop on the clicked slot
            int col = static_cast<int>(
                std::clamp(clickPos.x / spacing, 0.f, static_cast<float>(cols - 1)));
            int row = static_cast<int>(
                std::clamp(clickPos.y / spacing, 0.f, static_cast<float>(rows - 1)));
            int targetIndex = row * cols + col;

            if (slots[targetIndex].isEmpty())
                return false;

            sf::Vector2f snappedPos = slots[targetIndex].getCenter();

            // snap the other endpoint to its nearest grid position before measuring
            sf::Vector2f rawOther = (dragMode == DragMode::DraggingEnd)
                ? connections[selectedConnectionIndex].getStart()
                : connections[selectedConnectionIndex].getEnd();

            int otherCol = static_cast<int>(std::clamp(
                std::round((rawOther.x - radius) / spacing),
                0.f, static_cast<float>(cols - 1)));
            int otherRow = static_cast<int>(std::clamp(
                std::round((rawOther.y - radius) / spacing),
                0.f, static_cast<float>(rows - 1)));

            sf::Vector2f snappedOther = slots[otherRow * cols + otherCol].getCenter();

            float originalLength = connections[selectedConnectionIndex].getLength();
            float newLength = distance(snappedPos, snappedOther);

            if (std::abs(newLength - originalLength) > snapTolerance)
                return false;  // wrong length — reject drop

            if (dragMode == DragMode::DraggingEnd)
                connections[selectedConnectionIndex].setEnd(snappedPos);
            else
                connections[selectedConnectionIndex].setStart(snappedPos);

            selectedConnectionIndex = -1;
            dragMode = DragMode::None;
            hoveredSlotIndex = -1;
            return true;  // successful drop
        }
    }

    // ── mouse move — update live preview 

    if (event.is<sf::Event::MouseMoved>())
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            sf::Mouse::getPosition(window));

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

    return false;
}

//  checkWin — returns true if every connection has both endpoints sitting
//  on a slot whose colour matches the connection colour

bool checkWin(const std::vector<Connection>& connections,
    const std::vector<Slot>& slots, float snapTolerance)
{
    for (int i = 0; i < (int)connections.size(); ++i)
    {
        sf::Color c = connections[i].getColor();
        bool      startOk = false;
        bool      endOk = false;

        for (int j = 0; j < (int)slots.size(); ++j)
        {
            if (slots[j].isEmpty())        continue;
            if (slots[j].getColor() != c)  continue;

            if (distance(connections[i].getStart(), slots[j].getCenter()) < snapTolerance)
                startOk = true;
            if (distance(connections[i].getEnd(), slots[j].getCenter()) < snapTolerance)
                endOk = true;
        }

        if (!startOk || !endOk)
            return false;
    }
    return true;
}

//  scaleAndCenterSprite — scales a sprite to fit within maxW x maxH

void scaleAndCenterSprite(sf::Sprite& sprite, const sf::Texture& texture,
    float windowW, float maxW, float maxH, float topY)
{
    sf::Vector2u texSize = texture.getSize();
    if (texSize.x == 0 || texSize.y == 0) return;  // guard against empty texture

    float scale = std::min(maxW / texSize.x, maxH / texSize.y);
    sprite.setScale({ scale, scale });
    sprite.setPosition({
        (windowW - texSize.x * scale) / 2.f,
        topY
        });
}



int main()
{

    sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "Connecting dots");
    window.setFramerateLimit(60);

    const sf::Vector2u winSize = window.getSize();

    // game constants
    const int k_rows = 10;
    const int k_cols = 10;
    const float k_spacing = 50.f;
    const float k_radius = 20.f;
    const float k_snapTolerance = k_spacing * 0.75f;
    const int k_maxLevel = 3;

    //game state
    GameState gameState = GameState::StartScreen;
    int currentLevel = 1;
    int selectedRow = 0;
    int selectedCol = 0;
    int selectedConnectionIndex = -1;
    int hoveredSlotIndex = -1;
    DragMode dragMode = DragMode::None;

    sf::Clock clock;

    // build the slot grid 
    std::vector<Slot> slots;
    slots.reserve(k_rows * k_cols);

    for (int row = 0; row < k_rows; ++row)
        for (int col = 0; col < k_cols; ++col)
        {
            Slot slot(sf::Color::Cyan, k_radius);
            slot.setPosition({ col * k_spacing, row * k_spacing });
            slots.push_back(slot);
        }

    //connections (filled by setupLevel)
    std::vector<Connection> connections;
    setupLevel(currentLevel, slots, connections, k_cols, k_spacing, k_radius);

    //fonts
    sf::Font font;
    if (!font.openFromFile("assets/font.ttf"))
        printf("WARNING: could not load assets/font.ttf\n");

    //sounds
    sf::SoundBuffer dropBuffer;
    if (!dropBuffer.loadFromFile("assets/drop.wav"))
        printf("WARNING: could not load assets/drop.wav\n");
    sf::Sound dropSound(dropBuffer);

    sf::SoundBuffer winBuffer;
    if (!winBuffer.loadFromFile("assets/win.wav"))
        printf("WARNING: could not load assets/win.wav\n");
    sf::Sound winSound(winBuffer);

    //start screen
    sf::Texture startTexture;
    if (!startTexture.loadFromFile("assets/start.png"))
        printf("WARNING: could not load assets/start.png\n");
    sf::Sprite startSprite(startTexture);
    scaleAndCenterSprite(startSprite, startTexture,
        (float)winSize.x, winSize.x * 0.8f, winSize.y * 0.4f, winSize.y * 0.1f);

    sf::RectangleShape startButton({ 180.f, 60.f });
    startButton.setFillColor(sf::Color(50, 150, 255));
    startButton.setPosition({ winSize.x / 2.f - 90.f, winSize.y * 0.6f });

    sf::Text startText(font, "Play", 28);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({
        startButton.getPosition().x + 55.f,
        startButton.getPosition().y + 12.f });

    //win screen
    sf::Texture winTexture;
    if (!winTexture.loadFromFile("assets/win.png"))
        printf("WARNING: could not load assets/win.png\n");
    sf::Sprite winSprite(winTexture);
    scaleAndCenterSprite(winSprite, winTexture,
        (float)winSize.x, winSize.x * 0.8f, winSize.y * 0.4f, winSize.y * 0.1f);

    sf::RectangleShape restartButton({ 180.f, 60.f });
    restartButton.setFillColor(sf::Color(50, 200, 50));
    restartButton.setPosition({ winSize.x / 2.f - 200.f, winSize.y * 0.7f });

    sf::RectangleShape quitButton({ 180.f, 60.f });
    quitButton.setFillColor(sf::Color(200, 50, 50));
    quitButton.setPosition({ winSize.x / 2.f + 20.f, winSize.y * 0.7f });

    sf::Text restartText(font, "Restart", 28);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition({
        restartButton.getPosition().x + 20.f,
        restartButton.getPosition().y + 12.f });

    sf::Text quitText(font, "Quit", 28);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition({
        quitButton.getPosition().x + 45.f,
        quitButton.getPosition().y + 12.f });

    //  Game loop

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            else if (event->is<sf::Event::FocusGained>()) {}

            else if (gameState == GameState::StartScreen)
            {
                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f clickPos = window.mapPixelToCoords(
                        sf::Mouse::getPosition(window));

                    if (startButton.getGlobalBounds().contains(clickPos))
                    {
                        currentLevel = 1;
                        setupLevel(currentLevel, slots, connections,
                            k_cols, k_spacing, k_radius);
                        gameState = GameState::Playing;
                    }
                }
            }

            else if (gameState == GameState::Playing)
            {
                bool dropped = false;
                dropped |= selectCircle(*event,
                    selectedRow, selectedCol, slots, connections,
                    selectedConnectionIndex, dragMode,
                    k_rows, k_cols, k_spacing, k_radius, k_snapTolerance);
                dropped |= handleMouse(*event, window,
                    selectedRow, selectedCol, slots, connections,
                    selectedConnectionIndex, dragMode, hoveredSlotIndex,
                    k_rows, k_cols, k_spacing, k_radius, k_snapTolerance);

                if (dropped)
                {
                    dropSound.play();

                    if (checkWin(connections, slots, k_snapTolerance))
                    {
                        if (currentLevel < k_maxLevel)
                        {
                            currentLevel++;
                            setupLevel(currentLevel, slots, connections,
                                k_cols, k_spacing, k_radius);
                            selectedConnectionIndex = -1;
                            hoveredSlotIndex = -1;
                            dragMode = DragMode::None;
                        }
                        else
                        {
                            gameState = GameState::Won;
                            winSound.play();
                        }
                    }
                }
            }

            else if (gameState == GameState::Won)
            {
                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f clickPos = window.mapPixelToCoords(
                        sf::Mouse::getPosition(window));

                    if (restartButton.getGlobalBounds().contains(clickPos))
                    {
                        currentLevel = 1;
                        setupLevel(currentLevel, slots, connections,
                            k_cols, k_spacing, k_radius);
                        selectedConnectionIndex = -1;
                        hoveredSlotIndex = -1;
                        dragMode = DragMode::None;
                        gameState = GameState::Playing;
                    }

                    if (quitButton.getGlobalBounds().contains(clickPos))
                        window.close();
                }
            }
        }

        window.clear(sf::Color(20, 20, 40));

        if (gameState == GameState::StartScreen)
        {
            window.draw(startSprite);
            window.draw(startButton);
            window.draw(startText);
        }

        else if (gameState == GameState::Playing)
        {
            int  selectedIndex = selectedRow * k_cols + selectedCol;
            bool isDragging = (dragMode != DragMode::None
                && selectedConnectionIndex != -1);

            float draggedLength = -1.f;
            sf::Vector2f draggedOtherEnd = {};
            if (isDragging)
            {
                draggedLength = connections[selectedConnectionIndex].getLength();
                draggedOtherEnd = (dragMode == DragMode::DraggingEnd)
                    ? connections[selectedConnectionIndex].getStart()
                    : connections[selectedConnectionIndex].getEnd();
            }

            for (int i = 0; i < (int)slots.size(); ++i)
            {
                if (slots[i].isEmpty())
                {
                    slots[i].setSelected(false);
                    slots[i].setHovered(false);
                    slots[i].setValidTarget(false);
                    slots[i].draw(window, clock);
                    continue;
                }

                slots[i].setSelected(
                    (dragMode == DragMode::None) && (i == selectedIndex));
                slots[i].setHovered(i == hoveredSlotIndex);

                bool validTarget = false;
                if (isDragging)
                {
                    float newLength = distance(slots[i].getCenter(), draggedOtherEnd);
                    validTarget = std::abs(newLength - draggedLength) < k_snapTolerance;
                }
                slots[i].setValidTarget(validTarget);

                slots[i].draw(window, clock);
            }

            for (int i = 0; i < (int)connections.size(); ++i)
            {
                connections[i].setSelected(i == selectedConnectionIndex);
                connections[i].draw(window);
            }
        }

        else if (gameState == GameState::Won)
        {
            window.draw(winSprite);
            window.draw(restartButton);
            window.draw(quitButton);
            window.draw(restartText);
            window.draw(quitText);
        }

        window.display();
    }

    return 0;
}
