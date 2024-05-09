#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

#define window_width 800
#define window_height 800
#define window_size sf::VideoMode(window_width, window_height)
#define field_width 20
#define field_height 20

extern sf::Font font;
void initFont();


const float tileSize = 40;

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
    int maxBombs;
    int currentBombs;
    void init();
    void initWindow();
    void initVars();
    void initField();
    void initTiles();
    void update();
    void updateEvents();
    void render();
    void renderTiles();
    void renderNumbers();
    int calculateBombs(int x, int y);
    void calculateAllBombs();
    bool tileIsBomb(int x, int y);
    int returnIndex(int x, int y);
    void debug(); // lmao
public:
    game();
    ~game();
    void run();
    void debugMode();

};
