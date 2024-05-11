#include "../lib/game.h"

typedef enum gamemode{
    easy = 1,
    medium = 2,
    hard = 3
};

sf::Text createText(float size, sf::Vector2f pos, sf::Color color, std::string text);
const sf::Vector2f getMousePos(sf::RenderTarget &renderTarget);
bool pointInRect(sf::Vector2f point, sf::FloatRect rect);

void game::initInitalScreen()
{
    guiTexts[0] = (createText(60, {200, 0}, sf::Color::Black, "MINESWEEPER"));
    guiTexts[1] = (createText(50, {200, 200}, sf::Color::Black, "-> EASY"));
    guiTexts[2] = (createText(50, {200, 300}, sf::Color::Black, "-> MEDIUM"));
    guiTexts[3] = (createText(50, {200, 400}, sf::Color::Black, "-> HARD"));
    guiTexts[4] = (createText(50, {200, 500}, sf::Color::Black, "-> CREDITS"));
    guiTexts[5] = (createText(50, {200, 600}, sf::Color::Black, "-> EXIT"));
}

void game::inicialScreen()
{
    updateInitalScreen();
    renderInitalScreen();
}

void game::updateInitalScreen()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
            gameRunning = false;
        }
    }
    sf::Vector2f mousePos = getMousePos(*window);
    for (int i = 1; i < 6; i++)
    {
        sf::FloatRect playButtonRect = guiTexts[i].getGlobalBounds();

        if (pointInRect(mousePos, playButtonRect))
        {
            guiTexts[i].setFillColor(sf::Color::White);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                switch (i)
                {
                case 1:
                    gameStarted = true;
                    gameMode = easy;
                    break;
                case 2:
                    gameStarted = true;
                    gameMode = medium;
                    break;
                case 3:
                    gameStarted = true;
                    gameMode = hard;
                    break;
                case 4:
                    break;
                case 5:
                    gameRunning = false;
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            guiTexts[i].setFillColor(sf::Color::Black);
        }
    }
}

void game::renderInitalScreen()
{
    window->clear();
    renderTiles();
    renderTextInitalScreen();
    window->display();
}

void game::renderTextInitalScreen()
{
    for (sf::Text text : guiTexts)
    {
        window->draw(text);
    }
}

sf::Text createText(float size, sf::Vector2f pos, sf::Color color, std::string text)
{
    sf::Text temp;
    temp.setFont(font);
    temp.setCharacterSize(size);
    temp.setPosition(pos);
    temp.setFillColor(color);
    temp.setString(text);

    return temp;
}

const sf::Vector2f getMousePos(sf::RenderTarget &renderTarget)
{
    const sf::RenderWindow *window = dynamic_cast<const sf::RenderWindow *>(&renderTarget);

    if (window)
    {
        return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
    }
    else
    {
        return sf::Vector2f(-1, -1);
    }
}

bool pointInRect(sf::Vector2f point, sf::FloatRect rect)
{
    return rect.contains(point);
}