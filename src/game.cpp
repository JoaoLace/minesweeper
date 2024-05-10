#include "../lib/game.h"
sf::Font font;
game::game()
{
    init();
}
void game::init()
{
    srand(time(NULL));
    initVars();
    initFont();
    initWindow();
    initField();
    initTiles();
    initFlag();
    calculateAllBombs();
}
void game::initFlag()
{
    flags = new std::vector<sf::Sprite>();
    if (!flagTexture.loadFromFile("./assets/flag.png"))
    {
        std::cout << "ERROR: Could not load image from file: ./assets/flag.pn";
    }
}
void initFont()
{
    if (!font.loadFromFile("./assets/JetBrainsMono-Bold-Italic.ttf"))
    {
        std::cout << "ERROR: Could not load font from file: ./assets/JetBrainsMono-Bold-Italic.ttf";
    }
}
void game::initWindow()
{
    window = new sf::RenderWindow(window_size, "Minesweeper", sf::Style::Titlebar);
    window->setFramerateLimit(30);
    window->setPosition(sf::Vector2i(500, 500));
}
void game::initVars()
{
    gameRunning = true;
    maxBombs = 0.3 * (field_width * field_height);
    currentBombs = 0;
    changeColor = false;
}
void game::initField()
{
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            if (rand() % 8 == 0 && currentBombs < maxBombs)
            {
                field[w][h] = '*';
                currentBombs++;
            }
            else
            {
                field[w][h] = '#';
            }
        }
    }
}
void game::initTiles()
{
    tiles = new std::vector<tile>();
    int control = 0;
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            tile temp(sf::Vector2f(tileSize * w, tileSize * h));
            temp.x = w;
            temp.y = h;
            if (control == 0)
            {
                temp.sprite.setFillColor(sf::Color(167, 217, 72, 255));
                control = 1;
            }
            else
            {
                temp.sprite.setFillColor(sf::Color(142, 204, 57, 255));
                control = 0;
            }
            if (field[w][h] == '#')
            {
                temp.isBomb = false;
            }
            else
            {
                temp.isBomb = true;
            }
            tiles->push_back(temp);
        }
    }
}
game::~game()
{
    delete tiles;
    delete window;
    delete flags;
}
void game::update()
{
    updateEvents();
    updateTiles();
    if (testWin())
        {
            gameRunning = !gameRunning;
        }
}
void game::updateEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
            gameRunning = false;
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                int x = event.mouseButton.x / tileSize;
                int y = event.mouseButton.y / tileSize;
                updateClick(x, y);
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                int x = event.mouseButton.x / tileSize;
                int y = event.mouseButton.y / tileSize;
                testFlag(x, y);
            }
        }
    }
}
void game::updateTiles()
{
    int control = 0;
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            int index = returnIndex(w, h);
            if (tiles->at(index).isBomb and tiles->at(index).clicked)
            {
                tiles->at(index).sprite.setFillColor(sf::Color::Red);
            }
            // else if (!tiles->at(index).isBomb and tiles->at(index).clicked)
            // {
            //     if (control == 0)
            //     {
            //         tiles->at(index).sprite.setFillColor(sf::Color(215, 184, 153, 255));
            //         control = 1;
            //     }
            //     else
            //     {
            //         tiles->at(index).sprite.setFillColor(sf::Color(229, 194, 159, 255));
            //         control = 0;
            //     }
            // }
        }
    }
}
void game::updateClick(int x, int y)
{
    if (x >= 0 && x < field_width && y >= 0 && y < field_height)
    {
        handleClick(x, y);
    }
}

void game::render()
{
    window->clear();
    renderTiles();
    renderNumbers();
    renderFlags();
    window->display();
}
void game::renderFlags()
{
    for (const auto &flag : *flags)
    {
        window->draw(flag);
    }
}
void game::renderTiles()
{
    for (const auto &tile : *tiles)
    {

        window->draw(tile.sprite);
    }
}
void game::renderNumbers()
{
    for (const auto &tile : *tiles)
    {
        if (tile.clicked)
        {
            window->draw(tile.numberOfBombs);
        }
    }
}
void game::run()
{
    while (gameRunning)
    {
        update();
        render();
    }
}
int game::calculateBombs(int x, int y)
{
    int temp = 0;
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < field_width && ny >= 0 && ny < field_height)
            {
                if (tileIsBomb(nx, ny))
                {
                    temp++;
                }
            }
        }
    }
    return temp;
}
bool game::tileIsBomb(int x, int y)
{
    if (field[x][y] == '*')
        return true;

    return false;
}
void game::calculateAllBombs()
{
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            if (!tileIsBomb(w, h))
            {
                int index = returnIndex(w, h);
                int num = calculateBombs(w, h);
                tiles->at(index).bombsArround = num;
                setTileTextColor(index);
            }
        }
    }
}
int game::returnIndex(int x, int y)
{
    int count = 0;
    for (auto &temp : *tiles)
    {
        if (temp.x == x and temp.y == y)
            return count;

        count++;
    }
    return -1;
}
void game::debug()
{
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            int index = returnIndex(w, h);
            if (tiles->at(index).isBomb)
            {
                std::cout << "*";
            }
            else
                std::cout << tiles->at(index).bombsArround;
        }
        std::cout << "\n";
    }
}
void game::debugMode()
{
    debug();
}
void game::handleClick(int x, int y)
{
    int index = returnIndex(x, y);
    tiles->at(index).clicked = true;
    if (changeColor)
    {
        changeColor = !changeColor;
        tiles->at(index).sprite.setFillColor(sf::Color(215, 184, 153, 255));
    }
    else if (!changeColor)
    {
        changeColor = !changeColor;
        tiles->at(index).sprite.setFillColor(sf::Color(229, 194, 159, 255));
    }
    if (tiles->at(index).bombsArround != 0)
        return;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < field_width && ny >= 0 && ny < field_height)
            {
                if (!tiles->at(returnIndex(nx, ny)).clicked)
                {
                    handleClick(nx, ny);
                }
            }
        }
    }
}

void game::setTileTextColor(int index)
{
    // TODO: add more colors
    int num = tiles->at(index).bombsArround;
    switch (num)
    {
    case 0:
        tiles->at(index).numberOfBombs.setString("");
        break;
    case 1:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(sf::Color::Blue);
        break;
    case 2:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(sf::Color::Green);
        break;
    case 3:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(sf::Color::Red);
        break;
    case 4:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(sf::Color::Magenta);
        break;
    case 5:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(sf::Color::Yellow);
        break;

    default:
        break;
    }
}
void game::createFlag(int x, int y)
{
    tiles->at(returnIndex(x, y)).flaged = true;
    sf::Sprite temp;
    temp.setTexture(flagTexture);
    temp.setPosition(tiles->at(returnIndex(x, y)).sprite.getPosition());
    temp.setScale(0.4, 0.4);
    flags->push_back(temp);
}
void game::testFlag(int x, int y)
{
    int index = returnIndex(x, y);
    if (tiles->at(index).flaged)
    {
        removeFlag(x, y);
    }
    else
    {
        createFlag(x, y);
    }
}
void game::removeFlag(int x, int y)
{
    tiles->at(returnIndex(x, y)).flaged = false;
    int index = returnIndex(x, y);
    flags->erase(flags->begin() + index);
}
bool game::testWin()
{
    for (auto &tile : *tiles)
    {
        if (tile.isBomb and !tile.flaged)
            return false;
    }
    return true;
}