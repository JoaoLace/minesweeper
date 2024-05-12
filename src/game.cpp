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
    initSound();
    initTiles();
    initFlag();
    calculateAllBombs();
}
void game::initFlag()
{
    flags = new std::vector<sf::Sprite>();
    if (!flagTexture.loadFromFile("../assets/flag.png"))
    {
        std::cout << "ERROR: Could not load image from file: ./assets/flag.pn";
    }
    flagControl.setTexture(flagTexture);
    flagControl.setPosition(0,0);
    flagControl.setScale(0.3,0.3);
    

    tx_numberOfFlags.setFont(font);
    tx_numberOfFlags.setCharacterSize(24);
    tx_numberOfFlags.setPosition(30,0);
    tx_numberOfFlags.setFillColor(sf::Color::White);
}
void game::initSound()
{
    sb_bomb.loadFromFile("../assets/bomb2.wav");
    sb_win.loadFromFile ("../assets/bomb.wav");
    s_bombS.setBuffer(sb_bomb);
    s_win.setBuffer(sb_win);
}
void initFont()
{
    if (!font.loadFromFile("../assets/JetBrainsMono-Bold-Italic.ttf"))
    {
        std::cout << "ERROR: Could not load font from file: ./assets/JetBrainsMono-Bold-Italic.ttf";
    }
}
void game::initWindow()
{
    window = new sf::RenderWindow(window_size, "Minesweeper", sf::Style::Titlebar);
    window->setFramerateLimit(30);
    window->setPosition(sf::Vector2i(500, 100));
}
void game::initVars()
{
    gameRunning = true;
    gameMode = 3;
    maxBombs = (0.1 * gameMode) * (field_width * field_height);
    currentBombs = 0;
    changeColor = false;
    gameStarted = false;
    lostBool = false;
    maxFlags = 0;
    currentFlags = 0;
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
    maxFlags = currentBombs;
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
                temp.sprite.setFillColor(light_green);
                control = 1;
            }
            else
            {
                temp.sprite.setFillColor(green);
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
        win();
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
                if (!lostBool)
                {
                    lostBool = true;
                    lost();
                }
            }
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
    renderNumberOfFlags();
    window->display();
}
void game::renderNumberOfFlags()
{
    int num  = maxFlags-currentFlags;
    std::string str = "x" + std::to_string(num);
    tx_numberOfFlags.setString(str);

    window->draw(flagControl);
    window->draw(tx_numberOfFlags);

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

    initInitalScreen();
    while (gameRunning)
    {
        if (gameStarted)
        {
            update();
            render();
        }
        else if (!gameStarted)
        {
            inicialScreen();
        }
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
    if (tiles->at(index).sprite.getFillColor() == light_green)
    {
        tiles->at(index).sprite.setFillColor(light_brown);
    }
    else if (tiles->at(index).sprite.getFillColor() == green)
    {
        tiles->at(index).sprite.setFillColor(brown);
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
    case 6:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(light_green);
        break;
    case 7:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(pink);
        break;
    case 8:
        tiles->at(index).numberOfBombs.setString(std::to_string(num));
        tiles->at(index).numberOfBombs.setFillColor(orange);
        break;
    default:
        break;
    }
}
void game::createFlag(int x, int y)
{
    tiles->at(returnIndex(x, y)).flaged = true;
    currentFlags++;
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
    currentFlags--;
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
void game::lost()
{
    showBombs();
    delete tiles;
    delete window;
    delete flags;

    // Reinitialize game
    init();
    gameStarted = false;
}

void game::resetField()
{
    for (int i = 0; i < field_width; i++)
    {
        for (int j = 0; j < field_height; j++)
        {
            field[i][j] = '#';
        }
    }
    currentBombs = 0;
    initField();
}
void game::showBombs()
{
    for (int w = 0; w < field_width; w++)
    {
        for (int h = 0; h < field_height; h++)
        {
            int index = returnIndex(h, w);
            if (tiles->at(index).isBomb)
            {
                tiles->at(index).sprite.setFillColor(sf::Color::Red);
                render();
                s_bombS.play();
                sf::sleep(sf::milliseconds(1000));
            }
        }
    }
    s_bombS.play();
    sf::sleep(sf::milliseconds(2000));
}
void game::win()
{
    updateEvents();
    renderFlags();
    s_win.play();
    window->draw(guiTexts[6]);
    window->display();
    s_win.play();
    sf::sleep(sf::seconds(1.5));
    delete tiles;
    delete window;
    delete flags;

    init();
    gameStarted = false;
}