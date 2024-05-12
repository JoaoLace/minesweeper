#include "../lib/game.h"

enum gamemode{
    easy = 1,
    medium = 2,
    hard = 3
};

sf::Text createText(float size, sf::Vector2f pos, sf::Color color, std::string text);
const sf::Vector2f getMousePos(sf::RenderTarget &renderTarget);
bool pointInRect(sf::Vector2f point, sf::FloatRect rect);

void game::initInitalScreen()
{
    guiTexts[0] = (createText(60, {200, 0}, black, "MINESWEEPER"));
    guiTexts[1] = (createText(50, {200, 200}, black, "-> EASY"));
    guiTexts[2] = (createText(50, {200, 300}, black, "-> MEDIUM"));
    guiTexts[3] = (createText(50, {200, 400}, black, "-> HARD"));
    guiTexts[4] = (createText(50, {200, 500}, black, "-> CREDITS"));
    guiTexts[5] = (createText(50, {200, 600}, black, "-> EXIT"));
    guiTexts[6] = (createText(70, {window_width/2 - 200,window_height/2 - 70},black, "YOU WON!!!"));
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
            return; // Retorna imediatamente, não encerrando o loop principal
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
                    // TODO: Add credits
                    break;
                case 5:
                    gameRunning = false;
                    return; 
                default:
                    break;
                }
            }
        }
        else
        {
            guiTexts[i].setFillColor(black);
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
    for (int i = 0; i < 6; i++)
    {
        window->draw(guiTexts[i]);
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