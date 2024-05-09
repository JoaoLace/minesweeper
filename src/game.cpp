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
    calculateAllBombs();
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
    maxBombs = 50;
    currentBombs = 0;
}
void game::initField()
{
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            if (rand() % 10 == 0 && currentBombs < maxBombs)
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
    for (size_t w = 0; w < field_width; w++)
    {
        for (size_t h = 0; h < field_height; h++)
        {
            tile temp(sf::Vector2f(tileSize * w, tileSize * h));
            temp.x = w;
            temp.y = h;
            if (field[w][h] == '#')
            {
                temp.isBomb = false;
            }
            else
            {
                temp.isBomb = true;
                temp.sprite.setFillColor(sf::Color::Red);
            }
            tiles->push_back(temp);
        }
    }
}
game::~game()
{
    delete tiles;
    delete window;
}
void game::update()
{
    updateEvents();
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
    }
}
void game::render()
{
    window->clear();
    renderTiles();
    renderNumbers();
    window->display();
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
        window->draw(tile.numberOfBombs);
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
                tiles->at(index).numberOfBombs.setString(std::to_string(num));
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