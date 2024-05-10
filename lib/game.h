#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

#define window_width 800
#define window_height 800
#define window_size sf::VideoMode(window_width, window_height)
#define field_width 15
#define field_height 15

extern sf::Font font;
void initFont();

const float tileSize = window_width / field_width;

class tile
{
private:
    void init(sf::Vector2f position);

public:
    tile(sf::Vector2f position);
    ~tile();
    sf::RectangleShape sprite;
    sf::Text numberOfBombs;
    bool isBomb;
    bool clicked;
    bool flaged;
    int bombsArround;
    int x;
    int y;
};

class game
{
private:
    sf::RenderWindow *window;
    char field[field_width][field_height];
    bool gameRunning;
    std::vector<tile> *tiles;
    std::vector<sf::Sprite> *flags;
    sf::Texture flagTexture;
    int maxBombs;
    int currentBombs;
    bool changeColor;
    void init();
    void initWindow();
    void initVars();
    void initField();
    void initTiles();
    void initFlag();
    void update();
    void updateEvents();
    void updateTiles();
    void updateClick(int x, int y);
    void render();
    void renderTiles();
    void renderFlags();
    void renderNumbers();
    int calculateBombs(int x, int y);
    void calculateAllBombs();
    void handleClick(int x, int y);
    bool tileIsBomb(int x, int y);
    int returnIndex(int x, int y);
    void setTileTextColor(int index);
    void createFlag(int x, int y);
    void testFlag(int x, int y);
    bool testWin();
    void removeFlag(int x, int y);
    void debug(); // lmao
public:
    game();
    ~game();
    void run();
    void debugMode();
};
